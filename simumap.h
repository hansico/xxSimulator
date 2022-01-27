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
#ifndef SIMUMAP_H
#define SIMUMAP_H

#include <memory>
#include <vector>
#include <unordered_map>
#include "plantbase.h"

class CreatureBase;

class SimuMap
{
public:
  SimuMap(int n_columns, int n_rows);
  ~SimuMap();
  std::vector<int> &getNeighbors(int ind, std::vector<int> &arr);
  std::vector<int> &getUnoccupiedNeighbors(int ind, std::vector<int> &arr);
  std::vector<int> &neighborsInRadius(int ind, int radius, std::vector<int> &arr);
  int getDirectionOfNeighbor(int from, int to);
  int getIndexFromBearing(int from, int bearing);
  void reset();

  virtual int distance(int from, int to);
  virtual int distance(int from, int to, std::vector<int> &arr);

  int size() const;
  int randomUnoccupiedTile() const;
  void move(int from, int to);
  bool isOccupied(int ind) const;
  void addCreatureToMap(int ind, CreatureBase* creature);
  void addFoodToMap(int ind, PlantBase* food);
  void removeCreature(int ind);
  void removeFood(int ind);

  virtual bool findFood(int index);
  virtual bool findFood(std::vector<int> indices, std::vector<int> &foodInds);
  bool findCreatures(std::vector<int> indices, std::vector<int> &creatureInds);
  enum bearings {EAST, N_EAST, N_WEST, WEST, S_WEST, S_EAST};

private:
  std::unordered_map<int, CreatureBase* > creatures;
  std::unordered_map<int, PlantBase*> foods;
  int long_r;
  int short_r;
  int _rows;
  int _size;
};

#endif // SIMUMAP_H
