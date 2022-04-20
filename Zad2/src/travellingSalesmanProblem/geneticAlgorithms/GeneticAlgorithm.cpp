#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
//
// Created by Mikolaj Szymczyk on 23.01.2022.
//
#include <algorithm>
#include <vector>
#include "GeneticAlgorithm.h"

GeneticAlgorithm::GeneticAlgorithm(Graph *g, int startNode, std::mt19937 gen, int populationSize, double probability,
                                   int populationCopyNumber, int generationNumber, int selectionType, int crossoverType,
                                   int optimum, int inheritanceType, int startAge)
        : graph(g), startNode(startNode), populationSize(populationSize), engine(gen), probability(probability),
          noPopulationCopy(populationCopyNumber), noGeneration(generationNumber), selectionType(selectionType),
          crossoverType(crossoverType), optimum(optimum), lastIterationChange(0), lastGenChange(0),
          inheritanceType(inheritanceType), startAge(startAge) {
    fillPopulation();
    run();
}

Path GeneticAlgorithm::getGlobalPath() {
    return resultPath;
}

void GeneticAlgorithm::run() {
    for (int i = 0; i < noGeneration; i++) {
        for (int j = 0; j < populationSize; j++) {
            std::vector<Path> resultOfSelection;
            switch (selectionType) {
                case 0:
                    resultOfSelection = tournamentSelection();
                    break;
                case 1:
                    sort(population.begin(), population.end(), cmp);
                    resultOfSelection = rankSelection();
                    break;
                default:
                    std::cout << "\nBlad brak podanego typu selekcji! Podano: " << selectionType;
                    return;
            }
            auto p1 = resultOfSelection.at(1).path, p2 = resultOfSelection.at(0).path;
            p1.pop_back();
            p2.pop_back();
            p1.erase(p1.begin());
            p2.erase(p2.begin());
            std::vector<std::vector<int>> resultOfCrossover;
            switch (crossoverType) {
                case 0:
                    resultOfCrossover = makeOrderCrossoverOperator(p1, p2);
                    break;
                case 1:
                    resultOfCrossover = makePartiallyMappedCrossover(p1, p2);
                    break;
                default:
                    std::cout << "\nBlad brak podanego typu krzyzowania! Podano: " << crossoverType;
                    return;
            }
            resultOfCrossover.at(1).push_back(startNode);
            resultOfCrossover.at(0).push_back(startNode);
            resultOfCrossover.at(1).insert(resultOfCrossover.at(1).begin(), startNode);
            resultOfCrossover.at(0).insert(resultOfCrossover.at(0).begin(), startNode);
            Path childPath1(resultOfCrossover.at(1), graph);
            Path childPath2(resultOfCrossover.at(0), graph);
            addToPopulation(childPath1, i, j);
            addToPopulation(childPath2, i, j);
            auto mutation1Result = mutate(childPath1);
            auto mutation2Result = mutate(childPath2);
            if (mutation1Result.weight != -100)
                addToPopulation(mutation1Result, i, j);
            if (mutation2Result.weight != -100)
                addToPopulation(mutation2Result, i, j);
        }
        switch (inheritanceType) {
            case 0:
                sort(population.begin(), population.end(), cmp);
                break;
            case 1:
                sort(population.begin(), population.end(), cmp2);
                for (auto pop: population)
                    pop.decAge();
                break;
            default:
                std::cout << "\nBlad brak podanego typu dziedziczenia! Podano: " << inheritanceType;
                return;

        }
        copyPopulation(noPopulationCopy);
    }
    //showPRD(lastGenChange, lastIterationChange);
}

void GeneticAlgorithm::addToPopulation(Path &p, int gen, int iter) {
    if (inheritanceType == 1)
        p.setAge(startAge);
    population.push_back(p);

    if (resultPath.weight > p.weight) {
        resultPath = p;
        lastGenChange = gen;
        lastIterationChange = iter;
    }
}

/**
 * Metoda generuję początkową populację
 */
void GeneticAlgorithm::fillPopulation() {
    for (int i = 0; i < noPopulationCopy; i++) {
        Path p = Path(graph, startNode);
        p.shuffle(engine);
        population.push_back(p);
        if (p.weight < resultPath.weight) {
            resultPath = p;
        }
    }
}

/**
 *
 * @return
 */
std::vector<Path> GeneticAlgorithm::tournamentSelection() {
    std::uniform_int_distribution<> randomParent(0, static_cast<int>(population.size()) - 1);
    std::vector<Path> tournamentWinner;
    for (int i = 0; i < 2; ++i) {
        int aIndex = randomParent(engine), bIndex = randomParent(engine);
        Path a = population.at(aIndex), b = population.at(bIndex);

        if (a.weight > b.weight)
            tournamentWinner.push_back(b);
        else
            tournamentWinner.push_back(a);
    }
    return tournamentWinner;

}

