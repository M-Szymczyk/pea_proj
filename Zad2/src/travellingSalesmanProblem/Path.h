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

    Path(Graph *graph, int startNode);

    virtual ~Path();

    void display() const;

    int calculateWeight();

    void shuffle(std::mt19937 gen);
};

#include <list>
#include <map>
#include <unordered_map>
#include "../graphRepresentation/Graph.h"

#endif //PEAZAD2_PATH_H
