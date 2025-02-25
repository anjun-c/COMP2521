// by adrian chen z5592060

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "poodle.h"

////////////////////////////////////////////////////////////////////////
// structs & defines

#define NO_COMPUTER -1

// task 1
// struct for entry in hash table
struct slot {
    int key;
    int value;
    struct slot *next;
};

// hash table struct
struct hashTable {
    struct slot **table;
    int size;
};

// task 3
// minheap structs
struct minHeapNode {
    int computer;
    int time;
};

struct minHeap {
    int size;
    int capacity;
    int *position;
    struct minHeapNode **array;
};

////////////////////////////////////////////////////////////////////////
// prototypes

// task 1
// hashtable
struct hashTable *newHashTable(int size);
int hashFunction(int a, int b, int size);
void newConnection(struct hashTable *hashTable, int a, int b, int time);
int getConnection(struct hashTable *hashTable, int a, int b);
void freeHashTable(struct hashTable *hashTable);

// task 2

// bfs
int *bfsReachable(int start, struct hashTable *connTable, struct computer computers[], int numComputers, int *reachableCount);

// quicksort
void swap(int *a, int *b);
int partition(int arr[], int low, int high);
void quicksort(int arr[], int low, int high);

// task 3
// helper function to get neighbour of a computer in a connection
int getNeighbour(struct connection conn, int computer);
// min heap functions
struct minHeap *createMinHeap(int capacity);
void minHeapify(struct minHeap *mh, int idx);
int isEmpty(struct minHeap *mh);
struct minHeapNode *extractMin(struct minHeap *mh);
void decreaseKey(struct minHeap *mh, int v, int time);
bool isInMinHeap(struct minHeap *mh, int v);
void freeMinHeap(struct minHeap *mh);

////////////////////////////////////////////////////////////////////////
// Task 1

struct probePathResult probePath(
    struct computer computers[], int numComputers,
    struct connection connections[], int numConnections,
    int path[], int pathLength
) {
    struct probePathResult res = {SUCCESS, 0};
    res.elapsedTime = 0;
    if (pathLength == 0) {
        res.status = SUCCESS;
        return res;
    }

    // new hash table for connections[]
    struct hashTable *connTable = newHashTable(numComputers * 2);
    for (int i = 0; i < numConnections; i++) {
        newConnection(connTable, connections[i].computerA, 
        connections[i].computerB, connections[i].transmissionTime);
    }
    // visited array
    bool *visited = calloc(numComputers, sizeof(bool));

	// loop through path[]
    for (int i = 0; i < pathLength - 1; i++) {
        int curr = path[i];
        int next = path[i + 1];

        // mark visited
        if (!visited[curr]) {
            res.elapsedTime += computers[curr].poodleTime;
            visited[curr] = true;
        }
        if (curr == next) {
            continue;
        }

        // gets connection of current and next
        int connectionTime = getConnection(connTable, curr, next);
        if (connectionTime == -1) {
            res.status = NO_CONNECTION;
            freeHashTable(connTable);
            free(visited);
            return res;
        }

        // check security lvl
        if (computers[curr].securityLevel + 1 >= computers[next].securityLevel) {
            res.elapsedTime += connectionTime;
        } else {
            res.status = NO_PERMISSION;
            freeHashTable(connTable);
            free(visited);
            return res;
        }
    }

    // add the last comptuer's time
    if (!visited[path[pathLength - 1]]) {
        res.elapsedTime += computers[path[pathLength - 1]].poodleTime;
    }

    res.status = SUCCESS;
    freeHashTable(connTable);
    free(visited);
    return res;
}

////////////////////////////////////////////////////////////////////////
// Task 2

struct chooseSourceResult chooseSource(
    struct computer computers[], int numComputers,
    struct connection connections[], int numConnections
) {
    struct chooseSourceResult res = {0, 0, NULL};

    // new hash table for connections[]
    struct hashTable *connTable = newHashTable(numComputers * 2);
    for (int i = 0; i < numConnections; i++) {
        newConnection(connTable, connections[i].computerA, 
        connections[i].computerB, connections[i].transmissionTime);
    }

    int maxReachable = 0;
    int bestSource = -1;
    int *reachableComputers = NULL;

	// loop through computers
    for (int start = 0; start < numComputers; start++) {
        int reachableCount = 0;
		// get all reachable computers using bfs
        int *reachableTemp = bfsReachable(start, connTable, computers, numComputers, &reachableCount);

        if (reachableCount > maxReachable) {
            maxReachable = reachableCount;
            bestSource = start;
            if (reachableComputers != NULL) {
                free(reachableComputers);
            }
            reachableComputers = reachableTemp;
        } else {
            free(reachableTemp);
        }
    }

    res.sourceComputer = bestSource;
    res.numComputers = maxReachable;
    res.computers = reachableComputers;

    freeHashTable(connTable);
    return res;
}


////////////////////////////////////////////////////////////////////////
// Task 3

