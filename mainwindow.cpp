/***************************************************************************
**                                                                        **
** xxSimulator, "turn-based game-style" evolution simulator               **
** Copyright (C) 2022  hansico                                            **
**                                                                        **
** This program is free software: you can redistribute it and/or modify   **
** it under the terms of the GNU General Public License as published by   **
** the Free Software Foundation, either version 3 of the License, or      **
** (at your option) any later version.                                    **
**                                                                        **
** This program is distributed in the hope that it will be useful,        **
** but WITHOUT ANY WARRANTY; without even the implied warranty of         **
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          **
** GNU General Public License for more details.                           **
**                                                                        **
** You should have received a copy of the GNU General Public License      **
** along with this program.  If not, see <https://www.gnu.org/licenses/>. **
****************************************************************************
** Author: hansico                                                        **
** Contact: hansico@outlook.com                                           **
** Date: 27.01.2022                                                       **
** Version: 0.01                                                          **
****************************************************************************/
#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow(parent)
  , ui(new Ui::MainWindow)
{
  ui->setupUi(this);

  rend = new ImageRenderer(DEFAULT_MAPSIZE_COLUMNS, DEFAULT_MAPSIZE_ROWS, ui->label->size());

  sim = new SimuMain(DEFAULT_MAPSIZE_COLUMNS, DEFAULT_MAPSIZE_ROWS);
  QObject::connect(sim, &SimuMain::epochCompleted, this, &MainWindow::epochUpdate);
  QObject::connect(sim, &SimuMain::agesUpdated, this, &MainWindow::updateAges);
  QObject::connect(sim, &SimuMain::mutationsUpdated, this, &MainWindow::updateMutations);

  QObject::connect(sim, &SimuMain::drawACreature, rend, &ImageRenderer::drawCreature, Qt::QueuedConnection);
  QObject::connect(sim, &SimuMain::drawPlant, rend, &ImageRenderer::drawPlant, Qt::QueuedConnection);
  QObject::connect(sim, &SimuMain::drawClean, rend, &ImageRenderer::clean, Qt::QueuedConnection);
  QObject::connect(sim, &SimuMain::turnReset, rend, &ImageRenderer::show, Qt::QueuedConnection);
  connect(rend, &ImageRenderer::imageUpdate, this, &MainWindow::updateImage, Qt::BlockingQueuedConnection);


  sim->moveToThread(&simulationThread);
  connect(&simulationThread, &QThread::finished, sim, &QObject::deleteLater);
  connect(this, &MainWindow::startSimulationInThread, sim, &SimuMain::runSim);
  simulationThread.start();

  connect(&drawingThread, &QThread::finished, rend, &QObject::deleteLater);
  rend->moveToThread(&drawingThread);
  drawingThread.start();

  // QCUSTOMPLOT setup
  ui->age_plot->addGraph();
  ui->age_plot->graph(0)->setName("Minimum age");
  ui->age_plot->graph(0)->setPen(QPen(QColor(0,60,255)));
  ui->age_plot->addGraph();
  ui->age_plot->graph(1)->setName("Average age");
  ui->age_plot->graph(1)->setPen(QPen(QColor(60,255,0)));
  ui->age_plot->addGraph();
  ui->age_plot->graph(2)->setName("Maximum age");
  ui->age_plot->graph(2)->setPen(QPen(QColor(255,60,0)));
  ui->age_plot->legend->setVisible(true);

  ui->mutations_plot->addGraph();
  ui->mutations_plot->graph(0)->setName("Mutations (total)");
  ui->mutations_plot->graph(0)->setPen(QPen(QColor(0,100,255)));
  ui->mutations_plot->legend->setVisible(true);

  ui->progressBar->setRange(0, DEFAULT_N_EPOCHS-1);
  ui->progressBar->setFormat("-/%m");
  ui->progressBar->setValue(-1);
}

MainWindow::~MainWindow()
{
  drawingThread.quit();
  while(!drawingThread.wait());

  simulationThread.requestInterruption();
  simulationThread.quit();
  while(!simulationThread.wait());

  delete ui;
}

void MainWindow::epochUpdate(int e)
{
  ui->progressBar->setFormat("%v/%m");
  ui->progressBar->setValue(e);
}

void MainWindow::updateAges(QVector<double> *max_ages, QVector<double> *min_ages, QVector<double> *avg_ages)
{
  QVector<double> x = {};
  for(int i=0;i<max_ages->size();++i)
    x.push_back(i);

  ui->age_plot->graph(0)->setData(x, *min_ages);
  ui->age_plot->graph(0)->rescaleAxes(true);
  ui->age_plot->graph(1)->setData(x, *avg_ages);
  ui->age_plot->graph(1)->rescaleAxes(true);
  ui->age_plot->graph(2)->setData(x, *max_ages);
  ui->age_plot->graph(2)->rescaleAxes(true);
  ui->age_plot->replot();
}

void MainWindow::updateMutations(QVector<double> *mutations_incremental)
{
  QVector<double> x = {};
  for(int i=0;i<mutations_incremental->size();++i)
    x.push_back(i);

  ui->mutations_plot->graph(0)->setData(x, *mutations_incremental);
  ui->mutations_plot->graph(0)->rescaleAxes(true);
  ui->mutations_plot->replot();
}

void MainWindow::updateImage(QPixmap &pixmap)
{
  ui->label->setPixmap(pixmap);
  ui->label->show();
}

void MainWindow::on_btn_runsim_clicked()
{
  emit startSimulationInThread();
}


void MainWindow::on_btn_saveimg_clicked()
{
  rend->saveImageToFile();
}

