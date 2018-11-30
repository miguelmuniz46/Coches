# Prueba mpi

#variable compilador
CC=mpiCC
#optimizaciones
#CFLAGS=-DDEBUG -O3 #-D (pasar variables) -O[0,1,2,3](optimizacion)
CFLAGS=-g -DDEBUG
LDLIBS=
INCLUDE=-I. -I./include
EXEC1=master
EXEC2=coche
OBJS1=main_master.o
OBJS2=coche.o
all: $(OBJS1) $(OBJS2)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDLIBS) $(OBJS1) -o $(EXEC1)
	$(CC) $(CFLAGS) $(INCLUDE) $(LDLIBS) $(OBJS2) -o $(EXEC2)

%.o: %.cpp
	$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
#para cada fichero.o busca su equivalente .cpp // -c para no linkar //$< = %.cpp //  $@ = %.o

clean:
	rm *.o	
	rm $(EXEC)
