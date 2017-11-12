#include <mpi.h>
#include <iostream>
#include <cmath>

using namespace std;

long long N = 4 * (long long) pow(10, 5);
long long MAX_MESSAGE_LENGTH = (long long) pow(10, 4) + 1;

int SIZES[] = {1, 10, 100, 500, 1000, 2000, 3000, 4000, 5000};
int SIZES_COUNT = sizeof(SIZES) / sizeof(SIZES[0]);

void send_recv(int curr_rank, int *rbuf, int *sbuf) {
    double start, finish;
    MPI_Status status;

    if (curr_rank == 0) {
        cout << "\nSend and Recv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            if (curr_rank == 0) {
                MPI_Send(sbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            } else if (curr_rank == 1) {
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Send(sbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void ssend_recv(int curr_rank, int *rbuf, int *sbuf) {
    double start, finish;
    MPI_Status status;
    if (curr_rank == 0) {
        cout << "\nSsend and Recv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            if (curr_rank == 0) {
                MPI_Ssend(sbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            } else if (curr_rank == 1) {
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Ssend(sbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void rsend_recv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nRsend and Recv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            if (curr_rank == 0) {
                MPI_Rsend(sbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD);
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 1, 0, MPI_COMM_WORLD, &status);
            } else if (curr_rank == 1) {
                MPI_Recv(rbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
                MPI_Rsend(sbuf, SIZES[i], MPI_INT, 0, 0, MPI_COMM_WORLD);
            }
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void bsend_recv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nBsend and Recv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        int packed_message_size;
        MPI_Pack_size(SIZES[i], MPI_INT, MPI_COMM_WORLD, &packed_message_size);

        int buff_size = N * (MPI_BSEND_OVERHEAD + SIZES[i]);
        int *buf = new int[buff_size];

        MPI_Buffer_attach(buf, buff_size);

        for (int j = 0; j < N; j++) {
            MPI_Bsend(sbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD);
            MPI_Recv(rbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &status);
        }

        MPI_Buffer_detach(buf, &buff_size);

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void send_recv_combined(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Status status;

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nSendRecv method" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            MPI_Sendrecv(sbuf, SIZES[i], MPI_INT, other_process, 0,
                         rbuf, SIZES[i], MPI_INT, other_process, 0,
                         MPI_COMM_WORLD, &status);
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void isend_irecv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Request requests[2];
    MPI_Status statuses[2];

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nIsend and Irecv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            MPI_Isend(sbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(rbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, statuses);
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void issend_irecv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Request requests[2];
    MPI_Status statuses[2];

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nIssend and Irecv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            MPI_Issend(sbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(rbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, statuses);
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void irsend_irecv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Request requests[2];
    MPI_Status statuses[2];

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nIrsend and Irecv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        for (int j = 0; j < N; j++) {
            MPI_Irsend(sbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(rbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, statuses);
        }

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}

void ibsend_recv(int curr_rank, int *rbuf, int *sbuf) {
    int other_process = (curr_rank + 1) % 2;
    MPI_Request requests[2];
    MPI_Status statuses[2];

    double start, finish;
    if (curr_rank == 0) {
        cout << "\nIbsend and Irecv methods" << endl;
    }

    for (int i = 0; i < SIZES_COUNT; i++) {

        start = MPI_Wtime();

        int packed_message_size;
        MPI_Pack_size(SIZES[i], MPI_INT, MPI_COMM_WORLD, &packed_message_size);

        int buff_size = N * (MPI_BSEND_OVERHEAD + SIZES[i]);

        int *buf = new int[buff_size];

        MPI_Buffer_attach(buf, buff_size);

        for (int j = 0; j < N; j++) {
            MPI_Ibsend(sbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[0]);
            MPI_Irecv(rbuf, SIZES[i], MPI_INT, other_process, 0, MPI_COMM_WORLD, &requests[1]);
            MPI_Waitall(2, requests, statuses);
        }

        MPI_Buffer_detach(buf, &buff_size);
        delete[] buf;

        finish = MPI_Wtime();

        if (curr_rank == 0) {
            cout << "Message size : " << SIZES[i] << "\tTime: " << finish - start << endl;
        }
    }
}


int main(int argc, char **argv) {
    int n_rank, curr_rank;

    MPI_Init(NULL, NULL);

    MPI_Comm_size(MPI_COMM_WORLD, &n_rank);
    MPI_Comm_rank(MPI_COMM_WORLD, &curr_rank);

    int rbuf[MAX_MESSAGE_LENGTH];
    int sbuf[MAX_MESSAGE_LENGTH];

    fill_n(sbuf, MAX_MESSAGE_LENGTH, 1);

    send_recv(curr_rank, rbuf, sbuf);

    ssend_recv(curr_rank, rbuf, sbuf);

    rsend_recv(curr_rank, rbuf, sbuf);

    bsend_recv(curr_rank, rbuf, sbuf);

    send_recv_combined(curr_rank, rbuf, sbuf);

    isend_irecv(curr_rank, rbuf, sbuf);

    issend_irecv(curr_rank, rbuf, sbuf);

    irsend_irecv(curr_rank, rbuf, sbuf);

    ibsend_recv(curr_rank, rbuf, sbuf);

    MPI_Finalize();

    return 0;
}
