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

// Inserts a value at the end of the given list
// Returns the beginning of the updated list
struct node *listInsert(struct node *list, int value) {
    struct node *n = newNode(value);
    if (list == NULL) {
        return n;
    }
    struct node *curr = list;
    while (curr->next != NULL) {
        curr = curr->next;
    }
    curr->next = n;
    return list;
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

// Prints a list to stdout
void listShow(struct node *list) {
    for (struct node *curr = list; curr != NULL; curr = curr->next) {
        printf("%d -> ", curr->value);
    }
    printf("<END>\n");
}

// Prints the ascending prefix of a list to stdout
void listShowAscending(struct node *list) {
    if (list != NULL) {
        struct node *curr = list;
        int v = list->value;
        do {
            printf("%d -> ", curr->value);
            v = curr->value;
            curr = curr->next;
        } while (curr != NULL && curr->value > v);
    }
    printf("<END>\n");
}

// Creates a list by reading integer values from a line
struct node *listRead(FILE *fp) {
    struct node *head = NULL;
    struct node *curr = NULL;

    int value;
    while (fscanf(fp, "%d", &value) == 1) {
        if (head == NULL) {
            head = newNode(value);
            curr = head;
        } else {
            curr->next = newNode(value);
            curr = curr->next;
        }
    }

    return head;
}

