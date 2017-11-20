//
// Created by kirov on 17.11.17.
//

#include "implementations.h"


void serial_implemenation(int** matrix, int n, int** result){
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            int sum = 0;
            for(int k = 0; k < n; k++){
                sum += matrix[i][k]*matrix[k][j];
            }
            result[i][j] = sum;
        }
    }
}