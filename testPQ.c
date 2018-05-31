#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

ItemPQ *newPQnode(int key, int value) {
    ItemPQ *new = malloc(sizeof(ItemPQ));
    if (new == NULL) {
        fprintf(stderr, "Error!\n");
        return NULL;
    }
    new->key = key;
    new->value = value;
    return new;
}

int main(){
	PQ new = newPQ();
	ItemPQ *item = newPQnode(0, 5); //adds item
	addPQ(new, *item);
	showPQ(new);
	/*
	item = newPQnode(1, 9); //adds item
	addPQ(new, *item);
	item = newPQnode(2, 24); //adds item
	addPQ(new, *item);
	item = newPQnode(1, 23); //updates item 1's value
	addPQ(new, *item);
	item = newPQnode(2, 3); //updates item 2's value
	ItemPQ temp = *item;
	updatePQ(new, temp);
	item = newPQnode(4, 100); //do nothing
	temp = *item;
	updatePQ(new, temp);
	temp = dequeuePQ(new);
	printf("just dequeued\n");
	printf("key: %d value: %d\n", temp.key, temp.value); //should print key: 2, value: 3

	//PQEmpty(new); //empty the queue, TAKE THIS OUT TO TEST POPULATED

	printf("start show\n");
	showPQ(new);
	*/
	freePQ(new);
	return EXIT_SUCCESS;
}
