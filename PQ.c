#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PQ.h"

struct PQRep {
	int nitems;
	ItemPQ *front;
	ItemPQ *back;
}

PQ newPQ() {
	PQ new;
	if ((new = malloc(sizeof(struct PQRep))) == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}
	new->nitems = 0;
	new->front = NULL;
	new->back = NULL;
	return new;
}

void addPQ(PQ queue, ItemPQ item) {
	//scan the queue for a node with the same key as 'item', if one exists, update its value, otherwise insert onto the back of the queue
	if (item != NULL) {
		for (ItemPQ *scan = queue->front; scan != NULL; scan = scan->next) {
			if (scan->key == item->key) {
				scan->value = item->value;
				return;
			}
		}

		if (queue->back != NULL) {
			queue->back->next = item;
			item->next = NULL;
			queue->nitems++;
		} else {
			//queue is empty
			queue->front = item;
			queue->back = item;
			item->next = NULL;
			queue->nitems = 1;
		}
	}
}

//TODO: ASK LECTURER IF ItemPQ IS MEANT TO BE THE TYPEDEF OF POINTER, NOT STRUCT ITSELF
ItemPQ dequeuePQ(PQ queue) {
	/* Removes and returns the item (ItemPQ) with smallest 'value'.
	   For items with equal 'value', observes FIFO.
	   Returns null if this queue is empty.
	*/
	if (queue != NULL && queue->nitems > 0) {
		ItemPQ *smallest = queue->front;
		for (ItemPQ *item = queue->front->next; item != NULL; item = item->next) {
			if (item->value < smallest->value) { //not <= to observe FIFO
				smallest = item;
			}
		}
		return *smallest;
	}
	return NULL;
}

void updatePQ(PQ queue, ItemPQ item) {
	/* Updates item with a given 'key' value, by updating that item's value to the given 'value'.
	   If item with 'key' does not exist in the queue, no action is taken
	*/
	if (item != NULL) {
		for (ItemPQ *scan = queue->front; scan != NULL; scan = scan->next) {
			if (scan->key == item->key) {
				scan->value = item->value;
				return;
			}
		}
	}
}

int PQEmpty(PQ queue) {
	//TODO
}

void showPQ(PQ queue) {
	//TODO
}

void freePQ(PQ queue) {
	if (queue != NULL) {
		ItemPQ *delete;
		for (ItemPQ *scan = queue->front; scan != NULL; scan = scan->next) {
			delete = scan;
			scan = scan->next;
			free(delete);
		}
		free(queue);
	}
}