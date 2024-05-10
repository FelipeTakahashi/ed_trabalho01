#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../include/header.h"

#define MAXCHAR 256
#define MAXSIZE 11777
#define N_DIM 2

double distance(typeCity a, typeCity b) {
    double x = a.latitude - b.latitude;
    double y = a.longitude - b.longitude;
    return x*x + y*y;
}

typeNode* createNode(typeCity* city) {
	typeNode* node = malloc(sizeof(typeNode));

	node->city = city;
	node->left = NULL;
	node->right = NULL;

	return node;
}

void kdInsert(typeNode** node, typeCity* city, int dimension) {
	if(*node == NULL) {
	   *node = createNode(city);
		return;
	}

	if(dimension == 0) {
	   if(city->latitude < (*node)->city->latitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	} else {
	   if(city->longitude < (*node)->city->longitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	}
}

void knn(typeNode* node, typeCity* city, maxHeap* heap, int k, int dimension) {
    if(node == NULL) {
        return;
    }

    double dist = distance(*city, *(node->city));
    double currentMax = 0;

    if(heap->size > 0) currentMax = heap->heap[0].distance;

    if(strcmp(node->city->codigo_ibge, city->codigo_ibge) != 0 && !checkInHeap(node->city, heap)) {
        heapShift(heap, node->city, dist);
    }

    double reference;
    double current;

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

    if(reference < current) {
        knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        }
    } else {
        knn(node->right, city, heap, k, (dimension+1) % N_DIM);
        if(heap->size < k || planeDistance <= sqrt(currentMax)) {
            knn(node->left, city, heap, k, (dimension+1) % N_DIM);
        }
    }
}

void preorder(typeNode* tree) {
	if(tree != NULL) {
		printf("%s\n", tree->city->nome);
		preorder(tree->left);
		preorder(tree->right);
	}
}

void kdDestroy(typeNode* tree) {
    if(tree == NULL) return;
    if(tree->left != NULL) {
        kdDestroy(tree->left);
        tree->left = NULL;
    }
    if(tree->right != NULL) {
        kdDestroy(tree->right);
        tree->right = NULL;
    }
    if(tree->city != NULL) {
        free(tree->city);
        tree->city = NULL;
    }

    free(tree);
}
