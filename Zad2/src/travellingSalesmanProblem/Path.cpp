//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#include <algorithm>
#include <utility>
#include "held-Karp/HeldKarp.h"
#include "Path.h"

Path::~Path() {
    path.clear();
    weight = INT_MAX;
}

void Path::display() const {
    std::cout << "\nWaga wynosi: " << weight << "; Sciezka: ";
    for (auto v: path) {
        std::cout << v << ", ";
    }
}

Path::Path(int currentCost, std::vector<int> currentPath, Graph *g) : weight(currentCost), path(std::move(currentPath)), graph(g) {}

Path::Path(std::vector<int> path, Graph *g) : path(std::move(path)) ,graph(g){
    weight = calculateWeight();
}

int Path::calculateWeight(){
    int cost = 0;
    auto i = path.begin();
    while (i != (path.end() - 1)) {
        cost += graph->getEdge(i.operator*(),(i + 1).operator*());
        i.operator++();
    }
    return cost;
}

