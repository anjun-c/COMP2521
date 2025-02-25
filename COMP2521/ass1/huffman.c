// Implementation of the Huffman module

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "character.h"
#include "Counter.h"
#include "File.h"
#include "huffman.h"

struct map {
	char *character;
	char *encoding;
};

typedef enum {
	READ,
	WRITE,
} Mode;

struct file {
	FILE *fp;
	Mode mode;
};

//helpers
// task 3
static struct huffmanTree **huffmanArrayCreate(struct item *itemArray, int numItems);
static struct huffmanTree *createHuffmanTreeHelper(struct huffmanTree **huffmanArray, int numItems);

// task 4
static void createMap(struct huffmanTree *tree, struct map *mapArray, 
int depth, int *lastIndex, char *encoding);
static int getMaxDepth(struct huffmanTree *tree);
static void mapInsert(struct map *mapArray, char *character, char *encoding, int *lastIndex);
static void freeMap(struct map *mapArray, int numLeafNodes);
static int countLeafNodes(struct huffmanTree *tree, int *numLeafNodes);
long getFileCharacterCount(char *inputFilename);

////////////////////////////////////////////////////////////////
// HASH MAP STUFF

// Hash map entry
typedef struct hashMapEntry {
    char *character;
    char *encoding;
    struct hashMapEntry *next;
} hashMapEntry;

// Hash map
typedef struct hashMap {
    int size;
    hashMapEntry **table;
} hashMap;

// hashmap function prototypes 
hashMap *createHashMap(int size);
unsigned int hash(char *str, int size);
void hashMapInsert(hashMap *map, char *character, char *encoding);
char *hashMapLookup(hashMap *map, char *character);
void freeHashMap(hashMap *map);

////////////////////////////////////////////////////////////////

// Task 1
// decodes an encoding file given a tree and writes the decoded text to a file
void decode(struct huffmanTree *tree, char *encoding, char *outputFilename) {
	struct huffmanTree *curr = tree;
	// open the encoding file and the output file
	File outputFile = FileOpenToWrite(outputFilename);
	// read the encoding file character by character
	while(*encoding != '\0') {
		// if the character is 0, go left, else go right
		if (encoding[0] == '0') {
			curr = curr->left;
		} else {
			curr = curr->right;
		}
		// if we reach a leaf node, write the character to the output file
		if (curr->character != NULL) {
			FileWrite(outputFile, curr->character);
			curr = tree;
		}
		encoding++;
	}
	FileClose(outputFile);
}

// Task 3
// creates a huffman tree from a file
struct huffmanTree *createHuffmanTree(char *inputFilename) {
	// create a counter to count the frequency of each character
	Counter counter = CounterNew();
	// open the file to read
	File file = FileOpenToRead(inputFilename);
	char buffer[MAX_CHARACTER_LEN + 1];
	// read each file character and add the character to the counter
	while (FileReadCharacter(file, buffer)) {
		CounterAdd(counter, buffer);
	}
	int numItems = 0;
	// get an array of items from the counter
	struct item *itemArray = CounterItems(counter, &numItems);
	// create an array of huffman trees from the item array
	struct huffmanTree **huffmanArray = huffmanArrayCreate(itemArray, numItems);
	// create the huffman tree from the huffman array
	struct huffmanTree *tree = createHuffmanTreeHelper(huffmanArray, numItems);
	// free the memory
	free(huffmanArray);
	free(itemArray);
	CounterFree(counter);
	FileClose(file);
	return tree;
}

// helper functions
// creates an array of huffman trees from an array of items
static struct huffmanTree **huffmanArrayCreate(struct item *itemArray, int numItems) {
	// create an array of huffman trees
	struct huffmanTree **huffmanArray = malloc(sizeof(struct huffmanTree *) * numItems);
	// for each item in the item array, create a huffman tree and add it to the huffman array
	for (int i = 0; i < numItems; ++i) {
		struct huffmanTree *tree = malloc(sizeof(struct huffmanTree));
		// set the left and right pointers to NULL
		tree->left = NULL;
		tree->right = NULL;
		tree->character = strdup(itemArray[i].character);
		tree->freq = itemArray[i].freq;
		huffmanArray[i] = tree;
	}
	return huffmanArray;
}

