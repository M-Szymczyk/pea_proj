//
// Created by Mikolaj Szymczyk on 13.12.2021.
//

#ifndef PEAZAD2_TABUSEARCH_H
#define PEAZAD2_TABUSEARCH_H
#include "vector"
#include "../../graphRepresentation/Graph.h"
#include "../Path.h"
class Move{
public:
    int term, fsNeighbour, scNeighbour;
    Move(int fsNeighbour, int scNeighbour, int term);
    long long hashCode() const;
};

class TabuSearch {
    Graph *graph{};
    Path globalPath = Path(0, std::vector<int>(), nullptr);
    std::unordered_map<long long,Move> tabuList;
    int startTerm,dividerOfTerm,iterations, optimum,  maxTabuListSize;
public:
    const Path &getGlobalPath() const;

    void run();

    TabuSearch(Graph *graph, int startNode, std::mt19937 gen, int term, int divTerm, int iter, int opt,
               int maxTabuListSize);

    std::vector<int> findBestNeighbour(const std::vector<int> &locPath, int locCost);

    int countDifferenceIfSwapNeighbours(const std::vector<int> &path, int i, int j);

    bool checkAspirationCriteria(int i, int j, const std::vector<int> &path);

    void decrementTermInTabuList();

    void showPRD(int iter, int i) const;

    void showPRD(int iter) const;
};


#endif //PEAZAD2_TABUSEARCH_H
