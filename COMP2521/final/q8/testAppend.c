// Main program for testing append

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node *append(struct node *l1, struct node *l2);

int main(int argc, char *argv[]) {
    char buffer[1024];

    char *line1 = fgets(buffer, sizeof(buffer), stdin);
    struct node *l1 = listRead(line1);
    printf("List 1: ");
    listShow(l1);
    printf("\n");

    char *line2 = fgets(buffer, sizeof(buffer), stdin);
    struct node *l2 = listRead(line2);
    printf("List 2: ");
    listShow(l2);
    printf("\n");

    struct node *result = append(l1, l2);
    printf("Result: ");
    listShow(result);
    printf("\n");

    listFree(result);
}

