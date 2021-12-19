//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#ifndef PEAZAD2_PATH_H
#define PEAZAD2_PATH_H

class Path {
public:
    int weight;
    std::vector<int> path;
    Graph *graph;
    Path(int currentCost, std::vector<int> currentPath, Graph *g);

    Path(std::vector<int> path, Graph *g);

    virtual ~Path();

    void display() const;

    int calculateWeight();

    int countDifrenceIfSwapNeighbours(const std::vector<unsigned int> &path, int i, int j);
};

#include <list>
#include <map>
#include <unordered_map>
#include "../graphRepresentation/Graph.h"

#endif //PEAZAD2_PATH_H
