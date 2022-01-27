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
#include "simumap.h"
#include <iostream>

SimuMap::SimuMap(int n_columns, int n_rows)
{
  long_r  = n_columns;
  short_r = n_columns-1;
  _rows = n_rows;
  _size = n_columns*n_rows - int(n_rows/2);
}

SimuMap::~SimuMap(){

}

std::vector<int> &SimuMap::getNeighbors(int ind, std::vector<int> &arr){
  // TODO Could use some optimization?
  // cornerNW
  if(ind==0){
    arr.push_back(1);
    arr.push_back(ind+long_r);
    return arr;
  }
  // cornerNE
  if(ind==long_r-1){
    arr.push_back(ind-1);
    arr.push_back(ind+short_r);
    return arr;
  }
  // cornerSE
  if(ind==size()-1){
    arr.push_back(ind-1);
    arr.push_back(ind-long_r);
    if(_rows%2==0)
      arr.push_back(ind-short_r);
    return arr;
  }
  // cornerSW
  if(_rows%2==0){
    if(ind == (size()-short_r)){
      arr.push_back(ind+1);
      arr.push_back(ind-long_r+1);
      arr.push_back(ind-long_r);
      return arr;
    }
  }
  if(_rows%2!=0){
    if(ind == (size()-long_r)){
      arr.push_back(ind+1);
      arr.push_back(ind-short_r);
      return arr;
    }
  }
  // West edge
  if(!(ind%(long_r+short_r)==0     || ind%(long_r+short_r)==long_r))
    arr.push_back(ind-1);
  // East edge
  if(!((ind+1)%(long_r+short_r)==0 || ind%(long_r+short_r)==short_r)){
    arr.push_back(ind+1);
  }
  int tmp = 0;
  tmp = ind-long_r;

  if(tmp>0){
    if(ind%(long_r+short_r)!=0)
      arr.push_back(tmp);
    if(ind%(long_r+short_r)!=short_r)
      arr.push_back(ind-short_r);
  }

  tmp = ind+short_r;
  if(tmp<size()){
    if(ind%(long_r+short_r)!=0)
      arr.push_back(ind+short_r);
    if(ind%(long_r+short_r)!=short_r)
      arr.push_back(ind+long_r);
  }
  return arr;
}

std::vector<int> &SimuMap::getUnoccupiedNeighbors(int ind, std::vector<int> &arr)
{
  std::vector<int> tmp;
  getNeighbors(ind,tmp);
  for(const auto c: tmp){
    if(!isOccupied(c))
      arr.push_back(c);
  }
  return arr;
}

std::vector<int> &SimuMap::neighborsInRadius(int ind, int radius, std::vector<int> &arr)
{
  for(int i=0;i<6;++i){
    int dir_one = ind;
    int dir_two = ind;
    int dir_final= ind;
    int offset = 0;

    for(int r=0;r<radius;++r){
      dir_one = getIndexFromBearing(dir_one,i);
      dir_two = getIndexFromBearing(dir_two,(i+1)%6);
      if(dir_two < 0)
        break;
      if(dir_one < 0){
        offset=1;
        break;
      }
    }

    int helpi=i+2+3*offset;

    for(int r=0;r<radius;++r){
      dir_final = getIndexFromBearing(dir_final,(i+offset)%6);
      if(dir_final<0)
        break;
      if(offset!=1)
        arr.push_back(dir_final);
      int sec = dir_final;
      for(int s=0-offset;s<r;++s){
        sec=getIndexFromBearing(sec,(helpi)%6);

        if(sec<0)
          break;
        arr.push_back(sec);
      }
    }
  }
  return arr;
}

int SimuMap::getDirectionOfNeighbor(int from, int to)
{
  // TODO Edge cases? or trust that inputs are truly neighbors?
  int diff = to-from;  
  if(diff==1)
    return EAST;
  if(diff==-1)
    return WEST;
  if(diff==-short_r)
    return N_EAST;
  if(diff==-long_r)
    return N_WEST;
  if(diff==short_r)
    return S_WEST;
  if(diff==long_r)
    return S_EAST;

  return -1;
}

int SimuMap::getIndexFromBearing(int from, int bearing)
{
  if(bearing==EAST && !((from+1)%(long_r+short_r)==0 || from%(long_r+short_r)==short_r))
    return from+1;
  if(bearing==WEST && !(from%(long_r+short_r)==0 || from%(long_r+short_r)==long_r))
    return from-1;
  if(bearing==N_EAST && from>=long_r && (from+1)%(long_r+short_r)!=long_r)
    return from-short_r;
  if(bearing==N_WEST && from>=long_r && (from+1)%(long_r+short_r)!=1)
    return from-long_r;

  int shortorlong = _rows%2; // last row of map
  if(shortorlong==1){shortorlong=long_r;}else{shortorlong=short_r;}

  if(bearing==S_WEST && from<=_size-shortorlong && (from+1)%(long_r+short_r)!=1)
    return from+short_r;
  if(bearing==S_EAST && from<=_size-shortorlong && (from+1)%(long_r+short_r)!=long_r)
    return from+long_r;

  return -1;
}

void SimuMap::reset()
{
  creatures.clear();
  foods.clear();
}

