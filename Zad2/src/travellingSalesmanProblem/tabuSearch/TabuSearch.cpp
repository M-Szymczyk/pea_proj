//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#include <algorithm>
#include "TabuSearch.h"

TabuSearch::TabuSearch(Graph *graph, int startNode) {
    weight=0;
    path.push_back(startNode);
    for (int i = 0; i < graph->getNumberOfNodes(); i++) {
        if (i != startNode)
            path.push_back(i);
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 1000);
    for (int i = 0; i < dist(gen); i++)
        std::shuffle(path.begin() + 1, path.end(), gen);
}

