//
// Created by Mikolaj Szymczyk on 31.05.2021.
//

#ifndef SDIZO_E2_INCIDENCEMATRIXDIRECTEDGRAPH_H
#define SDIZO_E2_INCIDENCEMATRIXDIRECTEDGRAPH_H

#include <iostream>
#include "IncidenceMatrix.h"

class IncidenceMatrixDirectedGraph : public IncidenceMatrix {
public:
    IncidenceMatrixDirectedGraph(int noNodes, double density, std::random_device &rd, int max);

    void addEdge(int beginNode, int endNode, int weight) override;

    void removeEdge(int beginNode, int endNode) override;

    explicit IncidenceMatrixDirectedGraph(std::string &fileName);

    IncidenceMatrixDirectedGraph(ArrayList<Edge> *pList, int noNodes);

    /**
     * Method search for noEdge
     * @param bN begin Node
     * @param eN end Node
     * @return noEdge
     */
    int findNoEdge(int bN, int eN);


    int getEdge(int beginNode, int endNode) override;

    Array<int> *getNeighbours(int node) override;

    ArrayList<Edge> *convertIntoListOfEdges() override;

    void setWeightOfEdge(int beginNode, int endNode, int weight) override;
};


#endif //SDIZO_E2_INCIDENCEMATRIXDIRECTEDGRAPH_H
