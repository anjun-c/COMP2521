
#include <stdio.h>
#include <stdlib.h>

struct node {
	int value;
	struct node *next;
};

struct node *shift(struct node *list, int n);

struct node *readList(int size);
struct node *newNode(int value);
void printList(struct node *list);
void freeList(struct node *list);

int main(void) {
	printf("Enter list size: ");
	int size = 0;
	if (scanf("%d", &size) != 1) {
		fprintf(stderr, "error: failed to read list size\n");
		exit(EXIT_FAILURE);
	} else if (size < 0) {
		fprintf(stderr, "error: invalid list size\n");
		exit(EXIT_FAILURE);
	}

	if (size > 0) {
		printf("Enter list values: ");
	}

	struct node *list = readList(size);

	printf("List: ");
	printList(list);

	printf("Enter shift: ");
	int n = 0;
	if (scanf("%d", &n) != 1) {
		fprintf(stderr, "error: failed to read shift\n");
		exit(EXIT_FAILURE);
	} else if (n < 0) {
		fprintf(stderr, "error: invalid shift\n");
		exit(EXIT_FAILURE);
	}

	list = shift(list, n);
	printf("List after shifting %d time(s): ", n);
	printList(list);

	freeList(list);
	return EXIT_SUCCESS;
}

// !!! DO NOT MODIFY THE CODE ABOVE !!!
////////////////////////////////////////////////////////////////////////
// Your task

struct node *shift(struct node *list, int n) {
	// TODO
	int length = 0, i = 1;
	struct node *temp = list;
	while (temp != NULL) {
		temp = temp->next;
		length++;
	}
	if (length == 0) {
		return list;
	}
	int shift = n % length;
	temp = list;
	while (i < length - shift) {
		temp = temp->next;
		i++;
	}
	struct node *new_head = temp->next;
	temp->next = NULL;
	struct node *temp2 = new_head;
	while (temp2->next != NULL) {
		temp2 = temp2->next;
	}
	temp2->next = list;
	list = new_head;
	// int new_head = length - (n % length) + 1;
	// temp = list;
	// if (new_head != 1) {
	// 	while (i + 1 < new_head) {
	// 		temp = temp->next;
	// 		i++;
	// 	}
	// 	temp->next->next = list;
	// 	list = temp->next->next;
	// 	temp->next = NULL;
	// }
	return list;
}

////////////////////////////////////////////////////////////////////////
// !!! DO NOT MODIFY THE CODE BELOW !!!

struct node *readList(int size) {
	struct node *list = NULL;
	struct node *curr = NULL;
	for (int i = 0; i < size; i++) {
		int value = 0;
		if (scanf("%d", &value) != 1) {
			fprintf(stderr, "error: failed to read list value\n");
			exit(EXIT_FAILURE);
		}

		struct node *new = newNode(value);
		if (list == NULL) {
			list = new;
		} else {
			curr->next = new;
		}
		curr = new;
	}
	return list;
}

struct node *newNode(int value) {
	struct node *new = malloc(sizeof(struct node));
	if (new == NULL) {
		fprintf(stderr, "error: out of memory\n");
		exit(EXIT_FAILURE);
	}

	new->value = value;
	new->next = NULL;
	return new;
}

void printList(struct node *list) {
	printf("[");
	for (struct node *curr = list; curr != NULL; curr = curr->next) {
		printf("%d", curr->value);
		if (curr->next != NULL) {
			printf(", ");
		}
	}
	printf("]\n");
}

void freeList(struct node *list) {
	struct node *curr = list;
	while (curr != NULL) {
		struct node *temp = curr;
		curr = curr->next;
		free(temp);
	}
}

