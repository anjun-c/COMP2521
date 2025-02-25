// Main program for testing flas

// !!! DO NOT MODIFY THIS FILE !!!

#include <stdio.h>
#include <stdlib.h>

#include "list.h"

struct node *flas(struct node *l);

int main(int argc, char *argv[]) {
    struct node *l = listRead(stdin);
    printf("List: ");
    listShow(l);

    struct node *longest = flas(l);
    printf("FLAS: ");
    listShowAscending(longest);

    listFree(l);
}

