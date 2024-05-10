#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/header.h"

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

void removeQuotation(char* value) {
    if(value == NULL) return;

    int length = strlen(value);

    if(value[0] == '\"' && value[length-1] == '\"') {
        memmove(value, value + 1, length - 2);
        value[length - 2] = '\0';
    }
}

void parse(typeHash* hashC, typeNode** tree, typeHash* hashN) {
    FILE *readJSON;
    readJSON = fopen("../municipios.json", "r");

    char line[100];
    int dataCounter = 0;
    int inserted = 0;

    typeCity* city = malloc(sizeof(typeCity));
    while (fgets(line, sizeof(line), readJSON)) {
        char *token = strtok(line, ":\t\n\r");

        while (token != NULL) {
            token = strtok(NULL, ":\t\n,{}]");

            if (token == NULL) {
                break;
            }

            char* blankspacePointer = strchr(token, ' ');

            /*
            Para simplificar o processo e o código do parser,
            blankspacePointer existe pra localizar o primeiro espaço do valor
            "codigo_ibge": valor
            o blankspacePointer existe para localizar o primeiro ' ' após ':' e alterar
            o ponteiro para ignorar o ' ' durante o processo de inserção.
            */

            if (blankspacePointer != NULL) {
                dataCounter++;
                char* value = blankspacePointer + 1;

                switch (dataCounter) {
                case 1:
                    strcpy(city->codigo_ibge, value);
                    break;
                case 2:
                    removeQuotation(value);
                    strcpy(city->nome, value);
                    break;
                case 3:
                    city->latitude = atof(token);
                    break;
                case 4:
                    city->longitude = atof(token);
                    break;
                case 5:
                    city->capital = atoi(token);
                    break;
                case 6:
                    city->codigo_uf = atoi(token);
                    break;
                case 7:
                    city->siafi_id = atoi(token);
                    break;
                case 8:
                    city->ddd = atoi(token);
                    break;
                case 9:
                    removeQuotation(value);
                    strcpy(city->fuso_horario, value);
                    dataCounter = 0;

                    hashInsertByCode(hashC, copyCity(city));
                    hashInsertByName(hashN, copyCity(city));
                    kdInsert(tree, copyCity(city), 0);

                    break;
                }
            }
        }
    }
    free(city);
    fclose(readJSON);
}

void menu() {
    printf("\n-------------------------------\n");
    printf("\nSelecione uma operação: \n");
    printf("1 : Buscar cidade pelo código IBGE.\n");
    printf("2 : Buscar k vizinhos pelo código IBGE.\n");
    printf("3 : Buscar k vizinhos pelo nome.\n");
    printf("4 : Finalizar\n");
    printf("\n-------------------------------\n");
}

void taskOne(typeHash* hash) {
    char * lookFor = malloc(sizeof(char) * 10);
    printf("Digite o código IBGE: \n");
    scanf("%s", lookFor);

    typeCity* searched = hashSearchByCode(hash, lookFor);

    if(searched == NULL) {
        printf("Operação finalizada.\n");
        free(lookFor);
        return;
    }

    printf("\n1. ");
    printRow(searched);

    free(lookFor);
}

void taskTwo(typeHash* hash, typeNode* kdtree) {
    char * lookFor = malloc(sizeof(char) * 10);
    int k;

    printf("Digite o código IBGE: \n");
    scanf("%s", lookFor);

    printf("Digite quantos vizinhos procurar: \n");
    scanf("%d", &k);

    if(k > 5569 || k < 1) {
        printf("Valor inválido.\n");
        free(lookFor);
        return;
    }

    typeCity* searched = hashSearchByCode(hash, lookFor);

    if(searched == NULL) {
        printf("Operação finalizada.\n");
        free(lookFor);
        return;
    }

    maxHeap* heap = createMaxHeap(k);
    knn(kdtree, searched, &(*heap), k, 0);
    heapSort(heap);

    printf("\nOs %d vizinhos mais próximos de {%s, %s} são: \n", k, searched->nome, searched->codigo_ibge);

    for(int i = 0; i < k; i++) {
        if(heap->heap[i].city == NULL) {
            printf("Posição na heap inválida.\n");
        }
        printf("%d. %s\n", i+1, heap->heap[i].city->codigo_ibge);
    }

    freeHeap(heap);
    free(lookFor);
}

void taskThree(typeHash* hashC, typeHash* hashN, typeNode* kdtree) {
    char lookFor[50];

    printf("Digite o nome da cidade: \n");
    scanf(" %[^\n]s", lookFor);

    char* searchedCode = hashSearchByName(hashN, &(*lookFor));

    if(searchedCode == NULL) {
        printf("Operação finalizada.\n");
        return;
    }

    typeCity* searchedCity = hashSearchByCode(hashC, searchedCode);

    if(searchedCity == NULL) {
        printf("Operação finalizada.\n");
        return;
    }

    int k;
    printf("Digite quantos vizinhos procurar: \n");
    scanf("%d", &k);

    if(k <= 0 || k > 5569) {
        printf("Valor inválido.\n");
        return;
    }

    maxHeap* heap = createMaxHeap(k);
    knn(kdtree, searchedCity, &(*heap), k, 0);
    heapSort(heap);

    printf("\nOs %d vizinhos mais próximos de %s são: \n", k, searchedCity->nome);

    for(int i = 0; i < k; i++) {
        if(heap->heap[i].city == NULL) {
            printf("Posição na heap inválida.\n");
        }
        printf("%d. ", i+1);
        printRowWithDistance(heap->heap[i].city, heap->heap[i].distance);
    }
    freeHeap(heap);
}

void exec(typeHash* hashC, typeNode* kdtree, typeHash* hashN) {
    int op = 0;
    do {
        menu();
        printf("\nDigite a operação a ser realizada\n");
        scanf("%d", &op);

        int k;

        switch (op) {
            case 1:
                taskOne(hashC);
                break;
            case 2:
                taskTwo(hashC, kdtree);
                break;
            case 3:
                taskThree(hashC, hashN, kdtree);
                break;
            case 4:
                op = 4;
                break;
            default:
                op = 4;
                printf("Operação inválida.\n");
                break;
        }
    } while(op != 4);

    printf("Operação finalizada.\n");
}

int main() {
    typeHash* hashCode = malloc(sizeof(typeHash));
    typeHash* hashName = malloc(sizeof(typeHash));
    hashInit(hashCode, MAXSIZE);
    hashInit(hashName, MAXSIZE);

    typeNode* kdtree = NULL;

    parse(hashCode, &kdtree, hashName);

    printf("\n-------------------------------\n\n");
    printf("Estrutura de Dados - Trabalho 01\n");

    exec(hashCode, kdtree, hashName);

    hashDestroy(hashCode);
    hashDestroy(hashName);
    kdDestroy(kdtree);

    return EXIT_SUCCESS;
}
