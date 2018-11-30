#include <stdio.h>
#include <stdlib.h>
#include "plazas.h"
#include "mpi/mpi.h"

#define OCIOSO 1
#define ESPERANDO 2
#define APARCADO 3

//El semáforo tiene que estar en el main

int recv_ID(){
    int id = 0;

    MPI_Comm parent;
    MPI_Status status;
    MPI_Comm_get_parent(&parent);

    MPI_Recv(&id, 1, MPI_INT, 0, MPI_ANY_TAG, parent, &status);
    return id;
}

void send_MSG(char *msg, int size){
       MPI_Comm parent;
       MPI_Comm_get_parent(&parent);

       MPI_Send(msg, size, MPI_CHAR, 0, 0, parent);
}

int main(int argc, char **argv){
    char *msg=new char[100];
    int globalID;
    int rank = 0;
    int estado = OCIOSO;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    globalID = recv_ID();

    sprintf(msg, "soy el coche globalID %d, estado %d\n", globalID, estado);

    while(true){
        send_MSG(msg, strlen(msg)+1);
        //recibir mensaje
    }

    MPI_Finalize();

    return 0;
}
