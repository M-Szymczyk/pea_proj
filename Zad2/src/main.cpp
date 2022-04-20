#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include "graphRepresentation/Graph.h"
#include "graphRepresentation/matrix/AdjacencyMatrix.h"
#include "travellingSalesmanProblem/exhaustiveSearch/ExhaustiveSearch.h"
#include "travellingSalesmanProblem/held-Karp/HeldKarp.h"
#include "travellingSalesmanProblem/tabuSearch/TabuSearch.h"
#include "travellingSalesmanProblem/geneticAlgorithms/GeneticAlgorithm.h"


class Data {
    std::string fileName, correctPath;
    int repetitions, correctWeight, popSize, popCopy, noGeneration, selectionType, crossOverType, inheritanceType,
    startAge;
    double probability;
public:
    Data(std::string fileName, std::string correctPath, int repetitions, int correctWeight,int  popSize,int popCopy,
         int noGeneration,int selectionType, int crossOverType, double probability,int inheritanceType,
         int startAge)
        : fileName(std::move(fileName)), correctPath(std::move(correctPath)), repetitions(repetitions),
        correctWeight(correctWeight),popSize(popSize), popCopy(popCopy), noGeneration(noGeneration),
        selectionType(selectionType), crossOverType(crossOverType), probability(probability),inheritanceType(inheritanceType),
            startAge(startAge) {}

    const std::string &getFileName() const {
        return fileName;
    }

    const std::string &getCorrectPath() const {
        return correctPath;
    }

    int getRepetitions() const {
        return repetitions;
    }

    int getCorrectWeight() const {
        return correctWeight;
    }


    int getPopSize() const {
        return popSize;
    }

    int getPopCopy() const {
        return popCopy;
    }

    int getNoGeneration() const {
        return noGeneration;
    }

    int getSelectionType() const {
        return selectionType;
    }

    int getCrossOverType() const {
        return crossOverType;
    }

    double getProbability() const {
        return probability;
    }

    int getInheritanceType() const {
        return inheritanceType;
    }

    int getStartAge() const {
        return startAge;
    }

    void display() const {
        std::cout << "\nNazwa instancji: " << getFileName() << ", l. powtorzen: " << getRepetitions()
                  << ", poprawna wynik: " << getCorrectWeight() <<", rozmiar populacji: "<<getPopSize()
                  <<", liczba dzieci: "<<getPopCopy()<<", l.generacji: "<< getNoGeneration()<<", typ selekcji: "
                  <<getSelectionType()<<", typ krzyzowania: "<<getCrossOverType()<<", praw. mutacji:"<<getProbability()
                  <<", poprawna sciezka: "<< getCorrectPath();
        std::cout << "\nCzas dzialania algorytmu | Waga sciezki  | % poprawnego wyniku| Otrzymana sciezka ";
    }

