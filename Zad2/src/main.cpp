#include <iostream>
#include <windows.h>
#include <iomanip>
#include <string>
#include "graphRepresentation/Graph.h"
#include "graphRepresentation/matrix/AdjacencyMatrix.h"
#include "travellingSalesmanProblem/exhaustiveSearch/ExhaustiveSearch.h"
#include "travellingSalesmanProblem/held-Karp/HeldKarp.h"
#include "travellingSalesmanProblem/tabuSearch/TabuSearch.h"


class Data {
    std::string fileName, correctPath;
    int repetitions, correctWeight, term, divTerm, iterations;
public:
    Data(std::string fileName, std::string correctPath, int repetitions, int correctWeight, int ter, int div, int iter)
        : fileName(std::move(fileName)), correctPath(std::move(correctPath)), repetitions(repetitions),
        correctWeight(correctWeight), term(ter), divTerm(div), iterations(iter) {}

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

    int getTerm() const {
        return term;
    }

    int getDivTerm() const {
        return divTerm;
    }

    int getIterations() const {
        return iterations;
    }

    void display() const {
        std::cout << "\nNazwa instancji: " << getFileName() << ", liczba powtorzen: " << getRepetitions()
                  << ", poprawna waga sciezki: " << getCorrectWeight() <<", kadencja: "<< getTerm()
                  <<", dzielnik kadencji: "<<getDivTerm() <<", liczba iteracji: "<< getIterations()<< ", poprawna sciezka: "
                  << getCorrectPath();
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
                int repe, weight,term, divTerm, iterations;
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
                term = stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                divTerm= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                pos = s.find(delimiter);
                iterations= stoi(s.substr(0,pos));
                s.erase(0, pos + delimiter.length());
                data->addAtEnding(new Data(exampleFileName, s, repe, weight,term,divTerm,iterations));
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
        double time;

        //g->display();
        //std::cout << std::endl;
        TabuSearch *resTSP = nullptr;
/*//        if (g->getNumberOfNodes() < 1) {
//            TabuSearch *lastRepresentation = nullptr;
//            for (int i = 0; i < data->getRepetitions(); i++) {
//                for (int j = 0; j < 1000; j++) {
//                    start = read_QPC();
//                    resTSP = new TabuSearch(g, 0, gen, 1, 1, 10000, 0);
//
//                    elapsed = read_QPC() - start;
//                    time += (elapsed * 1000000) / frequency;//ns
//                    if (j == 999)
//                        lastRepresentation = resTSP;
//                    else
//                        delete resTSP;
//                    //system("pause");
//                }
//
//                time /= 1000;
//                writeToFile(file, time, data->getRepetitions(), lastRepresentation->getGlobalPath().path,
//                            lastRepresentation->getGlobalPath().weight, g->getNumberOfNodes() + 1);
//                printResult(time, lastRepresentation, nullptr);
//                delete lastRepresentation;
//
//            }
//        } else {*/
        for (int i = 0; i < data->getRepetitions(); i++) {
            start = read_QPC();
            resTSP = new TabuSearch(g, 0, gen, data->getTerm(), data->getDivTerm(),
                                    data->getIterations(), data->getCorrectWeight(), 900);
            elapsed = read_QPC() - start;
            time = (elapsed * 1000000) / frequency;//ns
            writeToFile(file, time, data->getRepetitions(), resTSP->getGlobalPath().path,
                        resTSP->getGlobalPath().weight, g->getNumberOfNodes() + 1);
            printResult(time, resTSP, g, data->getCorrectWeight());
            delete resTSP;
        }
        // }
        delete g;
    }

    static void printResult(double time, const TabuSearch *resTSP, AdjacencyMatrix *pMatrix, int opt) {
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
};

int main() {
    std::default_random_engine rd(time(0));
    std::mt19937 gen(rd());


    std::cout << "\nProblem komiwojazera - held-karp algorytm ";
    using namespace std;
    int **matrix = new int *[4];
    int matrixData[4][4] = {{0,  11, 7,  123},
                            {14, 0,  8,  6},
                            {1,  17, 0,  15},
                            {3,  15, 15, 0}
    };

    for (int i = 0; i < 4; i++)
        matrix[i] = new int[4];
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            matrix[i][j] = matrixData[i][j];

    //Graph *graph = new AdjacencyMatrix(4, matrix);
//    graph->display();
//    HeldKarp(graph,0).display();
//    auto res = ExhaustiveSearch(graph,0);
//    cout<<endl<<"Waga: 0"<<res.getWeight()<<" ";
//    for(int i =0;i<graph->getNumberOfNodes()+1;i++)
//        cout<<res.getPath()[i]<<" ";
//    std::string file = "tsp_6.txt";
//    auto graph =new  AdjacencyMatrix(file );
//    auto r = TabuSearch(graph, 0, gen, 1, 1, 10000);
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
