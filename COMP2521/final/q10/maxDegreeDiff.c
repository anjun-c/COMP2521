
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

typedef struct graph *Graph;

int maxDegreeDiff(Graph g) {
    int numV = GraphNumVertices(g);
    int *degreeArr = malloc(sizeof(int) * numV);
    for (int i = 0; i < numV; i++) {
        degreeArr[i] = 0;
    }
    for (int i = 0; i < numV; i++) {
        struct adjNode *currAdj = g->edges[i];
        while (currAdj != NULL) {
            degreeArr[i]++;
            degreeArr[currAdj->v]--;
            currAdj = currAdj->next;
        }
    }
    int max = 0;
    for (int i = 0; i < numV; i++) {
        if (abs(degreeArr[i]) > max) {
            max = abs(degreeArr[i]);
        }
    }
    free(degreeArr);
    return max;
}

