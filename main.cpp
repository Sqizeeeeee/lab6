#include "solver.h"
#include <iostream>

int main() {
    LinearSystem system = {
        {
            {{1,  3, -2},  7},
            {{2, -1,  1},  4}, 
            {{3,  2, -1}, 11}
        }
    };

    
    printSystem(system);
    solveJordanGauss(system);
    std::cout << "ended" << '\n';
    return 0;
}