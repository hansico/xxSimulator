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
#ifndef GENOME_H
#define GENOME_H

#include "gene.h"
#include "geneconn.h"
#include "layer.h"
#include <vector>
#include <random>
#include <memory>

struct IndexConn{
  int from;
  int conn;
  int to;
};

class Genome
{
public:
  Genome(int inputsize, int outputsize);
  Genome(const Genome& genome);
  Genome(Genome&& other);
  Genome& operator=(Genome other);

  void swap(Genome& lhs, Genome& rhs);
  ~Genome();

  virtual int predict(std::vector<float> input);
  virtual int predict(std::vector<float> input, std::vector<float> &preds);

  virtual GeneConn* addConnection();
  virtual GeneConn* addConnection(Gene* from, Gene* to);

  virtual Gene* addNode();
  virtual Gene* addNode(GeneConn* conn);

  virtual void alterWeight();
  virtual void alterWeight(GeneConn& conn);

  void printConnections();
  void printStructure();
  void printOuts();
  void printouthelper(Gene* gene) const;

  int n_hiddenlayers() const;

  std::vector<Gene> inputLayer;  // TODO change to layer, private
  std::vector<Gene> outputLayer; // TODO change to layer, private

private:
  std::vector<std::unique_ptr<Gene>> genes;
  std::vector<Layer> layers;
  std::vector<std::unique_ptr<GeneConn>> connections;
  std::vector<IndexConn> indexConnections;
  void reconstruct();

  bool connectableGenes(std::pair<Gene*, Gene*> &fromto);
  void recalculateLayers();
  int calculateDepth(Gene& gene, int c_depth);

  bool checkForLoops(Gene& gene);
  bool validateConnection(Gene* from, Gene* to);
  std::vector<int> findGreatestIndices(std::vector<float> &preds);
  std::default_random_engine rnd_gen;
};

#endif // GENOME_H
