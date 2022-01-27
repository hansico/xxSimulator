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
#include "gene.h"
#include <iostream>

Gene::Gene(int id, int activation, int type)
{
  _id = id;
  _activation = activation;
  _type = type;
  inputs = {};
  outputs = {};
}

Gene::Gene(const Gene& gene)
{
  _id = gene._id;
  _activation = gene._activation;
  _type = gene._type;
  layer = -1;
}

Gene::Gene(const Gene&& gene)
{
  _id = gene._id;
  _activation = gene._activation;
  _type = gene._type;
  layer = -1;
}

Gene& Gene::operator=(Gene gene)
{
  std::swap(*this, gene);
  this->inputs = {};
  this->outputs = {};
  return *this;
}

float Gene::value()
{
  return _v;
}

int Gene::type()
{
  return _type;
}

int Gene::id() const
{
  return _id;
}

bool Gene::removeInput(GeneConn *conn)
{
  for(auto it = inputs.begin(); it != inputs.end();++it){
    if(*it==conn){
      inputs.erase(it);
      return true;
    }
  }
  return false;
}

bool Gene::removeOutput(GeneConn *conn)
{
  int iter = 0;
  for(auto it = outputs.begin(); it != outputs.end(); ++it){
    if(*it==conn){
      outputs.erase(it);
      return true;
    }
  }
  return false;
}

void Gene::setValue(float newvalue)
{
  // TODO Only allow type:input to be set?
  _v = newvalue;
}

float Gene::evaluate()
{
  _v = 0;
  for(auto const &conn: inputs)
    _v += conn->value();
  // TODO ADD ACTIVATION
  return _v;
}
