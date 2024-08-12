#ifndef COIN_H
#define COIN_H

// Coin.h defines the coin structure for managing currency in the system. 
#define DELIM ","  // delimiter 
#include <vector>
#include <string>

// enumeration representing the various types of currency available in the system. 
enum Denomination {
    FIVE_CENTS, TEN_CENTS, TWENTY_CENTS, FIFTY_CENTS, ONE_DOLLAR, 
    TWO_DOLLARS, FIVE_DOLLARS, TEN_DOLLARS, TWENTY_DOLLARS, FIFTY_DOLLARS, UNKNOWN
};

// represents a coin type stored in the cash register perhaps. Each demonination
// will have exactly one of these in the cash register.
class Coin {
public:
    // the denomination type
    enum Denomination denom;
    
    // the count of how many of these are in the cash register
    unsigned count{};

    void setDenom(Denomination inputDenom);
    Denomination getDenom() const;
    void setCount(unsigned inputCount);
    unsigned getCount() const;
    int getIntValue() const;
    void addCount(unsigned int increment);
    std::string getStringSymbol() const;

    void saveCoins(const std::string& filename, const std::vector<Coin>& coins); // REQ6
};

std::string denomToString(Denomination denom);
Denomination stringToDenom(std::string value);
bool loadCoinData(const std::string& coins_path, std::vector<Coin>& coins);
void displayBalance(std::vector<Coin>& inputCoins);
std::vector<Coin> getCoinsFromDenoms(const std::vector<Denomination>& denoms);

#endif // COIN_H
