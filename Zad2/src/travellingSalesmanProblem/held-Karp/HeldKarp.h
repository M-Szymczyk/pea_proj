//
// Created by Mikolaj Szymczyk on 23.10.2021.
//

#ifndef ZAD2_HELDKARP_H
#define ZAD2_HELDKARP_H


#include <list>
#include <map>
#include <unordered_map>

#include "../../graphRepresentation/Graph.h"

class HeldKarpCost {
public:
    int weight;
    std::list<int> path;

    HeldKarpCost(int currentCost, std::list<int> currentPath);

    HeldKarpCost();

    virtual ~HeldKarpCost();

    void display() const;

    static long long int hashCode(const std::list<int> &list);
};

class HeldKarp {
    Graph *graph;
    std::unordered_map<long long, HeldKarpCost> costs;
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

    virtual ~HeldKarp();

    /**
     * Method find the best circuit for given set
     * @param source node
     * @param unvisitedNodesList rest of nodes
     * @return best circuit for given set
     */
    HeldKarpCost cost(int source, std::list<int> &unvisitedNodesList);

    static HeldKarpCost getCost(HeldKarpCost &result, HeldKarpCost &minCost);

    /**
     * Method search for result for given subset
     * @param unvisitedNodesList subset
     * @param source source of subset
     * @return result
     */
    HeldKarpCost find(std::list<int> &unvisitedNodesList, int source);
};


#endif //ZAD2_HELDKARP_H
