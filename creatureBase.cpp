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
#include "creatureBase.h"

CreatureBase::CreatureBase(int location, SimuMap *map, int _id, int bearing):
  brain(0, 0)
{
  locInd = location;
  simumap = map;
  id = _id;
  _age = 0;
  brain = Genome(9,8);  // TODO magic 9 and 8
  if(bearing<0)
    facing = rand() % simumap->bearings::S_EAST;
  else
    facing = bearing;
}

CreatureBase::~CreatureBase()
{

}

CreatureBase::CreatureBase(const CreatureBase &other):
  brain( other.brain.inputLayer.size(), other.brain.outputLayer.size())
{  
  movement = other.movement;
  initiative = other.initiative;
  perception = other.perception;
  sight = other.sight;
  hearing = other.hearing;
  energyMax = other.energyMax;
  simumap = other.simumap;
  _age = 0;
  id = other.id; // TODO unique
  locInd = other.locInd;
  facing = other.facing;
  alive = true;
  movementLeft = other.movementLeft;
  energy = other.energy;
}

bool CreatureBase::taketurn()
{
  // returns true if alive, false is deceased
  ++_age;
  bool turnIsDone = false;

  while(!turnIsDone){
    std::vector<float> inputs = calculateInputs();
    std::cout << "INPUTS: ";
    for(const auto &inp: inputs)
      std::cout << inp << ' ';
    std::cout << '\n';

    std::vector<float> preds;
    int action = brain.predict(inputs, preds);
    std::cout << "#" << id <<" CHOSE ACTION " << action << ".\n";
    turnIsDone = takeAction(action);
  }

  energy -= PASSIVE_ENERGY_LOSS;
  if(energy<=0)
    alive = false;
  return alive;
}

int CreatureBase::loc() const
{
  return locInd;
}

int CreatureBase::heading() const
{
  return facing;
}

int CreatureBase::age() const
{
  return _age;
}

void CreatureBase::resetBetweenTurns()
{
  movementLeft = movement;
}

void CreatureBase::resetBetweenEpochs()
{
  energy = energyMax;
  movementLeft = movement;
  _age = 0;
  alive = true;
}

bool CreatureBase::isAlive() const
{
    return alive;
}

int CreatureBase::evolve(double seed)
{
  if(seed<MUTATION_ALTER_WEIGHT_CHANCE){
    brain.alterWeight();
    std::cout << "## EVOLUTION >> WEIGHT ALTER.\n";
    return ALTER_WEIGHT;
  }

  if(seed<MUTATION_ADD_CONNECTION_CHANCE+MUTATION_ALTER_WEIGHT_CHANCE){
    brain.addConnection();
    std::cout << "## EVOLUTION >> NEW CONNECTION.\n";
    return ADD_CONNECTION;
  }

  brain.addNode();
  std::cout << "## EVOLUTION >> ADD NODE.\n";
  return ADD_NODE;
}

void CreatureBase::setLocation(int ind)
{
  locInd = ind;
}

std::vector<float> CreatureBase::calculateInputs()
{
  std::vector<float> rtn;
  float energyinp = (energy + energy)/energyMax - 1; // -1,1
  rtn.push_back(energyinp);

  // TODO USE SIGHT AND NOT HEARING FOR FOOD :D
  std::vector<int> hearingR={};
  hearingRange(hearingR);

  std::vector<float> detectFoodInput = {0.0, 0.0, 0.0, 0.0, 0.0, 0.0};
  std::vector<int> containsFood;
  if(simumap->findFood(hearingR,containsFood)){
    std::cout << "Indices ";
    for(const auto &i:containsFood){
      std::cout << i << " ";
      std::vector<int> dist_arr;
      int dist = simumap->distance(locInd,i,dist_arr);
      for(int d=0;d<detectFoodInput.size();++d){
        if(dist_arr.at(d)!=0){
          float tmp = 1.0/dist;
          std::cout << "DIST >> " << dist << " " << tmp << " " << dist_arr.at(d) << '\n';
          float val;
          if(dist!=dist_arr.at(d))
             val = tmp+tmp*tmp*1/(dist - dist_arr.at(d));
          else
            val = (tmp*( dist - dist_arr.at(d) )+1)/dist;
          // TODO clamp?
          detectFoodInput.at(d) += val;
        }
      }
    }
    std::cout << "contain food.\n";
  }
  for(const auto &df: detectFoodInput)
    rtn.push_back(df);
  // TODO On top of food, better way to indicate?
  if(simumap->findFood(locInd))
    rtn.push_back(1.0);
  else
    rtn.push_back(-1.0);

  float movementLeftinp = (movementLeft + movementLeft)/movement - 1; // -1,1
  rtn.push_back(movementLeftinp);

  return rtn;
}

