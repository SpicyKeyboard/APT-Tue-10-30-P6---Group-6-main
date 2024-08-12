
#include <iostream>
#include <fstream>
#include <vector>
#include "Coin.h"

// implement functions for managing coins; this may depend on your design.

std::string denomToString(Denomination denom) {
    std::string result;
    if (denom == FIVE_CENTS) {
        result = "5";
    } else if (denom == TEN_CENTS) {
        result = "10";
    } else if (denom == TWENTY_CENTS) {
        result = "20";
    } else if (denom == FIFTY_CENTS) {
        result = "50";
    } else if (denom == ONE_DOLLAR) {
        result = "100";
    } else if (denom == TWO_DOLLARS) {
        result = "200";
    } else if (denom == FIVE_DOLLARS) {
        result = "500";
    } else if (denom == TEN_DOLLARS) {
        result = "1000";
    } else if (denom == TWENTY_DOLLARS) {
        result = "2000";
    } else if (denom == FIFTY_DOLLARS) {
        result = "5000";
    } else {
        result = ""; // Error handle, better with exception throwing, but others code not adaptable.
    }
    return result;
}

Denomination stringToDenom(std::string value) {
    Denomination result;
    if (value == "5") {
        result = FIVE_CENTS;
    } else if (value == "10") {
        result = TEN_CENTS;
    } else if (value == "20") {
        result = TWENTY_CENTS;
    } else if (value == "50") {
        result = FIFTY_CENTS;
    } else if (value == "100") {
        result = ONE_DOLLAR;
    } else if (value == "200") {
        result = TWO_DOLLARS;
    } else if (value == "500") {
        result = FIVE_DOLLARS;
    } else if (value == "1000") {
        result = TEN_DOLLARS;
    } else if (value == "2000") {
        result = TWENTY_DOLLARS;
    } else if (value == "5000") {
        result = FIFTY_DOLLARS;
    } else {
        result = UNKNOWN; // Error handle, better with exception throwing, but others code not adaptable.
    }
    return result;
}

void Coin::setDenom(Denomination inputDenom) {
    this->denom = inputDenom;
}

Denomination Coin::getDenom() const {
    return denom;
}

void Coin::setCount(unsigned inputCount) {
    this->count = inputCount;
}

unsigned Coin::getCount() const {
    return count;
}

void Coin::addCount(unsigned increment) {
    this->count += increment;
}

int Coin::getIntValue() const {
    return std::stoi(denomToString(this->denom));
}

std::string Coin::getStringSymbol() const {
    std::string result;
    if (denom == FIVE_CENTS) {
        result = "5c";
    } else if (denom == TEN_CENTS) {
        result = "10c";
    } else if (denom == TWENTY_CENTS) {
        result = "20c";
    } else if (denom == FIFTY_CENTS) {
        result = "50c";
    } else if (denom == ONE_DOLLAR) {
        result = "$1";
    } else if (denom == TWO_DOLLARS) {
        result = "$2";
    } else if (denom == FIVE_DOLLARS) {
        result = "$5";
    } else if (denom == TEN_DOLLARS) {
        result = "$10";
    } else if (denom == TWENTY_DOLLARS) {
        result = "$20";
    } else if (denom == FIFTY_DOLLARS) {
        result = "$50";
    } else {
        result = ""; // Error handle, better with exception throwing, but others code not adaptable.
    }
    return result;
}

bool loadCoinData(const std::string &coins_path, std::vector<Coin> &coins) {
    std::string line;
    std::ifstream ReadFile(coins_path);
    bool failedRead = false;
    int coinsLinesCount = 0;
    coins.clear();  // clean value
    while (getline(ReadFile, line)) {
        coinsLinesCount++;
    }
    ReadFile.clear();  // clear eof
    ReadFile.seekg(0,
                   std::ios::beg);  // re-allocate pointer to the start of the file. Otherwise, there is no content since it is already the end of the file.
    while (getline(ReadFile, line)) {
        int lastDelim = line.find_last_of(DELIM);
        int delim = line.find(DELIM);
        std::string value = line.substr(0, delim);
        std::string quantity = line.substr(delim + 1, line.size());
        for (int i = 0; i < int(value.size()); i++) {
            if (!isdigit(value[i])) {
                failedRead = true;  //the varibles are not numbers
            }
        }
        for (int i = 0; i < int(quantity.size()); i++) {
            if (!isdigit(quantity[i])) {
                failedRead = true;  //the varibles are not numbers
            }
        }
        if (!(value == "5" || value == "10" || value == "20" || value == "50" || value == "100" || value == "200" ||
              value == "500" || value == "1000" || value == "2000" || value == "5000")) {
            failedRead = true; //checks the values are correct
        }
        if (coinsLinesCount != 10) {
            failedRead = true; // not enough values in coins.dat
        }
        if (delim != lastDelim || delim == -1) { //there is more than one "."
            failedRead = true;
        }
        if (failedRead) {
            std::cout << "There was an error loading the coins.dat file!" << std::endl;
            return false;
        } else { //load the file as it is correct
            int delim = line.find(DELIM);
            std::string value = line.substr(0, delim);
            int quantity = stoi(line.substr(delim + 1, line.size()));
            Coin tempCoin;
            Denomination tempDenom = stringToDenom(value);
            if (tempDenom != UNKNOWN) {
                tempCoin.setDenom(tempDenom);
                tempCoin.setCount(quantity);
                coins.push_back(tempCoin);
            }
        }
    }

    // simple bubble sort, lowest to highest
    int totalLength = coins.size();
    bool swapFlag = false;
    for (int i = 0; i < totalLength; ++i) {
        swapFlag = false;
        for (int j = 0; j < totalLength - i - 1; ++j) {
            if (coins[j].getIntValue() > coins[j + 1].getIntValue()) {
                Coin temp = coins[j];
                coins[j] = coins[j + 1];
                coins[j + 1] = temp;
                swapFlag = true;
            }
        }
        if (swapFlag == false) {
            break;
        }
    }


    return true;
}

void displayBalance(std::vector<Coin> &inputCoins) {
    std::cout << "Balance Summary\n";
    std::cout << "-------------\n";
    std::cout << "Denom  | Quantity | Value\n";
    std::cout << "---------------------------\n";

    float total = 0;
    int totalLength = inputCoins.size();
    for (int i = 0; i < totalLength; i++) {
        float denom = inputCoins[i].getIntValue();
        float quantity = inputCoins[i].getCount();
        float value = denom * quantity / 100;
        printf("%-6.f | %-8.f |$ %6.2f \n", denom, quantity, value);
        total += value;
    }
    std::cout << "---------------------------\n";
    printf("%18.s $ %.2f\n", "", total);
}

std::vector<Coin> getCoinsFromDenoms(const std::vector<Denomination> &denoms) {
    std::vector<Coin> coins;
    bool addFlag = false;
    for (const Denomination denom: denoms) {
        addFlag = false;
        if (coins.size() > 0) {
            for (Coin c: coins) {
                if (c.getDenom() == denom) {
                    c.addCount(1);
                    addFlag = true;
                }
            }
        }

        if (coins.size() <= 0 || (coins.size() > 0 && addFlag == false)) {
            Coin c;
            c.setDenom(denom);
            c.setCount(1);
            coins.push_back(c);
        }
    }
    return coins;
}

// REQ6: Save Coins
void Coin::saveCoins(const std::string &filename, const std::vector<Coin> &coins) {
    std::ofstream WriteFile(filename);
    for (const auto &coin: coins) {
        WriteFile << denomToString(coin.getDenom()) << DELIM << coin.getCount() << std::endl;
    }
    WriteFile.close();
}
