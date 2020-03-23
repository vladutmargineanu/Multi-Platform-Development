#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utils.h"
#include "hashTable.h"

#define ENOMEN 12

static int hashFunction(void *ptr, int size)
{
	return (int) (((long) ptr) % size);
}

static int growHashTable(HashTable ht)
{
	if (ht->countElements >= ht->size * 3 / 4)
		return 1;
	return -1;
}

static void addentryHashTable(Node *pprev, Node entry)
{
	entry->next = *pprev;
	*pprev = entry;
}

static int rehashTable(HashTable ht)
{
	/* Vectori de noduri pentru buckets */
	Node *current, *buckets;
	Node node, nextNode;
	unsigned int s, size;
	unsigned int i, index;

	current = ht->buckets;
	s = ht->size;
	size = s << 1;
	buckets = calloc(size, sizeof(Node));
	DIE(buckets < 0, "ENOMEM");
	for (i = 0; i < s; i++) {
		for (node = current[i]; node != NULL; node = nextNode) {
			index = hashFunction(node->key, size);
			nextNode = node->next;
			addentryHashTable(&buckets[index], node);
		}
	}

	free(ht->buckets);
	ht->buckets = buckets;
	ht->size = size;

	return 0;
}

void initHashTable(HashTable ht, unsigned int size)
{
	ht->size = size;
	ht->countElements = 0;
}

HashTable constructHashTable(unsigned int size)
{
	HashTable ht;
	
	ht = (HashTable) malloc(sizeof(struct hashTable));
	DIE(ht < 0, "ENOMEM");
	ht->buckets = calloc(size, sizeof(Node));
	DIE(ht->buckets < 0, "ENOMEM");
	initHashTable(ht, size);
	return ht;
}

void *getHashTable(HashTable ht, char *key)
{
	int index = hashFunction(key, ht->size);
	Node node;

	for (node = ht->buckets[index]; node != NULL; node = node->next) {
		if ((strcmp(node->key, key)) == 0)
			return node->value;
	}

	return NULL;
}

int putHashTable(HashTable ht, char *key, void *value)
{
	Node node, newNode;
	int index = hashFunction(key, ht->size);
	int err;

	for (node = ht->buckets[index]; node != NULL; node = node->next) {
		if ((strcmp(node->key, key)) == 0) {
			/* Cheia exista deja in HashTable */
			node->value = value;
			return 0;
		}
	}

	newNode = (Node) malloc(sizeof(struct nodeHashTable));
	DIE(newNode < 0, "ENOMEM");
	newNode->key = key;
	newNode->value = value;
	addentryHashTable(&(ht->buckets[index]), newNode);
	++ht->countElements;

	if (growHashTable(ht) > 0) {
		err = rehashTable(ht);
		if (err)
			return err;
	}
	return 0;
}

unsigned int sizeHashTable(HashTable ht)
{
	return ht->size;
}

unsigned int capacityHashTable(HashTable ht)
{
	return ht->countElements;
}

int isEmpty(HashTable ht)
{
	if (ht->countElements == 0)
		return 1;
	return -1;
}

void removeElement(HashTable ht, char *key)
{
	int index = hashFunction(key, ht->size);
	Node node, nextNode;

	node = ht->buckets[index];
	if (node == NULL)
		return;

	if (node->next == NULL) {
		free(node);
		ht->buckets[index] = NULL;
		ht->countElements--;
		return;
	}
	while (node != NULL) {
		nextNode = node->next;
		if (nextNode != NULL && (strcmp(nextNode->key, key)) == 0) {
			node->next = nextNode->next;
			ht->countElements--;
			free(nextNode);
			return;
		}
		node = nextNode;
	}
}

void freeHashTable(HashTable ht)
{
	unsigned int i;
	Node node, nextNode;

	for (i = 0; i < ht->size; i++) {
		for (node = ht->buckets[i]; node != NULL;) {
			nextNode = node->next;
			free(node);
			node = nextNode;
		}
	}

	free(ht->buckets);
	free(ht);
}