struct poodleResult poodle(
    struct computer computers[], int numComputers,
    struct connection connections[], int numConnections,
    int sourceComputer
) {
    struct poodleResult res = {0, NULL};
    // initialise data structures
    res.steps = malloc(numComputers * sizeof(struct step));
    int *poodleTime = malloc(numComputers * sizeof(int));
    bool *visited = calloc(numComputers, sizeof(bool));
    int *predecessor = malloc(numComputers * sizeof(int));
    struct minHeap* mh = createMinHeap(numComputers);
    for (int v = 0; v < numComputers; v++) {
        poodleTime[v] = INT_MAX;
        mh->array[v] = malloc(sizeof(struct minHeapNode));
        mh->array[v]->computer = v;
        mh->array[v]->time = poodleTime[v];
        mh->position[v] = v;
        predecessor[v] = NO_COMPUTER;
    }

    mh->array[sourceComputer]->time = computers[sourceComputer].poodleTime;
    poodleTime[sourceComputer] = computers[sourceComputer].poodleTime;
    decreaseKey(mh, sourceComputer, computers[sourceComputer].poodleTime);

    mh->size = numComputers;

    // dijkstra's algorithm modified
    int numCompLeft = numComputers;
    while (!isEmpty(mh)) {
        numCompLeft--;
        struct minHeapNode* minNode = extractMin(mh);
        int u = minNode->computer;
        free(minNode);
        
        if (poodleTime[u] == INT_MAX) {
            break;
        }

        visited[u] = true;
        res.steps[res.numSteps].computer = u;
        res.steps[res.numSteps].time = poodleTime[u];
        res.steps[res.numSteps].recipients = NULL;

        for (int i = 0; i < numConnections; i++) {
            int v = getNeighbour(connections[i], u);

            if (v != NO_COMPUTER && !visited[v] &&
                computers[u].securityLevel >= computers[v].securityLevel - 1) {
                int newPoodleTime = poodleTime[u] + computers[v].poodleTime + connections[i].transmissionTime;

                // prevent overflow
                if (newPoodleTime < poodleTime[v] && newPoodleTime > 0) {
                    poodleTime[v] = newPoodleTime;
                    decreaseKey(mh, v, newPoodleTime);
                    predecessor[v] = u;
                }
            }
        }

        res.numSteps++;
    }

    // get recipients
    for (int i = 0; i < res.numSteps; i++) {
        int current = res.steps[i].computer;
        for (int j = 0; j < numComputers; j++) {
            if (predecessor[j] == current) {
                struct computerList **ptr = &res.steps[i].recipients;
                while (*ptr != NULL && (*ptr)->computer < j) {
                    ptr = &(*ptr)->next;
                }
                struct computerList *newRecipient = malloc(sizeof(struct computerList));
                newRecipient->computer = j;
                newRecipient->next = *ptr;
                *ptr = newRecipient;
            }
        }
    }

    free(poodleTime);
    free(visited);
    free(predecessor);
    
    // free remaining nodes in minHeap if exited early
    if (numCompLeft > 0) {
        for (int i = 0; i < numCompLeft; i++) {
            free(mh->array[i]);
        }
    }
    freeMinHeap(mh);

    return res;
}

////////////////////////////////////////////////////////////////////////
// Task 4

/**
 * Describe your solution in detail here:
 *
 * TODO
 */
struct poodleResult advancedPoodle(
	struct computer computers[], int numComputers,
	struct connection connections[], int numConnections,
	int sourceComputer
) {
	struct poodleResult res = {0, NULL};

	return res;
}

////////////////////////////////////////////////////////////////////////
// Helper functions

// task 1

// Hashtable implementation (for connections)
// edited from lab09 https://cgi.cse.unsw.edu.au/~cs2521/24T2/view/main.cgi/tue18-kora/5592060/submission/lab09/

// make new hash table
struct hashTable *newHashTable(int size) {
    struct hashTable *new = malloc(sizeof(struct hashTable));
    new->table = malloc(size * sizeof(struct slot *));
    for (int i = 0; i < size; i++) {
        new->table[i] = NULL;
    }
    new->size = size;
    return new;
}

// hash function
int hashFunction(int a, int b, int size) {
    return (a * 37 + b) % size;
}

// insert a connection into the hash table
void newConnection(struct hashTable *hashTable, int a, int b, int time) {
    int hashIndex = hashFunction(a, b, hashTable->size);
    struct slot *newEntry = malloc(sizeof(struct slot));
    newEntry->key = b;
    newEntry->value = time;
    newEntry->next = hashTable->table[hashIndex];
    hashTable->table[hashIndex] = newEntry;
    hashIndex = hashFunction(b, a, hashTable->size);
    newEntry = malloc(sizeof(struct slot));
    newEntry->key = a;
    newEntry->value = time;
    newEntry->next = hashTable->table[hashIndex];
    hashTable->table[hashIndex] = newEntry;
}

// find a connection in the hash table
int getConnection(struct hashTable *hashTable, int a, int b) {
    int index = hashFunction(a, b, hashTable->size);
    struct slot *entry = hashTable->table[index];
    while (entry != NULL) {
        if (entry->key == b) {
            return entry->value;
        }
        entry = entry->next;
    }
    return -1;
}

