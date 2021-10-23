//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#include <iostream>
#include <fstream>



template <class T> bool AbstractArray<T>::empty() const {
    if (size == 0) {
        std::cout << "\nStruktura jest pusta!";
        return true;
    }
    return false;
}

template <class T> bool AbstractArray<T>::possibleIndex(int index) const {
    if (index < 0) {
        std::cout << "\nZbyt maly indeks! Podano: " << index;
        return false;
    } else if (index > size) {
        std::cout << "\nZbyt duzy indeks! Podano: " << index;
        return false;
    } else
        return true;
}


template <class T> int AbstractArray<T>::getSize() {
    return size;
}

/**
 * Method get value of replaced elements
 * remove element from first element, add element from second index in place of first
 * then remove element from second index and add element from first index in place of second
 */
template <class T> void AbstractArray<T>::replace(int firstIndex, int secondIndex) {
    if (possibleIndex(firstIndex) && possibleIndex(secondIndex)) {
        T *buffer = getElement(firstIndex),*buffer2 = getElement(secondIndex);
        removeFromIndex(firstIndex);
        addAtIndex(buffer2, firstIndex);
        removeFromIndex(secondIndex);
        addAtIndex(buffer, secondIndex);
    }
}

template<class T>
void AbstractArray<T>::setSize(int size) {
    AbstractArray::size = size;
}
//
//void AbstractArray::readFromFile(const std::string &fileName) {
//    using namespace std;
//    fstream file;
//    file.open(fileName, ios::in);
//    if (!file) {
//        cout << "\nBlad odczytu pilku!";
//    } else {
//        string line;
//        file >> line;
//        //int amountOfElements = stoi(line);
//
//        while (!file.eof()) {
//            file >> line;
//            addAtEnding(stoi(line));
//        }
//
//        file.close();
//    }
//}
//void AbstractArray::writeToFile(const std::string &fileName) {
//    using namespace std;
//    fstream file;
//    file.open(fileName, ios::out);
//    if (!file) {
//        cout << "\nBład odczytu pilku!";
//    } else {
//        file << getSize() << endl;
//
//        for (int i = 0; i < getSize(); i++) {
//            file << getElement(i);
//            if (i != getSize() - 1)
//                file << endl;
//        }
//
//        file.close();
//    }
//}

template <class T> AbstractArray<T>::~AbstractArray() = default;
