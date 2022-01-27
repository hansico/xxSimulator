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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include "creatureBase.h"
#include "imagerenderer.h"
#include "simumain.h"
#include "simumap.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
  Q_OBJECT
  QThread simulationThread;
  QThread drawingThread;

public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();

signals:
  void startSimulationInThread(int n_epochs = DEFAULT_N_EPOCHS, int epoch_length = DEFAULT_LEN_OF_EPOCH);

public slots:
  void epochUpdate(int e);
  void updateAges(QVector<double> *max_ages, QVector<double> *min_ages, QVector<double> *avg_ages);
  void updateMutations(QVector<double> *mutations_incremental);
  void updateImage(QPixmap &pixmap);

private slots:
  void on_btn_runsim_clicked();
  void on_btn_saveimg_clicked();

private:
  Ui::MainWindow *ui;
  SimuMain *sim;
  ImageRenderer *rend;
};
#endif // MAINWINDOW_H
