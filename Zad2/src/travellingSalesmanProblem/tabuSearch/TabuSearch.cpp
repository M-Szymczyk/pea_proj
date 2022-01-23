//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#include <algorithm>
#include "TabuSearch.h"
#include <iostream>
#include <utility>
#include <windows.h>

TabuSearch::TabuSearch(Graph *graph, int startNode, std::mt19937 gen, int term, int divTerm,
                       int iter, int opt, int maxTime) :
        graph(graph), startTerm(term), dividerOfTerm(divTerm), iterations(iter), optimum(opt),
        startNode(startNode){
    ///generowanie początkowej ścieżki
    globalPath = Path(graph,startNode);

    std::vector<int> path;
    path.push_back(startNode);
    for (int i = 0; i < graph->getNumberOfNodes(); i++) {
        if (i != startNode) {
            path.push_back(i);
        }
    }
    path.push_back(startNode);
    using namespace std;
    long long int frequency;
    QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);
    long long start = read_QPC();
   do {
    std::shuffle(path.begin() + 1, path.end() - 1, gen);
    run(path);

//        std::cout<<std::endl;
//        for (auto p: path)
//            std::cout << p << " ";
    } while ((read_QPC() - start)/ frequency < maxTime);
}
long long int TabuSearch::read_QPC() {
    LARGE_INTEGER count;
    QueryPerformanceCounter(&count);
    return ((long long int) count.QuadPart);
}
void TabuSearch::run(std::vector<int> path) {
    ///inicjalizacja lokalnych danych
    Path localPath = Path(std::move(path),graph);
    bool aspirationCriteria = false;
    Move move(0, 0, -1);
    int i = 0,j=0;

    while (i != iterations) {
        ///szukanie najlepszego sasiada
        auto bestNeighbour = findBestNeighbour(localPath.path, localPath.weight);
        ///dekrementacja kadencji w tabuliscie
        decrementTermInTabuList();
        std::swap(localPath.path.at(bestNeighbour.at(1)), localPath.path.at(bestNeighbour.at(0)));
        if (localPath.weight < globalPath.weight) {
            i = 0;
            globalPath= localPath;
            aspirationCriteria = true;
            //showPRD(j,move);
        } else
            i++;
        /// dodanie ruchu do tabu listy
        localPath.weight += bestNeighbour.at(2);
        move.fsNeighbour = localPath.path.at(bestNeighbour.at(1));
        move.scNeighbour = localPath.path.at(bestNeighbour.at(0));
        if (!aspirationCriteria)//wybor kadencji
            move.term = startTerm;
        else
            move.term = startTerm / dividerOfTerm;
        tabuList.insert(std::make_pair(move.hashCode(), move));
        aspirationCriteria = false;

        j++;
    }
    //showPRD(j);
}

/**
 * metoda szuka najtańszej zamiany sąsiadów,po znalezieniu mniejszej zamiany wagi rozpatruje się dodatkowe aspekty:
 *  jeżeli zamiana jest na tabu liście to
 *      jeżeli zamiana zmniejsza wagę globalnej ścieżki przyjmuje się takie rozwiązanie,
 *      w przeciwnym wypadku odrzuca się takie rozwianie
 *  natomiast jeżeli zamiany nie ma na tabu liście to przyjmuje się taką zamianę
 * @param locPath ścieżka dla której szukana jest najlepsza zamiana sąsiadowi
 * @param locCost obecna waga ścieżki
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
                    if (locCost + currentCost > globalPath.weight)
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

void TabuSearch::showPRD(int iter,Move move) const {
    std::cout <<"\ncalkowita liczba iteracji: "<< iter << "  waga: " << globalPath.weight << " pop.wyniku:  "
              << 100 * (((float) (globalPath.weight - optimum)) / (float) optimum)
              << "%   tabuListSize: "<<tabuList.size() << " rep:"<<move.fsNeighbour<<", "<<move.scNeighbour;
}

/**
 * Metoda oblicza zmianę wagi ścieżki po zamianie sąsiadów pod danym indeksem
 * @param path obecna ścieżka
 * @param i @param j indeksy sąsiadów
 * @return zmianę wagi ścieżki
 */
int TabuSearch::countDifferenceIfSwapNeighbours(const std::vector<int> &path, int i, int j) {

    int subtractOldEdges = 0;
    int addNewEdges = 0;
    if (j - i == 1) {//sąsiednie wierzchołki
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
 * metoda sprawdza czy rozwiązanie znajduje się na tabu liście
 * @param i indeks pierwszego sąsiada
 * @param j indeks drugiego rozwiązania
 * @param path obecna lokalna ścieżka
 * @return true jeżeli rozwiązania nie ma na tabu liście
 */
bool TabuSearch::checkAspirationCriteria(int i, int j, const std::vector<int> &path) {
    int node1 = path.at(i), node2 = path.at(j);
    long long m1 =Move(node1, node2, 0).hashCode(), m2=Move(node2, node1, 0).hashCode();
    return tabuList.find(m1) == tabuList.end()
           || tabuList.find(m2) == tabuList.end();
}

/**
 * metoda dekrementuje kadencje każdego elementu tabu listy,
 * jeżeli kadencja == 0 to dany element jest usuwany z tabu listy
 */
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
