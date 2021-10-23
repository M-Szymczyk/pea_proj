//
// Created by Mikolaj Szymczyk on 31.05.2021.
//

#include <iostream>
#include <sstream>
#include "Graph.h"


Graph::Graph(int numberOfNodes) : numberOfNodes(numberOfNodes), numberOfEdges(0) {}

int Graph::getNumberOfNodes() const {
    return numberOfNodes;
}

void Graph::generateRandomDirectedGraph(int noNodes, double density, std::random_device &rd, int max) {
    std::uniform_int_distribution<> dist(1, max);
    std::uniform_int_distribution<> distNode(0, noNodes - 1);
    for (int i = 1; i < noNodes; i++) {
        addEdge(0, i, dist(rd));
    }
    int noEdges = noNodes - 1;
    bool b1, b2;
    while (noEdges < (((noNodes * (noNodes - 1))) * density)) {
        int edgeBegin, edgeEnd;
        do {
            edgeBegin = distNode(rd), edgeEnd = distNode(rd);
            b1 = edgeBegin != edgeEnd;
            b2 = getEdge(edgeBegin, edgeEnd) == -1;
        } while (!(b1 && b2));
        addEdge(edgeBegin, edgeEnd, dist(rd));
        noEdges++;
    }
    numberOfEdges = noEdges;
}

#pragma clang diagnostic push
#pragma ide diagnostic ignored "bugprone-integer-division"

void Graph::generateRandomGraph(int noNodes, double density, std::random_device &rd, int max) {
    std::uniform_int_distribution<> dist(1, max);
    std::uniform_int_distribution<> distNode(0, noNodes - 1);
    for (int i = 1; i < noNodes; i++) {
        addEdge(0, i, dist(rd));
    }
    int noEdges = noNodes - 1;
    bool b1, b2;
    while (noEdges < (((noNodes * (noNodes - 1)) / 2) * density)) {
        int edgeBegin, edgeEnd;
        do {
            edgeBegin = distNode(rd), edgeEnd = distNode(rd);
            b1 = edgeBegin != edgeEnd;
            b2 = getEdge(edgeBegin, edgeEnd) == -1;
        } while (!(b1 && b2));
        addEdge(edgeBegin, edgeEnd, dist(rd));
        noEdges++;
    }
    numberOfEdges = noEdges;
}

#pragma clang diagnostic pop

void Graph::incrementNumberOfEdges() {
    numberOfEdges++;
}

void Graph::decrementNumberOfEdges() {
    numberOfEdges--;
}

int Graph::getNumberOfEdges() const {
    return numberOfEdges;
}

void Graph::addEdge(Edge *e) {
    addEdge(e->getBeginNode(), e->getEndNode(), e->getWeight());
}


bool Graph::file_read_line(std::ifstream &file, int tab[], int size) {
    using namespace std;
    string s;
    getline(file, s);

    if (file.fail() || s.empty())
        return (false);

    istringstream in_ss(s);

    for (int i = 0; i < size; i++) {
        in_ss >> tab[i];
        if (in_ss.fail())
            return (false);
    }
    return (true);
}

void Graph::setNumberOfNodes(int numberOfN) {
    Graph::numberOfNodes = numberOfN;
}

void Graph::setNumberOfEdges(int numberOfE) {
    Graph::numberOfEdges = numberOfE;
}

Graph::Graph() {
    numberOfEdges = 0;
    numberOfNodes = 0;
}
