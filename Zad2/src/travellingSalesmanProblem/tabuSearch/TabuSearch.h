//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#ifndef PEAZAD2_TABUSEARCH_H
#define PEAZAD2_TABUSEARCH_H
#include "vector"
#include "../../graphRepresentation/Graph.h"


class TabuSearch {
    int weight;
    std::vector<int> path;
public:
    TabuSearch(Graph *graph, int startNode);
};


#endif //PEAZAD2_TABUSEARCH_H
