#ifndef APARCAMIENTO_H
#define APARCAMIENTO_H

#include "plazas.h"
#include <iostream>
#include <unistd.h>
#include <vector>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "mpi/mpi.h"
#include <unistd.h>
#include <pthread.h>

using namespace std;

class Aparcamiento{
    Plaza_t *aparcamiento;
    int plazasLibres;
    bool stop;
public:
    Aparcamiento();
    void coche();
    void liberarPlaza(int globalId);
    void monitor();
    bool isOcupado(Plaza_t plaza);
    void imprimirAscii();
    void imprimirCochesEsperando();
    void imprimirCochesOciosos();
    void freePlazas();

    void menu();

    void setStop(bool estado);
    bool getStop();
};



#endif


