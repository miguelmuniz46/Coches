#include "aparcamiento.h"

std::vector<MPI_Comm*> listaCoches;
std::vector<int> listaCochesEsperando;
std::vector<int> listaCochesOciosos;

void nuevo_coche(){
    MPI_Comm *commCoche = new MPI_Comm[1];

    MPI_Comm_spawn("coche", MPI_ARGV_NULL, 1, MPI_INFO_NULL, 0, MPI_COMM_SELF, commCoche, MPI_ERRCODES_IGNORE);

    listaCoches.push_back(commCoche);
    int newID=listaCoches.size();
    MPI_Send(&newID,1,MPI_INT,0,0,*commCoche);
}

void *threadCrearCoches(void* args){
    int numCoches = 0;

    while(numCoches != 20){
        nuevo_coche();
        numCoches++;
    }
    pthread_exit(NULL);
}

void recibe_msg(char **msg){
    MPI_Status status;
    int flag;
    int msgfound = -1;
    int size = listaCoches.size();

    for(int i = 0; i < size && msgfound == -1; i++){
        flag = 0;
        MPI_Iprobe(MPI_ANY_SOURCE, MPI_ANY_TAG, *(listaCoches[i]), &flag, &status);
        if(flag){
            msgfound=i;
        }
    }

    if(msgfound != -1){
        int msgSize = 0;
        MPI_Get_count(&status, MPI_CHAR, &msgSize);
        (*msg)=new char[msgSize];
        MPI_Recv(*msg, msgSize, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, *(listaCoches[msgfound]), &status);
    }
}

Aparcamiento::Aparcamiento(){
    aparcamiento = (Plaza_t *)malloc(sizeof(Plaza_t)*PLAZAS);

    for (int i = 0; i < PLAZAS; i++) {
        aparcamiento[i].ocupado = false;
        aparcamiento[i].numeroPlaza = i + 1;
        aparcamiento[i].thread_Id_coche = 0;
    }

    stop = false;
    plazasLibres = PLAZAS;
}
/*

void Aparcamiento::liberarPlaza(Thread_coche_t *coche){

    omp_set_lock(&lock);
    for (int j = 0; j < PLAZAS; j++) {
        if (aparcamiento[j].thread_Id_coche == coche->thread_Id) {
            aparcamiento[j].ocupado = false;
            aparcamiento[j].thread_Id_coche = NULL;
            aparcamiento[j].tiempo = 0.0;
            coche->estado = OCIOSO;
            plazasLibres++;
        }
    }
    omp_unset_lock(&lock);
}

bool Aparcamiento::isOcupado(Plaza_t plaza){
    if (plaza.ocupado) {
        return true;
    }

    return false;
}

void Aparcamiento::monitor(Thread_coche_t *coches){
    while (true) {
        if (!stop) {
            Sleep(5000);

            if (!stop) {
                omp_set_lock(&lock);
                for (int j = 0; j < PLAZAS; j++) {
                    if ((omp_get_wtime() - aparcamiento[j].tiempo) > 2 && aparcamiento[j].ocupado) {
                        aparcamiento[j].ocupado = false;
                        aparcamiento[j].tiempo = 0.0;
                        printf("\nCoche %d eliminado por el monitor\n", aparcamiento[j].thread_Id_coche);
                        coches[aparcamiento[j].thread_Id_coche - 1].estado = OCIOSO;
                        aparcamiento[j].thread_Id_coche = NULL;
                        plazasLibres++;
                    }
                }
                omp_unset_lock(&lock);
            }

        }
    }
}

void Aparcamiento::imprimirAscii(){
    printf("\nAparcamiento:");
    printf("\n_________________________\n");

    for (int i = 0; i < PLAZAS; i++) {
        if (i <= 4) {
            if (aparcamiento[i].ocupado) {
                if (aparcamiento[i].thread_Id_coche < 10) {
                    printf("| 0%d ", aparcamiento[i].thread_Id_coche);
                }
                else {
                    printf("| %d ", aparcamiento[i].thread_Id_coche);
                }
            }
            else {
                printf("|    ");
            }
        }
        else {
            if (i == 5) {
                printf("|");
                printf("\n");
            }

            if (aparcamiento[i].ocupado) {
                if (aparcamiento[i].thread_Id_coche < 10) {
                    printf("| 0%d ", aparcamiento[i].thread_Id_coche);
                }
                else {
                    printf("| %d ", aparcamiento[i].thread_Id_coche);
                }
            }
            else {
                printf("|    ");
            }
        }
    }
    printf("|");

    printf("\n_________________________\n");
}

void Aparcamiento::imprimirCochesEsperando(vector<MPI_Comm*> listaCoches){
    for (int i = 0; i < num_coches; i++) {
        if (coches[i].estado == ESPERANDO) {
            printf("Coche: %d Estado: Esperando \n", coches[i].thread_Id);
        }
    }
}

void Aparcamiento::imprimirCochesOciosos(Thread_coche_t * coches, int num_coches){
    omp_set_lock(&lock);
    for (int i = 0; i < num_coches; i++) {
        if (coches[i].estado == OCIOSO) {
            printf("Coche: %d Estado: Ocioso \n", coches[i].thread_Id);
        }
    }
    omp_unset_lock(&lock);
}

void Aparcamiento::freePlazas(){
        free(aparcamiento);
}

void Aparcamiento::menu(Thread_coche_t * coches){
    int eleccion = 0;
    bool exit = false;

    while (!exit) {
        bool estado = getStop();
        printf("Seleccione una de las siguientes opciones\n");
        printf("1.- Parar/Continuar el sistema\n2.- Mostrar estado actual\n3.- Nuevo coche\n4.- Salida\n");
        scanf("%d", &eleccion);
        getchar();

        switch (eleccion) {
        case 1:
            if (estado) {
                setStop(false);
            }
            else {
                setStop(true);
            }
            break;
        case 2:
            //aparcamiento->imprimirAscii();
            imprimirAparcamiento();
            printf("\n");
            imprimirCochesEsperando(coches, getNum_Coches());
            printf("\n");
            imprimirCochesOciosos(coches, getNum_Coches());
            printf("\n");
            break;
        case 3:
            #pragma omp parallel sections
            {
                #pragma omp section
                {
                    coches = setNum_Coches(coches);
                    coche(&(coches[getNum_Coches()-1]));
                }

                #pragma omp section
                {
                    menu(coches);
                }
            }
            break;
        case 4:
            //#pragma omp cancellation point for
            setStop(true);
            exit = true;
            break;
        default:
            printf("Ninguna de las opciones ha sido escogida\n");
            break;
        }
    }

    liberarThreads(coches);
    freePlazas();
    delete[]aparcamiento;

    terminate();
}

void Aparcamiento::setStop(bool estado){
    stop = estado;
}

bool Aparcamiento::getStop(){
    return stop;
}
*/

