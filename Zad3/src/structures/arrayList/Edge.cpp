//
// Created by Mikolaj Szymczyk on 01.06.2021.
//

#include <random>
#include <iostream>
#include "Edge.h"

Edge::Edge(int beginNode, int endNode, int weight) : beginNode(beginNode), endNode(endNode), weight(weight) {}

void Edge::display() const {
    std::cout<<" b: "<<beginNode<<"-"<<weight<<"->e: "<<endNode;
}

int Edge::getWeight() const {
    return weight;
}

int Edge::getBeginNode() const {
    return beginNode;
}

int Edge::getEndNode() const {
    return endNode;
}

void Edge::setWeight(int w) {
    Edge::weight = w;
}
