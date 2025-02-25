// Main program for testing minDiff

// !!! DO NOT MODIFY THIS FILE !!!

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"

int minDiff(struct node *t, int l);

int main(int argc, char *argv[]) {
    char buffer[1024];

    char *line1 = fgets(buffer, sizeof(buffer), stdin);
    struct node *t = bstRead(line1);
    printf("\nTree:\n\n");
    bstShow(t);

    int l;
    if (scanf("%d", &l) != 1) {
        printf("error: failed to read l\n");
        return 1;
    }

    int ans = minDiff(t, l);
    printf("\nminimum difference on level %d: %d\n", l, ans);

    bstFree(t);
}

