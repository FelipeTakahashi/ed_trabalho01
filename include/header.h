#ifndef HEADER
#define HEADER

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

// Estruturas

typedef struct _City{
    char codigo_ibge[20];
    char nome[50];
    double latitude;
    double longitude;
    int capital;
    int codigo_uf;
    int siafi_id;
    int ddd;
    char fuso_horario[50];
}typeCity;

typedef struct _Hash{
    typeCity** table;
    int maxSize;
    int currentSize;
}typeHash;

typedef struct _heap{
    typeCity* city;
    double distance;
}typeHeap;

typedef struct _maxHeap{
    typeHeap* heap;
    int maxSize;
    int size;
}maxHeap;

typedef struct _node{
	typeCity* city;
	struct _node* left;
	struct _node* right;
}typeNode;

// Métodos

typeCity* cityAlloc(char* codigo_ibge, char* nome, double latitude, double longitude, int capital, int codigo_uf, int siafi_id, int ddd, char* fuso_horario);
typeCity* cityAllocSimplified(char* name, double latitude, double longitude);
typeCity* copyCity(typeCity* city);
void printCity(typeCity* city);
void printRow(typeCity* city);
void printRowWithDistance(typeCity* city, double distance);

double distance(typeCity a, typeCity b);

int hashFunction1(char *key);
int hashFunction2(char *key);
int hashInit(typeHash* hash, int buckets);
void hashInsertByCode(typeHash* hash, typeCity* bucket);
void hashInsertByName(typeHash* hash, typeCity* bucket);
typeCity* hashSearchByCode(typeHash* hash, char* key);
char* hashSearchByName(typeHash* hash, char* key);
void hashDestroy(typeHash* hash);

maxHeap* createMaxHeap(int maxSize);
void swap(typeHeap* a, typeHeap* b);
void heapifyDown(maxHeap* heap, int i);
void heapifyUp(maxHeap* heap, int i);
void heapShift(maxHeap* heap, typeCity* city, double distance);
void heapSort(maxHeap* heap);
int checkInHeap(typeCity* city, maxHeap* heap);
void freeHeap(maxHeap* heap);

typeNode* createNode(typeCity* city);
void kdInsert(typeNode** node, typeCity* city, int dimension);
void knn(typeNode* node, typeCity* city, maxHeap* heap, int k, int dimension);
void preorder(typeNode* tree);
void kdDestroy(typeNode* tree);

void removeQuotation(char* value);
void parse(typeHash* hashC, typeNode** tree, typeHash* hashN);
void menu();
void taskOne(typeHash* hash);
void taskTwo(typeHash* hash, typeNode* kdtree);
void taskThree(typeHash* hashC, typeHash* hashN, typeNode* kdtree);
void exec(typeHash* hashC, typeNode* kdtree, typeHash* hashN);

#endif
