//
// Created by Mikolaj Szymczyk on 05.10.2021.
//

#include "ExhaustiveSearch.h"
#include <vector>

void ExhaustiveSearch::generatePermutation(int a[], int size, int n) {

    if (size == 1)//then this permutation is ready
//    {
//        for (int i = 0; i < n; i++)
//            std::cout << a[i] << " ";
//        std::cout << std::endl;
//    }
        calculateWeight(a, n);

    for (int i = 0; i < size; i++) {
        generatePermutation(a, size - 1, n);
        if (size % 2 == 1)
            std::swap(a[0], a[size - 1]);
        else
            std::swap(a[i], a[size - 1]);
    }

}

ExhaustiveSearch::ExhaustiveSearch(Graph *g, int sNode) {
    startNode = sNode;
    graph = g;
    path = std::vector<int>();
    weight = INT_MAX;
    //prepare list of nodes (list of nodes without starting node
    auto sPath = new int[g->getNumberOfNodes() - 1];
    for (int i = 0; i < g->getNumberOfNodes() - 1; i++)
        sPath[i] = i < startNode ? i : i + 1;

    generatePermutation(sPath, g->getNumberOfNodes() - 1, g->getNumberOfNodes() - 1);
}

void ExhaustiveSearch::calculateWeight(int *a, int n) {

    int w = 0, t;
    //add weight of edge from start node to first node of permutation and from last node to start node
    w += graph->getEdge(startNode, a[0]);
    w += graph->getEdge(a[n - 1], startNode);
    //add weight of rest of edges
    for (int i = 0; i < n - 1; i++) {
        t = graph->getEdge(a[i], a[i + 1]);
        if (t == 0) {//check if edge exist
            return;
        }
        w += t;
        if (w > weight) {//check if current circuit weight is smaller than teh weight of the best circuit
            return;
        }
    }
    if (w < weight)//change circuit if is better
        setNewBestPath(a, w);
}

void ExhaustiveSearch::setNewBestPath(const int *nPath, int nWeight) {
    path.clear();
    for (int i = 0; i < graph->getNumberOfNodes() - 1; i++)
        path.push_back(nPath[i]);
    weight = nWeight;
}

std::vector<int> ExhaustiveSearch::getPath() const {
    std::vector<int> nPath= std::vector<int>();

    for (int i = 1; i < graph->getNumberOfNodes(); i++) {
        nPath.push_back( path[i-1]);
    }
    nPath.push_back(0);
    nPath.insert(nPath.begin(),0);
    //nPath[0]=nPath[graph->getNumberOfNodes()]=startNode;

    return nPath;
}

int ExhaustiveSearch::getWeight() const {
    return weight;
}
