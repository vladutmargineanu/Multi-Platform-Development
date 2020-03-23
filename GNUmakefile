CC=gcc
CFLAGS=-Wall
EXE=so-cpp

build: $(EXE)

$(EXE): tema01.o hashTable.o
	$(CC) -g tema01.o hashTable.o -o $(EXE)

tema01.o: tema01.c hashTable.h utils.h
	$(CC) -g $(CFLAGS) -c tema01.c  -o tema01.o 

hashTable.o: hashTable.c hashTable.h utils.h
	$(CC) -g $(CFLAGS) -c hashTable.c -o hashTable.o

clean: 
	rm -f tema01.o hashTable.o $(EXE)
