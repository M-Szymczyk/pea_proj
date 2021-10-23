//
// Created by Mikolaj Szymczyk on 16.03.2021.
//

#ifndef SDIZO_STRUCTURE_H
#define SDIZO_STRUCTURE_H


#include <string>

class Structure {
public:
    /**
     * Method add element to structure
     * @param newElement which will be added
     */
    virtual void add(int newElement) = 0;

    /**
     * Method remove top element from structure
     */
    virtual void remove() = 0;

    /**
     * Method remove given element from structure
     * @param element to remove
     * @warning if not override will call remove method
     */
    virtual void remove(int element);

    /**
     * Method check if there given element at structure
     * @param element searched element
     * @return true if there is given element
     */
    virtual bool find(int element) = 0;

    /**
     * Display structure in terminal
     */
    virtual void display() = 0;

    /**
     * @return number of elements in structure
     */
    virtual int getSize() = 0;

    /**
     * @return top element of structure
     */
    virtual int getTopElement() = 0;

    /**
     *  Method read elements from file and add them to structure
     * @param fileName
     */
    void readFromFile(const std::string &fileName);

    /**
     * Method writes all elements to file
     * @param fileName of read file
     */
    void writeToFile(const std::string &fileName);


    virtual ~Structure() = default;
};


#endif //SDIZO_STRUCTURE_H
