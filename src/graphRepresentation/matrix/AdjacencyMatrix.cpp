//
// Created by Mikolaj Szymczyk on 13.10.2021.
//

#include "AdjacencyMatrix.h"


AdjacencyMatrix::AdjacencyMatrix(int noNodes) : IncidenceMatrix() {
    setNumberOfNodes(noNodes);
    matrix = new int *[getNumberOfNodes()];
    for (int i = 0; i < getNumberOfNodes(); i++) {
        matrix[i] = new int[getNumberOfNodes()];
        for (int j = 0; j < getNumberOfNodes(); j++) {
            matrix[i][j] = 0;
        }

    }
}
void AdjacencyMatrix::setWeightOfEdge(int beginNode, int endNode, int weight) {
    IncidenceMatrix::setWeightOfEdge(beginNode, endNode, weight);
}

void AdjacencyMatrix::addEdge(int beginNode, int endNode, int weight) {
    matrix[beginNode][endNode]=weight;
    matrix[endNode][beginNode]=weight;
}

void AdjacencyMatrix::removeEdge(int beginNode, int endNode) {
    matrix[beginNode][endNode]=0;
    matrix[endNode][beginNode]=0;
}

void AdjacencyMatrix::display() {
    IncidenceMatrix::display();
}

AdjacencyMatrix::~AdjacencyMatrix() {
    IncidenceMatrix::~IncidenceMatrix();
}

int AdjacencyMatrix::getEdge(int beginNode, int endNode) {
    return IncidenceMatrix::getEdge(beginNode, endNode);
}

ArrayList<Edge> *AdjacencyMatrix::convertIntoSortedListOfEdges() {
    return IncidenceMatrix::convertIntoSortedListOfEdges();
}

ArrayList<Edge> *AdjacencyMatrix::convertIntoListOfEdges() {
    return IncidenceMatrix::convertIntoListOfEdges();
}

Array<int> *AdjacencyMatrix::getNeighbours(int node) {
    return IncidenceMatrix::getNeighbours(node);
}
