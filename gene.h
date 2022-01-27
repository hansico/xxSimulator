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
#ifndef GENE_H
#define GENE_H

#include "geneconn.h"
#include <vector>

class Gene
{
public:
  Gene(int id, int activation = Sigmoid, int type = Hidden);
  Gene(const Gene& gene);
  Gene(const Gene&& gene);
  Gene& operator=(Gene gene);

  enum Activations {None, Sigmoid};
  enum Types {Input, Output, Hidden};

  float value();
  int type();
  int id() const;
  int layer = -1;
  bool removeInput(GeneConn* conn);
  bool removeOutput(GeneConn* conn);

  void setValue(float newvalue);

  float evaluate();
  std::vector<GeneConn* > inputs;   // TODO private
  std::vector<GeneConn* > outputs;  // TODO private

private:
  int _id;
  int _activation;
  int _type;
  float _v;
};

#endif // GENE_H
