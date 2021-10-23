//
// Created by Mikolaj Szymczyk on 01.06.2021.
//

#ifndef SDIZO_E2_EDGE_H
#define SDIZO_E2_EDGE_H



class Edge {

    int beginNode;
    int endNode;
    int weight;
public:
    Edge(int beginNode, int endNode, int weight);

    int getWeight() const;

    void display() const;

    int getBeginNode() const;

    int getEndNode() const;

    void setWeight(int weight);
};


#endif //SDIZO_E2_EDGE_H
