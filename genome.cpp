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
#include "genome.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <limits>

Genome::Genome(int inputsize, int outputsize)
{
  for(int i=0;i<inputsize;++i){
    Gene gene = Gene(i, Gene::Activations::None, Gene::Types::Input);
    inputLayer.push_back(gene);    
  }

  for(int i=0;i<outputsize;++i){
    Gene gene = Gene(i+inputsize, Gene::Activations::None, Gene::Types::Output);
    outputLayer.push_back(gene);
  }
  std::random_device rd;
  std::mt19937 rnd_gen(rd());
  layers = {};
}

Genome::Genome(const Genome& genome)
{
  inputLayer = genome.inputLayer;
  outputLayer = genome.outputLayer;
  indexConnections = genome.indexConnections;

  genes.reserve(genome.genes.size());
  for(const auto& g: genome.genes)
    genes.push_back(std::make_unique<Gene>(*g));

  connections.reserve(genome.connections.size());
  for(const auto& c: genome.connections)
    connections.push_back(std::make_unique<GeneConn>(*c));

  reconstruct();
  recalculateLayers();
}

Genome::Genome(Genome&& other)
{
  swap(*this, other);
}

Genome& Genome::operator=(Genome other)
{
  swap(*this, other);
  return *this;
}

void Genome::swap(Genome &lhs, Genome &rhs)
{
  std::swap(lhs.inputLayer, rhs.inputLayer);
  std::swap(lhs.outputLayer, rhs.outputLayer);
  std::swap(lhs.indexConnections, rhs.indexConnections);

  for(const auto& g: rhs.genes)
    lhs.genes.push_back(std::make_unique<Gene>(*g));
  for(const auto& c: rhs.connections)
    lhs.connections.push_back(std::make_unique<GeneConn>(*c));

  lhs.reconstruct();
  lhs.recalculateLayers();
}

Genome::~Genome()
{

}

int Genome::predict(std::vector<float> input)
{
  std::vector<float>preds;
  return predict(input, preds);
}

int Genome::predict(std::vector<float> input, std::vector<float> &preds)
{ 
  if(input.size()!=inputLayer.size()){
    // TODO THROW
    std::cout << "Given input and input layer are non-matching size. " << input.size() << " " << inputLayer.size() << "\n";
    return -1;
  }
  int n = -1;
  for(Gene &inp: inputLayer)
    inp.setValue(input.at(++n));

  for(auto const &layer: layers){
    layer.calculateValues();
  }

  std::vector<int> maxIndices = findGreatestIndices(preds);

  std::cout << "OUTPUT VALUES: ";
  for(auto const &v: preds)
    std::cout << v << ' ';
  std::cout << '\n';

  if(maxIndices.size()==1)
    return maxIndices.at(0);

  std::uniform_int_distribution<int> distrib(0,maxIndices.size()-1);
  int rnd_ind = distrib(rnd_gen);
  return maxIndices.at(rnd_ind);
}

GeneConn* Genome::addConnection()
{
  std::pair<Gene*, Gene* > fromto;
  if(!connectableGenes(fromto)){
    // TODO
    std::cout << "Connection was not established this time.\n";
    return nullptr;
  }
  GeneConn* conn = addConnection(fromto.first, fromto.second);
  return conn;
}

GeneConn* Genome::addConnection(Gene* from, Gene* to)
{
  // TODO CHECK TO AND FROM FOR ILLEGALS
  std::unique_ptr<GeneConn> conn = std::make_unique<GeneConn>(from, to);
  connections.emplace_back(std::move(conn));
  GeneConn* addr = connections.back().get();
  from->outputs.push_back(addr);
  to->inputs.push_back(addr);

  indexConnections.push_back({from->id(), static_cast<int>(connections.size()-1), to->id()});

  recalculateLayers();
  return addr;
}

Gene* Genome::addNode()
{
  // TODO possibly change to something better
  if(connections.empty())
    return nullptr;

  std::uniform_int_distribution<int> distribution_c(0, connections.size()-1);
  int randomN = distribution_c(rnd_gen);

  Gene* newgene = addNode(connections.at(randomN).get());
  return newgene;
}

Gene* Genome::addNode(GeneConn* conn)
{
  // TODO in NEAT connections are disabled not moved
  Gene* fromOrig = conn->from;
  fromOrig->removeOutput(conn);
  int ind = static_cast<int>(genes.size()+inputLayer.size()+outputLayer.size());
  std::unique_ptr<Gene> uptr_newgene = std::make_unique<Gene>(ind);

  genes.emplace_back(std::move(uptr_newgene));
  Gene* newgene = genes.back().get();

  newgene->outputs.push_back(conn);
  conn->from = newgene;

  for(auto &indConn: indexConnections){
    if(indConn.from == fromOrig->id() && indConn.to == conn->to->id()){
      indConn.from = newgene->id();
      break;
    }
  }

  GeneConn* newconn = addConnection(fromOrig, newgene);

  recalculateLayers();  
  return newgene;
}

void Genome::alterWeight()
{
  if(connections.empty())
    return;

  std::uniform_int_distribution<int> distrib(0, connections.size()-1);
  GeneConn* conn = connections.at(distrib(rnd_gen)).get();
  alterWeight(*conn);
}

void Genome::alterWeight(GeneConn& conn)
{
  conn.alterWeight();
}

