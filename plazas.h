#ifndef PLAZAS_H
#define PLAZAS_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <vector>

#define PLAZAS 10

#define OCIOSO 1
#define ESPERANDO 2
#define APARCADO 3

using namespace std;

typedef struct Plaza_t{
    bool ocupado;
    int numeroPlaza;
    int thread_Id_coche;
}Plaza_t;

vector<string>* get_elements(string line,string split)
{
     vector<string>* out = new vector<string>();

     char * c_line = new char[line.size() + 1];
     std::copy(line.begin(), line.end(), c_line);
     c_line[line.size()] = '\0';

     char *token = (strtok(c_line, split.c_str()));
     if(token != NULL)
     {
         out->push_back(string(token));
         while (token != NULL) {
             token = (strtok(NULL, split.c_str()));
             if (token != NULL)
                 out->push_back(string(token));
         }
     }
     return out;
}

#endif
