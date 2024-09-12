#include <algorithm>
#include <iostream>
#include <cstdlib>
#include "container.h"
#include <cstdlib>
#include <ctime>

// #include <fstream>

int main(){

    std::srand(std::time(nullptr));
    
    const unsigned nRows = 2, nCols = 2;

    Container<int> matrix(nRows, nCols);       

    for (unsigned i = 0; i < nRows; ++i) {
        for (unsigned j = 0; j < nCols; ++j)
            matrix[i][j] = std::rand() % 100;
    }    
    
    std::cout << "Исходная матрица: " << std::endl;
    std::cout << matrix << std::endl;
    
    std::cout << *(matrix.begin()) << std::endl;
    std::cout << *(matrix.end()) << std::endl;


    std::sort(matrix.begin(), matrix.end());

    std::cout << "Матрица после сортировки: " << std::endl;
    std::cout << matrix << std::endl;
    
    Container<int>::Iterator iter = matrix.end();
    
    // std::cout << *(matrix.begin()) << std::endl;
    // std::cout << *(matrix.end()) << std::endl;

    // for(int i = nRows * nCols - 1; i <= 0; i--){
    //     std::cout << *(iter+i) << ' ';      
    // }
    
    for(iter = matrix.end(); iter != matrix.begin(); --iter){
        std::cout << *(iter) << ' ';      
    }
    
    // std::cout << *(iter) << std::endl;

    // std::cout << *(iter-1) << std::endl;
    // std::cout << *(iter-2) << std::endl;
    std:: cout << "AAAAAA"<< std:: endl;
    std::cout << *(matrix.begin()) << std::endl;

    std::cout << *(matrix.begin()+1) << std::endl;
    return 0;
}
