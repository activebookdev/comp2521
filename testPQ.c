#include "PQ.h"
#include <stdlib.h>
#include <stdio.h>

ItemPQ newPQitem(int key, int value) {
	ItemPQ item;
	item.key = key;
	item.value = value;
	return item;
}

int main(){
	PQ new = newPQ();
	ItemPQ item = newPQitem(1, 9);
	addPQ(new, item); //adds item
	item.key = 0;
	item.value = 5;
	addPQ(new, item); //adds item
	item.key = 2;
	item.value = 24;
	addPQ(new, item); //adds item
	showPQ(new);
	item.key = 1;
	item.value = 3;
	addPQ(new, item); //updates item 1's value
	showPQ(new);
	item.key = 0;
	item.value = 30;
	updatePQ(new, item); //update item 0's value
	showPQ(new);
	/*
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
