#include "LanceWilliamsHAC.h"
#include <stdio.h>
#include <stdlib.h>

Dendrogram createDNode(int v, Dendrogram left, Dendrogram right) {
	Dendrogram node = malloc(sizeof(Dendrogram));
	if (node == NULL) {
		fprintf(stderr, "Error!\n");
        return NULL;
	}
	node->vertex = v;
	node->left = left;
	node->right = right;
}

int *closest_clusters(double **distances, int size) {
	//return an array of the indexes of the two closest clusters
	int clusters[2] = {0};
	double distance = 0;
	for (int i = 0; i < size; i++) {
		for (int j = 0; j < size; j++) {
			if (i != j) {
				if (distances[i][j] < distance) {
					//these are the two closest clusters so far
					clusters[0] = i;
					clusters[1] = j;
				}
			}
		}
	}
	return clusters;
}

double LWcalculate(double i, double j, int method) {
	//given dist(ci, ck) and dist(cj, ck), and the LW method, calculate the result
	if (method == 1) {
		//Single-Link method
		if (i < j) return i;
		return j;
	} else {
		//Complete-Link method
		if (i > j) return i;
		return j
	}
}

double **combine_clusters_distances(double **distances, int size, int v, int w, int method) {
	//return a (size-1)*(size-1) array of distances, modified to combine clusters at indexes v and w, using method specified
	double **new_distances = malloc((size-1)*(size-1)*sizeof(double));
	if (new_distances == NULL) {
		fprintf(stderr, "Error!\n");
        return NULL;
	}

	//copy all but the vth and wth rows, and vth and wth columns
	int ir = 0, ic = 0, jr = 0, jc = 0;
	for (ir = 0; ir < size; ir++) {
		if (ir != v && ir != w) {
			for (ic = 0; ic < size; ic++) {
				if (ic != v && ic != w) {
					new_distances[jr][jc] = distances[ir][ic];
					jc++; //only increment if we copied across this element
				}
			}
			jr++; //only increment if we copied across this row
		}
	}

	//now we need to append the final vw row to new_distances, using the method specified
	int j = 0;
	for (int i = 0; i < size; i++) {
		if (i != v && i != w) {
			new_distances[size-2][j] = LWcalculate(distances[v][i], distances[w][i], method);
			j++;
		}
	}

	//now copy this final row to the final column
	for (i = 0; i < size-2; i++) {
		new_distances[i][size-2] = new_distances[size-2][i];
	}
	new_distances[size-2][size-2] = INT_MAX; //this is the bottom-right corner element

	return new_distances;
}

Dendrogram *combine_clusters_dgrams(Dendrogram *dgrams, int size, int v, int w) {
	//return a (size-1)-length array of deondrograms, modified to combine clusters at indexes v and w
	Dendrogram one = dgrams[v];
	Dendrogram two = dgrams[w];
	Dendrogram new = createDNode(-1, one, two); //vertex of -1 means its a multi-cluster (it can't be named by a single integer)

	Dendrogram *new_arr = malloc((size-1)*sizeof(Dendrogram)); //the new array of clusters is one shorter
	if (new_arr == NULL) {
		fprintf(stderr, "Error!\n");
        return NULL;
	}

	int j = 0;
	for (int i = 0; i < size; i++) {
		if (i != v && i != w) {
			//copy this cluster across to new_arr
			new_arr[j] = dgrams[i];
			j++;
		}
	}

	//free the old dgrams array, it will never be used again
	free(dgrams);

	//insert the new dendrogram onto the end of this new_arr
	new_arr[size-2] = new;
	return new_arr;
}

Dendrogram LanceWilliamsHAC(Graph g, int method) {
	double **distances = malloc(g->nV*g->nV*sizeof(double));
	if (distances == NULL) {
		fprintf(stderr, "Error!\n");
        return NULL;
	}
	for (int i = 0; i < g->nV; i++) {
		for (int j = 0; j < g->nV; j++) {
			distances[i][j] = INT_MAX; //set distances to infinity by default
		}
	}

	for (int v = 0; v < g->nV; v++) {
		for (AdjList scan = outIncident(g, v); scan != NULL; ) {
			if (1/(scan->weight) < distances[v][scan->w]) {
				//we have found a shorter distance, so update the array
				distances[v][scan->w] = (double)1/(scan->weight);
				distances[scan->w][v] = (double)1/(scan->weight);
			}
		}
		for (scan = inIncident(g, v); scan != NULL; ) {
			if (1/(scan->weight) < distances[v][scan->w]) {
				//we have found a shorter distance, so update the array
				distances[v][scan->w] = (double)1/(scan->weight);
				distances[scan->w][v] = (double)1/(scan->weight);
			}
		}
	}
	//now we have an array of the shortest distances between every pair of vertices (infinity if no direct edge)
	
	Dendrogram *dgrams = malloc(g->nV*sizeof(Dendrogram));
	if (dgrams == NULL) {
		fprintf(stderr, "Error!\n");
        return NULL;
	}
	for (i = 0; i < g->nV; i++) {
		dgrams[i] = createDNode(i, NULL, NULL);
	}
	//now we have an array of singular dendrograms, one for each vertex in g

	int closest[2] = {0};
	int size = g->nV;
	for (int k = 0; k < g->nV; k++) {
		//combine the two closest clusters
		closest = closest_clusters(distances, size);
		distances = combine_clusters_distances(distances, size, closest[0], closest[1], method);
		dgrams = combine_clusters_dgrams(dgrams, size, closest[0], closest[1]);
		size--;
	}

	return dgrams[0]; //return the final, complete cluster
}

void freeDendrogram(Dendrogram d) {
	//free each DNode in the Dendrogram
	free(d->left);
	free(d->right);
	free(d);
}