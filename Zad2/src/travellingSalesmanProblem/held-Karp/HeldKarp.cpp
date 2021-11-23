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
    costs = std::map<int, HeldKarpCost>();

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
            if (!costs.empty())
                minCost = find(unvisitedNodesList, source);
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
                costs.insert(std::make_pair(HeldKarpCost::hashCode(result.path), HeldKarpCost(result)));

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
        if (result2.weight != INT_MAX && minCost.weight != result2.weight) {
            std::cout << "\n-------------------------------------------------------------------------------";
            std::cout << "\nres1: ";
            minCost.display();
            result2.display();
            std::cout << "\n##############################################################################";
        }
//        if (result2.weight != INT_MAX)
//            return result2;
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

HeldKarpCost HeldKarp::find(std::list<int> &unvisitedNodesList, int source) {
    unvisitedNodesList.push_back(0);
    HeldKarpCost min = HeldKarpCost(), result;
    do {
//        for(auto e:unvisitedNodesList)
//            std::cout << e << " ";
//        std::cout << std::endl;
        unvisitedNodesList.push_front(source);
        auto searchingResult = costs.find(HeldKarpCost::hashCode(unvisitedNodesList));
        unvisitedNodesList.remove(source);
        if (searchingResult != costs.end())
            result = searchingResult->second;
        //else continue;
        if (result.weight <= min.weight)
            min = result;

    } while (std::next_permutation(unvisitedNodesList.begin(), unvisitedNodesList.end()));
    unvisitedNodesList.erase(unvisitedNodesList.begin());
    return {min};
}

HeldKarp::~HeldKarp() {
//    for (auto list: costs) {
//        list.clear();
//    }
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

long long int HeldKarpCost::hashCode(const std::list<int>& list) {
    std::hash<int> hash_pathElement;
    long long hashCode = 1;
    for (auto i: list) {
        hashCode = 31 * hashCode + hash_pathElement(i);
    }
    std::hash<int> hash_weight;
    return hashCode;
}


