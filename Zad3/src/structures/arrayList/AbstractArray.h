//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#ifndef SDIZO_ABSTRACTARRAY_H
#define SDIZO_ABSTRACTARRAY_H


template <class T>
class AbstractArray {
public:
    virtual ~AbstractArray();

protected:
    /**number of element in structure*/
    int size = 0;

    /**
    * Method check if given index is possible to use
    * @param index checked index
    *  @return true if index is correct or exception with message if it isn't correct
    */
    bool possibleIndex(int index) const;

public:

//    /**
//     * Method read elements from file and add them to structure
//     * @param fileName of read file
//     */
//    void readFromFile(const std::string &fileName);
//
//    /**
//     * Method writes all elements to file
//     * @param fileName
//     */
//    void writeToFile( const std::string &fileName);

    /**
    * Check if array is empty
    * @return true if array is empty
    */
    bool empty() const;

    /**
    * Method return size of structure
    * @return size of structure
    */
    virtual int getSize();

    /**
    * Method replace two elements place
    * @param firstIndex index of first element to replace
    * @param secondIndex index of second element to replace
    */
    void replace(int firstIndex, int secondIndex);

    /**
     * Method add element at beginning of structure
     * @param newElement  which will be add
     */
    virtual void addAtBeginning(T* newElement) = 0;

    /**
     * Method add element at ending of structure
     * @param newElement which will be add
     */
    virtual void addAtEnding(T * newElement) = 0;

    /**
     * Method add element at end of structure
     * @param newElement which will be add
     * @param index where element is added
     * @warning could write exception message if index is wrong
     */
    virtual void addAtIndex(T *newElement, int index) = 0;

    /**
     * Method remove element from begin of structure
     */
    virtual void removeFromBegin() = 0;

    /**
     * Method remove element from end of structure
     */
    virtual void removeFromEnd() = 0;

    /**
     * Method remove element from given index of structure
     * @param index from where element is removed
     * @warning could write exception message if index is wrong
     */
    virtual void removeFromIndex(int index) = 0;

    /**
     * Method search for first element which is equals
     * @param element searched element
     * @return index of element
     */
    virtual int find(T * element) = 0;

    /**
     * Show structure in terminal
     */
    virtual void display() = 0;

    /**
     * @param index of element
     * @return element of structure
     */
    virtual T* getElement(int index) = 0;

    void setSize(int size);
};

#include "AbstractArray.tpp"
#endif //SDIZO_ABSTRACTARRAY_H
