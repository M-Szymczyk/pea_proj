//
// Created by Hyperbook on 16.03.2021.
//

#include <fstream>
#include <iostream>
#include "Structure.h"

void Structure::readFromFile(const std::string &fileName) {
    using namespace std;
    fstream file;
    file.open(fileName, ios::in);
    if (!file) {
        cout << "\nBłąd oczytu pilku!";
    } else {
        string line;
        file >> line;
        //int amountOfElements = stoi(line);
        //read elements
        while (!file.eof()) {
            file >> line;
           add(stoi(line));
        }

        file.close();
    }
}

void Structure::writeToFile(const std::string &fileName){
    using namespace std;
    fstream file;
    file.open(fileName, ios::out);
    if (!file) {
        cout << "\nBłąd oczytu pilku!";
    } else {
        //write number of elements
        file <<getSize()<<endl;
        //write elements
        for(int i=0;i<getSize()+i;i++) {
            file << getTopElement();
            remove();

            if(i!=getSize()+i)//no last new line
                file <<endl;
        }

        file.close();
    }
    //read structure back to terminal
    readFromFile(fileName);
}

void Structure::remove(int element) {
    remove();
}