// free hash table
void freeHashTable(struct hashTable *hashTable) {
    for (int i = 0; i < hashTable->size; i++) {
        struct slot *entry = hashTable->table[i];
        while (entry != NULL) {
            struct slot *temp = entry;
            entry = entry->next;
            free(temp);
        }
    }
    free(hashTable->table);
    free(hashTable);
}

// task 2

// bfs to find all reachable computers
int *bfsReachable(int start, struct hashTable *connTable, struct computer computers[], int numComputers, int *reachableCount) {
    bool *visited = calloc(numComputers, sizeof(bool));
    int *queue = malloc(numComputers * sizeof(int));
    int *reachableTemp = malloc(numComputers * sizeof(int));
    int front = 0, back = 0;

    queue[back++] = start;
    visited[start] = true;
    *reachableCount = 0;

    while (front < back) {
        int curr = queue[front++];
        reachableTemp[(*reachableCount)++] = curr;

        for (int i = 0; i < numComputers; i++) {
            if (i != curr && !visited[i]) {
                int connectionTime = getConnection(connTable, curr, i);
                if (connectionTime != -1 && computers[curr].securityLevel + 1 >= computers[i].securityLevel) {
                    queue[back++] = i;
                    visited[i] = true;
                }
            }
        }
    }

    free(visited);
    free(queue);

    int *reachableComputers = malloc((*reachableCount) * sizeof(int));
	quicksort(reachableTemp, 0, *reachableCount - 1);
    for (int i = 0; i < *reachableCount; i++) {
        reachableComputers[i] = reachableTemp[i];
    }
    free(reachableTemp);

    return reachableComputers;
}

// quicksort to sort the array of reachable computers
// wk3 lecture
void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int partition(int arr[], int low, int high) {
	int pivot = arr[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++) {
		if (arr[j] < pivot) {
			i++;
			swap(&arr[i], &arr[j]);
		}
	}
	swap(&arr[i + 1], &arr[high]);
	return (i + 1);
}

void quicksort(int arr[], int low, int high) {
	if (low < high) {
		int pi = partition(arr, low, high);
		quicksort(arr, low, pi - 1);
		quicksort(arr, pi + 1, high);
	}
}

// task 3

// helper function to get neighbour of a computer in a connection

int getNeighbour(struct connection conn, int computer) {
	if (conn.computerA == computer) {
		return conn.computerB;
	} else if (conn.computerB == computer) {
		return conn.computerA;
	} else {
		return NO_COMPUTER;
	}
}

// min heap implementation

struct minHeap *createMinHeap(int capacity) {
    struct minHeap* newMinHeap = malloc(sizeof(struct minHeap));
    newMinHeap->position = malloc(capacity * sizeof(int));
    newMinHeap->size = 0;
    newMinHeap->capacity = capacity;
    newMinHeap->array = malloc(capacity * sizeof(struct minHeapNode*));
    return newMinHeap;
}

void minHeapify(struct minHeap* mh, int idx) {
    int smallest, left, right;
    smallest = idx;
    left = 2 * idx + 1;
    right = 2 * idx + 2;

    if (left < mh->size && mh->array[left]->time < mh->array[smallest]->time)
        smallest = left;

    if (right < mh->size && mh->array[right]->time < mh->array[smallest]->time)
        smallest = right;

    if (smallest != idx) {
        struct minHeapNode *smallestNode = mh->array[smallest];
        struct minHeapNode *idxNode = mh->array[idx];

        mh->position[smallestNode->computer] = idx;
        mh->position[idxNode->computer] = smallest;

        struct minHeapNode* temp = mh->array[smallest];
        mh->array[smallest] = mh->array[idx];
        mh->array[idx] = temp;

        minHeapify(mh, smallest);
    }
}

int isEmpty(struct minHeap* mh) {
    return mh->size == 0;
}

struct minHeapNode* extractMin(struct minHeap* mh) {
    if (isEmpty(mh))
        return NULL;

    struct minHeapNode* root = mh->array[0];

    struct minHeapNode* lastNode = mh->array[mh->size - 1];
    mh->array[0] = lastNode;

    mh->position[root->computer] = mh->size - 1;
    mh->position[lastNode->computer] = 0;

    --mh->size;
    minHeapify(mh, 0);

    return root;
}

void decreaseKey(struct minHeap* mh, int v, int time) {
    int i = mh->position[v];
    mh->array[i]->time = time;

    while (i && mh->array[i]->time < mh->array[(i - 1) / 2]->time) {
        mh->position[mh->array[i]->computer] = (i-1)/2;
        mh->position[mh->array[(i-1)/2]->computer] = i;

        struct minHeapNode* temp = mh->array[i];
        mh->array[i] = mh->array[(i - 1) / 2];
        mh->array[(i - 1) / 2] = temp;

        i = (i - 1) / 2;
    }
}

bool isInMinHeap(struct minHeap *mh, int v) {
    if (mh->position[v] < mh->size)
        return true;
    return false;
}

void freeMinHeap(struct minHeap *mh) {
    free(mh->position);
    free(mh->array);
    free(mh);
}