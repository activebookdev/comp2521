#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>

/*
typedef struct ItemPQ {
   int         key;
   int         value;
} ItemPQ;
*/

struct PQRep {
	ItemPQ **items; //an array of pointers to items, ordered primarily by value, then by entry order for same value
	int num_items; //the actual number of items in our queue
	int arr_size; //the physical size of the array (the number of items that COULD fit)
};

PQ newPQ() {
	//create a new priority queue
	PQ new = malloc(sizeof(struct PQRep));
	if (new == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}

	new->items = malloc(sizeof(ItemPQ)); //initialise the items array to fit 1 item only
	if (new->items == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}

	new->num_items = 0; //queue is empty
	new->arr_size = 1; // we just initialised the arr to size 1

	return new;
}

void shift(PQ queue, int start, int direction) {
	//starting at element queue->items[start], shift all elements above/below (depending on direction, including start) in the direction provided (0=up, 1=down)
	int i; //counting the items we're shifting
	if (direction == 0) {
		//shift up, assuming the element above start is not important
		i = start;
		while (i < queue->num_items) {
			queue->items[i-1] = queue->items[i]; //perform the shift
			i++;
		}
		queue->items[queue->num_items-1] = NULL; //to clean the array
	} else if (direction == 1) {
		//shift down
		i = queue->num_items-2; //start at the second last element of the array because the last element will be null
		while (i >= start) {
			queue->items[i+1] = queue->items[i]; //perform the shift
			i--;
		}
		queue->items[start] = NULL; //to clean the array
	}
}

ItemPQ *duplicate(ItemPQ item) {
	//make a copy of item, and return the address of that copy
	ItemPQ *copy = malloc(sizeof(ItemPQ));
	if (copy == NULL) {
		fprintf(stderr, "Error!\n");
		return NULL;
	}
	copy->key = item.key;
	copy->value = item.value;
	return copy;
}

void addPQ(PQ queue, ItemPQ item) {
	//add a duplicate of item to the array, ordered by value, unless its key is already present (in that case, update value and shift position accordingly)
	if (queue != NULL) {
		if (queue->num_items > 0) {
			//first, scan the queue for a match
			int i = 0;
			ItemPQ *scan;
			int j = 0;
			int match = 0;
			int insert = 0;
			while (i < queue->num_items) {
				scan = queue->items[i]; //this will hold onto our shifting node
				if (scan->key == item.key) {
					//we have a key match, so just update
					match = 1;
					insert = 0;
					if ((queue->num_items == 1) || (i == 0 && item.value <= queue->items[1]->value) || (i == queue->num_items-1 && item.value >= queue->items[i-1]->value) || (item.value >= queue->items[i-1]->value && item.value <= queue->items[i+1]->value)) {
						//don't shift because the value change holds its position
						scan->value = item.value;
					} else if (item.value < scan->value) {
						//we need to update and move scan up the array
						scan->value = item.value;
						shift(queue, i+1, 0); //remove scan from the array of items for now
						j = i-1;
						while (j >= 0) {
							if (queue->items[j]->value <= scan->value) {
								//we have found our insert position
								shift(queue, j+1, 1); //make room for scan's insertion
								queue->items[j+1] = scan; //insert scan in the correct, freed space
								insert = 1;
								break;
							}
							j--;
						}
						if (insert == 0) {
							//insert at the top
							shift(queue, 0, 1); //start at the top of the list and pull everything down
							queue->items[0] = scan;
						}
					} else if (item.value > scan->value) {
						//we need to update and move scan down the array
						scan->value = item.value;
						shift(queue, i+1, 0); //remove scan from the array of items for now
						j = i+1;
						while (j < queue->num_items-1) { //-1 since we removed scan
							if (queue->items[j]->value >= scan->value) {
								//we have found our insert position
								shift(queue, j-1, 0); //make room for scan's insertion
								queue->items[j-1] = scan; //insert scan in the correct, freed space
								insert = 1;
								break;
							}
							j++;
						}
						if (insert == 0) {
							//insert at the bottom
							queue->items[queue->num_items-1] = scan;
						}
					}
					break;
				}
				i++;
			}

			if (match == 0) {
				//the item does not exist in the queue yet, so we need to insert it into the correct place
				//duplicate item, to prevent meddling with the queue by user outside ADT
				ItemPQ *copy = duplicate(item);
				if (copy != NULL) {
					if (queue->arr_size == queue->num_items) {
						//we need to expand the array
						int size = queue->num_items;
						queue->items = (ItemPQ **)realloc(queue->items, (size+1)*sizeof(ItemPQ));
						queue->arr_size++;
					}
					insert = 0;
					i = 0;
					while (i < queue->num_items) {
						if (queue->items[i]->value > item.value) { //not >= because we want to preserve FIFO order for equivalent value items
							//insert in the items slot of i (above the current item in i which will be shifted down)
							queue->num_items++;
							shift(queue, i, 1);
							queue->items[i] = copy;
							insert = 1;
							break;
						}
						i++;
					}
					if (insert == 0) {
						//we need to insert the new item at the end of the array
						queue->items[queue->num_items] = copy;
						queue->num_items++;
					}
				}
			}
		} else {
			ItemPQ *copy = duplicate(item);
			if (copy != NULL) {
				queue->items[0] = copy;
				queue->num_items = 1;
			}
		}
	}
}

