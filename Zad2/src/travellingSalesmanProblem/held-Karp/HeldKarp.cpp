//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#include "HeldKarp.h"
#include <algorithm>

int HeldKarp::getWeight() const {
    return heldKarpCost.weight;
}

std::list<int> HeldKarp::getPath() const {
    return heldKarpCost.path;
}

HeldKarp::HeldKarp(Graph *graph, int startN) : graph(graph), startNode(startN) {
    /// inicjacja początkowych wartości
    heldKarpCost.weight = INT_MAX;
    heldKarpCost.path = std::list<int>();
    costs = std::unordered_map<long long, HeldKarpCost>();
    for (int i = 0; i < graph->getNumberOfNodes(); i++)
        heldKarpCost.path.push_back(i);
    heldKarpCost.path.pop_front();
    /// uruchomienie algorytmu
    auto result = cost(startNode, heldKarpCost.path);
    /// zapisanie wyniku w polach klasy
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
        auto node = unvisitedNodesList.begin();
        auto range = unvisitedNodesList.size();
        /// pętla generuje wszystkie podzbiory dla zbioru unvisitedNodesList
        for (int i = 0; i < range; i++) {
            bool lastElement = false;
            if (*node == unvisitedNodesList.back()) {//przypadek usuwanie ostatniego elemetu
                lastElement = true;
            }
            nodeVal = *node;// zapisanie usuwanego wierzchołka
            /// usuwanie kolejnych wierzchołków zbioru
            if (lastElement) {
                unvisitedNodesList.pop_back();
            } else {
                node = unvisitedNodesList.erase(node);
            }
            /// wyszukiwanie podzbioru
            if (!unvisitedNodesList.empty() && !costs.empty())
                result2 = find(unvisitedNodesList, nodeVal);
            /// jeżeli nie znaleziono podzbioru wyszukujemy najlepszej wartości dla danego podzbioru
            result = result2.weight != INT_MAX ? result2 : cost(nodeVal, unvisitedNodesList);
            /*           if (result2.weight != INT_MAX && result.weight != result2.weight) {
                           std::cout << "\n-------------------------------------------------------------------------------";
                           std::cout << "\nres1: ";
                           result.display();
                           result2.display();
                           std::cout << "\n##############################################################################";
                       }*/
            /// obliczanie kosztu danego zbioru
            result.weight += graph->getEdge(source, nodeVal);
            result.path.push_front(source);

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
        /// dodanie najelepszego rozwiazania dla danego zbioru do struktury
        unvisitedNodesList.push_front(source);
        costs.insert(std::make_pair(HeldKarpCost::hashCode(unvisitedNodesList), HeldKarpCost(minCost)));
        unvisitedNodesList.pop_front();

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
    HeldKarpCost result;
    // find hash code of set
    unvisitedNodesList.push_front(source);
    auto hashCode = HeldKarpCost::hashCode(unvisitedNodesList);
    auto searchingResult = costs.find(hashCode);
    //restore set
    unvisitedNodesList.remove(source);
    // if find then take result
    if (searchingResult != costs.end())
        result = searchingResult->second;

    return {result};
}

HeldKarp::~HeldKarp() {
    costs.clear();
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
                                                                path(std::move(currentPath)) {}

long long int HeldKarpCost::hashCode(const std::list<int> &list) {
    std::hash<int> hash_pathElement;
    long long hashCode = 1;
    for (auto i: list) {
        hashCode = 31 * hashCode + hash_pathElement(i);
    }
    return hashCode;
}