// creates a huffman tree from an array of huffman trees
static struct huffmanTree *createHuffmanTreeHelper(struct huffmanTree **huffmanArray, int numItems) {
	int numNodes = numItems;
	// while there is more than one node in the array
	while (numNodes > 1) {
		struct huffmanTree *lowest1 = NULL, *lowest2 = NULL;
		int lowest1Index, lowest2Index;
		int i = 0;
		// find the two lowest frequency nodes
		while (i < numItems) {
			if (huffmanArray[i] != NULL) {
				// if the lowest1 is NULL, set it to the current node
				if (lowest1 == NULL) {
					lowest1 = huffmanArray[i];
					lowest1Index = i;
				} else {
					// if the current node has a lower frequency than the lowest1, 
					// set the lowest2 to the lowest1 and the lowest1 to the current node
					// else if the current node has a lower frequency than the lowest2, 
					// set the lowest2 to the current node
					if (huffmanArray[i]->freq < lowest1->freq) {
						lowest2 = lowest1;
						lowest1 = huffmanArray[i];
						lowest2Index = lowest1Index;
						lowest1Index = i;
					} else if (lowest2 == NULL || huffmanArray[i]->freq < lowest2->freq) {
						lowest2 = huffmanArray[i];
						lowest2Index = i;
					}
				}
			}
			i++;
		}
		// create a new tree with the two lowest frequency nodes as children
		struct huffmanTree *newTree = malloc(sizeof(struct huffmanTree));
		newTree->left = lowest1;
		newTree->right = lowest2;
		newTree->character = NULL;
		newTree->freq = lowest1->freq + lowest2->freq;
		huffmanArray[lowest1Index] = newTree;
		huffmanArray[lowest2Index] = NULL;
		numNodes--;
	}
	// find the root of the huffman tree
	int i = 0;
	while (huffmanArray[i] == NULL && i < numItems) {
		i++;
	}
	struct huffmanTree *tree = huffmanArray[i];
	return tree;
}

// Task 4
// encodes a file using a huffman tree
char *encode(struct huffmanTree *tree, char *inputFilename) {
	// count the number of leaf nodes in the tree
    int numLeafNodes = 0, index = 0;
    countLeafNodes(tree, &numLeafNodes);
	// get the maximum depth of the tree
    int maxDepth = getMaxDepth(tree);
	// create an array of struct map to store the character and its encoding
    struct map *mapArray = malloc(sizeof(struct map) * numLeafNodes);
    if (mapArray == NULL) return NULL;
	// create an array of characters to store the encoding
    char *encoding = malloc((maxDepth + 1) * sizeof(char));
    if (encoding == NULL) return NULL;
    int mapLastIndex = 0;
	// create the map, and create a hashmap from the map
    createMap(tree, mapArray, 0, &mapLastIndex, encoding);
    hashMap *map = createHashMap(numLeafNodes * 2);
    for (int i = 0; i < numLeafNodes; ++i) {
        hashMapInsert(map, mapArray[i].character, mapArray[i].encoding);
    }
    File inputFile = FileOpenToRead(inputFilename);
    char buffer[MAX_CHARACTER_LEN + 1];
    int fileSize = getFileCharacterCount(inputFilename);
    if (fileSize == -1) return NULL;
	// create a string to store the encoded file
    char *encodedFile = malloc((maxDepth * fileSize + 1) * sizeof(char));
    if (encodedFile == NULL) return NULL;
	// read the input file character by character and encode it
    while (FileReadCharacter(inputFile, buffer)) {
		// lookup the character in the hashmap and add the encoding to the encoded file
        char *encoding = hashMapLookup(map, buffer);
        if (encoding != NULL) {
            for (int j = 0; encoding[j] != '\0'; j++) {
                encodedFile[index++] = encoding[j];
            }
        }
        encodedFile[index] = '\0';
    }
	// free the memory
    freeMap(mapArray, numLeafNodes);
    freeHashMap(map);
	free(encoding);
    FileClose(inputFile);
    return encodedFile;
}


// helper functions
// creates a map of characters and their encoding
static void createMap(struct huffmanTree *tree, struct map *mapArray, 
int depth, int *lastIndex, char *encoding) {
	// if the tree is a leaf node, add the character and its encoding to the map
	if (tree->left == NULL && tree->right == NULL) {	
		encoding[depth] = '\0';
		mapInsert(mapArray, tree->character, encoding, lastIndex);
	}
	// if the tree has a left child, add 0 to the encoding and recurse
	if (tree->left != NULL) {
		encoding[depth] = '0';
		createMap(tree->left, mapArray, depth + 1, lastIndex, encoding);
	}
	// if the tree has a right child, add 1 to the encoding and recurse
	if (tree->right != NULL) {
		encoding[depth] = '1';
		createMap(tree->right, mapArray, depth + 1, lastIndex, encoding);
	}
}

