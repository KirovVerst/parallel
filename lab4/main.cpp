#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

long long N = (long long) pow(10, 7);
long long MAX_MESSAGE_LENGTH = (long long) pow(10, 4) + 1;

void send_recv(int curr_rank, int *rbuf, int *sbuf, int message_size, MPI_Status &status) {

    for (int i = 0; i < N; i++) {
        if (curr_rank == 0) {
            MPI_Send(sbuf, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD);
            MPI_Recv(rbuf, message_size, MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
        } else if (curr_rank == 1) {
            MPI_Recv(rbuf, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
            MPI_Send(sbuf, message_size, MPI_INT, 0, 0, MPI_COMM_WORLD);
        }
    }
}

void ssend_recv(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;
    for (int i = 0; i < N; i++) {
        MPI_Ssend(sbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD);
        MPI_Recv(rbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &status);
    }
}

void rsend_recv(int curr_rank, int *rbuf, int *sbuf, int buff_length) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;
    for (int i = 0; i < N; i++) {
        MPI_Ssend(sbuf, buff_length, MPI_INT, other_process, 0, MPI_COMM_WORLD);
        MPI_Recv(rbuf, buff_length, MPI_INT, other_process, 0, MPI_COMM_WORLD, &status);
    }
}

void bsend_recv(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;

    int packed_message_size;
    MPI_Pack_size(message_size, MPI_INT, MPI_COMM_WORLD, &packed_message_size);

    int buff_size = N * (MPI_BSEND_OVERHEAD + message_size);
    int *buf = new int[buff_size];

    MPI_Buffer_attach(buf, buff_size);

    for (int i = 0; i < N; i++) {
        MPI_Bsend(sbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD);
        MPI_Recv(rbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &status);
    }

    MPI_Buffer_detach(buf, &buff_size);
}

void send_recv_combined(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;

    for (int i = 0; i < N; i++) {
        MPI_Sendrecv(sbuf, message_size, MPI_INT, other_process, 0,
                     rbuf, message_size, MPI_INT, other_process, 0,
                     MPI_COMM_WORLD, &status);
    }
}

void isend_irecv(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    MPI_Request requests[2];
    MPI_Status statuses[2];
    int other_process = (curr_rank + 1) % 2;

    for (int i = 0; i < N; i++) {
        MPI_Isend(sbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(rbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Waitall(2, requests, statuses);
    }
}

void issend_irecv(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    MPI_Request requests[2];
    MPI_Status statuses[2];
    int other_process = (curr_rank + 1) % 2;

    for (int i = 0; i < N; i++) {
        MPI_Issend(sbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(rbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Waitall(2, requests, statuses);
    }
}

void irsend_irecv(int curr_rank, int *rbuf, int *sbuf, int message_size) {
    MPI_Request requests[2];
    MPI_Status statuses[2];
    int other_process = (curr_rank + 1) % 2;

    for (int i = 0; i < N; i++) {
        MPI_Irsend(sbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
        MPI_Irecv(rbuf, message_size, MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
        MPI_Waitall(2, requests, statuses);
    }
}

int main(int argc, char **argv) {
    int sizes[] = {1, 10, 100, 1000, 2000, 5000};
    int sizes_count = sizeof(sizes) / sizeof(sizes[0]);
    MPI_Status status;
    int n_rank, curr_rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

    int rbuf[MAX_MESSAGE_LENGTH];
    int sbuf[MAX_MESSAGE_LENGTH];

    fill_n(sbuf, MAX_MESSAGE_LENGTH, 1);

    double start, finish;

    for (int i = 0; i < sizes_count; i++) {

        start = MPI_Wtime();

        irsend_irecv(curr_rank, &rbuf[0], &sbuf[0], sizes[i]);

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << sizes[i] << "\tRank " << curr_rank << " time: " << finish - start << endl;
        }
    }

    MPI_Finalize();

    return 0;
}
