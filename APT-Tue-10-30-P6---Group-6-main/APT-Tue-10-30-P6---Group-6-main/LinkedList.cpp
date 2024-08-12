
#include "LinkedList.h"
#include <iostream>
#include <fstream>

LinkedList::LinkedList() {
    head = NULL;
    tail = 0;
}

LinkedList::~LinkedList() {
    clear();
}

void LinkedList::append(FoodItem data){
    Node* newNode = new Node(data);
    if(!head){  // if empty linked list
        head = newNode;
    }else{
        Node* current = head;  // start from head
        while(current->next){
            current = current->next;  // loop until the last node
        }
        current->next = newNode;  // append to last place
    }
}

void LinkedList::bubbleSort(){
    // from small to large, bubble sort is simple and easy understanding
    if(!head){   // empty linked list
        return;
    }
    else{
        bool swapFlag = false;
        do{
            swapFlag = false;
            Node* current = head;
            Node* last = nullptr;
            Node* next = nullptr;

            while(current->next){  // loop until the last
                next = current->next;
                int firstID = stoi(current->data.id.substr(1));
                int secondID = stoi(next->data.id.substr(1));
                if(firstID > secondID){  // do swap
                    if(last){ // has last node
                        last->next = next;
                    }
                    else{  // no last node, this is the first node
                        head = next;
                    }
                    // swap
                    current->next = next->next;
                    next->next = current;
                    // record state
                    swapFlag = true;
                    last = next;
                }else{ // move to next
                    last = current;
                    current = current->next;
                }
            }

        }while(swapFlag);
    }
}


void LinkedList::setTail(unsigned inputTail){
    tail = inputTail;
}


bool LinkedList::createList(const std::string& food_path) {
    int lineCount = 0;
    bool failedRead = false;
    std::string line;
    std::ifstream ReadFile(food_path); //ERROR HANDLE FILE 1
    while(getline(ReadFile, line)) {
        // create node with food data
        int firstDelim = line.find("|");
        int secondDelim = line.find("|", firstDelim + 1);
        int thirdDelim = line.find("|", secondDelim + 1);
        int testDelim = line.find("|", thirdDelim + 1);
        if (firstDelim == -1 || secondDelim == -1 || thirdDelim == -1 || testDelim != -1) {//any of the firs three delims are npos or the fourth delim is not npos
            failedRead = true;
        }
        FoodItem currentItem;
        currentItem.id = line.substr(0, firstDelim);
        //test id validity
        if (currentItem.id.substr(0, 1) != "F" || currentItem.id.size() != 5) { //checks to make sure the first character is F, and that the lenght of the string is 5 chars
            failedRead = true;
        }
        for (int i = 1; i < int(currentItem.id.size()); i++) {
            if (!isdigit(currentItem.id[i])) {
                failedRead = true;  //the varibles are not numbers
            }
        }
        currentItem.name = line.substr(firstDelim + 1, secondDelim - firstDelim - 1);
        currentItem.description = line.substr(secondDelim + 1, thirdDelim - secondDelim - 1);
        std::string priceString = line.substr(thirdDelim + 1);
        int priceDelim = priceString.find(".");
        //test price validity
        int lastDelim = priceString.find_last_of(".");
        if (priceDelim == -1 || priceDelim != lastDelim) { //check if there is more than one delim
            failedRead = true;
        }
        std::string dollars = priceString.substr(0, priceDelim);
        std::string cents = priceString.substr(priceDelim + 1);
        for (int i = 0; i < int(dollars.size()); i++) {
            if (!isdigit(dollars[i])) {
                failedRead = true;  //the varibles are not numbers
            }
        }
        for (int i = 0; i < int(cents.size()); i++) {
            if (!isdigit(cents[i])) {
                failedRead = true;  //the varibles are not numbers
            }
        }
        
        if (failedRead) {
            std::cout << "There was an error loading the foods.dat file!" << std::endl;
            return false;
        }
        else { //file was of the correct format
            Price currentItemPrice;
            currentItemPrice.dollars = stoi(priceString.substr(0, priceDelim));
            currentItemPrice.cents = stoi(priceString.substr(priceDelim + 1));
            currentItem.price = currentItemPrice;
            currentItem.on_hand = DEFAULT_FOOD_STOCK_LEVEL;
            // append to list
            this->append(currentItem);
            lineCount++;
        }
        
    }
    this->tail = lineCount;

    // sort
    this->bubbleSort();
    return true;
}

void LinkedList::addNode(FoodItem item) {
    Node* newNode = new Node(item);
    if (head == NULL) {
        head = newNode;
    } else {
        Node* nextNode = head;
        while (nextNode->next != NULL) {
            nextNode = nextNode->next;
        }
        nextNode->next = newNode;
    }
    std::cout << "Added Food Item: " << item.name << " with ID: " << item.id << std::endl;
}

void LinkedList::deleteNode(FoodItem item) {
    Node* current = head;
    Node* previous = NULL;

    while (current != NULL && current->data.id != item.id) {
        previous = current;
        current = current->next;
    }

    if (current == NULL) {
        std::cout << "Food item with ID " << item.id << " not found." << std::endl;
        return;
    }

    if (previous == NULL) {
        head = current->next;
    } else {
        previous->next = current->next;
    }

    std::cout << item.id << " - " << current->data.name << " has been removed from the system." << std::endl;
    delete current;
}

void LinkedList::printList() {
    Node* nextNode = head;
    if (head == NULL) {
        std::cout << "No food items in list, populate list using LinkedList::createList first" << std::endl;
    } else {
        std::cout << "Food Menu" << std::endl;
        std::cout << "---------" << std::endl;
        std::cout << "ID   |Name                                              |Length" << std::endl;
        std::cout << "------------------------------------------------------------------" << std::endl;
        while (nextNode != NULL) {
            float price = static_cast<float>(nextNode->data.price.dollars) + static_cast<float>(nextNode->data.price.cents) / 100.f;
            printf("%-5s|%-50s|$%5.2f \n", (nextNode->data.id).c_str(), (nextNode->data.name).c_str(), price);
            nextNode = nextNode->next;
        }
    }
}

Node LinkedList::searchNodeById(const std::string& Id) {
    Node* nextNode = head;
    if (head == nullptr) {
        std::cout << "No food items in list, populate list using LinkedList::createList first" << std::endl;
        return Node();
    }
    while(nextNode != nullptr) {
        if (nextNode->data.id == Id) {
            return *nextNode;
        }
        nextNode = nextNode->next;
    }
    return Node();
}

// REQ6: Save List
void LinkedList::saveList(const std::string& filename) {
    if (head != NULL) {
        head->saveFoods(filename);
    }
}

// REQ7: Get Next ID
std::string LinkedList::getNextId() {
    if (head == NULL) {
        return "F0001";
    } else {
        Node* nextNode = head;
        int maxId = 0;
        while (nextNode != NULL) {
            int currentId = std::stoi(nextNode->data.id.substr(1));
            if (currentId > maxId) {
                maxId = currentId;
            }
            nextNode = nextNode->next;
        }
        return "F" + std::string(4 - std::to_string(maxId + 1).length(), '0') + std::to_string(maxId + 1);
    }
}

// clear method
void LinkedList::clear() {
    Node* current = head;
    while (current != NULL) {
        Node* next = current->next;
        delete current;
        current = next;
    }
    head = NULL;
    tail = 0;
}
