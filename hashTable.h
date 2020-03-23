#ifndef HASHTABLE_HEADER
#define HASHTABLE_HEADER

typedef struct nodeHashTable *Node;
typedef struct hashTable *HashTable;

struct nodeHashTable {
	Node next;
	char *key;
	void *value;
};

struct hashTable {
	Node *buckets;  // vector de noduri
	unsigned int size;
	unsigned int countElements;
};

HashTable constructHashTable(unsigned int size);
void *getHashTable(HashTable hm, char *key);
int putHashTable(HashTable hm, char *key, void *value);
unsigned int sizeHashTable(HashTable hm);
unsigned int capacityHashTable(HashTable hm);
int isEmpty(HashTable hm);
void removeElement(HashTable hm, char *key);
void freeHashTable(HashTable hm);

#endif