// wybieranie rodziców do krzyżowania na podstawie selekcji rankingowej
std::vector<Path> GeneticAlgorithm::rankSelection() {
    std::vector<float> fitness(population.size(), 0);
    float sum = 0;
    for (int i = 0; i < population.size(); i++) {
        fitness.at(i) = (float) (population.size() - i) / (population.size() * (population.size() - 1));
        sum += fitness.at(i);
    }

    std::uniform_real_distribution<float> randomNumber(0, sum);
    float r = randomNumber(engine), r2 = randomNumber(engine);

    if (r2 < r)
        std::swap(r2, r);

    int fsIndex = -1, scIndex = 0;
    float sum2 = 0;

    for (int i = 0; i < population.size(); i++) {
        sum2 += fitness.at(i);
        if (r <= sum2 && fsIndex == -1)
            fsIndex = i;
        else if (r2 <= sum2) {
            scIndex = i;
            break;
        }
    }
    std::vector<Path> parents;
    parents.push_back(population.at(scIndex));
    parents.push_back(population.at(fsIndex));
    return parents;

}

std::vector<std::vector<int>> GeneticAlgorithm::makeOrderCrossoverOperator
        (const std::vector<int> &parent1, const std::vector<int> &parent2) {
    std::vector<int> child1(parent1.size(), 0), child2(parent1.size(), 0);
    std::vector<std::vector<int>> result;
    std::uniform_int_distribution<> cutPoints(1, static_cast<int>(parent1.size()) - 2);
    int fsPointOfCross = 0, scPointOdCross = 0;

    //losowanie punktów przecięcia
    while (fsPointOfCross == scPointOdCross) {
        fsPointOfCross = cutPoints(engine);
        scPointOdCross = cutPoints(engine);
    }
    if (fsPointOfCross > scPointOdCross)
        std::swap(fsPointOfCross, scPointOdCross);

    //kopiowanie obszarów przecięcia
    for (int i = fsPointOfCross; i <= scPointOdCross; i++) {
        child1.at(i) = parent1.at(i);
        child2.at(i) = parent2.at(i);
    }

    result.push_back(copySequenceFromOrderCrossoverOperator(parent2, child1, fsPointOfCross, scPointOdCross));
    result.push_back(copySequenceFromOrderCrossoverOperator(parent1, child2, fsPointOfCross, scPointOdCross));

    return result;
}

// kopiowanie pozostałych wierzchołków sekwencji, które nie były w wyciętym obszarze
std::vector<int> GeneticAlgorithm::copySequenceFromOrderCrossoverOperator
        (const std::vector<int> &parent, std::vector<int> &child, const int &fsPointOfCross,
         const int &scPointOfCross) {
    int copyDestination = scPointOfCross + 1;
    auto pointerToValToCopy = parent.begin() + scPointOfCross + 1;

    do {
        if (find(child.begin() + fsPointOfCross, child.begin() + scPointOfCross + 1, *pointerToValToCopy)
            == child.begin() + scPointOfCross + 1) {
            child.at(copyDestination) = *pointerToValToCopy;
            if (pointerToValToCopy != parent.end() - 1)
                pointerToValToCopy++;
            else
                pointerToValToCopy = parent.begin();
        } else {
            if (pointerToValToCopy != parent.end() - 1)
                pointerToValToCopy++;
            else
                pointerToValToCopy = parent.begin();
            continue;
        }

        if (copyDestination < child.size() - 1)
            copyDestination++;
        else
            copyDestination = 0;

    } while (copyDestination != fsPointOfCross);

    return child;
}

// metoda krzyżowania - Partially Mapped Crossover
std::vector<std::vector<int>> GeneticAlgorithm::makePartiallyMappedCrossover(const std::vector<int> &parent1,
                                                                             const std::vector<int> &parent2) {
    std::uniform_int_distribution<> cutPoints(1, graph->getNumberOfNodes() - 1);
    std::vector<int> child1(parent1.size()-1, 0), child2(parent1.size()-1, 0);

    int a = 0, b = 0;

    //losowanie punktów przecięcia
    while (a == b || a > b) {
        a = cutPoints(engine);
        b = cutPoints(engine);
    }

    //kopiowanie obszarów przecięcia
    for (int i = a; i <= b; i++) {
        child1.at(i) = parent2.at(i);
        child2.at(i) = parent1.at(i);
    }

    int firstNoCopied1 = a, firstNoCopied2 = a;
    for (int i = 0; i < parent1.size()-1; i++) {
        if (i < a || i > b) {
            child1 = copyCutPoints(parent1, child1, a, b, i, firstNoCopied1);
            child2 = copyCutPoints(parent2, child2, a, b, i, firstNoCopied2);
        }
    }

    child1 = completeRestPoints(firstNoCopied1, parent2, child1, a, b);
    child2 = completeRestPoints(firstNoCopied2, parent1, child2, a, b);

    std::vector<std::vector<int>> vectors;
    vectors.push_back(child1);
    vectors.push_back(child2);
    return vectors;
}

