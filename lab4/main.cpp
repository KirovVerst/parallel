#include <mpi.h>
#include <iostream>

using namespace std;

long long N = 10000000;
long long MAX_MESSAGE_LENGTH = 1000;

void send_recv(int curr_rank, int *rbuf, int *sbuf, int buff_length, MPI_Status &status) {

    for (int i = 0; i < N; i++) {
        if (curr_rank == 0) {
            MPI_Send(sbuf, buff_length, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(rbuf, buff_length, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        } else if (curr_rank == 1){
            MPI_Recv(rbuf, buff_length, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(sbuf, buff_length, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
}


int main(int argc, char **argv) {
    MPI_Status status;
    int n_rank, curr_rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

    int rbuf[MAX_MESSAGE_LENGTH];
    int sbuf[MAX_MESSAGE_LENGTH];

    fill_n(sbuf, MAX_MESSAGE_LENGTH, 1);

    double start, finish;

    for (int size = 1; size <= 1000; size *= 10) {

        start = MPI_Wtime();

        send_recv(curr_rank, &rbuf[0], &sbuf[0], size, status);

        finish = MPI_Wtime();

        cout << "Message size : " << size << "\tRank " << curr_rank << " time: " << finish - start << endl;
    }

    MPI_Finalize();

    return 0;
}
