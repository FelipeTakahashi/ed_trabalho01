#include "../../include/header.h"

void heapShift(maxHeap* heap, typeCity* city, double distance){
    if(heap->size == heap->maxSize){
        // Caso a heap esteja cheia, apenas altera o nó de valor máximo
        if(distance < heap->heap[0].distance){
            heap->heap[0].city = city;
            heap->heap[0].distance = distance;
            heapifyDown(heap, 0);
        }
    } else {
        // Alternativamente, inserção comum
        heap->heap[heap->size].city = city;
        heap->heap[heap->size].distance = distance;
        heapifyUp(heap, heap->size);
        heap->size++;
    }
}

// Todas as outras funções são comuns à estrutura. Portanto, semelhantes as explicações em sala de aula
