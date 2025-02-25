
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"

int dayTrip(Graph g, Vertex s, Vertex vs[]) {
    int numV = g->nV;
    int numReach = 0;
    for (int i = 0; i < numV; i++) {
        if (g->stdEdges[s][i] == true) {
            vs[numReach] = i;
            numReach++;
        }
    }
    for (int i = 0; i < numV; i++) {
        if (g->fastEdges[s][i] == true) {
            bool invs = false;
            for (int j = 0; j < numReach; j++) {
                if (vs[j] == i) {
                    invs = true;
                }
            }
            if (invs == false) {
                vs[numReach] = i;
                numReach++;
            }
            invs = false;
            for (int k = 0; k < numV; k++) {
                if (g->fastEdges[i][k] == true && k != s) {
                    for (int j = 0; j < numReach; j++) {
                        if (vs[j] == k) {
                            invs = true;
                        }
                    }
                    if (invs == false) {
                        vs[numReach] = k;
                        numReach++;
                    }
                }
            }
        }
    }
    return numReach;
}

