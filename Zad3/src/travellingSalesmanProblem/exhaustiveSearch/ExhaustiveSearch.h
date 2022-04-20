//
// Created by Mikolaj Szymczyk on 05.10.2021.
//

#ifndef PEAZAD1_EXHAUSTIVESEARCH_H
#define PEAZAD1_EXHAUSTIVESEARCH_H


#include "../../graphRepresentation/Graph.h"

class ExhaustiveSearch {
    Graph *graph;
    int *path;
    int weight;
    int startNode;
public:
    /**
     * Constructor of class ExhaustiveSearch
     * @param graph graph for which will be  search the best circuit
     * @param startNode starting node of circuit
     */
    explicit ExhaustiveSearch(Graph *graph, int startNode);

    /**
     * @return solved tsp problem for given graph
     */
    int *getPath() const;

    /**
     * @return weight of circuit
     */
    int getWeight() const;
private:
    /**
     * method generate permutations
     * @param a nodes to permute
     * @param size noNodes
     * @param n noNodes
     */
    void generatePermutation(int *a, int size, int n);

    /**
     * method set new best path and weight
     * @param nPath new path
     * @param nWeight new weight
     */
    void setNewBestPath(const int *nPath, int nWeight);

    /**
     * method calculate weight of given circuit
     * @param a order of nodes
     * @param n noNodes
     */
    void calculateWeight(int a[], int n);
};

#endif //PEAZAD1_EXHAUSTIVESEARCH_H
