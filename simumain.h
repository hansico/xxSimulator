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
#ifndef SIMUMAIN_H
#define SIMUMAIN_H

#include <vector>
#include <QVector>
#include <simumap.h>
#include "rulebook.h"
#include "creatureBase.h"
#include "imagerenderer.h"
#include "plantbase.h"

class SimuMain : public QObject
{
  Q_OBJECT
public:
  SimuMain(int mapsize_x, int mapsize_y);
  ~SimuMain();
  void createCreatures(int n);
  void createFood(int n);

public slots:
  void runSim(int n_epochs = DEFAULT_N_EPOCHS, int epoch_len = DEFAULT_LEN_OF_EPOCH);

signals:
  void epochCompleted(int e);
  void agesUpdated(QVector<double> *maxs, QVector<double> *mins, QVector<double> *avgs);
  void mutationsUpdated(QVector<double> *mutations);
  void drawACreature(int location, int facing,
                    const std::vector<int> &sightRange={},
                    const std::vector<int> &hearingRange={});
  void drawPlant(int location, int color=2);
  void drawClean();
  void turnReset();

private:
  SimuMap *simumap;  
  std::vector<CreatureBase> creatures;
  std::vector<PlantBase> plants;
  void resetBetweenTurns();
  void resetBetweenEpochs();
  void placeCreatures();  
  void naturalSelection(int n);
  std::uniform_real_distribution<> rng;

  void collectStatistics();

  // VARIABLES FOR STATISTICS
  int plants_eaten_per_epoch = 0;
  int plants_eaten_total = 0;
  int creatures_deceased_per_epoch = 0;
  int creatures_deceased_total= 0;
  int mutations_total = 0;
  QVector<double> mutations_incremental = {};
  QVector<double> max_ages = {};
  QVector<double> min_ages = {};
  QVector<double> avg_ages = {};
};

#endif // SIMUMAIN_H
