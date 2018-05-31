#ifndef GRAPH
	#define GRAPH
	#include "Graph.h"
#endif
#include "GraphVis.h"
#include <stdlib.h>
#include <stdio.h>

int main(){
	// some very simple code to show you
	// how to use graph vis

  Graph g = newGraph(5);
  insertEdge(g,1,2,1);
  insertEdge(g,2,1,10);
  insertEdge(g,4,2,60);
  insertEdge(g,3,2,6);
  insertEdge(g,0,4,69);
  removeEdge(g,4,2); //remove edge 4->2
  removeEdge(g,0,1); //do nothing
  printf("%d\n", adjacent(g, 2, 1)); //print true
  printf("%d\n", adjacent(g, 0, 2)); //print false
  printf("%d\n", numVerticies(g)); //print 3

  AdjList test1 = outIncident(g, 2);
  AdjList test2 = inIncident(g, 2);

  while (test1 != NULL) { //node 1 weight 10
  	printf("v: %d, w: %d\n", test1->w, test1->weight);
  	test1 = test1->next;
  }

  while (test2 != NULL) { //node 3 weight 6 node 1 weight 1
  	printf("v: %d, w: %d\n", test2->w, test2->weight);
  	test2 = test2->next;
  }

	// draws the graph
  graphVis(g, DEFAULT);
	return EXIT_SUCCESS;
}
