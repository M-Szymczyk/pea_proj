//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#include "HeldKarp.h"

int HeldKarp::getWeight() const {
    return weight;
}

std::list<int> *HeldKarp::getPath() const {
    return path;
}

HeldKarp::HeldKarp(Graph *graph, int startN) : graph(graph), startNode(startN) {
    weight = INT_MAX;
    path = new std::list<int>;

    for (int i = 0; i < graph->getNumberOfNodes(); i++)
        path->push_back(i);
    path->pop_front();
    auto result = cost(startNode, *path);
    weight = result->currentCost;
    delete path;
    path = result->currentPath;
}

HeldKarpCost *HeldKarp::cost(int source, std::list<int> &unvisitedNodesList) {
    if (unvisitedNodesList.size() == 1) {/// zbior z jednym elementem: utworz sciezke i oblicz jej wage

        int unvisitedNode = unvisitedNodesList.front();
        auto v = new std::list<int>;

        v->push_front(startNode);
        v->push_front(unvisitedNode);
        v->push_front(source);
        //std::cout<<"\ncost: "<<source<<"w: "<<graph->getEdge(source, unvisitedNode)<<" - "<<unvisitedNode<<"w: "<< graph->getEdge(unvisitedNode, startNode)<<" - "<< startNode;
        return new HeldKarpCost(graph->getEdge(source, unvisitedNode)
                                + graph->getEdge(unvisitedNode, startNode), v);
    } else {/// zbior z wieksza liczba elementow: znajdz najtansza sciezke
        int nodeVal;
        HeldKarpCost *result, *minCost = new HeldKarpCost(INT_MAX, nullptr);
        std::_List_iterator<int> node = unvisitedNodesList.begin();
        auto range = unvisitedNodesList.size();
        for (int i = 0; i < range; i++) {
            nodeVal = *node;
            /// usuwanie kolejnych wierzcholkow zbioru
            if (*node == unvisitedNodesList.back()) {
                unvisitedNodesList.pop_back();
                result = cost(nodeVal, unvisitedNodesList);
                result->currentCost += graph->getEdge(source, nodeVal);
                minCost = getCost(source, result, minCost);
                unvisitedNodesList.push_back(nodeVal);
                break;
            } else {
                node = unvisitedNodesList.erase(node);
            }

            /// obliczanie kosztu danego zbioru
            result = cost(nodeVal, unvisitedNodesList);
            result->currentCost += graph->getEdge(source, nodeVal);

            /// zmiana najlepszej sciezki jezeli znaleziono lepsza
            minCost = getCost(source, result, minCost);
//            if (minCost->currentPath->size() > 5)
//                minCost->display();

            /// dodanie usunietego wierzcholka
            node = unvisitedNodesList.begin();
            std::advance(node, i);
            node = unvisitedNodesList.insert(node, nodeVal);

            /// zmiana wierzcholka
            if (*node != unvisitedNodesList.back()) {
                ++node;
            }
        }

        ///dodanie
        return minCost;
    }
}

HeldKarpCost *HeldKarp::getCost(int source, HeldKarpCost *result, HeldKarpCost *minCost) {
    if (result->currentCost < minCost->currentCost) {
        delete minCost;
        minCost = result;
        result = nullptr;
        minCost->currentPath->push_front(source);
        //std::cout<<" 0 :"<<graph->getEdge()
    } else {
        delete result;
    }
    return minCost;
}

void HeldKarp::display() {
    std::cout << "\nWaga wynosi: " << weight << "; Sciezka: ";
    for (auto v: *path) {
        std::cout << v << ", ";
    }
}

HeldKarp::~HeldKarp() {
    delete path;
}

HeldKarpCost::HeldKarpCost(int currentCost, std::list<int> *currentPath) : currentCost(currentCost),
                                                                           currentPath(currentPath) {}

HeldKarpCost::~HeldKarpCost() {
    delete currentPath;
}

void HeldKarpCost::display() const {
    std::cout << "\nWaga sciezki: " << currentCost << ", sciezka: ";
    for (auto v: *currentPath)
        std::cout << v << ", ";
}
