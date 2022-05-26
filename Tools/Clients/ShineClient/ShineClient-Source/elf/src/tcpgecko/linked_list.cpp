#include "linked_list.h"

#include <stdio.h>
#include <stdlib.h>

void destroy(struct node *list) {
	struct node *currentNode = list;

	while (currentNode != NULL) {
		struct node *previousNode = currentNode;
		currentNode = currentNode->next;
		free(previousNode);
	}
}

struct node *insert(struct node *list, void *data) {
	size_t structureSize = sizeof(struct node);
	struct node *addedNode = (struct node *) malloc(structureSize);

	addedNode->data = data;
	addedNode->next = list;
	list = addedNode;

	return list;
}

int length(struct node *list) {
	int length = 0;
	struct node *current;

	for (current = list; current != NULL; current = current->next) {
		length++;
	}

	return length;
}

void reverse(struct node **list) {
	struct node *previous = NULL;
	struct node *current = *list;
	struct node *next;

	while (current != NULL) {
		next = current->next;
		current->next = previous;
		previous = current;
		current = next;
	}

	*list = previous;
}