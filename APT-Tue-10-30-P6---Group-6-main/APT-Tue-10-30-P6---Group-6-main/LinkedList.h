#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Node.h"

class LinkedList {
public:
    LinkedList();
    ~LinkedList();

    void addNode(FoodItem item);
    void deleteNode(FoodItem item);
    void append(FoodItem item);
    void bubbleSort();
    bool createList(const std::string& food_path);
    void printList();
    Node searchNodeById(const std::string& Id);
    void saveList(const std::string& filename); // added REQ6 
    std::string getNextId(); // added REQ7 
    void clear(); // added
    void setTail(unsigned inputTail);


private:
    Node* head;
    unsigned tail;
};


#endif  // LINKEDLIST_H
