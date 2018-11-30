#include <stdio.h>
#include <stdlib.h>
#include "plazas.h"
#include "mpi/mpi.h"

//El semáforo tiene que estar en el main

int recv_ID(){
    int id = 0;

    MPI_Comm parent;
    MPI_Status status;
    MPI_Comm_get_parent(&parent);

    MPI_Recv(&id, 1, MPI_INT, 0, MPI_ANY_TAG, parent, &status);
    return id;
}

int recv_estado(){
    int estado = 0;
    MPI_Comm parent;
    MPI_Status status;
    MPI_Comm_get_parent(&parent);

    MPI_Recv(&estado, 1, MPI_INT, 0, MPI_ANY_TAG, parent, &status);
}

void send_MSG(char *msg, int size){
       MPI_Comm parent;
       MPI_Comm_get_parent(&parent);

       MPI_Send(msg, size, MPI_CHAR, 0, 0, parent);
}

int main(int argc, char **argv){
    char *msg_globalId=new char[100];
    char *msg_estado=new char[100];
    int globalID;
    int rank = 0;
    int estado = OCIOSO;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalID = recv_ID();

    while(true){
        sprintf(msg_globalId, "%d", globalID);
        sprintf(msg_estado, "%d", estado);

        send_MSG(msg_globalId, strlen(msg_globalId)+1);
        send_MSG(msg_estado, strlen(msg_estado)+1);

        estado = recv_estado();
    }

    MPI_Finalize();

    return 0;
}