ItemPQ dequeuePQ(PQ queue) {
	//remove the top element of the sorted array of items
	ItemPQ *top = queue->items[0];
	queue->items[0] = NULL;
	shift(queue, 1, 0);
	queue->num_items--;
	return *top;
}

void updatePQ(PQ queue, ItemPQ item) {
	//scan the items array for the matching key, and update the node's value, then shift its position accordingly
	if (queue != NULL) {
		if (queue->num_items > 0) {
			//first, scan the queue for a match
			int i = 0;
			ItemPQ *scan;
			int j = 0;
			int insert = 0;
			while (i < queue->num_items) {
				scan = queue->items[i]; //this will hold onto our shifting node
				if (scan->key == item.key) {
					//we have a key match, so just update
					insert = 0;
					if ((queue->num_items == 1) || (i == 0 && item.value <= queue->items[1]->value) || (i == queue->num_items-1 && item.value >= queue->items[i-1]->value) || (item.value >= queue->items[i-1]->value && item.value <= queue->items[i+1]->value)) {
						//don't shift because the value change holds its position
						scan->value = item.value;
					} else if (item.value < scan->value) {
						//we need to update and move scan up the array
						scan->value = item.value;
						shift(queue, i+1, 0); //remove scan from the array of items for now
						j = i-1;
						while (j >= 0) {
							if (queue->items[j]->value <= scan->value) {
								//we have found our insert position
								shift(queue, j+1, 1); //make room for scan's insertion
								queue->items[j+1] = scan; //insert scan in the correct, freed space
								insert = 1;
								break;
							}
							j--;
						}
						if (insert == 0) {
							//insert at the top
							shift(queue, 0, 1); //start at the top of the list and pull everything down
							queue->items[0] = scan;
						}
					} else if (item.value > scan->value) {
						//we need to update and move scan down the array
						scan->value = item.value;
						shift(queue, i+1, 0); //remove scan from the array of items for now
						j = i+1;
						while (j < queue->num_items-1) { //-1 since we removed scan
							if (queue->items[j]->value >= scan->value) {
								//we have found our insert position
								shift(queue, j-1, 0); //make room for scan's insertion
								queue->items[j-1] = scan; //insert scan in the correct, freed space
								insert = 1;
								break;
							}
							j++;
						}
						if (insert == 0) {
							//insert at the bottom
							queue->items[queue->num_items-1] = scan;
						}
					}
					break;
				}
				i++;
			}
		}
	}
}

int PQEmpty(PQ queue) {
	//check if queue is empty
	if (queue->num_items == 0) return 1;
	return 0;
}

void showPQ(PQ queue) {
	//print the items in the queue in order
	printf("Number of items = %d, array length = %d\n", queue->num_items, queue->arr_size);
	int i = 0;
	while (i < queue->num_items) {
		printf("Node %d = (key %d, value %d)\n", i, queue->items[i]->key, queue->items[i]->value);
		i++;
	}
}

void freePQ(PQ queue) {
	free(queue->items);
	free(queue);
}