    static std::basic_string<char> readData(ArrayList<Data> *data) {
        using namespace std;
        ifstream file;
        std::string inputFileName = ".INI";
        file.open(inputFileName.c_str());
        while (!file.eof()) {
            if (file.is_open()) {

                string s, exampleFileName;
                int repe, weight, popSize, PopCopy, noGen,typeSelec, typeCross,inheritanceType, startAge;
                double probl;
                getline(file, s);

                if (file.fail() || s.empty())
                    return "";

                size_t pos;
                std::string delimiter = " ", outfileEnd = ".csv";
                pos = s.find(delimiter);

                if (pos > 100)
                    return s;

                exampleFileName = s.substr(0, pos);
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                repe = stoi(s.substr(0, pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                weight = stoi(s.substr(0, pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                popSize = stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                PopCopy= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                noGen= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                typeSelec= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                typeCross= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                probl= stod(s.substr(0,pos))/100;
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                inheritanceType= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                startAge= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                data->addAtEnding(new Data(exampleFileName, s, repe, weight,popSize,
                                           PopCopy,noGen,typeSelec,typeCross,probl,inheritanceType, startAge));
            } else
                cout << "File error not opened  - OPEN" << endl;
        }
        file.close();
    }

    static void writeDataToFile(const std::string &outputFileName, Data *data) {
        using namespace std;
        fstream file;
        file.open(outputFileName, ios::app);
        if (!file) {
            cout << "\nBłąd oczytu pilku!";
        } else {
            file << data->getFileName() << " " << data->getRepetitions() << " "
                 << data->getCorrectWeight() << " " << data->getCorrectPath() << "" << endl;
        }
    }

};

class EngineTerminal {

    static long long int read_QPC() {
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        return ((long long int) count.QuadPart);
    }

    static void writeToFile(const std::string &outputFileName, long long array, int size, std::vector<int> resultPath,
                            int resultWeight, int pathSize) {
        using namespace std;
        fstream file;
        file.open(outputFileName, ios::app);
        if (!file) {
            cout << "\nBłąd oczytu pilku!";
        } else {

            file << array << ";" << resultWeight << "; [";
            for (auto p: resultPath)
                file << p << " ";
            file << " ]";
            file << endl;


            file.close();
        }
    }

public:
    static void automaticTest(const std::string &file, Data *data) {
        using namespace std;
        std::default_random_engine rd(time(0));
        std::mt19937 gen(rd());
        //to count time of operations
        long long int frequency, start, elapsed;
        QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);
        string inputFile = data->getFileName();
        auto *g = new AdjacencyMatrix(inputFile);
        double time, avgTime=0, avgMis=0;
        int avgWeight=0;

        GeneticAlgorithm *resTSP = nullptr;

        for (int i = 0; i < data->getRepetitions(); i++) {
            std::uniform_int_distribution<> startNode(0, g->getNumberOfNodes() - 1);
            start = read_QPC();
            resTSP = new GeneticAlgorithm(g,startNode(gen),gen,data->getPopSize(),data->getProbability(),data->getPopCopy(),
                                          data->getNoGeneration(),data->getSelectionType(),data->getCrossOverType(),
                                          data->getCorrectWeight(),data->getInheritanceType(),data->getStartAge());
            elapsed = read_QPC() - start;
            time = (elapsed * 1000000) / frequency;//ns
            writeToFile(file, time, data->getRepetitions(), resTSP->getGlobalPath().path,
                        resTSP->getGlobalPath().weight, g->getNumberOfNodes() + 1);
            printResult(time, resTSP, g, data->getCorrectWeight());
            avgTime+=time; avgWeight+=resTSP->getGlobalPath().weight;
            avgMis+=100 * (((float) (resTSP->getGlobalPath().weight -  data->getCorrectWeight())) / (float)  data->getCorrectWeight());
            delete resTSP;
        }
        printResult(avgTime/data->getRepetitions(),avgWeight/data->getRepetitions(),avgMis/data->getRepetitions());
        delete g;
    }

    static void printResult(double time, GeneticAlgorithm *resTSP, AdjacencyMatrix *pMatrix, int opt) {
        // std::cout << "\nCzas dzialania algorytmu: " << std::dec;
        std::cout << "\n";
        int i = 0;
        while (true) {
            if (time < 1000)
                break;
            time /= 1000;
            i++;
            if (i == 3)
                break;
        }
        if (time < 100)
            printf("        %3.2f", time);
        else
            printf("        %3.f", time);
        switch (i) {
            case 0:
                std::cout << " ns";
                break;
            case 1:
                std::cout << " ms";
                break;
            case 2:
                std::cout << " s";
                break;
            default:
                std::cout << "";
        }
        printf("       |       %3.d", resTSP->getGlobalPath().weight);
        std::cout << "     | " << 100 * (((float) (resTSP->getGlobalPath().weight - opt)) / (float) opt)
                  << "% |[";
//        for (auto p: resTSP->getGlobalPath().path)
//            std::cout << p << " ";

    }
    static void printResult(double time,int avgWeight , double avgMis) {
        std::cout << "\n-------------------------------------------------------------\n";
        // std::cout << "\nCzas dzialania algorytmu: " << std::dec;
        int i = 0;
        while (true) {
            if (time < 1000)
                break;
            time /= 1000;
            i++;
            if (i == 3)
                break;
        }

        printf("        %3.2f", time);
        switch (i) {
            case 0:
                std::cout << " ns";
                break;
            case 1:
                std::cout << " ms";
                break;
            case 2:
                std::cout << " s";
                break;
            default:
                std::cout << "";
        }

        printf("       |       %3.d", avgWeight);
        printf("       |       %3.2f", avgMis);

    }
};

int main() {
//    std::default_random_engine rd(time(0));
//    std::mt19937 gen(rd());

    std::random_device r;
    std::seed_seq seed{r(), r(), r(), r(), r(), r(), r(), r()};
    std::mt19937 gen(rand());
    std::cout << "\nProblem komiwojazera - algorytm genetyczny";
    using namespace std;

//    int **matrix = new int *[4];
//    int matrixData[4][4] = {{0,  11, 7,  123},
//                            {14, 0,  8,  6},
//                            {1,  17, 0,  15},
//                            {3,  15, 15, 0}
//    };
//
//    for (int i = 0; i < 4; i++)
//        matrix[i] = new int[4];
//    for (int i = 0; i < 4; i++)
//        for (int j = 0; j < 4; j++)
//            matrix[i][j] = matrixData[i][j];
//
//    Graph *graph = new AdjacencyMatrix(4, matrix);
   // graph->display();
    //HeldKarp(graph,0).display();
//    auto res = ExhaustiveSearch(graph,0);
//    cout<<endl<<"Waga: 0"<<res.getWeight()<<" ";
//    for(int i =0;i<graph->getNumberOfNodes()+1;i++)
//        cout<<res.getPath()[i]<<" ";
//    std::string file = "tsp_10.txt";
//    auto graph =new  AdjacencyMatrix(file );
//    auto result = GeneticAlgorithm(graph, 0, gen,10,0.3,10,
//                                   10,0,0,10);
//    std::cout<<"\nWaga:"<<r.getGlobalPath().weight<<" ";
//    for(auto res:r.getGlobalPath().path){
//        std::cout<<res<<" ";
//    }

    string outputFileName;
    auto data = new ArrayList<Data>;

    outputFileName = Data::readData(data);
    Data *poitData = nullptr;
    for (int i = 0; i < data->getSize(); i++) {
        poitData = data->getElement(i);
        cout << "\n-----------------------------------------------------------------------------";
//        int a[]={0,1,2,3};
//        ExhaustiveSearch::generatePermutation(a,4,4);
        poitData->display();
        poitData->writeDataToFile(outputFileName, data->getElement(i));
        EngineTerminal::automaticTest(outputFileName, data->getElement(i));
    }
    std::cout << std::endl;
    system("pause");
    return 0;
}