int SimuMap::distance(int from, int to)
{
  int doublerow = long_r+short_r;
  int from_row = from/doublerow;
  int from_mod = from % doublerow;

  int to_row = to/doublerow;
  int to_mod = to % doublerow;

  from_row = 2*from_row;
  if(from_mod>=long_r)
    from_row += 1;
  to_row = 2*to_row;
  if(to_mod>=long_r)
    to_row += 1;

  from_mod = from_mod % long_r;
  to_mod = to_mod % long_r;

  int EW_diff = abs(from_mod - to_mod);
  int rowdiff = abs(from_row-to_row);
  int EW_cover_by_rowdiff;

  // going WEST and SHORT row
  if(from_mod >= to_mod && from_row%2==1)
    EW_cover_by_rowdiff = rowdiff/2;
  // going EAST and LONG row
  else if(to_mod >= from_mod && from_row%2==0)
    EW_cover_by_rowdiff = rowdiff/2;
  else
    EW_cover_by_rowdiff = (rowdiff+1)/2;

  if(EW_cover_by_rowdiff >= EW_diff)
    return rowdiff;

  return rowdiff + abs(EW_diff - EW_cover_by_rowdiff);
}

int SimuMap::distance(int from, int to, std::vector<int> &arr)
{
  // TODO arr validation?
  if(arr.size()!=6)
    arr = {0,0,0,0,0,0};

  int EW_dir = EAST;
  int NS_offset = 1; // NORTH SOUTH OFFSET

  int doublerow = long_r+short_r;
  int from_row = from/doublerow;
  int from_mod = from % doublerow;

  int to_row = to/doublerow;
  int to_mod = to % doublerow;

  from_row = 2*from_row;
  if(from_mod>=long_r)
    from_row += 1;
  to_row = 2*to_row;
  if(to_mod>=long_r)
    to_row += 1;

  from_mod = from_mod % long_r;
  to_mod = to_mod % long_r;

  if(from_mod > to_mod){
    EW_dir = WEST;
    NS_offset = 5;
  }

  if(from_row < to_row)
    NS_offset = -NS_offset;

  int EW_diff = abs(from_mod-to_mod);
  int rowdiff = abs(from_row-to_row);
  int EW_cover_by_rowdiff;

  // going WEST and SHORT row
  if(EW_dir == WEST && from_row%2==1){
    EW_cover_by_rowdiff = rowdiff/2;
  // going EAST and LONG row
  }else if(EW_dir == EAST && from_row%2==0){
    EW_cover_by_rowdiff = rowdiff/2;
  }else{
    EW_cover_by_rowdiff = (rowdiff+1)/2;
  }

  if(EW_cover_by_rowdiff > EW_diff){
    arr.at((6+EW_dir+NS_offset)%6) += (rowdiff - (EW_cover_by_rowdiff - EW_diff));
    arr.at((12+EW_dir+2*NS_offset)%6) += EW_cover_by_rowdiff - EW_diff;
    return rowdiff;
  }

  arr.at((6+EW_dir+NS_offset)%6) += rowdiff;

  if(EW_cover_by_rowdiff == EW_diff){
    return rowdiff;
  }

  arr.at(EW_dir) = EW_diff - EW_cover_by_rowdiff;
  return rowdiff + abs(EW_diff - EW_cover_by_rowdiff);
}

int SimuMap::size() const
{
    return _size;
}

int SimuMap::randomUnoccupiedTile() const
{
  if(creatures.size() == _size)
    return -1;

  bool check = true;
  int ind;
  while(check){
    ind = rand() % _size;    
    check = isOccupied(ind);
  }
  return ind;
}

void SimuMap::move(int from, int to)
{
  CreatureBase* creature = creatures.at(from);
  creatures.erase(from);
  creatures.insert({to, creature});
}

bool SimuMap::isOccupied(int ind) const
{
  std::unordered_map<int,CreatureBase*>::const_iterator iter = creatures.find(ind);

  if ( iter == creatures.end() )
    return false;
  else
    return true;
}

void SimuMap::addCreatureToMap(int ind, CreatureBase* creature)
{
  // TODO
  creatures.insert({ind, creature});
}

void SimuMap::addFoodToMap(int ind, PlantBase* food)
{
  // TODO
  foods.insert({ind, food});
}

void SimuMap::removeCreature(int ind)
{
  creatures.erase(ind);
}

void SimuMap::removeFood(int ind)
{
  foods.erase(ind);
}

bool SimuMap::findFood(int index)
{
  std::unordered_map<int, PlantBase*>::const_iterator iter = foods.find(index);
  if(iter != foods.end()){
    return true;
  }
  return false;
}

bool SimuMap::findFood(std::vector<int> indices, std::vector<int> &foodInds)
{
  bool foundflag = false;
  for(const int &ind: indices){
    std::unordered_map<int, PlantBase*>::const_iterator iter = foods.find(ind);
    if(iter != foods.end()){
      foodInds.push_back(ind);
      foundflag = true;
    }
  }
  return foundflag;
}

bool SimuMap::findCreatures(std::vector<int> indices, std::vector<int> &creatureInds)
{
  bool foundflag = false;
  for(const int &ind: indices){
    std::unordered_map<int, CreatureBase*>::const_iterator iter = creatures.find(ind);
    if(iter != creatures.end()){
      creatureInds.push_back(ind);
      foundflag = true;
    }
  }
  return foundflag;
}
