// Implementation of the FriendBook ADT

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Fb.h"
#include "List.h"
#include "Map.h"
#include "Queue.h"

#define DEFAULT_CAPACITY 1 // DO NOT change this line

struct adjNode {
    int v;
    struct adjNode *next;
};

// DO NOT modify this struct
struct fb {
    int numPeople;
    int capacity;

    char **names; // the id of a person is simply the index
                  // that contains their name in this array
    
    Map nameToId; // maps names to ids

    struct adjNode **adj; // adjacency lists, kept in increasing order
};

static void increaseCapacity(Fb fb);
static int nameToId(Fb fb, char *name);

static struct adjNode *newAdjNode(int v);
static bool inAdjList(struct adjNode *l, int v);
static void freeAdjList(struct adjNode *l);

// Please ignore this line
static struct adjNode * __attribute__((unused)) newAdjNode(int v);

// helper functions
static struct adjNode **removeNode(struct adjNode **head, int id);
static int compareRecommendations(const void *a, const void *b);

////////////////////////////////////////////////////////////////////////

// Creates a new instance of FriendBook
Fb FbNew(void) {
    Fb fb = malloc(sizeof(*fb));
    if (fb == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    fb->numPeople = 0;
    fb->capacity = DEFAULT_CAPACITY;
    
    fb->names = calloc(fb->capacity, sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    
    fb->nameToId = MapNew();

    fb->adj = calloc(fb->capacity, sizeof(struct adjNode *));
    if (fb->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    return fb;
}

void FbFree(Fb fb) {
    for (int i = 0; i < fb->capacity; i++) {
        freeAdjList(fb->adj[i]);
    }
    free(fb->adj);

    MapFree(fb->nameToId);

    for (int i = 0; i < fb->numPeople; i++) {
        free(fb->names[i]);
    }
    free(fb->names);
    
    free(fb);
}

int FbNumPeople(Fb fb) {
    return fb->numPeople;
}

bool FbAddPerson(Fb fb, char *name) {
    if (fb->numPeople == fb->capacity) {
        increaseCapacity(fb);
    }

    if (!MapContains(fb->nameToId, name)) {
        int id = fb->numPeople++;
        fb->names[id] = strdup(name);
        MapSet(fb->nameToId, name, id);
        return true;
    } else {
        return false;
    }
}

static void increaseCapacity(Fb fb) {
    int newCapacity = fb->capacity * 2;
    
    fb->names = realloc(fb->names, newCapacity * sizeof(char *));
    if (fb->names == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->names[i] = NULL;
    }
    
    fb->adj = realloc(fb->adj, newCapacity * sizeof(struct adjNode));
    if (fb->adj == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }
    for (int i = fb->capacity; i < newCapacity; i++) {
        fb->adj[i] = NULL;
    }
    
    fb->capacity = newCapacity;
}

bool FbHasPerson(Fb fb, char *name) {
    return MapContains(fb->nameToId, name);
}

List FbGetPeople(Fb fb) {
    List l = ListNew();
    for (int id = 0; id < fb->numPeople; id++) {
        ListAppend(l, fb->names[id]);
    }
    return l;
}

bool FbIsFriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    return inAdjList(fb->adj[id1], id2);
}

////////////////////////////////////////////////////////////////////////
// Your tasks

bool FbFriend(Fb fb, char *name1, char *name2) {
    int personId1 = nameToId(fb, name1);
    int personId2 = nameToId(fb, name2);
    if (inAdjList(fb->adj[personId1], personId2)) {
        return false;
    }
    struct adjNode *newNode2 = newAdjNode(personId2);
    struct adjNode **curr1 = &fb->adj[personId1];
    while (*curr1 != NULL && (*curr1)->v < personId2) {
        curr1 = &(*curr1)->next;
    }
    newNode2->next = *curr1;
    *curr1 = newNode2;
    struct adjNode *newNode1 = newAdjNode(personId1);
    struct adjNode **curr2 = &fb->adj[personId2];
    while (*curr2 != NULL && (*curr2)->v < personId1) {
        curr2 = &(*curr2)->next;
    }
    newNode1->next = *curr2;
    *curr2 = newNode1;
    return true;
}

int FbNumFriends(Fb fb, char *name) {
    int id = nameToId(fb, name);
    struct adjNode *curr = fb->adj[id];
    int count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

List FbMutualFriends(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    struct adjNode *list1 = fb->adj[id1];
    struct adjNode *list2 = fb->adj[id2];
    List mutualFriends = ListNew();
    struct adjNode *curr1 = list1;
    struct adjNode *curr2 = list2;
    while (curr1 != NULL && curr2 != NULL) {
        if (curr1->v < curr2->v) {
            curr1 = curr1->next;
        } else if (curr1->v > curr2->v) {
            curr2 = curr2->next;
        } else {
            ListAppend(mutualFriends, fb->names[curr1->v]);
            curr1 = curr1->next;
            curr2 = curr2->next;
        }
    }
    return mutualFriends;
}

bool FbUnfriend(Fb fb, char *name1, char *name2) {
    int id1 = nameToId(fb, name1);
    int id2 = nameToId(fb, name2);
    if (!inAdjList(fb->adj[id1], id2)) {
        return false;
    }
    fb->adj[id1] = *removeNode(&fb->adj[id1], id2);
    fb->adj[id2] = *removeNode(&fb->adj[id2], id1);
    return true;
}

struct adjNode **removeNode(struct adjNode **head, int v) {
    struct adjNode *curr = *head;
    struct adjNode *prev = NULL;
    while (curr != NULL && curr->v != v) {
        prev = curr;
        curr = curr->next;
    }
    if (curr == NULL) {
        return head;
    }
    if (prev == NULL) {
        *head = curr->next;
    } else {
        prev->next = curr->next;
    }
    free(curr);
    return head;
}


int FbFriendRecs1(Fb fb, char *name, struct recommendation recs[]) {
    int id = nameToId(fb, name);
    int numRecs = 0;
    bool *visited = calloc(fb->numPeople, sizeof(bool));
    for (struct adjNode *friendNode = fb->adj[id]; friendNode != NULL; friendNode = friendNode->next) {
        int friendId = friendNode->v;

        for (struct adjNode *friendOfFriendNode = fb->adj[friendId]; friendOfFriendNode != NULL; friendOfFriendNode = friendOfFriendNode->next) {
            int friendOfFriendId = friendOfFriendNode->v;

            if (friendOfFriendId == id || FbIsFriend(fb, fb->names[id], fb->names[friendOfFriendId])) {
                continue;
            }

            List mutualFriends = FbMutualFriends(fb, fb->names[id], fb->names[friendOfFriendId]);
            int numMutualFriends = ListSize(mutualFriends);
            ListFree(mutualFriends);

            if (numMutualFriends > 0 && !visited[friendOfFriendId]) {
                recs[numRecs].name = fb->names[friendOfFriendId];
                recs[numRecs].numMutualFriends = numMutualFriends;
                visited[friendOfFriendId] = true;
                numRecs++;
            }
        }
    }
    qsort(recs, numRecs, sizeof(struct recommendation), compareRecommendations);
    free(visited);
    return numRecs;
}

int compareRecommendations(const void *a, const void *b) {
    const struct recommendation *rec1 = (const struct recommendation *)a;
    const struct recommendation *rec2 = (const struct recommendation *)b;
    if (rec1->numMutualFriends > rec2->numMutualFriends) {
        return -1;
    } else if (rec1->numMutualFriends < rec2->numMutualFriends) {
        return 1;
    } else {
        return strcmp(rec1->name, rec2->name);
    }
}


////////////////////////////////////////////////////////////////////////
// Optional task

List FbFriendRecs2(Fb fb, char *name) {
    // TODO: Complete this function
    List l = ListNew();
    return l;
}

////////////////////////////////////////////////////////////////////////
// Helper Functions

// Converts a name to an ID. Raises an error if the name doesn't exist.
static int nameToId(Fb fb, char *name) {
    if (!MapContains(fb->nameToId, name)) {
        fprintf(stderr, "error: person '%s' does not exist!\n", name);
        exit(EXIT_FAILURE);
    }
    return MapGet(fb->nameToId, name);
}

static struct adjNode *newAdjNode(int v) {
    struct adjNode *n = malloc(sizeof(*n));
    if (n == NULL) {
        fprintf(stderr, "error: out of memory\n");
        exit(EXIT_FAILURE);
    }

    n->v = v;
    n->next = NULL;
    return n;
}

static bool inAdjList(struct adjNode *l, int v) {
    for (struct adjNode *n = l; n != NULL && n->v <= v; n = n->next) {
        if (n->v == v) {
            return true;
        }
    }
    return false;
}

static void freeAdjList(struct adjNode *l) {
    struct adjNode *n = l;
    while (n != NULL) {
        struct adjNode *temp = n;
        n = n->next;
        free(temp);
    }
}

