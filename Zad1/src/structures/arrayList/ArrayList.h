//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#ifndef SDIZO_ARRAYLIST_H
#define SDIZO_ARRAYLIST_H


#include "AbstractArray.h"
#include "Edge.h"
/**
 * Additional class use to encapsulate fragment of arrayList
 */
template< typename T>
class ArrayListElement {
    T *element;
    ArrayListElement<T> *previousElement;
    ArrayListElement<T> *nextElement;
public:
    virtual ~ArrayListElement<T>();

    ArrayListElement<T>();

    explicit ArrayListElement<T>(T *ele);

    void display() ;

    T *getElement() const;

    ArrayListElement<T> *getPreviousElement() const;

    void setPreviousElement(ArrayListElement<T> *previousElement);

    ArrayListElement<T> *getNextElement() const;

    void setNextElement(ArrayListElement<T> *nextElement);

};


template< typename T>
class ArrayList : public AbstractArray<T> {
    ArrayListElement<T> *firstElement;
    ArrayListElement<T>  *lastElement;

    /**
     * Method use when first element is adding to ArrayList
     * @param element to add
     */
    void addFirst (T * element);

    /**
     * Method use when last element is removing
     */
    void removeLast();

public:
    T * getElement(int index);

    void addAtBeginning(T * newElement);

    void addAtEnding(T * newElement);

    void addAtIndex(T * newElement, int index);

    void removeFromBegin() override;

    void removeFromEnd() override;

    void removeFromIndex(int index) override;

    int find(T *element);

    void display() override;

    ArrayListElement<T> *getFirstElement() const;

    ArrayList();

    ~ArrayList() override;

    void addEdgeToList(T *edge);
};

#include "ArrayList.tpp"


#endif //SDIZO_ARRAYLIST_H