void coche(){
    int *estado;
    srand(time(NULL));
    int random;

    while (true) {
            //recibe el mensaje, en este caso el coche con el estado
            recibe_msg(estado);
            switch (estado) {
            case APARCADO:
                random = rand() % (3000 + 1);
                sleep(random);


                liberarPlaza(coche);
                break;
            case OCIOSO:
                random = rand() % (3000 + 1);
                sleep(3000);


                //enviar a ese coche el mensaje, que será que ha cambiado de estado
                listaCochesEsperando.push_back(globalId);
                break;
            case ESPERANDO:
                if (plazasLibres > 0) {
                    for (int j = 0; j < PLAZAS; j++) {
                        if (!isOcupado(aparcamiento[j])) {
                            aparcamiento[j].ocupado = true;
                            aparcamiento[j].thread_Id_coche = globalId;
                            //quitamos de la lista esperando
                            plazasLibres--;
                            break;
                        }
                    }
                }
                else {
                    sleep(3000);
                }
            }
        }
    }
void liberarPlaza(){

}
void monitor(){

}
bool isOcupado(){
    return true;
}
void imprimirAscii(){

}
void imprimirCochesEsperando(){

}
void imprimirCochesOciosos(){

}
void freePlazas(){

}

void menu(){

}

int main(int argc, char **argv){
    int rank = 0;
    int eleccion = 0;
    bool exit = false;
    pthread_t threadCoche;

    Aparcamiento *aparcamiento;
    aparcamiento = new Aparcamiento();

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    int *msg;
    int stop = 0;

    pthread_create(&threadCoche, NULL, threadCrearCoches, NULL);

    /*
    while (!exit) {
        bool estado = getStop();
        sprintf("Seleccione una de las siguientes opciones\n");
        sprintf("1.- Parar/Continuar el sistema\n2.- Mostrar estado actual\n3.- Salida\n");
        cin >> eleccion;

        switch (eleccion) {
        case 1:
            if (estado) {
                setStop(false);
            }
            else {
                setStop(true);
            }
            break;
        case 2:
            aparcamiento->imprimirAscii();
            sprintf("\n");
            imprimirCochesEsperando(coches, getNum_Coches());
            sprintf("\n");
            imprimirCochesOciosos(coches, getNum_Coches());
            sprintf("\n");
            break;
        case 3:
            setStop(true);
            exit = true;
            break;
        default:
            sprintf("Ninguna de las opciones ha sido escogida\n");
            break;
    }

    liberarThreads(coches);
    freePlazas();
    delete[]aparcamiento;

    terminate();
    */
    /*
    while(1){
        estado = 0;
        recibe_estado(estado);

        if(estado!=0){
            if(estado == 1){
                printf("Estoy ocioso\n");
            }
            else if(estado == 2){
                printf("Estoy esperando\n");
            }
            else if(estado == 3){
                printf("Estoy aparcado\n");
            }
        }

        sleep(1);
    }
    */

    //espera mensaje de entrada para el menu
    while(true){
        aparcamiento->coche();
    }


    //Si el coche intenta entrar y no puede lo añadimos a la lista
    //Cuando sale se mira la lista de esperando para ocuparlo
    MPI_Finalize();

    return 0;
}
