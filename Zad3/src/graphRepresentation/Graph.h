//
// Created by Mikolaj Szymczyk on 31.05.2021.
//
#include <random>
#include <climits>
#include "../structures/arrayList/Edge.h"
#include "../structures/arrayList/Edge.h"
#include "../structures/arrayList/ArrayList.h"
#include "../structures/arrayList/array/Array.h"

#ifndef SDIZO_E2_GRAPH_H
#define SDIZO_E2_GRAPH_H
/**
 * Base class for incidenceMatrix and adjacencyList
 */
class Graph {
    int numberOfNodes, numberOfEdges;
protected:
    /**
     * Method read line of text file
     * @param file which will be read
     * @param tab there will be return result
     * @param size how many elements in line
     * @return true if everything is correct
     */
    static bool file_read_line(std::ifstream &file, int *tab, int size);

public:
    explicit Graph(int numberOfNodes);

    Graph();

    virtual ~Graph() = default;

    /**
     * Method generate graph with randoms wages of edges
     * @param noNodes
     * @param density
     * @param rd
     * @param max weight
     */
    void generateRandomGraph(int noNodes, double density, std::random_device &rd, int max);

    /**
     * Method generate directed graph with randoms wages of edges
     * @param noNodes
     * @param density
     * @param rd
     * @param max weight
     */
    void generateRandomDirectedGraph(int noNodes, double density, std::random_device &rd,
                                     int max);

    /**
     * Method add edge to graph
     * @param beginNode
     * @param endNode
     * @param weight
     */
    virtual void addEdge(int beginNode, int endNode, int weight) = 0;

    /**
     * Method add edge to graph
     * @param e
     */
    void addEdge(Edge *e);

    /**
     * method remove edge or set their weight on 0
     * @param beginNode
     * @param endNode
     */
    virtual void removeEdge(int beginNode, int endNode) = 0;

    /**
     * Method display graph
     */
    virtual void display() = 0;

    /**
     * Method set weight of edge
     * @param beginNode
     * @param endNode
     * @param weight
     */
    virtual void setWeightOfEdge(int beginNode, int endNode,int weight)=0;

    /**
     * Method check if given edge exist
     * @param beginNode
     * @param endNode
     * @return weight if edge exist or -1 if not
     */
    virtual int getEdge(int beginNode, int endNode) = 0;

    /**
     * Method convert all edges of graph to list
     * @return sorted ArrayList of Edges
     */
    virtual ArrayList<Edge> *convertIntoSortedListOfEdges() = 0;

    /**
     * Method convert all edges of graph to list
     * @return list
     */
    virtual ArrayList<Edge> *convertIntoListOfEdges()=0;

   /**
    * @param node which neighbours need
    * @return neighbours of node
    */
    virtual Array<int> *getNeighbours(int node) = 0;
/////////////////////////////////////////////////////////////////////////////////
    void setNumberOfNodes(int numberOfNodes);

    void setNumberOfEdges(int numberOfEdges);

    int getNumberOfNodes() const;

    int getNumberOfEdges() const;

    void incrementNumberOfEdges();

    void decrementNumberOfEdges();


};

#endif //SDIZO_E2_GRAPH_H
