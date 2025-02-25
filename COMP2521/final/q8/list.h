// Linked list definition and interface to utility functions

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <stdlib.h>

struct node {
    int value;
    struct node *next;
};

// Creates a new empty list
struct node *listNew(void);

// Frees all memory associated with the given list
void listFree(struct node *list);

// Prints a list to stdout
void listShow(struct node *list);

// Creates a list by reading integer values from a file
struct node *listRead(char *line);

#endif

