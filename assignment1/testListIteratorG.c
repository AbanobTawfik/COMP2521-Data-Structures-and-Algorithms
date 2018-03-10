/*
  testListIteratorG.c ... client to test Generic List Iterator
*/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "listIteratorG.h"
#include "positiveIntType.h"
#include "stringType.h"

typedef struct Node *node;
typedef struct Node {
	void *data;
	node next;
	node prev;
} Node;

typedef struct IteratorGRep {
	int size;
	node first;
	node last;
	node current;
	node returnNode;
	int cursorPosition;
	ElmFreeFp freeFunction;
	ElmNewFp newelm;
	ElmCompareFp cmp;
} IteratorGRep;


#define MAXARRAY 5

void runtest(void);

void showmenu();

void addNode(IteratorG list);

void setNode(IteratorG list);

void deleteNode(IteratorG list);

void prevNode(IteratorG list);

void nextNode(IteratorG list);

void checkNext(IteratorG list);

void checkPrevious(IteratorG list);

void findNexts(IteratorG list);

void findPreviouss(IteratorG list);

void showlist(IteratorG list);

int main(int argc, char *argv[]) {
	runtest();
	return EXIT_SUCCESS;


}

void runtest(void) {
	IteratorG list = IteratorGNew(positiveIntCompare, positiveIntNew, positiveIntFree);
	showmenu();
	char input[50];
	fgets(input, 50, stdin);
	while (input[0] != 'q') {
		switch (input[0]) {
			case 'a':
				printf("Enter element to insert\n");
				addNode(list);
				break;
			case 's':
				printf("Enter element to set\n");
				setNode(list);
				break;
			case 'd':
				deleteNode(list);
				break;
			case 'p':
				prevNode(list);
				break;
			case 'n':
				nextNode(list);
				break;
			case 'h':
				checkNext(list);
				break;
			case 'z':
				checkPrevious(list);
				break;
			case 'e':
				printf("Enter an element to find next instance of \n");
				findNexts(list);
				break;
			case 'g':
				printf("Enter an element to find previous instance of \n");
				findPreviouss(list);
				break;
			case 'i':
				showlist(list);
				break;
			case 'r':
				printf("Resetting the list\n");
				reset(list);
				break;
			case '1':
				showmenu();
				break;
		}
		showlist(list);
		fgets(input, 50, stdin);
	}
	freeIt(list);
}

void addNode(IteratorG list) {
	int input;
	scanf("%d", &input);
	int adding = add(list, &input);
	printf("Inserting %d: %s \n", input, (adding == 1 ? "Success" : "Failed"));
}

void setNode(IteratorG list) {
	int input2;
	scanf("%d", &input2);
	int setting = set(list, &input2);
	printf("setting %d: %s \n", input2, (setting == 1 ? "Success" : "Failed"));
}

void deleteNode(IteratorG list) {
	int deleting = delete(list);
	printf("%s \n", (deleting == 1 ? "Success" : "Failed"));
}

void prevNode(IteratorG list) {
	int *v1 = (int *) previous(list);
	if (v1 == NULL) {
		printf("No Previous Element\n");
	} else {
		printf("Previous value is: %d \n", *(int *) v1);
	}
}

void nextNode(IteratorG list) {
	int *v2 = (int *) next(list);
	if (v2 == NULL) {
		printf("No Next Element\n");
	} else {
		printf("Next value is: %d \n", *(int *) v2);
	}
}

void checkNext(IteratorG list) {
	int check1;
	check1 = hasNext(list);
	printf("%s \n", (check1 == 1 ? "Has a next element" : "No next element"));
}

void checkPrevious(IteratorG list) {
	int check2;
	check2 = hasPrevious(list);
	printf("%s \n", (check2 == 1 ? "Has a previous element" : "No previous element"));
}

void findNexts(IteratorG list) {
	int input3;
	scanf("%d", &input3);
	int *v3 = (int *) findNext(list, &input3);
	if (v3 == NULL) {
		printf("Failed findNext\n");
	} else {
		printf("The current node is %d\n", *(int *) v3);
	}
}

void findPreviouss(IteratorG list) {
	int input4;
	scanf("%d", &input4);
	int *v4 = (int *) findPrevious(list, &input4);
	if (v4 == NULL) {
		printf("Failed findPrevious\n");
	} else {
		printf("The current node is %d\n", *(int *) v4);
	}
}

void showlist(IteratorG list) {
	node curr = list->first;
	if (list->size == 0) {
		printf("|..");
		return;
	}
	int sizer = 1;
	int max = list->size + 1;
	while (sizer < max + 1) {
		if (sizer == list->cursorPosition) {
			printf("|..");
		}
		if (curr != NULL) {
			printf("%d...", *(int *) curr->data);
			curr = curr->next;
		}
		sizer = sizer + 1;
	}
	printf("\n");
}

void showmenu(void) {
	printf("here is your menu: \n");
	printf("a-> add a node\n");
	printf("s-> set a node\n");
	printf("d-> delete node\n");
	printf("p-> move cursor to previous node\n");
	printf("n-> move cursor to next node\n");
	printf("h-> check hasNext\n");
	printf("z-> check hasPrevious\n");
	printf("e-> move cursor to next (findNext)\n");
	printf("g-> move cursor to find previous (findPrevious)\n");
	printf("i-> showlist\n");
	printf("r-> reset list move cursor to start\n");
	printf("q-> exit program\n");
}
