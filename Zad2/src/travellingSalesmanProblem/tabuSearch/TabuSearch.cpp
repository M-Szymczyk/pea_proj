//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#include <algorithm>
#include "TabuSearch.h"

TabuSearch::TabuSearch(Graph *graph, int startNode, std::mt19937 gen, int term, int divTerm, int iter, int opt,
                       int maxTabuListSize) :
        graph(graph), startTerm(term), dividerOfTerm(divTerm), iterations(iter),optimum(opt),maxTabuListSize(maxTabuListSize) {
    std::vector<int> path;
    path.push_back(startNode);
    for (int i = 0; i < graph->getNumberOfNodes(); i++) {
        if (i != startNode)
            path.push_back(i);
    }
    std::uniform_int_distribution<> dist(1, 1000);
//    int a = dist(gen);
//    for (int i = 0; i < a; i++)
    //std::shuffle(path.begin() + 1, path.end(), gen);
    path.push_back(startNode);

    globalPath = Path(path, graph);
    run();

}

void TabuSearch::run() {
    ///inicjalizacja lokalnych danych
    Path localPath = globalPath;
    bool aspirationCriteria = false;
    Move move(0, 0, -1);
    int i = 0,j=0;
    long long hashCodeOfLastInsertedElement=0;

    while (i != iterations) {
        ///szukanie najlepszego sasiada
        auto bestNeighbour = findBestNeighbour(localPath.path, localPath.weight);
        ///dekrementacja kadencji w tabuliscie
        decrementTermInTabuList();
        /// dodanie ruchu do tabu listy
        localPath.weight += bestNeighbour.at(2);
        move.fsNeighbour = localPath.path.at(bestNeighbour.at(1));
        move.scNeighbour = localPath.path.at(bestNeighbour.at(0));
//        if( i > 230 ){
//            for(auto a:tabuList){
//                std::cout<<"\n fs: "<<a.second.fsNeighbour<<",sc: "<<a.second.scNeighbour<<", t:"<<a.second.term<<", hc:"<<a.first;
//            }
//            system("pause");
//        }
        if (!aspirationCriteria)
            move.term = startTerm;
        else
            move.term = startTerm / dividerOfTerm;
        int size = tabuList.size();
        if(tabuList.size()==maxTabuListSize){
            tabuList.erase(hashCodeOfLastInsertedElement);
        }
        tabuList.insert(std::make_pair(move.hashCode(), move));
        hashCodeOfLastInsertedElement = move.hashCode();
        std::swap(localPath.path.at(bestNeighbour.at(1)), localPath.path.at(bestNeighbour.at(0)));
        if (localPath.weight < globalPath.weight) {
            i = 0;
            globalPath= localPath;
            aspirationCriteria = true;
            //showPRD(j);
        } else
            i++;
        j++;
    }
    showPRD(j);
}

/**
 *
 * @param locPath
 * @return nowa wage; 1. sasiad, 2.sasiad
 */
std::vector<int> TabuSearch::findBestNeighbour(const std::vector<int> &locPath, int locCost) {
    std::vector<int> neighbours;
    int minCost = INT_MAX, currentCost;
    for (int i = 1; i < locPath.size() - 1; i++) {
        for (int j = i + 1; j < locPath.size() - 1; j++) {
            currentCost = countDifferenceIfSwapNeighbours(locPath, i, j);
            if (currentCost < minCost) {
                if (!checkAspirationCriteria(i, j, locPath))
                    if (locCost + currentCost >= globalPath.weight)
                        continue;
                minCost = currentCost;
                neighbours.clear();
                neighbours.push_back(j);
                neighbours.push_back(i);
                neighbours.push_back(minCost);
            }
        }
    }
    return neighbours;
}

void TabuSearch::showPRD(int iter, int i) const {
    std::cout <<"\ncalkowita liczba iteracji: "<< iter <<"i: "<< i<< "  waga: " << globalPath.weight << " pop.wyniku:  "
        << 100 * (((float) (globalPath.weight - optimum)) / (float) optimum)
        << "%   tabuListSize: "<<tabuList.size()<<" \n";
}

void TabuSearch::showPRD(int iter) const {
    std::cout <<"\ncalkowita liczba iteracji: "<< iter << "  waga: " << globalPath.weight << " pop.wyniku:  "
              << 100 * (((float) (globalPath.weight - optimum)) / (float) optimum)
              << "%   tabuListSize: "<<tabuList.size();
}

/// podliczanie zmiany kosztów po zamianie wierzchołków
int TabuSearch::countDifferenceIfSwapNeighbours(const std::vector<int> &path, int i, int j) {

    int subtractOldEdges = 0;
    int addNewEdges = 0;
    if (j - i == 1) {
        subtractOldEdges += graph->getEdge(path.at(i - 1), path.at(i));
        subtractOldEdges += graph->getEdge(path.at(i), path.at(j));
        subtractOldEdges += graph->getEdge(path.at(j), path.at(j + 1));

        addNewEdges += graph->getEdge(path.at(i - 1), path.at(j));
        addNewEdges += graph->getEdge(path.at(j), path.at(i));
        addNewEdges += graph->getEdge(path.at(i), path.at(j + 1));

    } else {
        subtractOldEdges += graph->getEdge(path.at(i - 1), path.at(i));
        subtractOldEdges += graph->getEdge(path.at(i), path.at(i + 1));
        subtractOldEdges += graph->getEdge(path.at(j - 1), path.at(j));
        subtractOldEdges += graph->getEdge(path.at(j), path.at(j + 1));

        addNewEdges += graph->getEdge(path.at(i - 1), path.at(j));
        addNewEdges += graph->getEdge(path.at(j), path.at(i + 1));
        addNewEdges += graph->getEdge(path.at(j - 1), path.at(i));
        addNewEdges += graph->getEdge(path.at(i), path.at(j + 1));
    }

    return addNewEdges - subtractOldEdges;
}

/**
 *
 * @param i
 * @param j
 * @param path
 * @return
 */
bool TabuSearch::checkAspirationCriteria(int i, int j, const std::vector<int> &path) {
    int node1 = path.at(i), node2 = path.at(j);
    long long m1 =Move(node1, node2, 0).hashCode(), m2=Move(node2, node1, 0).hashCode();
    return tabuList.find(m1) == tabuList.end()
           || tabuList.find(m2) == tabuList.end();
/*//    for (std::vector<unsigned int> v: tabuList) {
//        if (v.at(0) == path->at(i) && v.at(1) == path->at(j))
//            return false;
//        if (v.at(0) == path->at(j) && v.at(1) == path->at(i))
//            return false;
//    }
//    return true;*/
}

void TabuSearch::decrementTermInTabuList() {
    auto tList = tabuList.begin();
    while (tList != tabuList.end()) {
        tList->second.term--;
        if (tList->second.term == 0) {
            tList = tabuList.erase(tList);
        }
        else
            tList++;
    }
}

const Path &TabuSearch::getGlobalPath() const {
    return globalPath;
}

Move::Move(int fsNeighbour, int scNeighbour, int sterm)
        : fsNeighbour(fsNeighbour), scNeighbour(scNeighbour), term(sterm) {}

long long Move::hashCode() const {
    return fsNeighbour * 10139 + scNeighbour * 17033 ;
}
