#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashTable.h"
#include "utils.h"

#define BUFSIZE         256
#define MAX_DIRECTIVE   8

static void definePreprocesor(HashTable ht, FILE *fd1, FILE *fd2)
{
	char *token = NULL;
	char *mapping = NULL;
	char buffer[BUFSIZE];
	char bufferCopy[BUFSIZE];
	char *delim = "\t []{}<>=+-*/%!&|^.,:;()\\";
	char *line = NULL;
	int pos;
	/* Functia pentru directivele #define */
	while (fgets(buffer, BUFSIZE, fd1) != NULL) {
		strcpy(bufferCopy, buffer);
			token = strtok(buffer, delim);
			/* Citim linie cu linie si 
			inlocuim simbolul cu valoarea */
			while (token != NULL) { 
				mapping = getHashTable(ht, token);
				if (mapping != NULL) {
					pos = strstr(bufferCopy, token) - bufferCopy;
					strncat(line, bufferCopy, pos);
					strcat(line, mapping);
					strcat(line, bufferCopy + pos + strlen(mapping));
				} else {
					strcpy(line, bufferCopy);
				}
				token = strtok(NULL, delim); 
			} 
		/* Scriem in fisierul de iesire */    
		fwrite(line, 1, sizeof(line), fd2);
	}
	/* Inchidem fisierele de in si out */
	fclose(fd1);
	fclose(fd2);
}

static void parseLine(int numberArgv, char **copyLine)
{	char *token = NULL;
	char *retStr;
	char *symbol = NULL, *mapping = NULL;
	char director[BUFSIZE][BUFSIZE];
	int numberDirector = 0;
	char outFile[BUFSIZE];
	char inFile[BUFSIZE];
	int findInFile = 0;
	int findOutFile = 0;
	FILE *fd1, *fd2;
	char buffer[BUFSIZE];
	char bufferCopy[BUFSIZE];
	char *ret;
	HashTable ht;
	int number = 1;
	char *delim = "=";
	char str1[BUFSIZE];
	char *delim2 = " ";

	ht = constructHashTable(50);
	/* Parsam linia de comanda cu parametrii dupa caz */
	while (number < numberArgv) {
		if (strcmp(copyLine[number], "-D") == 0) {
			strcpy(bufferCopy, copyLine[number + 1]);
			token = strtok(bufferCopy, delim);
			symbol = token;
			token = strtok(NULL, delim);
			mapping = token;
			putHashTable(ht, symbol, mapping);
			number++;
		} else if (strncmp(copyLine[number],
					"-D", strlen("-D")) == 0) {            
			strcpy(bufferCopy, copyLine[number]);
			retStr = bufferCopy;
			retStr = retStr + 2;
			token = strtok(retStr, delim);
			symbol = token;
			token = strtok(NULL, delim);
			mapping = token;
			putHashTable(ht, symbol, mapping);
			number++;
		} else if (strcmp(copyLine[number], "-I") == 0) {
			strcpy(director[numberDirector], copyLine[number + 1]);
			numberDirector++;
			number++;
		} else if (strncmp(copyLine[number], "-I",
							 strlen("-I")) == 0) {
			strcpy(bufferCopy, copyLine[number]);
			retStr = bufferCopy; 
			retStr = retStr + 2;
			strcpy(director[numberDirector], retStr);
			numberDirector++;
			number++;
		} else if (strcmp(copyLine[number], "-o") == 0
								 && findOutFile == 0) {
			strcpy(outFile, copyLine[number + 1]);
			number++;
			findOutFile = 1;
		} else if (strncmp(copyLine[number], "-o", 
					strlen("-o")) == 0 && findOutFile == 0) {
			strcpy(outFile, copyLine[number]);
			retStr = outFile;
			retStr = retStr + 2;
			strcpy(outFile, retStr);
			number++;
			findOutFile = 1;
		} else if (findInFile == 0) {
			strcpy(inFile, copyLine[number]);
			number++;
			findInFile = 1;
		} else if (findOutFile == 0) {
			strcpy(outFile, copyLine[number]);
			printf("%s\n", outFile);
			number++;
			findOutFile = 1;
		} else if (findInFile == 1 && findOutFile == 0) {
			/* Caz in care avem doar fisier
			 de in si nu avem de out */
			fd1 = fopen(inFile, "w+");
			fscanf(fd1, "%s", str1);
			printf("%s", str1);
		} else if (findInFile == 1 && findOutFile == 1) {
			/* Caz in care avem mai multi parametrii */
			DIE(findInFile == 1 && findOutFile == 1, "Error");
		}
	}
	/* Daca avem fisier de in si out le deschidem pentru preprocesare */
	if (findInFile != 0) {
		fd1 = fopen(inFile, "r");
		DIE(fd1 < 0, "open in");
	} else {
		fd1 = stdin;
	}

	if (findOutFile != 0) {
		fd2 = fopen(outFile, "w");
		DIE(fd2 < 0, "open out");
	} else {
		fd2 = stdout;
	}
	/* Verificam daca avem #define uri in fisierul in */
	while (fgets(buffer, BUFSIZE, fd1) != NULL) {

		ret = strstr("#define", buffer);
		if (ret != NULL) {
			ret = ret + strlen("#define") + 1;
			token = strtok(ret, delim2);
			symbol = token;
			printf("%s\n", symbol);
			token = strtok(NULL, delim2);
			mapping = token;
			printf("sfarsit %s\n", mapping);
			/* Mapam simbolul si valoarea in hash */
			putHashTable(ht, symbol, mapping);
		}
	}

	definePreprocesor(ht, fd1, fd2);
	freeHashTable(ht);
}

int main(int argc, char **argv) 
{
	char str1[256];

	if (argc < 2) {
		scanf("%s", str1);
		printf("%s", str1);
	} else 
		if (argc >= 2)
		  parseLine(argc, argv);

	return 0;
}