void Genome::recalculateLayers()
{
  layers.clear();

  for(auto &g: genes)
    g->layer = -9;

  int n=0;
  for(auto &g: inputLayer){
    n = std::max(n, calculateDepth(g, -1)); // -1 since inputLayer is not meant to increase depth
  }

  for(int i=0; i<n;++i)
    layers.emplace_back(Layer());

  for(auto const &g: genes){
    layers.at(g->layer).addNode(g.get());
  }
}

int Genome::calculateDepth(Gene &gene, int c_depth)
{
  if(gene.layer < c_depth) // indexing starts from 0
    gene.layer = c_depth;

  int max_depth = c_depth;
  ++c_depth;

  for(auto const &gout: gene.outputs){    
    max_depth = std::max(calculateDepth(*gout->to, c_depth), max_depth);
  }
  return max_depth;
}

void Genome::printConnections()
{ std::cout << "INPUTS:";
  for(const auto& put: inputLayer){
    std::cout << &put << " ";
  }
  std::cout << '\n';
  std::cout << "OUTPUTS:";
  for(const auto& put: outputLayer){
    std::cout << &put << " ";
  }
  std::cout << '\n';

  for(const auto &conn: connections){
    std::cout << "Connection FROM "<< conn->from << " TO " << conn->to << " VIA " << &conn <<'\n';
  }

}

void Genome::printStructure()
{
  std::cout << "INPUT SIZE: " << inputLayer.size() << '\n';
  int n=0;
  for(auto const &layer: layers)
    std::cout << "LAYERx" << ++n << ": " << layer.size() << '\n';
  std::cout << "OUTPUT SIZE: " << outputLayer.size() << '\n';
}

void Genome::printOuts()
{
  std::cout << "\n\n\n";
  for(auto &inp: inputLayer)
    printouthelper(&inp);
  std::cout << "\n\n\n";
}

void Genome::printouthelper(Gene* gene) const
{
  std::cout << "FROM " << gene << '\n';
  for(auto con: gene->outputs){
    std::cout << "     " << con << " > " << con->to << '\n';
  }
  for(auto con: gene->outputs){
    printouthelper(con->to);
  }
}

int Genome::n_hiddenlayers() const
{
  return layers.size();
}

void Genome::reconstruct()
{
  for(const auto& ic: indexConnections){
    Gene* f = nullptr;
    Gene* t = nullptr;

    if(ic.from<inputLayer.size())
      f = &inputLayer.at(ic.from);
    else if(ic.from<inputLayer.size()+outputLayer.size()) // TODO from should not be on output layer
      f = &outputLayer.at(ic.from-inputLayer.size());
    else
      f = genes.at(ic.from-inputLayer.size()-outputLayer.size()).get();

    if(ic.to<inputLayer.size()) // TODO to should not be on input layer
      t = &inputLayer.at(ic.to);
    else if(ic.to<inputLayer.size()+outputLayer.size())
      t = &outputLayer.at(ic.to-inputLayer.size());
    else
      t = genes.at(ic.to-inputLayer.size()-outputLayer.size()).get();

    connections.at(ic.conn)->from = f;
    connections.at(ic.conn)->to = t;
    f->outputs.push_back(connections.at(ic.conn).get());
    t->inputs.push_back(connections.at(ic.conn).get());
  }

}

bool Genome::connectableGenes(std::pair<Gene*, Gene*> &fromto)
{
  std::uniform_int_distribution<int> distribution_f(1, genes.size()+inputLayer.size());
  int randomN = distribution_f(rnd_gen);
  Gene* random_g_f;
  if(randomN > inputLayer.size()){
    randomN -= inputLayer.size();
    random_g_f = genes.at(randomN-1).get();
  }else{
    random_g_f = &inputLayer.at(randomN-1);
  }

  std::uniform_int_distribution<int> distribution_t(1, genes.size()+outputLayer.size());
  randomN = distribution_t(rnd_gen);
  Gene* random_g_t;
  if(randomN > genes.size()){
    randomN -= genes.size();
    random_g_t = &outputLayer.at(randomN-1);
  }else{
    random_g_t = genes.at(randomN-1).get();
  }

  if(!validateConnection(random_g_f, random_g_t))
    return false;

  fromto = {random_g_f, random_g_t};
  return true;
}

bool Genome::checkForLoops(Gene& gene)
{
  std::queue<Gene > checklist;
  checklist.push(gene);
  while(!checklist.empty()){
    Gene currentg = checklist.front();
    checklist.pop();
    for(auto &conn: currentg.outputs){
      if(&gene==conn->to)
        return false;
      checklist.push(*conn->to);
    }
  }
  return true;
}

bool Genome::validateConnection(Gene* gene_from, Gene* gene_to)
{
  if(gene_from == gene_to)
   return false;

  for(auto const &conn: gene_from->outputs)
  {
    if(conn->to == gene_to)
      return false;
  }

  if(!checkForLoops(*gene_from))
    return false;

  return true;
}

std::vector<int> Genome::findGreatestIndices(std::vector<float> &preds)
{
  float currentMax = -std::numeric_limits<float>::max();
  std::vector<int> maxIndices = {};
  int ind = 0;

  for(Gene &out: outputLayer){
    float current = out.evaluate();
    preds.push_back(current);

    if(current>currentMax){      
      maxIndices.clear();
      maxIndices.push_back(ind);
      currentMax = current;
    }
    if(current==currentMax)
      maxIndices.push_back(ind);

    ++ind;
  }
  return maxIndices;
}