bool CreatureBase::takeAction(int action)
{
  switch(action)
  {
    case MOVE_EAST:
      return(!moveToDirection(simumap->EAST));
    case MOVE_N_EAST:
      return(!moveToDirection(simumap->N_EAST));
    case MOVE_N_WEST:
      return(!moveToDirection(simumap->N_WEST));
    case MOVE_WEST:
      return(!moveToDirection(simumap->WEST));
    case MOVE_S_WEST:
      return(!moveToDirection(simumap->S_WEST));
    case MOVE_S_EAST:
      return(!moveToDirection(simumap->S_EAST));
    case EAT:
      eat();
      return true;
    case STAY:
      return true;
  default :
    // TODO
    std::cout << "THIS ACTION IS NOT IMPLEMENTED!\n";
    return true;
  }
}

bool CreatureBase::moveToDirection(int direction)
{
  if(movementLeft<=0){
    std::cout << "#"<< id << " tried to move but has no movement left.\n";
    energy -= EXHAUSTION_COST;
    return false;
  }
  if(energy < MOVEMENT_ENERGY_COST){
     std::cout << "#"<< id << " tried to move but does not have enough energy left.\n";
     return false;
  }

  int newindex = simumap->getIndexFromBearing(locInd, direction);

  if(newindex < 0)
    return false;
  // TODO HERBIVORE IS ALLOWED TO MOVE TO BE EATEN BY CARNIVORE
  if(simumap->isOccupied(newindex)==true)
    return false;

  moveToIndex(newindex);
  movementLeft -= 1;
  energy -= MOVEMENT_ENERGY_COST;
  return true;
}

std::vector<int> &CreatureBase::sightRange(std::vector<int> &arr) const
{
  int tmp = simumap->getIndexFromBearing(locInd,facing);
  if(tmp>=0){
    arr.push_back(tmp);
    arr.push_back(simumap->getIndexFromBearing(tmp,facing));
    arr.push_back(simumap->getIndexFromBearing(tmp,(facing+1)%6));
    arr.push_back(simumap->getIndexFromBearing(tmp,(facing-1)%6));
  }
  return arr;
}

std::vector<int> &CreatureBase::hearingRange(std::vector<int> &arr) const
{
  simumap->neighborsInRadius(locInd,hearing,arr);
  return arr;
}

void CreatureBase::moveToIndex(int newindex)
{
  simumap->move(locInd,newindex);
  int newfacing = simumap->getDirectionOfNeighbor(locInd,newindex);
  if(newfacing>=0)
    facing = newfacing;
  locInd = newindex;
}

void CreatureBase::eat()
{
  float energy_before = energy;

  if(!simumap->findFood(locInd)){
    energy -= ENERGY_LOSS_FOR_EATING_DIRT;
    std::cout << "#" << id << " ate dirt. Energy dropped " << energy_before << " -> " << energy << '\n';
    return;
  }

  energy = energy+ENERGY_FROM_PLANTS > energyMax ? energyMax : energy+ENERGY_FROM_PLANTS;
  std::cout << "#" << id << " ate a plant. Energy increased " << energy_before << " -> " << energy << '\n';
  simumap->removeFood(locInd);
}
