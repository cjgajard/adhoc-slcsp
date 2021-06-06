#include <stdlib.h>
#include <string.h>
#include "slcsp.h"

int list_add (struct list *this, void *v)
{
	struct listnode *node;
	node = malloc(sizeof(*node));
	if (!node) {
		perror(NULL);
		return 1;
	}
	if (!v) {
		fprintf(stderr, "must not add null data\n");
		return 2;
	}

	node->data = malloc(this->size);
	if (!node->data) {
		perror(NULL);
		return 3;
	}
	memcpy(node->data, v, this->size);

	if (!this->head) {
		this->head = node;
		this->tail = node;
	}
	else {
		this->tail->next = node;
		this->tail = node;
	}
	return 0;
}

void list_destroy (struct list *this)
{
	struct listnode *now = this->head;
	while (now) {
		struct listnode *next = now->next;
		free(now->data);
		free(now);
		now = next;
	}
}

void list_print (struct list *this)
{
	struct listnode *now = this->head;
	while (now) {
		printf("%p\n", now->data);
		now = now->next;
	}
}

int list_len (struct list *this)
{
	int i = 0;
	struct listnode *now = this->head;
	while (now) {
		i++;
		now = now->next;
	}
	return i;
}
