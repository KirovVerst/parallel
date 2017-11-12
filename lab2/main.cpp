#include <iostream>
#include <cmath>
#include "omp.h"

using namespace std;

int MAX_THREAD_NUMBER = omp_get_max_threads();
long long MAX_VECTOR_SIZE = 15 * (long long) pow(10, 8);


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

void serial_scalar_multiplication(int *v1, int *v2, long long size, long long &result) {
    result = 0;

    for (long long i = 0; i < size; i++) {
        result += v1[i] * v2[i];
    }
}

void parallel_scalar_multiplication(int *v1, int *v2, long long size, int thread_number, long long &result) {

    result = 0;

#pragma omp parallel num_threads(thread_number)
#pragma omp for schedule(static) reduction(+:result)
    for (long long i = 0; i < size; i++) {
        result += v1[i] * v2[i];
    }
}


void init_vector(int *vect, long long size) {
    for (long long i = 0; i < size; i++) {
        vect[i] = -10 + (double) rand() * 20 / RAND_MAX;
    }
}

void vector_task_run() {
    int *v1 = new int[MAX_VECTOR_SIZE];
    int *v2 = new int[MAX_VECTOR_SIZE];

    init_vector(v1, MAX_VECTOR_SIZE);
    init_vector(v2, MAX_VECTOR_SIZE);

    long long vector_sizes[] = {(long long) 10e4, (long long) 10e5, (long long) 10e6, (long long) 10e7,
                                (long long) 10e8, MAX_VECTOR_SIZE};

    double start, finish;
    long long result;

    for (long long &vector_size: vector_sizes) {
        cout << "Vector size : " << vector_size << endl;

        start = omp_get_wtime();

        serial_scalar_multiplication(v1, v2, vector_size, result);

        finish = omp_get_wtime();

        cout << "Serial scalar time: " << finish - start << endl;

        for (int thread_number = 2; thread_number <= MAX_THREAD_NUMBER; thread_number++) {
            start = omp_get_wtime();

            parallel_scalar_multiplication(v1, v2, vector_size, thread_number, result);

            finish = omp_get_wtime();

            cout << thread_number << " threads' time: " << finish - start << endl;
        }
        cout << endl;
    }

    delete[] v1;
    delete[] v2;
}

int main() {
    srand((unsigned int) time(NULL));
    cout.precision(10);
    vector_task_run();

    return 0;
}