//
// Created by Mikolaj Szymczyk on 13.10.2021.
//

#ifndef PEAZAD1_ADJACENCYMATRIX_H
#define PEAZAD1_ADJACENCYMATRIX_H


#include "IncidenceMatrix.h"

class AdjacencyMatrix: public IncidenceMatrix{
public:
    explicit AdjacencyMatrix(int noNodes);

    explicit AdjacencyMatrix(std::string &fileName);

    void setWeightOfEdge(int beginNode, int endNode, int weight) override;

    void addEdge(int beginNode, int endNode, int weight) override;

    void removeEdge(int beginNode, int endNode) override;

    void display() override;

    ~AdjacencyMatrix() override;

    int getEdge(int beginNode, int endNode) override;

    ArrayList<Edge> *convertIntoSortedListOfEdges() override;

    ArrayList<Edge> *convertIntoListOfEdges() override;

    Array<int> *getNeighbours(int node) override;

};


#endif //PEAZAD1_ADJACENCYMATRIX_H
