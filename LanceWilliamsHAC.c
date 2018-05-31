#include "LanceWilliamsHAC.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct DNode *Dendrogram;
typedef struct DNode {
   int  vertex;
   Dendrogram left, right;
} DNode;

Dendrogram createDendrogram() {
	//TODO
}

Dendrogram LanceWilliamsHAC(Graph g, int method) {
	
}

void freeDendrogram(Dendrogram d) {
	//free each DNode in the Dendrogram
	free(d->left);
	free(d->right);
	free(d);
}