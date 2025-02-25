// Linked list utility functions

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"

static struct node *newNode(int value);

// Creates a new empty list
struct node *listNew(void) {
    return NULL;
}

// Frees all memory associated with the given list
void listFree(struct node *list) {
    struct node *curr = list;
    while (curr != NULL) {
        struct node *temp = curr;
        curr = curr->next;
        free(temp);
    }
}

// Prints a list to stdout
void listShow(struct node *list) {
    printf("[");
    for (struct node *curr = list; curr != NULL; curr = curr->next) {
        printf("%d", curr->value);
        if (curr->next != NULL) {
            printf(", ");
        }
    }
    printf("]");
}

// Creates a list by reading integer values from a line 
struct node *listRead(char *line) {
    char delim[] = ", ";
    int value;

    struct node *head = NULL;
    struct node *curr = NULL;

    char *tkn = strtok(line, delim);

    while (tkn != NULL) {
        int count = sscanf(tkn, "%d", &value);
        if (count == 1) {
            if (head == NULL) {
                head = newNode(value);
                curr = head;
            } else {
                curr->next = newNode(value);
                curr = curr->next;
            }
        }
        tkn = strtok(NULL, delim);
    }

    return head;
}

// Creates a new list node
static struct node *newNode(int value) {
    struct node *n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    n->value = value;
    n->next = NULL;
    return n;
}

