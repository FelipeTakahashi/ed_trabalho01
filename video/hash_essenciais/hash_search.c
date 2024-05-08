#include "../../include/header.h"

/*
   Modelo para busca na hash
   -Este exemplo mostra como o caso de cidades com mesmo nome é tratado.
   -A busca por código funciona de maneira similar. Por ter chave única, retorna o bucket em sí.
*/

char* hashSearchByName(typeHash* hash, char* key) {
    int candidatesMax = 5;
    int candidatesSize = 0;

    typeCity** candidates = malloc(candidatesMax * sizeof(typeCity*));
    // Cria uma lista para armanezar cidades com o mesmo nome

    typeCity* ret = NULL;

    int occurrences = 1;
    int index = hashFunction1(key) % hash->maxSize;

    while(hash->table[index] != NULL) {
        if(strcmp(hash->table[index]->nome, key) == 0) {

            if(candidatesMax <= candidatesSize) {
                // Caso haja a necessidade de mais espaço para candidatos
                candidates = realloc(candidates, candidatesMax * 2 * sizeof(typeCity*));
                candidatesMax *= 2;
            }

            ret = hash->table[index];
            candidates[candidatesSize++] = hash->table[index];
            // Adiciona a cidade na lista

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
        // Se houver mais de uma cidade com o nome em questão, abre um menu com a array
        printf("\nMais de uma cidade com o nome \"%s\" foi encontrada.\n", ret->nome);
        printf("Digite a opção de seu destino: \n");

        for(int i = 0; i < candidatesSize; i++) {
            printf("%d. ", i+1);
            printRow(candidates[i]);
        }

        int option;
        scanf("%d", &option);

        if (option > candidatesSize || option < 0) return NULL;

        return candidates[option-1]->codigo_ibge;
        // Retorna a opção decidida pelo usuário
    }

    return ret->codigo_ibge;
    // Retorna o código da única cidade encontrada
}
