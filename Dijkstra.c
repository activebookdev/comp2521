#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dijkstra.h"
#include "PQ.h"
#include <limits.h>


/*PredNode newPred() {
    PredNode new;
    if ((new = malloc(sizeof(struct PredNode))) == NULL) { //malloc memory space for the new node, but return an error if there's a problem
		fprintf(stderr, "Error!\n");
		return NULL;
    }
    new->next = NULL;
    new->v = 0;
}*/



ShortestPaths dijkstra(Graph g, Vertex v) {
    ShortestPaths new;
    new = malloc (sizeof (struct ShortestPaths));
    if (new==NULL) {
        fprintf(stderr, "Error!\n");
		return NULL;
    }
    
    PQ PathPQ = newPQ();
    Vertex visited[g->nV];
    Vertex distance[g->nV];
    adjListNode current;
    int i =0;
    ItemPQ temp;
    ItemPQ save;
    int alt;
    while (i<g->nV) {
        if (i==v) {
            temp->value = 0;
            dist[i] = 0;
        }
        else {
             temp->value = i;//this doesnt really matter
             dist[i] = INT_MAX;
        }
        temp->key = i;
        visited[i] = -1;
        addPQ(temp);
        i++;
    }
    while (PQEmpty(PathPQ)==0) {
        temp = dequeuePQ(PathPQ);
        current = g->edges[temp->key];
        while (current!=NULL) {
            alt = dist[temp->key] + current->weight;
            if (alt < dist[current->w]) {
                save->key = current->w;
                save->value = alt;  
                dist[current->w] = alt; 
                updatePQ(PathPQ, save);
            }
            current = current->next;    
        }
    } 
}     

void  showShortestPaths(ShortestPaths p) {

}

void  freeShortestPaths(ShortestPaths p) {

}
