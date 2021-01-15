TARGETS =p2-dogProgramServer p2-dogProgramClient Crear Generador-de-tabla-hash
CC=gcc

all: $(TARGETS)

p2-dogProgramServer: p2-dogProgramServer.c 
	$(CC) $^ -pthread -o $@ 
