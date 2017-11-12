#include <iostream>
#include <cstdlib>
#include "omp.h"

using namespace std;

void get_min_element(int **matrix, int *result, int length) {

#pragma omp parallel for
    for (int i = 0; i < length; i++) {

        int index = 0, value = matrix[i][0];

        for (int j = 1; j < length; j++) {
            if ((value > matrix[i][j])) {
                value = matrix[i][j];
                index = j;
            }
        }
        result[i] = index;
    }
}

int main() {

    int n = 10000 ;
    double start, finish;

    auto matrix = new int *[n];

    start = omp_get_wtime();
    cout << "Initialization start" << endl;

    for (int i = 0; i < n; i++) {
        matrix[i] = new int[n];
        for (int j = 0; j < n; j++) {
            matrix[i][j] = rand() % 10000;
        }
    }

    finish = omp_get_wtime();

    cout << "Initialization finish" << endl;
    cout << "Initialization time: " << finish - start << endl;

    auto result = new int[n];

    start = omp_get_wtime();

    get_min_element(matrix, result, n);

    finish = omp_get_wtime();

    cout << "Parallel part time: " << finish - start << endl;

    return 0;
}