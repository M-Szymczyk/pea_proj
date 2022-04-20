//
// Created by Mikolaj Szymczyk on 31.05.2021.
//


#include "IncidenceMatrixDirectedGraph.h"

void IncidenceMatrixDirectedGraph::addEdge(int beginNode, int endNode, int weight) {
    int w = findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = weight;
    matrix[endNode][w] = -weight;
    incrementNumberOfEdges();
}


IncidenceMatrixDirectedGraph::IncidenceMatrixDirectedGraph(int noNodes, double density, std::random_device &rd, int max)
        : IncidenceMatrix(noNodes, (((noNodes * (noNodes - 1))) * density)) {
    std::uniform_int_distribution<> dist(1, max);
    std::uniform_int_distribution<> distNode(0, noNodes - 1);
    int i, weight;
    for (i = 1; i < noNodes; i++) {
        weight = dist(rd);
        matrix[0][i - 1] = weight;
        matrix[i][i - 1] = -weight;
    }

    bool b1, b2;
    while (i < getNumberOfEdges()) {
        int edgeBegin, edgeEnd;
        do {
            edgeBegin = distNode(rd), edgeEnd = distNode(rd);
            b1 = edgeBegin != edgeEnd;
            b2 = IncidenceMatrixDirectedGraph::getEdge(edgeBegin, edgeEnd) == -1;
        } while (!(b1 && b2));
        matrix[edgeBegin][i - 1] = weight;
        matrix[edgeEnd][i - 1] = -weight;
        i++;

    }
}

void IncidenceMatrixDirectedGraph::removeEdge(int beginNode, int endNode) {
    int w = IncidenceMatrixDirectedGraph::findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = 0;
    matrix[endNode][w] = 0;
    decrementNumberOfEdges();

}

void IncidenceMatrixDirectedGraph::setWeightOfEdge(int beginNode, int endNode, int weight) {
    int w = IncidenceMatrixDirectedGraph::findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = weight;
    matrix[endNode][w] = -weight;
}

int IncidenceMatrixDirectedGraph::findNoEdge(int bN, int eN) {
    for (int i = 0; i < getNumberOfEdges(); i++) {
        if (matrix[bN][i] > 0) {
            if (matrix[eN][i] < 0)
                return i;
        }
    }
    return -1;
}

IncidenceMatrixDirectedGraph::IncidenceMatrixDirectedGraph(std::string &fileName) {
    using namespace std;
    ifstream file;
    int tab[4];
    file.open(fileName.c_str());

    if (file.is_open()) {
        if (Graph::file_read_line(file, tab, 2)) {
            int iterations = tab[0];
            int numberOfNodes = tab[1];
            setNumberOfNodes(numberOfNodes);
            setNumberOfEdges(iterations);

            matrix = new int *[getNumberOfNodes()];
            for (int i = 0; i < getNumberOfNodes(); i++) {
                matrix[i] = new int[getNumberOfEdges()];
            }
            for (int i = 0; i < (getNumberOfNodes()); i++) {
                for (int j = 0; j < getNumberOfEdges(); j++) {
                    matrix[i][j] = 0;
                }
            }

//            graph_start = tab[2];
//            graph_end = tab[3];
            for (int i = 0; i < iterations; i++)
                if (Graph::file_read_line(file, tab, 3)) {
                    matrix[tab[0]][i] = tab[2];
                    matrix[tab[1]][i] = -tab[2];
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

IncidenceMatrixDirectedGraph::IncidenceMatrixDirectedGraph(ArrayList<Edge> *pList, int noNodes) {
    setNumberOfEdges(pList->getSize());
    setNumberOfNodes(noNodes);
    matrix = new int *[getNumberOfNodes()];
    for (int i = 0; i < getNumberOfNodes(); i++) {
        matrix[i] = new int[getNumberOfEdges()];
    }
    for (int i = 0; i < (getNumberOfNodes()); i++) {
        for (int j = 0; j < getNumberOfEdges(); j++) {
            matrix[i][j] = 0;
        }
    }
    for (int i = 0; i < pList->getSize(); i++) {
        matrix[pList->getElement(i)->getBeginNode()][i] = pList->getElement(i)->getWeight();
        matrix[pList->getElement(i)->getEndNode()][i] = -pList->getElement(i)->getWeight();
    }
}

Array<int> *IncidenceMatrixDirectedGraph::getNeighbours(int node) {
    auto *listOfNeighbours = new Array<int>();
    for (int i = 0; i < getNumberOfNodes(); i++) {
        if (findNoEdge(node, i) != -1)
            listOfNeighbours->addAtEnding(new int(i));
    }
    return listOfNeighbours;
}

int IncidenceMatrixDirectedGraph::getEdge(int beginNode, int endNode) {
    int w = IncidenceMatrixDirectedGraph::findNoEdge(beginNode, endNode);
    if (w >= 0)
        return matrix[beginNode][w];
    else return -1;
}

ArrayList<Edge> *IncidenceMatrixDirectedGraph::convertIntoListOfEdges() {
    auto *list = new ArrayList<Edge>();
    int bn, en;
    for (int j = 0; j < getNumberOfEdges(); j++) {
        int i;
        for (i = 0; i < getNumberOfNodes(); i++) {
            if (matrix[i][j] > 0) {
                bn = i;
                i++;
                break;
            }
        }
        for (; i < getNumberOfNodes(); i++) {
            if (matrix[i][j] < 0)
                en = i;
        }

        list->addAtBeginning(new Edge(bn, en, matrix[bn][j]));
    }
    return list;
}