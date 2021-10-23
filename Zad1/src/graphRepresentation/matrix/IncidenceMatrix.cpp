//
// Created by Mikolaj Szymczyk on 29.05.2021.
//

#include <iostream>
#include "IncidenceMatrix.h"


void IncidenceMatrix::display() {
    using namespace std;

    cout << endl;
    for (int i = 0; i < getNumberOfNodes(); i++) {
        cout << endl << i << ".  :";
        for (int j = 0; j < getNumberOfEdges(); j++) {
            cout << matrix[i][j];
            if (j != getNumberOfEdges() - 1)
                cout << ", ";
        }

    }
}

void IncidenceMatrix::addEdge(int beginNode, int endNode, int weight) {
    int w = findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = weight;
    matrix[endNode][w] = weight;
    incrementNumberOfEdges();
}

IncidenceMatrix::~IncidenceMatrix() {

    for (int i = 0; i < getNumberOfNodes(); i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

IncidenceMatrix::IncidenceMatrix(int noNodes, int noEdges) : Graph(noNodes) {
    setNumberOfEdges(noEdges);
    matrix = new int *[getNumberOfNodes()];
    for (int i = 0; i < getNumberOfNodes(); i++) {
        matrix[i] = new int[getNumberOfEdges()];
    }
    for (int i = 0; i < (getNumberOfNodes()); i++) {
        for (int j = 0; j < getNumberOfEdges(); j++) {
            matrix[i][j] = 0;
        }
    }
}

void IncidenceMatrix::removeEdge(int beginNode, int endNode) {
    int w = findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = 0;
    matrix[endNode][w] = 0;
    decrementNumberOfEdges();
}

int IncidenceMatrix::getEdge(int beginNode, int endNode) {
    int w = findNoEdge(beginNode, endNode);
    if (w >= 0)
        return matrix[beginNode][w];
    else return -1;
}

IncidenceMatrix::IncidenceMatrix(int noNodes, double density, std::random_device &rd, int max) : Graph(noNodes) {
    setNumberOfEdges((((noNodes * (noNodes - 1)) / 2) * density));
    setNumberOfNodes(noNodes);
    matrix = new int *[getNumberOfNodes()];
    for (int i = 0; i < getNumberOfNodes(); i++) {
        matrix[i] = new int[getNumberOfEdges()];
    }
    for (int i = 0; i < getNumberOfNodes(); i++) {
        for (int j = 0; j < getNumberOfEdges(); j++) {
            matrix[i][j] = 0;
        }
    }
    std::uniform_int_distribution<> dist(1, max);
    std::uniform_int_distribution<> distNode(0, noNodes - 1);

    int i, weight;
    for (i = 1; i < noNodes; i++) {
        weight = dist(rd);
        matrix[0][i - 1] = weight;
        matrix[i][i - 1] = weight;
    }

    bool b1, b2;
    while (i < getNumberOfEdges()) {
        weight = dist(rd);
        int edgeBegin, edgeEnd;
        do {
            edgeBegin = distNode(rd), edgeEnd = distNode(rd);
            b1 = edgeBegin != edgeEnd;
            b2 = IncidenceMatrix::getEdge(edgeBegin, edgeEnd) == -1;
        } while (!(b1 && b2));
        matrix[edgeBegin][i - 1] = weight;
        matrix[edgeEnd][i - 1] = weight;
        i++;
    }
}

ArrayList<Edge> *IncidenceMatrix::convertIntoSortedListOfEdges() {
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
            if (matrix[i][j] > 0)
                en = i;
        }

        list->addEdgeToList(new Edge(bn, en, matrix[bn][j]));
    }
    return list;
}

Array<int> *IncidenceMatrix::getNeighbours(int node) {
    auto *listOfNeighbours = new Array<int>();
    for (int i = 0; i < getNumberOfNodes(); i++) {
        if (findNoEdge(node, i) != -1 && node != i)
            listOfNeighbours->addAtEnding(new int(i));
        else if (findNoEdge(i, node) != -1 && node != i)
            listOfNeighbours->addAtEnding(new int(i));
    }
    return listOfNeighbours;
}

int IncidenceMatrix::findNoEdge(int bN, int eN) {
    for (int i = 0; i < getNumberOfEdges(); i++) {
        if (matrix[bN][i] > 0) {
            if (matrix[eN][i] > 0)
                return i;
        }
    }
    return -1;
}

IncidenceMatrix::IncidenceMatrix(std::string &fileName) {
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
                    matrix[tab[1]][i] = tab[2];
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

IncidenceMatrix::IncidenceMatrix(ArrayList<Edge> *pList, int noNodes) {
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
    auto p = pList->getFirstElement();
    for (int i = 0; i < pList->getSize(); i++) {
        matrix[p->getElement()->getBeginNode()][i] = p->getElement()->getWeight();
        matrix[p->getElement()->getEndNode()][i] = p->getElement()->getWeight();
        p = p->getNextElement();
    }
}

IncidenceMatrix::IncidenceMatrix() {
    matrix = nullptr;
}

ArrayList<Edge> *IncidenceMatrix::convertIntoListOfEdges() {
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
            if (matrix[i][j] > 0)
                en = i;
        }

        list->addAtBeginning(new Edge(bn, en, matrix[bn][j]));
    }
    return list;
}

void IncidenceMatrix::setWeightOfEdge(int beginNode, int endNode, int weight) {
    int w = findNoEdge(beginNode, endNode);
    matrix[beginNode][w] = weight;
    matrix[endNode][w] = weight;
}
