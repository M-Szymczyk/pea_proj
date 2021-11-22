//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#include "HeldKarp.h"

#include <utility>
#include <algorithm>

int HeldKarp::getWeight() const {
    return heldKarpCost.weight;
}

std::list<int> HeldKarp::getPath() const {
    return heldKarpCost.path;
}

HeldKarp::HeldKarp(Graph *graph, int startN) : graph(graph), startNode(startN) {
    // początkowe wartości
    heldKarpCost.weight = INT_MAX;
    heldKarpCost.path = std::list<int>();
    costs = std::vector<std::list<HeldKarpCost>>();
    for (int i = 0; i < graph->getNumberOfNodes(); i++) {
        costs.push_back(*new std::list<HeldKarpCost>);
    }

    for (int i = 0; i < graph->getNumberOfNodes(); i++)
        heldKarpCost.path.push_back(i);
    heldKarpCost.path.pop_front();

    auto result = cost(startNode, heldKarpCost.path);
    heldKarpCost.weight = result.weight;

    heldKarpCost.path.clear();
    heldKarpCost.path = result.path;
}

HeldKarpCost HeldKarp::cost(int source, std::list<int> &unvisitedNodesList) {
    if (unvisitedNodesList.empty()) {/// zbiór z jednym elementem: utwórz ścieżkę i oblicz jej wage
        auto v = std::list<int>();
        v.push_front(startNode);
        v.push_front(source);
        return {graph->getEdge(source, startNode), v};
    } else {/// zbiór z większą liczba elementów: znajdź najtańszą ścieżkę
        int nodeVal;
        HeldKarpCost result = HeldKarpCost(), minCost = HeldKarpCost(), result2 = HeldKarpCost();
        if (unvisitedNodesList.size() >= 2) {
            minCost = find(unvisitedNodesList);
            if (minCost.weight != INT_MAX) {
                minCost.weight += graph->getEdge(source, minCost.path.front());
                minCost.path.push_front(source);
            }
            //find(unvisitedNodesList);
        }
        if (minCost.weight == INT_MAX) {
            auto node = unvisitedNodesList.begin();
            auto range = unvisitedNodesList.size();

            for (int i = 0; i < range; i++) {
                bool lastElement = false;
                if (*node == unvisitedNodesList.back()) {
                    lastElement = true;
                }
                nodeVal = *node;

                /// usuwanie kolejnych wierzchołków zbioru
                if (lastElement) {
                    unvisitedNodesList.pop_back();
                } else {
                    node = unvisitedNodesList.erase(node);
                }
                result = cost(nodeVal, unvisitedNodesList);
                /// obliczanie kosztu danego zbioru
                result.weight += graph->getEdge(source, nodeVal);
                result.path.push_front(source);
                auto pointerToLists = costs.begin();
                std::advance(pointerToLists, result.path.size() - 3);
                pointerToLists->push_back(HeldKarpCost(result));



                /// zmiana najlepszej ścieżki jeżeli znaleziono lepsza
                minCost = getCost(result, minCost);

                /// dodanie usuniętego wierzchołka
                if (lastElement) {
                    unvisitedNodesList.push_back(nodeVal);
                    break;
                }
                node = unvisitedNodesList.begin();
                std::advance(node, i);
                node = unvisitedNodesList.insert(node, nodeVal);
                ++node;
                result = HeldKarpCost();
            }
        }
//        if (result2.weight != INT_MAX && minCost.weight != result2.weight) {
//            std::cout << "\n-------------------------------------------------------------------------------";
//            std::cout << "\nres1: ";
//            minCost.display();
//            result2.display();
//            std::cout << "\n##############################################################################";
//        }
        return minCost;
    }
}

HeldKarpCost HeldKarp::getCost(HeldKarpCost &result, HeldKarpCost &minCost) {
    if (result.weight < minCost.weight) {
        minCost = result;
        result = HeldKarpCost();
    } else {
        result = HeldKarpCost();
    }
    return minCost;
}

HeldKarpCost HeldKarp::find(std::list<int> &unvisitedNodesList) {
    HeldKarpCost min = HeldKarpCost(),result;
    do {
//        for(auto e:unvisitedNodesList)
//            std::cout << e << " ";
//        std::cout << std::endl;
        result = findPrevious(unvisitedNodesList);
        if(result.weight<=min.weight)
            min = result;
    } while (std::next_permutation(unvisitedNodesList.begin(), unvisitedNodesList.end()));
    return min;
}

HeldKarpCost HeldKarp::findPrevious(std::list<int> &unvisitedNodesList) {
    /// Przesuniecie iteratora na vectr zawierający ścieżki o tej samej wielkości
    auto pointerToLists = costs.begin();
    std::advance(pointerToLists, unvisitedNodesList.size() - 2);
    /// wyszukiwanie tej samej ścieżki
    auto pointerToElementOfList = pointerToLists->begin();
    while (pointerToElementOfList != pointerToLists->end()) {
        auto val2 = unvisitedNodesList.begin(), val1 = pointerToElementOfList->path.begin();
        if (*val1 == *val2) {
            bool theSamePath = true;
            for (int i = 0; i < unvisitedNodesList.size(); i++) {
                if (*val1 != *val2) {
                    theSamePath = false;
                    break;
                }
                val1++;
                val2++;
            }
            if (theSamePath) {
//                std::cout<<"\nfind  ";
//                for(auto un:unvisitedNodesList)
//                    std::cout<<un<<"  ";
//                pointerToElementOfList->display();
                return *pointerToElementOfList;
            }
        }
        pointerToElementOfList++;
    }
    return {};
}

HeldKarp::~HeldKarp() {
    for (auto list: costs) {
        list.clear();
    }
    costs.clear();
}

void HeldKarp::display() {

}


HeldKarpCost::HeldKarpCost() {
    weight = INT_MAX;
    path = std::list<int>();
}

HeldKarpCost::~HeldKarpCost() {
    path.clear();
    weight = INT_MAX;
}

void HeldKarpCost::display() const {
    std::cout << "\nWaga wynosi: " << weight << "; Sciezka: ";
    for (auto v: path) {
        std::cout << v << ", ";
    }
}

HeldKarpCost::HeldKarpCost(int c, std::list<int> currentPath) : weight(c),
                                                                path(std::move(currentPath)) {


}
