//
// Created by Mikolaj Szymczyk on 13.10.2021.
//

#include "AdjacencyMatrix.h"


AdjacencyMatrix::AdjacencyMatrix(int noNodes) : IncidenceMatrix() {
    setNumberOfNodes(noNodes);
    matrix = new int *[getNumberOfNodes()];
    for (int i = 0; i < getNumberOfNodes(); i++) {
        matrix[i] = new int[getNumberOfNodes()];
        for (int j = 0; j < getNumberOfNodes(); j++) {
            matrix[i][j] = 0;
        }

    }
}
void AdjacencyMatrix::setWeightOfEdge(int beginNode, int endNode, int weight) {
    matrix[beginNode][endNode] = weight;
    matrix[endNode][beginNode] = weight;
}

void AdjacencyMatrix::addEdge(int beginNode, int endNode, int weight) {
    matrix[beginNode][endNode]=weight;
    matrix[endNode][beginNode]=weight;
}

void AdjacencyMatrix::removeEdge(int beginNode, int endNode) {
    matrix[beginNode][endNode]=0;
    matrix[endNode][beginNode]=0;
}

void AdjacencyMatrix::display() {
    using namespace std;

    cout << endl;
    for (int i = 0; i < getNumberOfNodes(); i++) {
        cout << endl << i << ".  :";
        for (int j = 0; j < getNumberOfNodes(); j++) {
            printf("%3d",matrix[i][j]);

            if (j != getNumberOfNodes() - 1)
                cout << ", ";
        }

    }
}

int AdjacencyMatrix::getEdge(int beginNode, int endNode) {
    return matrix[beginNode][endNode];
}

ArrayList<Edge> *AdjacencyMatrix::convertIntoSortedListOfEdges() {
    auto *list = new ArrayList<Edge>();
    for (int i = 0; i < getNumberOfNodes(); i++)
        for (int j = 0; j < getNumberOfNodes(); j++)
            if(matrix[i][j]!=0)
                list->addEdgeToList(new Edge(i,j,matrix[i][j]));
            //todo trzeba jeszcze posortowac ta liste
    return list;
}

ArrayList<Edge> *AdjacencyMatrix::convertIntoListOfEdges() {
    auto *list = new ArrayList<Edge>();
    for (int i = 0; i < getNumberOfNodes(); i++)
        for (int j = 0; j < getNumberOfNodes(); j++)
            if(matrix[i][j]!=0)
                list->addEdgeToList(new Edge(i,j,matrix[i][j]));
    return list;
}

Array<int> *AdjacencyMatrix::getNeighbours(int node) {
    auto *listOfNeighbours = new Array<int>();
    for (int i = 0; i < getNumberOfNodes(); i++)
        if (matrix[node][i]!=0)
            listOfNeighbours->addAtEnding(new int(i));
    return listOfNeighbours;
}

AdjacencyMatrix::AdjacencyMatrix(std::string &fileName) {
    using namespace std;
    ifstream file;
    int tab[4];
    file.open(fileName.c_str());

    if (file.is_open()) {
        if (Graph::file_read_line(file, tab, 1)) {

            int numberOfNodes = tab[0];
            setNumberOfNodes(numberOfNodes);
            setNumberOfEdges(0);
            matrix = new int *[getNumberOfNodes()];
            for (int i = 0; i < getNumberOfNodes(); i++) {
                matrix[i] = new int[getNumberOfNodes()];
            }
            for (int i = 0; i < (getNumberOfNodes()); i++) {
                for (int j = 0; j < getNumberOfNodes(); j++) {
                    matrix[i][j] = 0;
                }
            }
//            graph_start = tab[2];
//            graph_end = tab[3];
            int ntab[1000];
            for (int i = 0; i < getNumberOfNodes(); i++)
                if (Graph::file_read_line(file, ntab, numberOfNodes)) {
                    for(int k=0;k<getNumberOfNodes();k++){
                        matrix[i][k]=ntab[k];
                        if(ntab[k]!=0)
                            setNumberOfEdges(getNumberOfEdges()+1);
                    }
                } else {
                    cout << "File error  couldn't read line of edges- READ EDGE" << endl;
                    break;
                }
        } else
            cout << "File error couldn't read line- READ INFO" << endl;
        file.close();
    } else
        cout << "File error not opened  - OPEN" << endl;
}

AdjacencyMatrix::AdjacencyMatrix(int noNodes, int **matrix) {
    setMatrix(matrix);
    setNumberOfNodes(noNodes);
}