// kopiowanie punktów cięcia
std::vector<int> GeneticAlgorithm::copyCutPoints(const std::vector<int> &parent, std::vector<int> &child,
                                                 int a, int b, int i, int &firstNoCopied) {

    unsigned int vertex = (parent.begin() + i).operator*();
    if (find(child.begin() + a, child.begin() + b + 1, vertex) == child.begin() + b + 1)
        (child.begin() + i).operator*() = vertex;
    else if (firstNoCopied == a)
        firstNoCopied = i;

    return child;
}

// uzupełnianie wektora o pozostałe wierzchołki, które nie zostały przekopiowane w obszarze kopiowania
std::vector<int> GeneticAlgorithm::completeRestPoints(const int firstNoCopied, const std::vector<int> &parent,
                                                      std::vector<int> &child,
                                                      int a, int b) {
    for (unsigned int i = firstNoCopied; i < child.size(); i++) {
        if (i < a || i > b) {
            for (int j = 0; j < parent.size(); j++) {
                if (j < a || j > b) {
                    if (find(child.begin(), child.end(), parent.at(j)) == child.end()) {
                        (child.begin() + i).operator*() = parent.at(j);
                        break;
                    }
                }
            }
        }
    }
    return child;
}

// sprawdzenie na podstawie prawdopodobieństwa czy może zajść mutacja
Path GeneticAlgorithm::mutate(Path &child) {
    std::uniform_real_distribution<double> generateProbability(0.0, 1.0);
    double genProb = generateProbability(engine);
    Path result(-100, std::vector<int>(), graph);
    if (genProb <= 0.1)
        return makeMutationRandomly(child);
    else if (genProb <= probability)
        return makeMutationBest(child);
    else
        return result;
}

// wyznaczanie najlepszej mutacji
Path GeneticAlgorithm::makeMutationBest(Path &path) {
    int bestI, bestJ, bestSwap = INT_MAX;
    int swapValue;

    for (int i = 1; i < path.path.size() - 3; i++) {
        for (int j = i + 1; j < path.path.size() - 2; j++) {
            swapValue = countDifferenceIfSwapNeighbours(path.path, i, j);
            if (swapValue < bestSwap) {
                bestI = i;
                bestJ = j;
                bestSwap = swapValue;
            }
        }
    }
    std::swap(path.path.at(bestI), path.path.at(bestJ));
    path.weight += bestSwap;
    return path;
}

// wyznaczanie mutacji losowo
Path GeneticAlgorithm::makeMutationRandomly(Path &path) {
    std::uniform_int_distribution<> swapIndex(1, path.path.size() - 2);

    int i = 0, j = 0;

    while (i == j) {
        i = swapIndex(engine);
        j = swapIndex(engine);
    }

    if (j < i)
        std::swap(i, j);

    path.weight += countDifferenceIfSwapNeighbours(path.path, i, j);
    std::swap(path.path.at(i), path.path.at(j));
    return path;
}

/**
 * Metoda oblicza zmianę wagi ścieżki po zamianie sąsiadów pod danym indeksem
 * @param path obecna ścieżka
 * @param i @param j indeksy sąsiadów
 * @return zmianę wagi ścieżki
 */
int GeneticAlgorithm::countDifferenceIfSwapNeighbours(const std::vector<int> &path, int i, int j) {

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

void GeneticAlgorithm::showPRD(int gen, int iter) const {
    std::cout << "\ngen: " << gen << ", iter: " << iter
              << " wei: "
              << resultPath.weight
              << "   blad: "
              << 100 * (((float) (resultPath.weight - optimum))
                        / (float) optimum)
              << "% ";
    // resultPath.display();
}

// kopiowanie najlepszych jednostek z populacji do nowej generacji
void GeneticAlgorithm::copyPopulation(int number) {
    std::vector<Path> population2;
    for (int i = 0; i < number; i++) population2.push_back(population.at(i));
    population = population2;
}

bool GeneticAlgorithm::cmp(Path &a, Path &b) {
    return a.weight < b.weight;
}

bool GeneticAlgorithm::cmp2(Path &a, Path &b) {

    if (a.age == b.age)
        return a.weight < b.weight;
    return a.age < b.age;
}