TARGETS =p2-dogProgramServer p2-dogProgramClient
CC=gcc
LDFLAGS=-pthread

all: $(TARGETS)

p2-dogProgramServer: p2-dogProgramServer.c 
	$(CC) $^ $(LDFLAGS) -o $@ 

p2-dogProgramClient: p2-dogProgramClient.c
	$(CC) $^ -o $@ 
