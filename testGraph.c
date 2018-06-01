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

  Graph g = newGraph(3);
  insertEdge(g,0, 1, 2);
  insertEdge(g,1, 0, 2);
  insertEdge(g,2, 1, 1);
  insertEdge(g,0, 2, 1);
	// draws the graph
  graphVis(g, DEFAULT);
    return EXIT_SUCCESS;
}
