//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#ifndef SDIZO_ARRAY_H
#define SDIZO_ARRAY_H


#include "../AbstractArray.h"
template< typename T>
class Array : public AbstractArray<T> {
    T *array;
public:

    Array();

    ~Array() override;

    void addAtBeginning(T* newElement) override;

    void addAtEnding(T*  newElement) override;

    void addAtIndex(T* newElement, int index) override;

    void removeFromBegin() override;

    void removeFromEnd() override;

    void removeFromIndex(int index) override;

    /**
    * Method search element
    * @param element searched element
    * @return index of first element of -1 if element isn't in array
    */
    int find(T*  element) override;

    void display() override;

    T* getElement(int index) override;

    int getSize();

    Array(int *tab, int size);

    void add(T* pArray);
};
#include "Array.tpp"

#endif //SDIZO_ARRAY_H
