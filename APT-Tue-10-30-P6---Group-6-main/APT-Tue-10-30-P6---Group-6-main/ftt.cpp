#include <iostream>
#include <limits>
#include <fstream>
#include <map>
#include <vector>
#include <algorithm>

#include "Coin.h"
#include "LinkedList.h"

/**
 * manages the running of the program, initialises data structures, loads
 * data, display the main menu, and handles the processing of options. 
 * Make sure free memory and close all files before exiting the program.
 **/
std::vector<Coin> coins;
LinkedList foodList;

int getValidCents(int centNumber){
    // The ones place of the cent number can only be 5 and 0 since the minimum cent domination value is 5.
    // the value will be the nearest downgrade of the cent number. It's not good to charge more cents to customers.
    // eg: 56 will be 55, 51 will be 50, 49 will be 45.
    int result;
    int reminder = centNumber % 10;  // cent can not reach 100
    if(reminder < 5){
        result = centNumber - reminder;
    }else{
        result = centNumber - reminder + 5;
    }
    return result;

}

void foodMenu() {
    // this displays all the foodItems that have been loaded into the linked list, i have not tried to modify the items within the linked list so it may not work 100% but it should work well.
    foodList.printList();
    std::cout << std::endl;
}

void purchaseMeal() {
    // If the user presses enter on a new line, refund all the coins/notes they have entered so far and return them to the main menu
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cout << "Purchase Meal" << std::endl;
    std::cout << "-------------" << std::endl;
    std::cout << "Please enter the ID of the food you wish to purchase:" << std::endl;
    std::string foodId;
    if (!std::getline(std::cin, foodId)) {
        if (std::cin.eof()) {
            std::cout << "Transaction Cancelled By User!" << std::endl;
            std::cin.clear();
            exit(EXIT_SUCCESS);
        }
    } else {
        if (foodId.empty() || foodId == "^Z") {  // in windows, ctrl + d will send ^Z as eof
            std::cout << "Transaction Cancelled By User!" << std::endl;
            std::cin.clear();
        } else {
            Node food = foodList.searchNodeById(foodId);
            // check if empty
            if (food.data.id.empty()) {
                std::cout << "Food ID not exists!" << std::endl;
            } else {
                std::cout << "You have selected \"" << food.data.name << " - " << food.data.description
                          << "\". This will cost you $";
                std::cout << food.data.price.dollars << "." << getValidCents(food.data.price.cents) << "." << std::endl;
                std::cout << "Please hand over the money - type in the value of each note/coin in cents." << std::endl;
                std::cout << "Please enter ctrl-D or enter on a new line to cancel this purchase." << std::endl;
                // purchase process with validation
                int totalInputCents = 0;
                int dollars = static_cast<int>(food.data.price.dollars);
                int cents = static_cast<int>(getValidCents(food.data.price.cents));
                int targetCents = dollars * 100 + cents;
                std::vector<Denomination> paidCoins;
                bool cancelFlag = false;
                while (totalInputCents < targetCents && !cancelFlag) { // end if paid value over food price
                    std::cout << "You still need to give us $" << dollars << "." << cents << ": ";
                    std::string inputDenom;
                    if (!std::getline(std::cin, inputDenom)) {
                        if (std::cin.eof()) {
                            std::cout << "Transaction Cancelled By User!" << std::endl;
                            std::cin.clear();
                            exit(EXIT_SUCCESS);
                        }
                    } else {
                        if (inputDenom.empty() || inputDenom == "^Z") {  // in windows, ctrl + d will send ^Z as eof
                            std::cout << "Transaction Cancelled By User!" << std::endl;
                            std::cin.clear();
                            cancelFlag = true;
                        } else {
                            Denomination denom = stringToDenom(inputDenom);
                            if (denom == UNKNOWN) { // skip unknown denomination
                                std::cout << "Error: invalid denomination encountered." << std::endl;
                            } else {
                                // displaying how much money need to be paid
                                int denomValue = stoi(inputDenom);
                                if (denomValue < 100) {  // cents
                                    if (cents < denomValue) {
                                        cents = 100 + cents - denomValue;
                                        dollars = dollars - 1;
                                    }
                                } else {  // dollars
                                    dollars = dollars - denomValue /
                                                        100;  // no need to check if result over 0, if result < 0, next loop will not be triggered
                                }

                                // record customer paid denominations
                                paidCoins.push_back(denom);
                                // add up total paid value
                                totalInputCents += stoi(inputDenom);
                            }
                        }
                    }
                }

                if (!cancelFlag) {
                    // the coins array is from low to high, to increase efficiency, the change should start from larger denomination.
                    // so the coins array should be looked up from backward.
                    std::vector<Coin> combineCoins;  // combine current coins and paid coins
                    for (Coin c: coins) {
                        combineCoins.push_back(c);
                    }
                    for (Denomination d: paidCoins) {
                        for (Coin &c: combineCoins) {
                            if (c.getDenom() == d) {
                                c.addCount(1);
                                break;
                            }
                        }
                    }

                    // calculate changes
                    int changeCents = totalInputCents - targetCents;
                    if (changeCents <= 0) { // skip exact paid
                        std::cout << "You have paid Exact Amount - No Change Given" << std::endl;
                        std::cout << "Thank you for your purchase!" << std::endl;
                        // assign combine coins to current coins
                        coins.clear();
                        for (Coin c: combineCoins) {
                            coins.push_back(c);
                        }

                        food.data.on_hand -= 1;
                    } else {
                        // calculate change coins
                        std::vector<Coin> changeCoins;
                        for (auto i = combineCoins.rbegin();
                             i != combineCoins.rend(); ++i) {  // start from larger value
                            int value = i->getIntValue();
                            int counter = 0;
                            int restCount = i->getCount();
                            while (changeCents >= value && restCount > 0) {  // keep taking the value of coins
                                changeCents = changeCents - value;
                                counter += 1;
                                restCount -= 1;
                            }
                            if (counter > 0) {  // record taken coins
                                Coin c;
                                c.setCount(counter);
                                c.setDenom(i->getDenom());
                                changeCoins.push_back(c);
                                i->setCount(restCount);
                            }
                        }

                        if (changeCents > 0) {  // changes not enough
                            std::cout << "Not Enough Changes! Transaction Cancelled!" << std::endl;
                        } else {
                            std::cout << "Your change is ";
                            // reverse to from small to large
                            for (auto i = changeCoins.rbegin(); i != changeCoins.rend(); ++i) {
                                std::cout << i->getStringSymbol() << "*" << i->getCount() << " ";
                            }
                            std::cout << std::endl;
                            std::cout << std::endl;
                            // assign combine coins to current coins
                            coins.clear();
                            for (Coin c: combineCoins) {
                                coins.push_back(c);
                            }

                            food.data.on_hand -= 1;
                        }
                    }
                }
            }
        }
    }
}

