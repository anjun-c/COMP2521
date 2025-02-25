// Implementation of the Counter ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Counter.h"

// function prototype
struct counter *CounterItemsHelper(Counter c, int *numItems, struct item *itemArray);

// struct for counter
struct counter {
	struct counter *left;
	struct counter *right;
	int count;
	char *character;
};

// create a new Counter
Counter CounterNew(void) {
	// allocate memory for Counter
	struct counter *new = malloc(sizeof(struct counter));
	if (new == NULL) {
		fprintf(stderr, "Failed to allocate memory for Counter\n");
		return NULL;
	}
	// initialise values
	new->left = NULL;
	new->right = NULL;
	new->count = 0;
	new->character = NULL;
	return new;
}

// free memory associated with a Counter
void CounterFree(Counter c) {
	if (c == NULL) return;
	// recursively free memory
	CounterFree(c->left);
	CounterFree(c->right);
	free(c->character);
	free(c);
	return;
}

// adds a new counter node or increments the count of an existing node
void CounterAdd(Counter c, char *character) {
	// base case
	if (c->character == NULL) {
		c->character = strdup(character);
		if (c->character == NULL) {
			fprintf(stderr, "Failed to allocate memory for character\n");
			return;
		}
		c->count++;
		return;
	}
	// recursive cases
	int cmp = strcmp(c->character, character);
	// same
	if (cmp == 0) {
		// increment count
		c->count++;
		return;
	}
	// left
	if (cmp > 0) {
		// create new node
		if (c->left == NULL) {
			c->left = CounterNew();
			if (c->left == NULL) {
				fprintf(stderr, "Failed to allocate memory for Counter\n");
				return;
			}
		}
		// recursive call
		CounterAdd(c->left, character);
		return;
	}
	// right
	if (cmp < 0) {
		// create new node
		if (c->right == NULL) {
			c->right = CounterNew();
			if (c->right == NULL) {
				fprintf(stderr, "Failed to allocate memory for Counter\n");
				return;
			}
		}
		// recursive call
		CounterAdd(c->right, character);
		return;
	}
}

// returns the number of unique items in the Counter
int CounterNumItems(Counter c) {
	// base case
	if (c == NULL || c->character == NULL) return 0;
	// recursive case to traverse entire tree
	return 1 + CounterNumItems(c->left) + CounterNumItems(c->right);
}

// returns the total count for all items in the Counter
int CounterGet(Counter c, char *character) {
	// base case
	if (c == NULL || c->character == NULL) return 0;
	// recursive cases to traverse entire tree
	if (strcmp(c->character, character) == 0) return c->count;
	if (strcmp(c->character, character) > 0) return CounterGet(c->left, character);
	if (strcmp(c->character, character) < 0) return CounterGet(c->right, character);
	return 0;
}

// returns an array of all items in the Counter
struct item *CounterItems(Counter c, int *numItems) {
	int count = CounterNumItems(c);
	if (count == 0) return NULL;
	// allocate memory for itemArray
	struct item *itemArray = malloc(sizeof(struct item) * count);
	if (itemArray == NULL) {
		fprintf(stderr, "Failed to allocate memory for itemArray\n");
		return NULL;
	}
	// call helper function
	CounterItemsHelper(c, numItems, itemArray);
	return itemArray;
}

// helper function for CounterItems
struct counter *CounterItemsHelper(Counter c, int *numItems, struct item *itemArray) {
	// base case
	if (c == NULL || c->character == NULL) return NULL;
	// recursive cases - copies character and count to itemArray
	strcpy(itemArray[*numItems].character, c->character);
	itemArray[*numItems].freq = c->count;
	(*numItems)++;
	// recursive calls to traverse entire tree
	CounterItemsHelper(c->left, numItems, itemArray);
	CounterItemsHelper(c->right, numItems, itemArray);
	return c;
}
