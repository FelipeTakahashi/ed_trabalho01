#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/header.h"

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

maxHeap* createMaxHeap(int maxSize){
    maxHeap* heap = malloc(sizeof(maxHeap));
    heap->heap = malloc(sizeof(typeHeap) * maxSize);
    heap->maxSize = maxSize;
    heap->size = 0;

    return heap;
}

void swap(typeHeap* a, typeHeap* b){
    typeHeap aux = *a;
    *a = *b;
    *b = aux;
}

void heapifyDown(maxHeap* heap, int i) {
    int left = 2 * i + 1;
    int right = 2 * i + 2;
    int max = i;

    if(left < heap->size && heap->heap[left].distance > heap->heap[max].distance){
        max = left;
    }

    if(right < heap->size && heap->heap[right].distance > heap->heap[max].distance){
        max = right;
    }

    if(max != i){
        swap(&heap->heap[i], &heap->heap[max]);
        heapifyDown(heap, max);
    }
}

void heapifyUp(maxHeap* heap, int i){
    int max = (i-1)/2;
    if(heap->heap[max].distance < heap->heap[i].distance){
        swap(&heap->heap[max], &heap->heap[i]);
        heapifyUp(heap, max);
    }
}

void heapShift(maxHeap* heap, typeCity* city, double distance){
    if(heap->size == heap->maxSize){
        if(distance < heap->heap[0].distance){
            heap->heap[0].city = city;
            heap->heap[0].distance = distance;
            heapifyDown(heap, 0);
        }
    } else {
        heap->heap[heap->size].city = city;
        heap->heap[heap->size].distance = distance;
        heapifyUp(heap, heap->size);
        heap->size++;
    }
}

void heapSort(maxHeap* heap){
    for(int i = heap->size-1; i > 0; i--){
        swap(&heap->heap[0], &heap->heap[i]);
        heap->size--;
        heapifyDown(heap, 0);
    }
}

int checkInHeap(typeCity* city, maxHeap* heap) {
    for(int i = 0;  i < heap->size; i++) {
        if(strcmp(city->codigo_ibge, heap->heap[i].city->codigo_ibge) == 0) {
            return 1;
        }
    }
    return 0;
}

void freeHeap(maxHeap* heap){
    if (heap == NULL) {
        return;
    }

    if(heap->heap != NULL) {
        free(heap->heap);
        heap->heap = NULL;
    }

    free(heap);
}
