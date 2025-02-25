
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "bst.h"
void returnLevelNodes(struct node *root, int lvl, int lvlNodes[], int *numNodes);

int minDiff(struct node *t, int l) {
    if (l > 3 || l == 0) {
        return 0;
    }
    int maxNodes = 1;
    for (int i = 0; i < l; i++) {
        maxNodes = maxNodes * 2;
    }
    int numNodes = 0;
    int *lvlNodes = malloc(sizeof(int) * maxNodes);
    returnLevelNodes(t, l, lvlNodes, &numNodes);
    int minD = -1;
    for (int i = 0; i < numNodes; i++) {
        for (int j = 0; j < numNodes; j++) {
            if (i != j && (abs(lvlNodes[i] - lvlNodes[j]) < minD || minD == -1)) {
                //printf("%d - %d aaaaaaaaaaa\n", lvlNodes[i], lvlNodes[j]);
                minD = abs(lvlNodes[i] - lvlNodes[j]);
            }
        }
    }
    if (minD == -1) {
        minD = 0;
    }
    free(lvlNodes);
    return minD;
}

void returnLevelNodes(struct node *root, int lvl, int lvlNodes[], int *numNodes) {
    if (root == NULL) return;
    if (lvl == 0) {
        lvlNodes[*numNodes] = root->key;
        *numNodes = *numNodes + 1;
    }
    
    returnLevelNodes(root->left, lvl - 1, lvlNodes, numNodes);
    returnLevelNodes(root->right, lvl - 1, lvlNodes, numNodes);
}