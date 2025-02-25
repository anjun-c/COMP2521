#include <stdbool.h>
#include <stdio.h>

#include "Graph.h"

int main(void) {
    printf("Enter number of vertices: ");
    int nV = 0;
    scanf("%d", &nV);
    Graph g = GraphNew(nV);
    
    int u, v;
    printf("Enter edges: ");
    while (scanf("%d %d", &u, &v) == 2) {
        g->edges[u][v] = true;
        g->edges[v][u] = true;
    }
    
    if (GraphNumComponents(g) == 1) {
        printf("The graph is connected\n");
    } else {
        printf("The graph is not connected\n");
    }
    
    GraphFree(g);
}