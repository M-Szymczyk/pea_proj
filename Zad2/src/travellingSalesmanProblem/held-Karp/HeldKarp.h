//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#ifndef ZAD2_HELDKARP_H
#define ZAD2_HELDKARP_H


#include <list>
#include "../../graphRepresentation/Graph.h"

class HeldKarpCost{
public:
    int currentCost;
    std::list<int> *currentPath;

    HeldKarpCost(int currentCost, std::list<int> *currentPath);

    virtual ~HeldKarpCost();
    void display() const;
};

class HeldKarp {
    Graph *graph;
    std::list<int> *path;
    int weight;
    int startNode;
public:
    HeldKarp(Graph *graph, int startNode);

    /**
    * @return solved tsp problem for given graph
    */
    //int *getPath() const;

    /**
     * @return weight of circuit
     */
    int getWeight() const;



    std::list<int> *getPath() const;

    void display();

    virtual ~HeldKarp();

    HeldKarpCost *cost(int source, std::list<int> &unvisitedNodesList);

    static HeldKarpCost *getCost(int source, HeldKarpCost *result, HeldKarpCost *minCost) ;
};


#endif //ZAD2_HELDKARP_H