// REQ6: Save and Exit
void saveAndExit() {
    std::cout << "Saving data and exiting..." << std::endl;
    foodList.saveList("foods.dat");
    Coin().saveCoins("coins.dat", coins);
    foodList.clear();
    exit(EXIT_SUCCESS);
}

// REQ7: Add Food Item

void removePipeCharacter(std::string& str) {
    str.erase(std::remove(str.begin(), str.end(), '|'), str.end());
}

void addFoodItem() {
    FoodItem newItem;
    newItem.id = foodList.getNextId();
    std::cout << "This new meal item will have the Item id of " << newItem.id << "." << std::endl;

    std::cout << "Enter the item name: ";
    std::cin.ignore();
    std::getline(std::cin, newItem.name);
    removePipeCharacter(newItem.name);  

    std::cout << "Enter the item description: ";
    std::getline(std::cin, newItem.description);
    removePipeCharacter(newItem.description);  

    std::cout << "Enter the price for this item (in dollars.cents): ";
    std::string priceStr;
    std::getline(std::cin, priceStr);
    int priceDelim = priceStr.find(".");
    newItem.price.dollars = stoi(priceStr.substr(0, priceDelim));
    newItem.price.cents = stoi(priceStr.substr(priceDelim + 1));
    newItem.on_hand = DEFAULT_FOOD_STOCK_LEVEL;

    foodList.addNode(newItem);
    std::cout << "This item \"" << newItem.name << " - " << newItem.description << "\" has now been added to the food menu" << std::endl;
}
// REQ8: Remove Food Item
void removeFoodItem() {
    std::string id;
    std::cout << "Enter the food id of the food to remove from the menu: ";
    std::cin >> id;

    FoodItem tempItem;
    tempItem.id = id;
    foodList.deleteNode(tempItem);
}

void mainMenu() {
    int option =0;
    while (option != 7 && std::cin.good()) {
        std::cin.clear();
        // Display Main Menu
        std::cout << "Main Menu:" << std::endl;
        std::cout << "  1. Display Meal Options" << std::endl;
        std::cout << "  2. Purchase Meal" << std::endl;
        std::cout << "  3. Save and Exit" << std::endl;
        std::cout << "Administration Menu:" << std::endl;
        std::cout << "  4. Add Food" << std::endl;
        std::cout << "  5. Remove Food" << std::endl;
        std::cout << "  6. Display Balance" << std::endl;
        std::cout << "  7. Abort Program" << std::endl;
        std::cout << "Select your option (1-7) :" << std::endl;

        // get option
        std::cin >> option;

        // call function by option
        if (option == 1) {
            // display food menu
            foodMenu();
        } else if (option == 2) {
            // Purchase Meal
            purchaseMeal();
        } else if (option == 3) {
            // Save and Exit
            saveAndExit();
        } else if (option == 4) {
            // Add Food
            addFoodItem();
        } else if (option == 5) {
            // Remove Food
            removeFoodItem();
        } else if (option == 6) {
            // Display Balance
            displayBalance(coins);
        } else if (option == 7) {
            // Abort Program
            std::cout << "Quitting..." << std::endl;
            return;
        } else {
            // Error handle
            // clear input to avoid infinite loop
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Option should be an integer from 1 to 7!" << std::endl;
        }
    }
}

int main(int argc, char **argv) {
    /* validate command line arguments */
    if (argc != 3) {
        std::cout << "Usage: " << argv[0] << " <foods_file> <coins_file>" << std::endl;
        return EXIT_FAILURE;
    }
    // Get input file name
    const std::string foods_path = argv[1];
    const std::string coins_path = argv[2];

    // load data
    if (!foodList.createList(foods_path) || !loadCoinData(coins_path, coins)) {
        std::cout << "Terminating program due to error loading!" << std::endl;
    }
    else {
        mainMenu();
    }

    return EXIT_SUCCESS;
}