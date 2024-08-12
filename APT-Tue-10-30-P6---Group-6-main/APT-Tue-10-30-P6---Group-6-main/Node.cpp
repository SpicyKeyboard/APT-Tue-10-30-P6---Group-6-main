#include "Node.h"
#include <fstream>
#include <iostream>

Node::Node() {
    FoodItem tempItem;
    data = tempItem;
    next = NULL;
}

Node::Node(FoodItem data) {
    this->data = data;
    this->next = NULL;
}

Node::~Node() {
    
}

// REQ6: Save Foods
void Node::saveFoods(const std::string& filename) {
    std::ofstream WriteFile(filename);
    Node* current = this;
    while (current != NULL) {
        WriteFile << current->data.id << "|"
                  << current->data.name << "|"
                  << current->data.description << "|"
                  << current->data.price.dollars << "."
                  << current->data.price.cents << std::endl;


        current = current->next;


    }
    WriteFile.close();
}
