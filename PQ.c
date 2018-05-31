#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "PQ.h"

typedef struct _ItemPQnode {
	ItemPQ *item;
	struct _ItemPQnode *next;
} *ItemPQnode;

struct PQRep {
	int nitems;
	ItemPQnode front;
	ItemPQnode back;
};

//since ItemPQ doesn't have a next field, it needs to be put into a wrapping struct ItemPQnode with a next field in order to use it with the queue

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

ItemPQnode wrap_item(ItemPQ *item) {
	ItemPQnode new = malloc(sizeof(struct _ItemPQnode));
	if (new == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}
	new->item = item;
	new->next = NULL;
	return new;
}

void addPQ(PQ queue, ItemPQ item) {
	//scan the queue for a node with the same key as 'item', if one exists, update its value, otherwise insert onto the back of the queue
	ItemPQnode node = wrap_item(&item);
	for (ItemPQnode scan = queue->front; scan != NULL; scan = scan->next) {
		if (scan->item->key == item.key) {
			scan->item->value = item.value;
			return;
		}
	}

	if (queue->back != NULL) {
		queue->back->next = node;
		node->next = NULL;
		queue->back = node;
		queue->nitems++;
	} else {
		//queue is empty
		queue->front = node;
		queue->back = node;
		node->next = NULL;
		queue->nitems = 1;
	}
}

ItemPQ dequeuePQ(PQ queue) {
	/* Removes and returns the item (ItemPQ) with smallest 'value'.
	   For items with equal 'value', observes FIFO.
	   Returns null if this queue is empty.
	*/
	if (queue != NULL && queue->nitems > 0) {
		ItemPQnode smallest = queue->front;
		for (ItemPQnode node = queue->front->next; node != NULL; node = node->next) {
			if (node->item->value < smallest->item->value) { //not <= to observe FIFO
				smallest = node;
			}
		}
		return *smallest->item;
	}
	fprintf(stderr, "Error!\n");
	exit(EXIT_FAILURE);
}

void updatePQ(PQ queue, ItemPQ item) {
	/* Updates item with a given 'key' value, by updating that item's value to the given 'value'.
	   If item with 'key' does not exist in the queue, no action is taken
	*/
	printf("updating\n");
	for (ItemPQnode scan = queue->front; scan != NULL; scan = scan->next) {
		printf("compare %d,%d against %d,%d\n", scan->item->key, scan->item->value, item.key, item.value);
		if (scan->item->key == item.key) {
			printf("match at %d, %d\n", item.key, item.value);
			scan->item->value = item.value;
			return;
		}
	}
}

int PQEmpty(PQ queue) {
	if (queue->nitems == 0) return 1;
	return 0;
}

void showPQ(PQ queue) {
	//print the queue
	if (queue != NULL) {
		ItemPQnode scan = queue->front;
		while (scan != NULL) {
			printf("key: %d value: %d\n", scan->item->key, scan->item->value);
			scan = scan->next;
		}
	}
}

void freePQ(PQ queue) {
	if (queue != NULL) {
		ItemPQnode delete;
		for (ItemPQnode scan = queue->front; scan != NULL; scan = scan->next) {
			delete = scan;
			scan = scan->next;
			free(delete->item);
			free(delete);
		}
		free(queue);
	}
}