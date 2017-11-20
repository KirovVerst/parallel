#include <iostream>
#include <ctime>
#include <cstdlib>
#include "mpi.h"

using namespace std;

int SIZES[] = {1000, 2000, 3000};

void show_matrix(int **matrix, int size) {
    cout << "Show Matrix" << endl;
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            cout << matrix[i][j] << '\t';
        }
        cout << endl;
    }
}


void init_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        fill_n(matrix[i], size, i);
    }
}


void string_c_to_matrix(int *string, int size, int **matrix) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i][j] = string[i * size + j];
        }
    }
}

void matrix_a_to_string(int **matrix, int size, int *string) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            string[i * size + j] = matrix[i][j];
        }
    }
}

void matrix_b_to_string(int **matrix, int size, int *string) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            string[i * size + j] = matrix[j][i];
        }
    }
}

void destroy_matrix(int **matrix, int size) {
    for (int i = 0; i < size; i++) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void run_serial() {

    for (int i = 0; i < sizeof(SIZES) / sizeof(SIZES[0]); i++) {
        int size = SIZES[i];
        int **matrix = new int *[size];
        int **result = new int *[size];

        for (int i = 0; i < size; i++) {
            matrix[i] = new int[size];
            result[i] = new int[size];
        }

        init_matrix(matrix, size);

        int start = clock();

        int *string = new int[size * size];
        matrix_a_to_string(matrix, size, string);


        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                int sum = 0;
                for (int k = 0; k < size; k++) {
                    sum += string[i * size + k] * string[k * size + j];
                }
                //result[i][j] = sum;
            }
        }

        int finish = clock();

        cout << size << " : " << (double) (finish - start) / CLOCKS_PER_SEC << endl;

        delete[] string;
    }


}


void parallel_multiplication(int *string_a, int *string_b, int *string_c, int size) {
    int rank, comm_size;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    int i, j, k, p, ind;

    int temp;

    MPI_Status Status;
    int rows_number = size / comm_size;
    int elements_number = rows_number * size;
    int *buffer_a = new int[elements_number];
    int *buffer_b = new int[elements_number];
    int *buffer_c = new int[elements_number];

    MPI_Scatter(string_a, elements_number, MPI_INT, buffer_a, elements_number, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Scatter(string_b, elements_number, MPI_INT, buffer_b, elements_number, MPI_INT, 0, MPI_COMM_WORLD);

    temp = 0;
    for (i = 0; i < rows_number; i++) {
        for (j = 0; j < rows_number; j++) {
            for (k = 0; k < size; k++) {
                temp += buffer_a[i * size + k] * buffer_b[j * size + k];
            }
            buffer_c[i * size + j + rows_number * rank] = temp;
            temp = 0;
        }
    }

    int next_rank;
    int prev_rank;

    for (p = 1; p < comm_size; p++) {

        if (rank == comm_size - 1) {
            next_rank = 0;
        } else {
            next_rank = rank + 1;
        }

        if (rank == 0) {
            prev_rank = comm_size - 1;
        } else {
            prev_rank = rank - 1;
        }

        MPI_Sendrecv_replace(buffer_b, elements_number, MPI_INT, next_rank, 0, prev_rank, 0, MPI_COMM_WORLD, &Status);

        temp = 0;

        for (i = 0; i < rows_number; i++) {
            for (j = 0; j < rows_number; j++) {
                for (k = 0; k < size; k++) {
                    temp += buffer_a[i * size + k] * buffer_b[j * size + k];
                }
                if (rank - p >= 0) {
                    ind = rank - p;
                } else {
                    ind = (comm_size - p + rank);
                }

                buffer_c[i * size + j + ind * rows_number] = temp;

                temp = 0;
            }
        }
    }

    MPI_Gather(buffer_c, elements_number, MPI_INT, string_c, elements_number, MPI_INT, 0, MPI_COMM_WORLD);

    delete[]buffer_a;
    delete[]buffer_b;
    delete[]buffer_c;
}

void run_parallel() {

    MPI_Init(NULL, NULL);

    int *string_a, *string_b, *string_c;
    int **matrix_a, **matrix_b, **matrix_c;
    int comm_size;
    int rank;

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);
    if (rank == 0) {
        cout << "Number of processes: " << comm_size << endl;
    }

    for (int i = 0; i < sizeof(SIZES) / sizeof(SIZES[0]); i++) {
        int size = SIZES[i];
        if (rank == 0) {
            matrix_a = new int *[size];
            matrix_b = new int *[size];
            matrix_c = new int *[size];
            for (int i = 0; i < size; i++) {
                matrix_a[i] = new int[size];
                matrix_b[i] = new int[size];
                matrix_c[i] = new int[size];
            }
            init_matrix(matrix_a, size);
            init_matrix(matrix_b, size);
        }

        double start = MPI_Wtime();

        if (rank == 0) {
            string_a = new int[size * size];
            matrix_a_to_string(matrix_a, size, string_a);

            string_b = new int[size * size];
            matrix_b_to_string(matrix_b, size, string_b);

            string_c = new int[size * size];

        }

        parallel_multiplication(string_a, string_b, string_c, size);

        if (rank == 0) {
            string_c_to_matrix(string_c, size, matrix_c);
            cout << size << " : " << MPI_Wtime() - start << endl;
            destroy_matrix(matrix_a, size);
            destroy_matrix(matrix_b, size);
            destroy_matrix(matrix_c, size);
        }
    }

    MPI_Finalize();
}


int main() {

    run_parallel();

    return 0;
}
