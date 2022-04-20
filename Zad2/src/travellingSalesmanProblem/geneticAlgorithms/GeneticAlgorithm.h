//
// Created by Mikolaj Szymczyk on 23.01.2022.
//

#ifndef PEAZAD2_GENETICALGORITHM_H
#define PEAZAD2_GENETICALGORITHM_H

#include "../Path.h"

class GeneticAlgorithm {
    Graph *graph;
    double probability;
    int startNode, populationSize, noPopulationCopy, noGeneration, selectionType, crossoverType, optimum,inheritanceType,
    startAge;
    std::vector< Path> population;
    Path resultPath = Path(INT_MAX, std::vector<int>(), nullptr);
    std::mt19937 engine;
    int lastGenChange, lastIterationChange;
public:
    GeneticAlgorithm(Graph *g, int startNode, std::mt19937 gen, int populationSize, double probability,
                     int populationCopyNumber, int generationNumber, int selectionType, int crossoverType, int optimum,
                     int inheritanceType,int startAge);

    void run();

    Path getGlobalPath();

    void fillPopulation();

    std::vector<Path> tournamentSelection();

    std::vector<Path> rankSelection();

    static std::vector<int> copySequenceFromOrderCrossoverOperator
        (const std::vector<int> &parent, std::vector<int> &child, const int &fsPointOfCross, const int &scPointOfCross);

    std::vector<std::vector<int>>
    makeOrderCrossoverOperator(const std::vector<int> &parent1, const std::vector<int> &parent2);

    int countDifferenceIfSwapNeighbours(const std::vector<int> &path, int i, int j);

    Path  makeMutationRandomly(Path &path);

    Path  makeMutationBest(Path &path);

    Path  mutate(Path &child);

    void copyPopulation(int number);

    static bool cmp(Path &a, Path &b);

    void showPRD(int gen, int iter) const;



    static std::vector<int>
    completeRestPoints(int firstNoCopied, const std::vector<int> &parent, std::vector<int> &child, int a, int b);

    static std::vector<int> copyCutPoints(const std::vector<int> &parent, std::vector<int> &child, int a, int b, int i,
                                          int &firstNoCopied);

    std::vector<std::vector<int>>
    makePartiallyMappedCrossover(const std::vector<int> &parent1, const std::vector<int> &parent2);

    void addToPopulation(Path &p, int gen, int iter);

    static bool cmp2(Path &a, Path &b);
};


#endif //PEAZAD2_GENETICALGORITHM_H
