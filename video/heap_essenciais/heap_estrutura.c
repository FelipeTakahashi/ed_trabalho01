#include <stdio.h>

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

typedef struct _heap{ // Representa um nó da heap
    typeCity* city;
    double distance; // Distância em relação ao alvo
}typeHeap;

typedef struct _maxHeap{ // Representa o conjunto de nós _heap
    typeHeap* heap;
    int maxSize;
    int size;
}maxHeap;