// gets the maximum depth of a huffman tree
static int getMaxDepth(struct huffmanTree *tree) {
	// if the tree is a leaf node, return 0
	if (tree->left == NULL && tree->right == NULL) {
		return 0;
	}
	int leftDepth = 0, rightDepth = 0;
	// if the tree has a left child, get the maximum depth of the left child
	if (tree->left != NULL) {
		leftDepth = getMaxDepth(tree->left);
	}
	// if the tree has a right child, get the maximum depth of the right child
	if (tree->right != NULL) {
		rightDepth = getMaxDepth(tree->right);
	}
	// return the maximum depth of the left and right children
	return 1 + (leftDepth > rightDepth ? leftDepth : rightDepth);
}

// inserts a character and its encoding into the map
static void mapInsert(struct map *mapArray, char *character, char *encoding, int *lastIndex) {
	mapArray[*lastIndex].character = strdup(character);
	mapArray[*lastIndex].encoding = strdup(encoding);
	// increment the index
	(*lastIndex)++;
}

// frees the map
static void freeMap(struct map *mapArray, int numLeafNodes) {
	for (int i = 0; i < numLeafNodes; ++i) {
		free(mapArray[i].character);
		free(mapArray[i].encoding);
	}
	free(mapArray);
}

// counts the number of leaf nodes in a huffman tree
static int countLeafNodes(struct huffmanTree *tree, int *numLeafNodes) {
	if (tree->left == NULL && tree->right == NULL) {
		*numLeafNodes += 1;
		return *numLeafNodes;
	}
	if (tree->left != NULL) {
		countLeafNodes(tree->left, numLeafNodes);
	}
	if (tree->right != NULL) {
		countLeafNodes(tree->right, numLeafNodes);
	}
	return *numLeafNodes;
}

// gets the number of characters in a file for estimating
// the size of the encoded file
long getFileCharacterCount(char *inputFilename) {
	// open the file to read
	File file = FileOpenToRead(inputFilename);
	if (!file || !file->fp) return -1;
	// moves the file pointer to the end of the file
	if (fseek(file->fp, 0, SEEK_END) != 0) {
		FileClose(file);
		return -1;
	}
	// gets the size of the file
	long size = ftell(file->fp);
	if (size == -1L) {
		FileClose(file);
		return -1;
	}
	FileClose(file);
	return size;
}

////////////////////////////////////////////////////////////////
// HASH MAP STUFF

// create a hash map
hashMap *createHashMap(int size) {
    hashMap *map = malloc(sizeof(hashMap));
    map->size = size;
    map->table = malloc(sizeof(hashMapEntry*) * size);
    for (int i = 0; i < size; i++) {
        map->table[i] = NULL;
    }
    return map;
}

// hash function
unsigned int hash(char *str, int size) {
    unsigned int hash = 5381;
    int c;
	// hash the string
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % size;
}

// insert into hash map
void hashMapInsert(hashMap *map, char *character, char *encoding) {
	// hash the character to get the index
    unsigned int index = hash(character, map->size);
	// create a new hash map entry
    hashMapEntry *newEntry = malloc(sizeof(hashMapEntry));
    newEntry->character = strdup(character);
    newEntry->encoding = strdup(encoding);
    newEntry->next = map->table[index];
    map->table[index] = newEntry;
}

// lookup in hash map
char *hashMapLookup(hashMap *map, char *character) {
	// hash the character to get the index
    unsigned int index = hash(character, map->size);
	// search for the character in the hash map
    hashMapEntry *entry = map->table[index];
	// return the encoding if the character is found
    while (entry != NULL) {
        if (strcmp(entry->character, character) == 0) {
            return entry->encoding;
        }
        entry = entry->next;
    }
    return NULL;
}

// free hash map
void freeHashMap(hashMap *map) {
    for (int i = 0; i < map->size; i++) {
        hashMapEntry *entry = map->table[i];
        while (entry != NULL) {
            hashMapEntry *tmp = entry;
            entry = entry->next;
            free(tmp->character);
            free(tmp->encoding);
            free(tmp);
        }
    }
    free(map->table);
    free(map);
}