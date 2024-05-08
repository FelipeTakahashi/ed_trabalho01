#include "../../include/header.h"

// Modelo para inserção na hash

void hashInsert(typeHash* hash, typeCity* bucket) {
    int index = hashFunction1(bucket->codigo_ibge) % hash->maxSize;
    // Nesse caso, a chave é o codigo_ibge;

    if(hash->maxSize == hash->currentSize + 1) {
        printf("Impossível Alocar\n");
        return;
    } else {
        int occurrences = 1;
        while(hash->table[index] != NULL) {
            index = index + (occurrences++ * hashFunction2(bucket->codigo_ibge)) % hash->maxSize;
            // Tratamento com hash duplo
        }

        hash->table[index] = bucket;
        hash->currentSize++;
    }
    return;
}
