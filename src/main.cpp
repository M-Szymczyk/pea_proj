#include <iostream>
#include "graphRepresentation/Graph.h"
#include "graphRepresentation/matrix/AdjacencyMatrix.h"
#include "travellingSalesmanProblem/ExhaustiveSearch.h"

int main() {
    std::cout << "Hello, World!" << std::endl;
    std::string file= "tsp6_1.txt",file2="tsp6_2.txt";
    auto *g= new AdjacencyMatrix( file);
    g->display();
    ExhaustiveSearch(g,0);
    delete g;

    return 0;
}
