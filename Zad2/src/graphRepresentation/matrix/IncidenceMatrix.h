//
// Created by Mikolaj Szymczyk on 29.05.2021.
//

#ifndef SDIZO_E2_INCIDENCEMATRIX_H
#define SDIZO_E2_INCIDENCEMATRIX_H


#include "../Graph.h"

class IncidenceMatrix : public Graph {
protected:
    int **matrix;
protected:
    /**
     * Method search for noEdge
     * @param bN begin Node
     * @param eN end Node
     * @return noEdge
     */
    int findNoEdge(int bN, int eN);

public:
    void setMatrix(int **matrix);

public:
    void setWeightOfEdge(int beginNode, int endNode, int weight) override;

    void addEdge(int beginNode, int endNode, int weight) override;

    void removeEdge(int beginNode, int endNode) override;

    void display() override;

    ~IncidenceMatrix() override;

    explicit IncidenceMatrix(int noNodes,int noEdges);

    IncidenceMatrix(int noNodes,double density, std::random_device &rd ,int max);

    explicit IncidenceMatrix(std::string &fileName);

    IncidenceMatrix(ArrayList<Edge> *pList, int noNodes);

    IncidenceMatrix();

    int getEdge(int beginNode, int endNode) override;

    ArrayList<Edge> *convertIntoSortedListOfEdges() override;

    ArrayList<Edge> *convertIntoListOfEdges() override;

    Array<int> *getNeighbours(int node) override;
};


#endif //SDIZO_E2_INCIDENCEMATRIX_H
