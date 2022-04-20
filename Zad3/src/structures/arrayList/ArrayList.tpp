//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#include <iostream>
#include <random>
#include "Edge.h"


template<typename T>
ArrayListElement<T>::ArrayListElement() {
    element = nullptr;
    previousElement = nullptr;
    nextElement = nullptr;
}
//template<>
//void ArrayListElement<Edge>::display() {
//        element->display();
//}
//template<>
//void ArrayListElement<int>::display<int>() {
//    std::cout<<element;
//}
template<typename T>
void ArrayListElement<T>::display() {
    std::cout<<element;
}
template<typename T>
T *ArrayListElement<T>::getElement() const {
    return element;
}

template<typename T>
ArrayListElement<T> *ArrayListElement<T>::getPreviousElement() const {
    return previousElement;
}

template<typename T>
void ArrayListElement<T>::setPreviousElement(ArrayListElement<T> *previousEle) {
    ArrayListElement::previousElement = previousEle;
}

template<typename T>
ArrayListElement<T> *ArrayListElement<T>::getNextElement() const {
    return nextElement;
}

template<typename T>
void ArrayListElement<T>::setNextElement(ArrayListElement<T> *nextEle) {
    ArrayListElement::nextElement = nextEle;
}

template<typename T>
ArrayListElement<T>::ArrayListElement(T *ele) {
    element = ele;
    nextElement = nullptr;
    previousElement = nullptr;
}

template<typename T>
ArrayListElement<T>::~ArrayListElement() {
    previousElement = nullptr;
//    delete element;
    nextElement = nullptr;
}

template<typename T>
void ArrayList<T>::addFirst(T *element) {
    auto *ele = new ArrayListElement<T>(element);
    firstElement = ele;
    lastElement = ele;
    AbstractArray<T>::size++;
}

/*
 * Method add first element if ArrayList was empty by calling addFirst method,
 * then set next pointer to old first element, and similar set previous pointer to new first element.
 * Obligatory set firstElement Pointer to newFirst element
 */
template<typename T>
void ArrayList<T>::addAtBeginning(T *newElement) {
    if (AbstractArray<T>::getSize() == 0)
        addFirst(newElement);
    else {
        auto *ele = new ArrayListElement<T>(newElement);
        ele->setNextElement(firstElement);
        firstElement->setPreviousElement(ele);
        firstElement = ele;
        AbstractArray<T>::size++;
    }
}


template<typename T>
void ArrayList<T>::addAtEnding(T *newElement) {
    if (AbstractArray<T>::getSize() == 0)
        addFirst(newElement);
    else {
        auto *ele = new ArrayListElement<T>(newElement);
        ele->setPreviousElement(lastElement);
        lastElement->setNextElement(ele);
        lastElement = ele;
        AbstractArray<T>::size++;
    }
}

/*
 * Method use method addFirst if it is first element,
 * if the index is 0 method call addAtBegging
 * if it last index method call addAtEnding
 * else go to searched element add new in given place and set pointers
 */
template<typename T>
void ArrayList<T>::addAtIndex(T *newElement, int index) {
    //check if index is correct
    if (AbstractArray<T>::possibleIndex(index)) {
        if (AbstractArray<T>::getSize() == 0) //if it first element
            addFirst(newElement);
        else if (index == 0)
            addAtBeginning(newElement);
        else if (index == AbstractArray<T>::getSize())
            addAtEnding(newElement);
        else {
            auto *ele = new ArrayListElement<T>(newElement);
            auto *pointer = firstElement;
            for (int i = 0; i < index; i++)
                pointer = pointer->getNextElement();
            ele->setPreviousElement(pointer->getPreviousElement());
            ele->setNextElement(pointer);
            pointer->setPreviousElement(ele);
            ele->getPreviousElement()->setNextElement(ele);
            AbstractArray<T>::size++;
        }
    }
}

template<typename T>
void ArrayList<T>::removeFromBegin() {
    if (!AbstractArray<T>::empty()) {
        if (AbstractArray<T>::getSize() == 1)
            removeLast();
        else {
            auto *pointer = firstElement->getNextElement();
            pointer->setPreviousElement(nullptr);
            delete firstElement;
            firstElement = pointer;
            AbstractArray<T>::size--;
        }
    }
}

template<typename T>
void ArrayList<T>::removeFromEnd() {
    if (!AbstractArray<T>::empty()) {
        if (AbstractArray<T>::getSize() == 1)
            removeLast();
        else {
            auto *pointer = lastElement->getPreviousElement();
            pointer->setNextElement(nullptr);
            delete lastElement;
            lastElement = pointer;
            AbstractArray<T>::size--;
        }
    }
}

template<typename T>
void ArrayList<T>::removeLast() {
    lastElement = nullptr;
    delete firstElement;
    firstElement = nullptr;
    AbstractArray<T>::size--;
}

template<typename T>
void ArrayList<T>::removeFromIndex(int index) {
    if (!AbstractArray<T>::empty() && AbstractArray<T>::possibleIndex(index)) {
        if (index == 0)
            removeFromBegin();
        else if (index == AbstractArray<T>::getSize() - 1)
            removeFromEnd();
        else {
            auto *pointer = firstElement;
            for (int i = 0; i < index; i++)
                pointer = pointer->getNextElement();
            pointer->getNextElement()->setPreviousElement(pointer->getPreviousElement());
            pointer->getPreviousElement()->setNextElement(pointer->getNextElement());
            delete pointer;
            AbstractArray<T>::size--;
        }
    }
}

template<typename T>
int ArrayList<T>::find(T *element) {
    if (!AbstractArray<T>::empty()) {
        auto *pointer = firstElement;
        for (int i = 0; i < AbstractArray<T>::getSize(); i++) {
            if (pointer->getElement() == element)
                return i;
            pointer = pointer->getNextElement();
        }
    }
    return -1;
}

template<typename T>
void ArrayList<T>::display() {
    if (AbstractArray<T>::getSize() != 0) {
        ArrayListElement<T> *pointer = firstElement;
        std::cout << "\n";
        while (pointer != lastElement) {
            pointer->display();
            pointer = pointer->getNextElement();
            std::cout << ", ";
        }
        pointer->display();
    } else
        std::cout << "\nLista jest pusta!";
}

template<typename T>
ArrayList<T>::ArrayList() {
    firstElement = nullptr;
    lastElement = nullptr;
    AbstractArray<T>::size = 0;
}

template<typename T>
T *ArrayList<T>::getElement(int index) {
    if (!AbstractArray<T>::empty() && AbstractArray<T>::possibleIndex(index)) {
        auto *pointer = firstElement;
        for (int i = 0; i < index; i++)
            pointer = pointer->getNextElement();
        return pointer->getElement();
    }
    return nullptr;
}

template<typename T>
ArrayList<T>::~ArrayList() {
    while (AbstractArray<T>::size != 0) {
        removeFromBegin();
    }
}

template<typename T>
void ArrayList<T>::addEdgeToList(T *edge) {
    auto *pointer = firstElement;
    int i = 0;
    if (firstElement == nullptr) {
        addAtBeginning(edge);
        return;
    }
    while (pointer != nullptr) {
        if (pointer->getElement()->getWeight() > edge->getWeight()) {
            addAtIndex(edge, i);
            return;
        }
        pointer = pointer->getNextElement();
        i++;
    }
    addAtEnding(edge);
}

template<typename T>
ArrayListElement<T> *ArrayList<T>::getFirstElement() const {
    return firstElement;
}


