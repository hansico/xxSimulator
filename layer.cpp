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
#include "layer.h"

Layer::Layer()
{
    _nodes = {};
}

Layer::~Layer()
{
    
}

bool Layer::addNode(Gene* node)
{
  _nodes.push_back(node);
  return true;
}

bool Layer::removeNode()
{
  // TODO Not implemented
  return false;
}

void Layer::calculateValues() const
{
  for(auto const &node: _nodes){
    node->evaluate();
  }
}

std::vector<Gene *> Layer::nodes()
{
  return _nodes;
}

int Layer::size() const
{
  return _nodes.size();
}
