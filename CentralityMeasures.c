#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "CentralityMeasures.h"

NodeValues *newCentralityStruct(int num_nodes) {
	NodeValues *new = malloc(sizeof(NodeValues));
	if (new == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}

	new->noNodes = num_nodes;
	new->values = malloc(num_nodes*sizeof(double));
	if (new->values == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}

	return new;
}

//this function will be used instead of the Graph ADT's outIncident function, for the sake of efficiency
int outdegree(Graph g, Vertex v) {
	//returns the number of adjacent vertices on outgoing edges from v
	if (g->edges[v] != NULL) {
		int degree = 0;
		AdjList scan = g->edges[v];
		for (AdjList scan = g->edges[v]; scan != NULL; scan = scan->next) {
			degree++;
		}
		return degree;
	}
	return 0;
}

//again, this function will be used instead of the Graph ADT's inIncident function
int indegree(Graph g, Vertex v) {
	//returns the number of adjacent vertices on incoming edges to v
	AdjList scan;
	int degree = 0;
	if (g->nE > 0) {
		for (Vertex i = 0; i < g->nV; i++) {
			if (i != v) {
				//inspect g->edges[i] for any edges incident on v
				for (scan = g->edges[i]; scan != NULL; scan = scan->next) {
					if (scan->w == v) {
						//this edge is incident on v
						degree++;
						break;
					}
				}
			}
		}
	}
	return degree;
}

NodeValues outDegreeCentrality(Graph g) {
	//return a struct with an array of the number of outgoing edges from each vertex
	NodeValues *outgoing = newCentralityStruct(g->nV);
	double degree = 0;
	for (int v = 0; v < g->nV; v++;) {
		degree = (double)outdegree(g, v);
		outgoing->values[v] = degree;
	}
	return *outgoing;
}

NodeValues inDegreeCentrality(Graph g) {
	//return a struct with an array of the number of incoming edges for each vertex
	NodeValues *incoming = newCentralityStruct(g->nV);
	double degree = 0;
	for (int v = 0; v < g->nV; v++;) {
		degree = (double)indegree(g, v);
		incoming->values[v] = degree;
	}
	return *incoming;
}

NodeValues degreeCentrality(Graph g) {
	//for undirected graph
	return outDegreeCentrality(g); //because the outgoing edges are the same as the incoming ones (there's no direction)
}

void visit_reach(Vertex v, int *neighbours) {
	//for each unvisited neighbour of v, mark it as visited and call this function on it
	for (AdjList neighbours = g->edges[v]; neighbours != NULL; neighbours = neighbours->next) {
		if (visited[neighbours->w] == 0) {
			visited[neighbours->w] = 1;
			visit_reach(neighbours->w, &neighbours);
		}
	}
}

int numReach(Graph g, Vertex v) {
	//depth-first search to count the number of nodes that v can reach
	int num = 0;
	int visited[g->nV];
	for (int i = 0; i < g->nV; i++) {
		visited[i] = 0;
		if (i == v) {
			visited[i] = 1;
		}
	}

	//for each neighbour of v, visit it and then visit its neighbours
	for (AdjList neighbours = g->edges[v]; neighbours != NULL; neighbours = neighbours->next) {
		visited[neighbours->w] = 1;
		visit_reach(neighbours->w, &neighbours);
	}

	//sum the number of visited nodes
	for (i = 0; i < g->nV; i++) {
		if (visited[i] == 1) {
			num++;
		}
	}
	return num-1; //-1 because we don't want to count v itself
}

NodeValues closenessCentrality(Graph g) {
	NodeValues *centrality = newCentralityStruct(g->nV);
	double closeness = 0;
	int reach = 0;
	int dsum = 0;
	int i = 0;
	ShortestPaths *paths;
	for (int v = 0; v < g->nV; v++;) { //for each vertex v, calculate the value of the closeness formula
		reach = numReach(g, v); //the number of nodes that v can reach (not including itself)
		paths = &pathsdijkstra(g, v); //the result of dijkstra's algorithm
		dsum = 0;
		for (i = 0; i < g->nV; i++;) { //sum the shortest path distances for each vertex
			dsum += paths->dist[i];
		}
		closeness = (double)(reach-1)*(reach-1)/((g->nV-1)*dsum);
		centrality->values[v] = closeness;
	}
	return *centrality;
}

int num_pred_paths(int s, int t, ShortestPaths *paths, int v) {
	//return the number of paths from s to t that pass through v
	int num = 0;
	PredNode *predecessor = paths->pred[t]; //the list of predecessors of node t
	while (predecessor != NULL) {
		if (predecessor->v == v) {
			//this path passes through v
			num += 1;
		} else if (predecessor->v != s) {
			//this path might pass through v, so keep scanning back
			num += num_pred_paths(s, predecessor->v, paths, v);
		}
		//otherwise the path can't pass through v, so add nothing
		predecessor = predecessor->next;
	}
	return num;
}

int betweenness(Graph g, Vertex v) {
	int betweenness = 0;
	ShortestPaths *paths;
	for (int s = 0; s < g->nV; s++) {
		if (s != v) {
			paths = &pathsdijkstra(g, v);
			for (int t = 0; t < g->nV; t++) {
				if (t != v) {
					//find the number of shortest paths from s to t that pass through v
					betweenness += num_pred_paths(s, t, paths, v);
				}
			}
		}
	}
	return betweenness;
}

NodeValues betweennessCentrality(Graph g) {
	//return a struct with an array of the number of incoming edges for each vertex
	NodeValues *centrality = newCentralityStruct(g->nV);
	double betweenness = 0;
	for (int v = 0; v < g->nV; v++;) {
		betweenness = (double)betweenness(g, v);
		centrality->values[v] = betweenness;
	}
	return *centrality;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	//return a normalised version of the betweenness centrality struct
	NodeValues *centrality = betweennessCentrality(g);
	double betweenness = 0;
	for (int v = 0; v < g->nV; v++;) {
		centrality->values[v] = (centrality->values[v])/((g->nV-1)*(g->nV-2));
	}
	return *centrality;
}

void  showNodeValues(NodeValues v) {
	//TODO
}

void  freeNodeValues(NodeValues v) {
	NodeValues *delete = &v;
	free(delete->values)
	free(delete);
}