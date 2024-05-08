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

typedef struct _node{
	typeCity* city;
	struct _node* left;
	struct _node* right;
}typeNode;

// Estrutura de uma árvore comum, dispensa comentários
