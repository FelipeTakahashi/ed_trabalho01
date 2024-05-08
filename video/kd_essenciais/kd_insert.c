#include "../../include/header.h"

// #define N_DIM 2

// Consideração sobre o balanceamento da KD Tree.

void kdInsert(typeNode** node, typeCity* city, int dimension) {
    // Pelos parâmetros, a inserção é feita de acordo com sua dimensão, implementando a característica da KD Tree

	if(*node == NULL) {
	   *node = createNode(city);
		// malloc...
		// node->city = null
		// node->left = null
		// node->right = null
		return;
	}

	if(dimension == 0) { // Comparação por latitude
	   if(city->latitude < (*node)->city->latitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	} else { // Quando a dimensão é 1, compara longitude
	   if(city->longitude < (*node)->city->longitude) {
			kdInsert(&((*node)->left), city, (dimension+1) % N_DIM);
	   } else {
			kdInsert(&((*node)->right), city, (dimension+1) % N_DIM);
	   }
	}
}
