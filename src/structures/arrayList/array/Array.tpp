//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#include <iostream>
#include <random>

template<typename T>
Array<T>::Array() {
    array = nullptr;
}

template<typename T>
Array<T>::~Array() {
    delete[] array;
}
template<typename T>
void Array<T>::addAtBeginning(T* newElement) {
    AbstractArray<T>::size++;
    int *newArray = new T[getSize()];
    //add first element
    newArray[0] = *newElement;
    //rewrite rest of array
    for (int i = 1; i < getSize(); i++)
        newArray[i] = array[i - 1];

    delete[] array;
    array = newArray;
}
template<typename T>
void Array<T>::addAtEnding(T* newElement) {
    AbstractArray<T>::size++;
    int *newArray = new T[getSize()];
    //rewrite begin of array

    for (int i = 0; i < (getSize() - 1); i++)
        *(newArray+i) = *(array+i);
    //memcpy(newArray,array,  (size - 1)*sizeof(int));
    //std::copy(array, array + size - 1, newArray);
    //add last
    newArray[getSize() - 1] = *newElement;
    delete[] array;
    array = newArray;
}
template<typename T>
void Array<T>::addAtIndex(T* newElement, int index) {
    if (AbstractArray<T>::possibleIndex(index)) {
        AbstractArray<T>::size++;
        int *newArray = new T[getSize()];
        //rewrite begin of array
        for (int i = 0; i < index; i++) {
            newArray[i] = array[i];
        }

        newArray[index] = *newElement;
        //rewrite rest of array
        for (int i = index + 1; i < getSize(); i++) {
            newArray[i] = array[i - 1];
        }
        delete[] array;
        array = newArray;
    }
}
template<typename T>
void Array<T>::removeFromBegin() {
    if (!AbstractArray<T>::empty()) {
        int *newArray = new T[getSize() - 1];
        for (int i = 0; i < getSize() - 1; i++)
            newArray[i] = array[i + 1];
        AbstractArray<T>::size--;
        delete[] array;
        array = newArray;
    }
}
template<typename T>
void Array<T>::removeFromEnd() {
    if (!AbstractArray<T>::empty()) {
        int *newArray = new T[getSize() - 1];
        for (int i = 0; i < getSize() - 1; i++)
            newArray[i] = array[i];
        AbstractArray<T>::size--;
        delete[] array;
        array = newArray;
    }
}
template<typename T>
void Array<T>::removeFromIndex(int index) {
    if (!AbstractArray<T>::empty() &&AbstractArray<T>::possibleIndex(index)) {
        int *newArray = new T[getSize() - 1];
        for (int i = 0; i < index; i++)
            newArray[i] = array[i];
        for (int i = index + 1; i < getSize(); i++)
            newArray[i - 1] = array[i];
        AbstractArray<T>::size--;
        delete[] array;
        array = newArray;
    }
}
template<typename T>
int Array<T>::find(T* element) {
    for (int i = 0; i < getSize(); i++) {
        if (array[i] == *element)
            return i;
    }
    return -1;
}
template<typename T>
void Array<T>::display() {
    std::cout << "\n";
    for (int i = 0; i < getSize(); i++) {
        std::cout << array[i] << ", ";
    }
}
template<typename T>
T* Array<T>::getElement(int index) {
    if (AbstractArray<T>::possibleIndex(index))
        return &(array[index]);
    return new int(-1);
}

template<typename T>
int Array<T>::getSize() {
    return AbstractArray<T>::getSize();
}


