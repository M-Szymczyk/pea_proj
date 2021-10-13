#include <iostream>
#include "graphRepresentation/Graph.h"
#include "graphRepresentation/matrix/AdjacencyMatrix.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    auto *g= new AdjacencyMatrix(10);
    g->addEdge(0,1,100);
    g->display();
    return 0;
}
