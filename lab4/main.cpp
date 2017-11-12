#include <mpi.h>
#include <iostream>

using namespace std;

int main(int argc, char **argv) {
    MPI_Status status;
    int rbuf, sbuf, TAG = 0;

    MPI_Init(NULL, NULL);

    int n_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);

    int curr_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

    sbuf = curr_rank;

    if (curr_rank == 0) {
        MPI_Send(&sbuf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD);
        MPI_Recv(&rbuf, 1, MPI_INT, 1, TAG, MPI_COMM_WORLD, &status);
    } else {
        MPI_Recv(&rbuf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD, &status);
        MPI_Send(&sbuf, 1, MPI_INT, 0, TAG, MPI_COMM_WORLD);
    }

    cout << "Hello from " << rbuf << " to " << curr_rank << endl;

    MPI_Finalize();
}
