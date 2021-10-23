#include <iostream>
#include <windows.h>
#include <iomanip>

#include "graphRepresentation/Graph.h"
#include "graphRepresentation/matrix/AdjacencyMatrix.h"
#include "travellingSalesmanProblem/exhaustiveSearch/ExhaustiveSearch.h"

std::string inputFileName = ".INI";

class Data {
    std::string fileName, correctPath;
    int repetitions, correctWeight;
public:
    Data(std::string fileName, std::string correctPath, int repetitions, int correctWeight) : fileName(std::move(
            fileName)), correctPath(std::move(correctPath)), repetitions(repetitions), correctWeight(correctWeight) {}

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

    void display() const {
        std::cout << "\nNazwa instancji: " << getFileName() << ", liczba powtorzen: " << getRepetitions()
                  << ", poprawna waga sciezki: " << getCorrectWeight() << ", poprawna sciezka: "
                  << getCorrectPath();
    }

    static std::basic_string<char> readData(ArrayList<Data> *data) {
        using namespace std;
        ifstream file;
        file.open(inputFileName.c_str());
        while (!file.eof()) {
            if (file.is_open()) {

                string s, exampleFileName;
                int repe, weight;
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
                data->addAtEnding(new Data(exampleFileName, s, repe, weight));
            } else
                cout << "File error not opened  - OPEN" << endl;
        }
        file.close();
    }

    void writeDataToFile(const std::string &outputFileName, Data *data) {
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

    static void
    writeToFile(const std::string &outputFileName, long long array, int size, int *resultPath,
                int resultWeight, int pathSize) {
        using namespace std;
        fstream file;
        file.open(outputFileName, ios::app);
        if (!file) {
            cout << "\nBłąd oczytu pilku!";
        } else {

            file << array << ";" << resultWeight << "; [";
            for (int j = 0; j < pathSize; j++)
                file << resultPath[j] << " ";
            file << " ]";
            file << endl;


            file.close();
        }
    }

public:
    static void automaticTest(const std::string &file, Data *data) {
        using namespace std;

        //to count time of operations
        long long int frequency, start, elapsed;
        QueryPerformanceFrequency((LARGE_INTEGER *) &frequency);

        string inputFile = data->getFileName();
        auto *g = new AdjacencyMatrix(inputFile);
        auto time = 0;

        //g->display();
        std::cout << std::endl;
        ExhaustiveSearch *resTSP = nullptr;
        if(g->getNumberOfNodes()<=10){
            for (int i = 0; i < data->getRepetitions(); i++) {
                start = read_QPC();
                resTSP = new ExhaustiveSearch(g, 0);

                elapsed = read_QPC() - start;
                time += (elapsed * 1000) / frequency;
            }
            time /= data->getRepetitions();
            writeToFile(file, time, data->getRepetitions(), resTSP->getPath(),
                        resTSP->getWeight(), g->getNumberOfNodes() + 1);
            cout << "\nCzas dzialania algorytmu: " << std::dec << time << " ms; Waga sciezki: "
                 << resTSP->getWeight() << "; Poprawna sciezka: [";//mikro sekundy
            for (int j = 0; j < g->getNumberOfNodes() + 1; j++)
                cout << resTSP->getPath()[j] << " ";
            cout << "]";
        }
        else {
            for (int i = 0; i < data->getRepetitions(); i++) {
                start = read_QPC();
                resTSP = new ExhaustiveSearch(g, 0);
                elapsed = read_QPC() - start;
                time = (elapsed * 1000) / frequency;

                writeToFile(file, time, data->getRepetitions(), resTSP->getPath(),
                            resTSP->getWeight(), g->getNumberOfNodes() + 1);
                cout << "\nCzas dzialania algorytmu: " << std::dec << time << " ms; Waga sciezki: "
                     << resTSP->getWeight() << "; Poprawna sciezka: [";//mikro sekundy
                for (int j = 0; j < g->getNumberOfNodes() + 1; j++)
                    cout << resTSP->getPath()[j] << " ";
                cout << "]";
            }
        }

        if (resTSP != nullptr) {
            delete resTSP;
            delete g;
        }
    }
};


int main() {
    std::cout << "\nProblem komiwojazera - metoda brute force";
    using namespace std;

    string outputFileName;
    auto data = new ArrayList<Data>;

    outputFileName = Data::readData(data);
    Data *poitData= nullptr;
    for (int i = 0; i < data->getSize(); i++) {
        poitData = data->getElement(i);
        cout << "\n-----------------------------------------------------------------------------";
//        int a[]={0,1,2,3};
//        ExhaustiveSearch::generatePermutation(a,4,4);
        poitData->display();
        poitData->writeDataToFile(outputFileName, data->getElement(i));
        EngineTerminal::automaticTest(outputFileName, data->getElement(i));
    }
    system("pause");
    return 0;
}
