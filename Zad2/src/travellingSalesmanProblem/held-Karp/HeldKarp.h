//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#ifndef ZAD2_HELDKARP_H
#define ZAD2_HELDKARP_H


#include <list>
#include "../../graphRepresentation/Graph.h"

class HeldKarpCost{
public:
    int weight;
    std::list<int> path;

    HeldKarpCost(int currentCost, std::list<int> currentPath);

    HeldKarpCost();

    virtual ~HeldKarpCost();
    void display() const;
};

class HeldKarp {
    Graph *graph;
    std::vector<std::list<HeldKarpCost>> costs;
    HeldKarpCost heldKarpCost;
    int startNode;
public:

    HeldKarp(Graph *graph, int startNode);



    /**
     * @return weight of circuit
     */
    int getWeight() const;

    /**
    * @return solved tsp problem for given graph
    */
    std::list<int> getPath() const;

    void display();

    virtual ~HeldKarp();

    /**Method find the best circle of all nodes*/
    HeldKarpCost cost(int source, std::list<int> &unvisitedNodesList);
    /**
     * Metoda sprawdza czy nie obliczono już poprzednika dla danego zestawu
     * @param unvisitedNodesList
     * @return
     */
    HeldKarpCost findPrevious(std::list<int> &unvisitedNodesList);

    static HeldKarpCost getCost(HeldKarpCost &result, HeldKarpCost &minCost);

    HeldKarpCost find(std::list<int> &unvisitedNodesList);
};


#endif //ZAD2_HELDKARP_H
