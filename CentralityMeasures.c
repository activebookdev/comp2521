#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Dijkstra.h"
#include "CentralityMeasures.h"

int *visited; //will be a global array for the recursive dfs reach function

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
	int degree = 0;
	for (AdjList scan = outIncident(g, v); scan != NULL; scan = scan->next) {
		degree++;
	}
	return degree;
}

//again, this function will be used instead of the Graph ADT's inIncident function
int indegree(Graph g, Vertex v) {
	//returns the number of adjacent vertices on incoming edges to v
	AdjList scan;
	int degree = 0;
	if (numVerticies(g) > 0) {
		for (Vertex i = 0; i < numVerticies(g); i++) {
			if (i != v) {
				//inspect g->edges[i] for any edges incident on v
				for (scan = outIncident(g, i); scan != NULL; scan = scan->next) {
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
	NodeValues *outgoing = newCentralityStruct(numVerticies(g));
	double degree = 0;
	for (int v = 0; v < numVerticies(g); v++) {
		degree = (double)outdegree(g, v);
		outgoing->values[v] = degree;
	}
	return *outgoing;
}

NodeValues inDegreeCentrality(Graph g) {
	//return a struct with an array of the number of incoming edges for each vertex
	NodeValues *incoming = newCentralityStruct(numVerticies(g));
	double degree = 0;
	for (int v = 0; v < numVerticies(g); v++) {
		degree = (double)indegree(g, v);
		incoming->values[v] = degree;
	}
	return *incoming;
}

NodeValues degreeCentrality(Graph g) {
	//for undirected graph, simply add the indegree and outdegree of each node
	NodeValues *incoming = newCentralityStruct(numVerticies(g));
	double degree = 0;
	for (int v = 0; v < numVerticies(g); v++) {
		degree = (double)indegree(g, v) + (double)outdegree(g, v);
		incoming->values[v] = degree;
	}
	return *incoming;
}

void visit_reach(Graph g, Vertex v) {
	//for each unvisited neighbour of v, mark it as visited and call this function on it
	for (AdjList neighbours = outIncident(g, v); neighbours != NULL; neighbours = neighbours->next) {
		if (visited[neighbours->w] == 0) {
			visited[neighbours->w] = 1;
			visit_reach(g, neighbours->w);
		}
	}
}

int numReach(Graph g, Vertex v) {
	//depth-first search to count the number of nodes that v can reach
	int num = 0;
	visited = malloc(numVerticies(g)*sizeof(int)); //initialise the visited array for this instance
	if (visited == NULL) {
		fprintf(stderr, "Error!\n");
		return 0;
	}
	for (int i = 0; i < numVerticies(g); i++) {
		visited[i] = 0;
		if (i == v) {
			visited[i] = 1;
		}
	}

	//for each neighbour of v, visit it and then visit its neighbours
	for (AdjList neighbours = outIncident(g, v); neighbours != NULL; neighbours = neighbours->next) {
		visited[neighbours->w] = 1;
		visit_reach(g, neighbours->w);
	}

	//sum the number of visited nodes
	for (int i = 0; i < numVerticies(g); i++) {
		if (visited[i] == 1) {
			num++;
		}
	}
	return num;
}

NodeValues closenessCentrality(Graph g) {
	NodeValues *centrality = newCentralityStruct(numVerticies(g));
	double closeness = 0;
	int reach = 0;
	int dsum = 0;
	int i = 0;
	ShortestPaths temp;
	ShortestPaths *paths;
	for (int v = 0; v < numVerticies(g); v++) { //for each vertex v, calculate the value of the closeness formula
		reach = numReach(g, v); //the number of nodes that v can reach (not including itself)
		temp = dijkstra(g, v); //the result of dijkstra's algorithm
		paths = &temp; //this temp is needed because the & operator cannot be placed in front of a function
		dsum = 0;
		for (i = 0; i < numVerticies(g); i++) { //sum the shortest path distances for each vertex
			dsum += paths->dist[i];
		}
		//printf("For node %d: numvertices=%d, reach=%d, dsum=%d\n", v, numVerticies(g), reach, dsum); used for debugging
		closeness = (double)(reach-1)*(reach-1)/((numVerticies(g)-1)*dsum);
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
	ShortestPaths temp;
	ShortestPaths *paths;
	for (int s = 0; s < numVerticies(g); s++) {
		if (s != v) {
			temp = dijkstra(g, v);
			paths = &temp;
			for (int t = 0; t < numVerticies(g); t++) {
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
	NodeValues *centrality = newCentralityStruct(numVerticies(g));
	for (int v = 0; v < numVerticies(g); v++) {
		centrality->values[v] = (double)betweenness(g, v);
	}
	return *centrality;
}

NodeValues betweennessCentralityNormalised(Graph g) {
	//return a normalised version of the betweenness centrality struct
	NodeValues centrality = betweennessCentrality(g);
	for (int v = 0; v < numVerticies(g); v++) {
		centrality.values[v] = (centrality.values[v])/((numVerticies(g)-1)*(numVerticies(g)-2));
	}
	return centrality;
}

void showNodeValues(NodeValues v) {
	//print out the value-key pairs
	for (int i = 0; i < v.noNodes; i++) {
		printf("%d: %lf\n", i, v.values[i]);
	}
}

void freeNodeValues(NodeValues v) {
	//TODO
}