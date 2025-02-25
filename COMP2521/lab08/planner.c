// Algorithms to design electrical grids

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "place.h"
#include "Pq.h"

////////////////////////////////////////////////////////////////////////
// Your task

// helper to calculate distance
static double calculateDistance(struct place p1, struct place p2) {
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * from a power plant to all the given cities. Power lines must be built
 * between cities or between a city and a power plant.  Cost is directly
 * proportional to the total length of power lines used.
 * Assumes  that  numCities  is at least 1 (numCities is the size of the
 * cities array).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid1(struct place cities[], int numCities, 
              struct place powerPlant, 
              struct powerLine powerLines[]) {
    // creating a graph with numCities + 1 vertices (cities + power plant)
    int totalVertices = numCities + 1; 
    Graph graph = GraphNew(totalVertices);
    Pq pq = PqNew();

    // inserts edges from the power plant to all cities
    for (int i = 0; i < numCities; i++) {
        double distance = calculateDistance(cities[i], powerPlant);
        struct edge e = {numCities, i, distance};
        GraphInsertEdge(graph, e);
    }

    // inserts edges between all cities
    for (int i = 0; i < numCities; i++) {
        for (int j = i + 1; j < numCities; j++) {
            double distance = calculateDistance(cities[i], cities[j]);
            struct edge e = {i, j, distance};
            GraphInsertEdge(graph, e);
        }
    }

    // new graph to store the minimum spanning tree
    Graph mst = GraphNew(totalVertices);
    // array to store the vertices in mst
    bool *inMST = calloc(totalVertices, sizeof(bool));
    
    int numPowerLines = 0;
    inMST[numCities] = true;
    int includedVertices = 1;

    // add edges from the power plant to the priority queue
    for (int i = 0; i < numCities; i++) {
        double distance = calculateDistance(cities[i], powerPlant);
        struct edge e = {numCities, i, distance};
        PqInsert(pq, e);
    }

    // prim's algorithm
    while (!PqIsEmpty(pq) && includedVertices < totalVertices) {
        struct edge e = PqExtract(pq);

        if (inMST[e.v] && inMST[e.w]) continue;
        // add the edge to the minimum spanning tree
        GraphInsertEdge(mst, e);
        // add the power line to the powerLines array
        powerLines[numPowerLines].p1 = (e.v == numCities) ? powerPlant : cities[e.v];
        powerLines[numPowerLines].p2 = (e.w == numCities) ? powerPlant : cities[e.w];
        numPowerLines++;

        int newVertex = inMST[e.v] ? e.w : e.v;
        inMST[newVertex] = true;
        includedVertices++;

        // Add edges from the new vertex to the priority queue
        for (int i = 0; i < totalVertices; i++) {
            if (!inMST[i]) {
                // calculate distance between the new vertex and the current vertex
                double distance = (newVertex == numCities || i == numCities) ?
                calculateDistance((newVertex == numCities) ? powerPlant : cities[newVertex],
                (i == numCities) ? powerPlant : cities[i]) :
                calculateDistance(cities[newVertex], cities[i]);
                // insert the edge into the priority queue
                struct edge newEdge = {newVertex, i, distance};
                PqInsert(pq, newEdge);
            }
        }
    }

    // free memory
    GraphFree(graph);
    GraphFree(mst);
    PqFree(pq);
    free(inMST);
    return numPowerLines;
}

////////////////////////////////////////////////////////////////////////
// Optional task

/**
 * Designs  a minimal cost electrical grid that will deliver electricity
 * to all the given cities.  Power lines must be built between cities or
 * between a city and a power plant.  Cost is directly  proportional  to
 * the  total  length of power lines used.  Assume that each power plant
 * generates enough electricity to supply all cities, so not  all  power
 * plants need to be used.
 * Assumes  that  numCities and numPowerPlants are at least 1 (numCities
 * and numPowerPlants are the sizes of the cities and powerPlants arrays
 * respectively).
 * Stores the power lines that need to be built in the given  powerLines
 * array, and returns the number of power lines stored. Assumes that the
 * powerLines array is large enough to store all required power lines.
 */
int planGrid2(struct place cities[], int numCities,
              struct place powerPlants[], int numPowerPlants,
              struct powerLine powerLines[]) {
    // TODO: Complete this function
    return 0;
}
