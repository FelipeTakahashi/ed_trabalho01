#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/header.h"

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

int hashFunction1(char *key) {
    int sum = 0;
    int length = strlen(key);
    for(int i = 0; i < length; i++) {
        sum += key[i];
    }
    return sum % MAXSIZE;
}

int hashFunction2(char *key) {
    int sum = 0;
    int length = 0;
    for(int i = 0; i < length; i++) {
        sum += key[i];
    }

    return 1 + (sum % (MAXSIZE-1)); // incremento primo
}

int hashInit(typeHash* hash, int buckets) {
    int limit = buckets+1;

    hash->table = calloc(sizeof(typeCity*), limit);

    if(hash->table == NULL) {
        printf("Não foi possível alocar a Hash.\n");
        return EXIT_FAILURE;
    }

    hash->maxSize = limit;
    hash->currentSize = 0;

    return EXIT_SUCCESS;
}

void hashInsertByCode(typeHash* hash, typeCity* bucket) {
    int index = hashFunction1(bucket->codigo_ibge) % hash->maxSize;

    if(hash->maxSize == hash->currentSize + 1) {
        printf("Impossível Alocar\n");
        return;
    } else {
        int occurrences = 1;
        while(hash->table[index] != NULL) {
            index = index + (occurrences++ * hashFunction2(bucket->codigo_ibge)) % hash->maxSize;
        }

        hash->table[index] = bucket;
        hash->currentSize++;
    }
    return;
}

void hashInsertByName(typeHash* hash, typeCity* bucket) {
    int index = hashFunction1(bucket->nome) % hash->maxSize;

    if(hash->maxSize == hash->currentSize + 1) {
        printf("Impossível Alocar\n");
        return;
    } else {
        int occurrences = 1;
        while(hash->table[index] != NULL) {
            index = index + (occurrences++ * hashFunction2(bucket->nome)) % hash->maxSize;
        }

        hash->table[index] = bucket;
        hash->currentSize++;
    }
    return;
}

typeCity* hashSearchByCode(typeHash* hash, char* key) {
    int index = hashFunction1(key) % hash->maxSize;
    typeCity* ret = NULL;

    int occurrences = 1;
    while(hash->table[index] != NULL && ret == NULL) {
        if(strcmp(hash->table[index]->codigo_ibge, key) == 0) {
            ret = hash->table[index];
        } else {
            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        }
    }

    if(ret == NULL) {
        printf("Cidade não encontrada.\n");
    }
    return ret;
}

char* hashSearchByName(typeHash* hash, char* key) {
    int candidatesMax = 5;
    int candidatesSize = 0;

    typeCity** candidates = malloc(candidatesMax * sizeof(typeCity*));
    typeCity* ret = NULL;

    int occurrences = 1;
    int index = hashFunction1(key) % hash->maxSize;

    while(hash->table[index] != NULL) {
        if(strcmp(hash->table[index]->nome, key) == 0) {

            if(candidatesMax <= candidatesSize) {
                candidates = realloc(candidates, candidatesMax * 2 * sizeof(typeCity*));
                candidatesMax *= 2;
            }

            ret = hash->table[index];
            candidates[candidatesSize++] = hash->table[index];

            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        } else {
            index = index + (occurrences++ * hashFunction2(key)) % hash->maxSize;
        }
    }

    if(ret == NULL) {
        printf("Cidade não encontrada.\n");
        return NULL;
    }

    if(candidatesSize > 1) {
        printf("\nMais de uma cidade com o nome \"%s\" foi encontrada.\n", ret->nome);
        printf("Digite a opção de seu destino: \n");

        for(int i = 0; i < candidatesSize; i++) {
            printf("%d. ", i+1);
            printRow(candidates[i]);
        }

        int option;
        scanf("%d", &option);

        if(option > candidatesSize || option < 0) return NULL;

        return candidates[option-1]->codigo_ibge;
    }

    return ret->codigo_ibge;
}

void hashDestroy(typeHash* hash) {
    for(int i = 0; i < hash->maxSize; i++) {
        if(hash->table[i] != NULL) {
            free(hash->table[i]);
            hash->table[i] = NULL;
        }
    }

    free(hash->table);
    free(hash);

    return;
}
