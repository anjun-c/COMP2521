// Implementation of the Undirected Weighted Graph ADT
// Uses an adjacency matrix

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "Pq.h"

// DO NOT modify this struct
struct graph {
    int nV;         // #vertices
    int nE;         // #edges
    double **edges; // adjacency matrix storing positive weights
                    // 0 if nodes not adjacent
};

static bool validVertex(Graph g, Vertex v);

////////////////////////////////////////////////////////////////////////

Graph GraphNew(int nV) {
    assert(nV > 0);

    Graph g = malloc(sizeof(*g));
    if (g == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    g->nV = nV;
    g->nE = 0;

    g->edges = malloc(nV * sizeof(double *));
    if (g->edges == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < nV; i++) {
        g->edges[i] = calloc(nV, sizeof(double));
        if (g->edges[i] == NULL) {
            fprintf(stderr, "error: out of memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return g;
}

void GraphFree(Graph g) {
    for (int i = 0; i < g->nV; i++) {
        free(g->edges[i]);
    }
    free(g->edges);
    free(g);
}

////////////////////////////////////////////////////////////////////////

int GraphNumVertices(Graph g) {
    return g->nV;
}

bool GraphInsertEdge(Graph g, struct edge e) {
    assert(validVertex(g, e.v));
    assert(validVertex(g, e.w));
    assert(e.v != e.w);
    assert(e.weight > 0.0);

    if (g->edges[e.v][e.w] == 0.0) {
        g->edges[e.v][e.w] = e.weight;
        g->edges[e.w][e.v] = e.weight;
        g->nE++;
        return true;
    } else {
        return false;
    }
}

bool GraphRemoveEdge(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));

    if (g->edges[v][w] != 0.0) {   // edge e in graph
        g->edges[v][w] = 0.0;
        g->edges[w][v] = 0.0;
        g->nE--;
        return true;
    } else {
        return false;
    }
}

double GraphIsAdjacent(Graph g, Vertex v, Vertex w) {
    assert(validVertex(g, v));
    assert(validVertex(g, w));
    
    return g->edges[v][w];
}

void GraphShow(Graph g) {
    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (int v = 0; v < g->nV; v++) {
        for (int w = v + 1; w < g->nV; w++) {
            if (g->edges[v][w] != 0.0) {
                printf("Edge %d - %d: %lf\n", v, w, g->edges[v][w]);
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////
// Your task

Graph GraphMst(Graph g) {
    // check if the graph is empty
    int nV = GraphNumVertices(g);
    if (nV == 0) return NULL;

    // creates a new graph to store the minimum spanning tree
    Graph mst = GraphNew(nV);

    // makes a priority queue to store the edges
    Pq pq = PqNew();

    // makes an array to store the vertices that are included in the MST
    bool *inMST = calloc(nV, sizeof(bool));

    // the source vertex is the first vertex
    Vertex src = 0;
    inMST[src] = true;
    int noVertices = 1;

    // adds all edges from the source vertex to the priority queue
    for (Vertex v = 0; v < nV; v++) {
        if (g->edges[src][v] > 0) {
            struct edge e = {src, v, g->edges[src][v]};
            PqInsert(pq, e);
        }
    }

    // uses prim's algo to find mst
    while (!PqIsEmpty(pq) && noVertices < nV) {
        // gets the edge with the smallest weight
        struct edge e = PqExtract(pq);
 
        // chceks if the edge is already in the MST
        if (inMST[e.v] && inMST[e.w]) continue;

        // adds the edge to the MST
        GraphInsertEdge(mst, e);

        // adds the new vertex to the MST
        Vertex newVertex = inMST[e.v] ? e.w : e.v;
        inMST[newVertex] = true;
        noVertices++;

        // adds all edges from the new vertex to the priority queue
        for (Vertex v = 0; v < nV; v++) {
            if (g->edges[newVertex][v] > 0 && !inMST[v]) {
                struct edge newEdge = {newVertex, v, g->edges[newVertex][v]};
                PqInsert(pq, newEdge);
            }
        }
    }

    // checks if the MST has the same number of vertices as the original graph
    if (noVertices < nV) {
        PqFree(pq);
        free(inMST);
        GraphFree(mst);
        return NULL;
    }

    // free memory
    PqFree(pq);
    free(inMST);
    return mst;
}

////////////////////////////////////////////////////////////////////////

static bool validVertex(Graph g, Vertex v) {
    return v >= 0 && v < g->nV;
}

