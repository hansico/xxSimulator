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
#ifndef RULEBOOK_H
#define RULEBOOK_H

static const float MOVEMENT_ENERGY_COST = 1.0;
static const float ENERGY_FROM_PLANTS = 60.0;
static const float ENERGY_LOSS_FOR_EATING_DIRT = 3.5;
static const float PASSIVE_ENERGY_LOSS = 0.21;
static const float EXHAUSTION_COST = 0.8; // Trying to move, when no movement left

static const int DEFAULT_N_EPOCHS = 99;
static const int DEFAULT_LEN_OF_EPOCH = 10000;

static const double MUTATION_CHANCE = 0.05;
static const double MUTATION_ALTER_WEIGHT_CHANCE = 1.0/3.0;
static const double MUTATION_ADD_CONNECTION_CHANCE = 1.0/3.0;
static const double MUTATION_ADD_NODE_CHANCE = 1.0/3.0;

static const float MAX_WEIGHT_ALTER = 3.0;
static const int DEFAULT_N_CREATURES = 250;
static const int DEFAULT_N_NATURAL_SELECTION = 28;

static const int DEFAULT_N_PLANTS = 480;

static const int DEFAULT_MAPSIZE_COLUMNS = 48;
static const int DEFAULT_MAPSIZE_ROWS = 50;

#endif // RULEBOOK_H
