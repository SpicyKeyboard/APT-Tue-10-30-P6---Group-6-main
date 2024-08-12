Information for compiling and running the program.

compile code:

g++ -Wall -Werror -std=c++14 -O -o ftt Coin.cpp LinkedList.cpp Node.cpp ftt.cpp

run program:

./ftt foods.dat coins.dat


two lines for memory leaks / abuse checking:

valgrind --leak-check=full --show-leak-kinds=all ./ftt foods.dat coins.dat

(^- should report 0)

valgrind --track-origins=yes ./ftt foods.dat coins.dat

(^- should report 0)


test case commands:

req 2

./ftt foods.dat coins.dat < loadData.input > loadData.actual_output

diff -w loadData.expected_output loadData.actual_output

diff -w -y loadData.expected_coins coins.dat

diff -w -y loadData.expected_foods foods.dat


req 9

./ftt foods.dat coins.dat < displayBalance.input > displayBalance.actual_output

diff -w displayBalance.expected_output displayBalance.actual_output

diff -w -y displayBalance.expected_coins coins.dat
