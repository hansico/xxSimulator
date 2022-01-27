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
#include "geneconn.h"
#include "gene.h"
#include "rulebook.h"

GeneConn::GeneConn(Gene* pfrom, Gene* pto)
{
  from = pfrom;
  to = pto;
  active = true;

  std::random_device rd;
  rng.seed(rd());

  std::normal_distribution<float> d{0.0, MAX_WEIGHT_ALTER};
  w = d(rng);
}

float GeneConn::value() const
{
  return w*from->value();
}

float GeneConn::weight() const
{
  return w;
}

void GeneConn::setWeight(float new_w)
{
  w = new_w;
}

void GeneConn::alterWeight()
{
  std::uniform_real_distribution<float> distrib(-MAX_WEIGHT_ALTER, MAX_WEIGHT_ALTER);
  w += distrib(rng);
}
