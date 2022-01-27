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
#ifndef CREATUREBASE_H
#define CREATUREBASE_H

#include <math.h>
#include <iostream>
#include "simumap.h"
#include "genome.h"
#include "rulebook.h"

class CreatureBase
{
public:
  CreatureBase(int location, SimuMap *simumap, int id, int bearing=-1);
  ~CreatureBase();
  CreatureBase(const CreatureBase& other);

  std::vector<int> &sightRange(std::vector<int> &arr) const;
  std::vector<int> &hearingRange(std::vector<int> &arr) const;
  bool taketurn();
  int loc() const;
  int heading() const;
  int age() const;
  void resetBetweenTurns();
  void resetBetweenEpochs();
  bool isAlive() const;
  int evolve(double seed);
  void setLocation(int ind);
  enum EvolvementTypes{ALTER_WEIGHT, ADD_CONNECTION, ADD_NODE};
  Genome brain;

private:
  int id;
  int locInd;
  int facing;
  int _age;
  bool alive = true;

  SimuMap *simumap;
  std::vector<float> calculateInputs();
  enum Actions {MOVE_EAST, MOVE_N_EAST, MOVE_N_WEST, MOVE_WEST, MOVE_S_WEST, MOVE_S_EAST, EAT, STAY};
  bool takeAction(int action);


  bool moveToDirection(int direction);
  void moveToIndex(int newindex);
  void eat();

  // ATTRIBUTES
  int movement = 2;
  int movementLeft = movement;
  int initiative = 1;
  int perception = 1;
  int sight = 1;
  int hearing = 2;
  float energy = 100.0;
  float energyMax = 100.0;
};

#endif // CREATUREBASE_H
