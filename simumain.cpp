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
#include "simumain.h"
#include <stdlib.h>
#include <QThread>

SimuMain::SimuMain(int map_columns, int map_rows)
{
  simumap = new SimuMap(map_columns, map_rows);
}

SimuMain::~SimuMain()
{
  delete simumap;
}

void SimuMain::createCreatures(int n)
{
  for(int i=0;i<n;++i){
    CreatureBase creature(simumap->randomUnoccupiedTile(), simumap, i);
    creatures.push_back(creature);
  }
  std::cout << creatures.size() << "/" << n << " creatures were created.\n";
}

void SimuMain::createFood(int n)
{
  for(int i=0;i<n;++i){
    // TODO MAP GIVES THE RANDOM LOC directly
    int place = rand() % simumap->size();
    if(!simumap->isOccupied(place) && !simumap->findFood(place)){      
      PlantBase food = PlantBase(place);
      plants.push_back(food);
      simumap->addFoodToMap(place, &food);

      emit drawPlant(place, 6);
    }
  }
  std::cout << plants.size() << "/" << n << " food was planted.\n";
}

void SimuMain::runSim(int n_epochs, int epoch_len)
{
  // TODO ADD A RESET BUTTON to clear said progress allowing start
  // with clean plate after already running it (at least) once
  if(creatures.empty())
    createCreatures(DEFAULT_N_CREATURES);

  for(int epoch=0;epoch<n_epochs;++epoch){
    resetBetweenEpochs();

    for(int r=0;r<epoch_len;++r){
      if(QThread::currentThread()->isInterruptionRequested()){
        return;
      }

      if(creatures_deceased_per_epoch==creatures.size()){
        std::cout << "ALL CREATURES HAVE DECEASED :(.\n";
        break;
      }
      resetBetweenTurns();

      for(auto const &f: plants){
        emit drawPlant(f.loc(), 6);
      }

      for(auto &c: creatures)
      {
        if(!c.isAlive())
          continue;        

        bool alive = c.taketurn();

        if(!alive){
          simumap->removeCreature(c.loc());
          creatures_deceased_per_epoch += 1;
          continue;
        }

        emit drawACreature(c.loc(), c.heading());
      }      
      emit turnReset();
      emit drawClean();
    }
    // TODO EPOCH STATISTICS
    mutations_incremental.push_back(mutations_total);
    collectStatistics();
    // SELECTION
    naturalSelection(DEFAULT_N_NATURAL_SELECTION);

    emit epochCompleted(epoch);
    emit agesUpdated(&max_ages, &min_ages, &avg_ages);
    emit mutationsUpdated(&mutations_incremental);
  }
}

void SimuMain::resetBetweenTurns()
{
  for(auto &creature: creatures){
    creature.resetBetweenTurns();
  }
}

void SimuMain::resetBetweenEpochs()
{
  simumap->reset();
  plants.clear();
  plants_eaten_per_epoch = 0;
  creatures_deceased_per_epoch = 0;
  std::vector<CreatureBase> mutants;
  std::random_device rd;
  std::mt19937 gen(rd());

  // Expected value of creatures will be ~DEFAULT_N_CREATURES after mutations
  int reprod = (DEFAULT_N_CREATURES-creatures.size()-creatures.size()*MUTATION_CHANCE)/(1+MUTATION_CHANCE);
  if(reprod > 0){
    std::uniform_int_distribution<> dist(0, creatures.size()-1);
    for(int i=0;i<reprod;++i){
      CreatureBase creaturecopy = creatures.at(dist(gen));
      creatures.push_back(creaturecopy);
    }
  }

  for(auto &c: creatures){
    c.resetBetweenEpochs();
    if(rng(gen)<=MUTATION_CHANCE){
      // TODO Change id of mutants
      CreatureBase mutant = c;

      // TODO stat types
      int e_type = mutant.evolve(rng(gen));
      mutants.push_back(mutant);
      // TODO Make sure mutation "goes" through ie. weight alter fails,
      // when there are no connections.
      ++mutations_total;
    }
  }  
  for(auto &mutant: mutants)
    creatures.push_back(mutant);

  createFood(DEFAULT_N_PLANTS);

  placeCreatures();

}

void SimuMain::placeCreatures()
{
  for(auto &c: creatures){
    int location = simumap->randomUnoccupiedTile();

    // TODO should throw not break
    if(location<0)
      break;
    c.setLocation(location);
    simumap->addCreatureToMap(location, &c);

    emit drawACreature(c.loc(), c.heading());
  }
}

void SimuMain::naturalSelection(int n)
{  
  std::sort(std::begin(creatures),std::end(creatures),
            [](const CreatureBase &c1, const CreatureBase &c2) {
    return c1.age() > c2.age();
  });
  creatures.erase(creatures.begin()+n, creatures.end());
}

void SimuMain::collectStatistics()
{
   // AGES
  int max_age = 0;
  int min_age = std::numeric_limits<int>::max();
  int avg_age = 0;

  for(auto const &c: creatures){
    avg_age += c.age();

    if(c.age() > max_age)
      max_age = c.age();

    if(c.age() < min_age)
      min_age = c.age();
  }

  avg_age = avg_age/creatures.size();

  max_ages.push_back(max_age);
  min_ages.push_back(min_age);
  avg_ages.push_back(avg_age);
}
