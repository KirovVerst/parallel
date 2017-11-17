#include <iostream>
#include <mpi.h>

int main() {
    int n_rank, n_size;
    MPI_Init(NULL, NULL);

    MPI_Comm_rank(MPI_COMM_WORLD, &n_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_size);
    std::cout << "Hello, World!" << std::endl;
    MPI_Finalize();
    return 0;
}
