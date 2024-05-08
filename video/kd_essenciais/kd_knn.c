#include "../../include/header.h"

void knn(typeNode* node, typeCity* city, maxHeap* heap, int k, int dimension) {
    if(node == NULL) {
        return;
    }

    double dist = distance(*city, *(node->city)); // Distância Euclidiana
    double currentMax = 0;

    if(heap->size > 0) currentMax = heap->heap[0].distance;

    if(strcmp(node->city->codigo_ibge, city->codigo_ibge) != 0 && !checkInHeap(node->city, heap)) {
        // Não adiciona cidades repetidas na heap
        heapShift(heap, node->city, dist);
    }

    double reference;
    double current;

    // Por simplicidade, reference e current guardam o valor a ser comparado de acordo com a dimensão

    if(dimension == 0) {
        reference = city->latitude;
        current = node->city->latitude;
    } else {
        reference = city->longitude;
        current = node->city->longitude;
    }

    double planeDistance;
    planeDistance = reference - current;
    if(planeDistance < 0) planeDistance = -planeDistance;

    // planeDistance representa a distância entre dois pontos no plano

    if(reference < current) {
        knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            /*
            Quando a heap não estiver cheia ou se o plano a ser dividido (subárvore oposta) pode possuir um
            candidato melhor, indica que compensa realizar a busca no eixo que está sendo dividido.
            */
            knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        }
    } else {
        knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        }
    }
}
