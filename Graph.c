#include <stdlib.h>
#include <stdio.h>
#include "Graph.h"

struct GraphRep {
	AdjList *edges; //an array, with an element for each vertex, pointing to a linked list of vertices that have an edge with that vertex
	int nE;	//the number of edges in the graph
	int nV; //the number of vertices in the graph
};

AdjList newListNode(int dest, int weight) {
	if (weight >= 0) {
		AdjList new;
		if ((new = malloc(sizeof(struct _adjListNode))) == NULL) { //malloc memory space for the new node, but return an error if there's a problem
			fprintf(stderr, "Error!\n");
			return NULL;
		}
		new->w = dest;
		new->weight = weight;
		new->next = NULL;
		return new;
	}
	return NULL;
}

Graph newGraph(int noNodes) {
	if (noNodes >= 0) {
		Graph new;
		if ((new = malloc(sizeof(struct GraphRep))) == NULL) { //malloc memory space for the new graph, but return an error if there's a problem
			fprintf(stderr, "Error!\n");
			return NULL;
		}
		new->nE = 0;
		new->nV = noNodes;
		if ((new->edges = malloc(noNodes*sizeof(adjListNode))) == NULL) { //malloc space for the adjacency list, return error if there's a problem
			fprintf(stderr, "Error!\n");
			return NULL;
		}
		for (int i = 0; i < noNodes; i++) {
			new->edges[i] = NULL;
		}
		return new;
	}
	return NULL;
}

void insertEdge(Graph g, Vertex src, Vertex dest, int weight) {
	//insert an edge in g between src and dest of the given weight (if the edge already exists, just update the weight)
	int exists = 0;
	for (AdjList curr = g->edges[src]; curr != NULL; curr = curr->next) {
		if (curr->w == dest) {
			//the edge already exists
			curr->weight = weight;
			exists = 1;
			break;
		}
	}

	if (exists == 0) {
		//the edge doesn't already exist
		AdjList insert = newListNode(dest, weight);
		insert->next = g->edges[src];
		g->edges[src] = insert;
		g->nE++;
	}
}

void removeEdge(Graph g, Vertex src, Vertex dest) {
	//if an edge from src to dest exists, remove it from src's adjacency list
	AdjList prev = g->edges[src];
	if (prev->w == dest) {
		//the edge to delete is the first node in the adjlist
		g->edges[src] = prev->next;
		free(prev);
		g->nE--;
	} else {
		int exists = 0;
		AdjList curr = prev->next;
		while (curr != NULL) {
			if (curr->w == dest) {
				exists = 1;
				break;
			}
			prev = curr;
			curr = curr->next;
		}

		if (exists == 1) {
			//prev points to the node before the one we want to delete, and curr points to that node
			prev->next = curr->next;
			free(curr);
			g->nE--;
		}
	}
}

bool adjacent(Graph g, Vertex src, Vertex dest) {
	for (AdjList curr = g->edges[src]; curr != NULL; curr = curr->next) {
		if (curr->w == dest) {
			return true;
		}
	}
	return false;
}

int numVerticies(Graph g) {
	return g->nV;
}

AdjList outIncident(Graph g, Vertex v) {
	//returns a list of adjacent vertices on outgoing edges from v
	AdjList original = g->edges[v];
	if (original != NULL) {
		AdjList new = newListNode(original->w, original->weight);
		AdjList newscan = new;
		for (original = original->next; original != NULL; original = original->next) {
			//copy this node into the new list
			newscan->next = newListNode(original->w, original->weight);
			newscan = newscan->next;
		}
		return new;
	}
	return NULL;
	//TODO: CHECK IF return g->edges[v]; IS ACCEPTABLE, OR SHOULD WE DUPLICATE THE LIST AND RETURN THE NEW ONE?
}

AdjList inIncident(Graph g, Vertex v) {
	//returns a list of adjacent vertices on incoming edges to v
	AdjList new = NULL;
	AdjList newscan = NULL;
	AdjList scan;
	if (g->nE > 0) {
		for (Vertex i = 0; i < g->nV; i++) {
			if (i != v) {
				//inspect g->edges[i] for any edges incident on v
				for (scan = g->edges[i]; scan != NULL; scan = scan->next) {
					if (scan->w == v) {
						//this edge is incident on v
						if (new == NULL) {
							//this is the first such edge
							new = newListNode(i, scan->weight);
							newscan = new;
						} else {
							//append this to the end of the new list
							newscan->next = newListNode(i, scan->weight);
							newscan = newscan->next;
						}
						break;
					}
				}
			}
		}
		return new;
	}
	return NULL;
}

void showGraph(Graph g) {
	//TODO
}

void freeGraph(Graph g) {
	//free each node in g's adjacency lists and then free g itself
	AdjList scan;
	AdjList delete;
	for (Vertex i = 0; i < g->nV; i++) {
		for (scan = g->edges[i]; scan != NULL;) {
			delete = scan;
			scan = scan->next;
			free(delete);
		}
	}
	free(g);
}