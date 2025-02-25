// BST definition and interface to utility functions

// !!! DO NOT MODIFY THIS FILE !!!

#ifndef BST_H
#define BST_H

struct node {
    int key;
    struct node *left;
    struct node *right;
};

// Creates a new empty BST
struct node *bstNew(void);

// Frees all memory associated with the given BST
void bstFree(struct node *t);

// Prints a BST to stdout
void bstShow(struct node *t);

// Inserts a new key into a BST
struct node *bstInsert(struct node *t, int key);

// Creates a BST by reading integer values from a line 
struct node *bstRead(char *line);

#endif

