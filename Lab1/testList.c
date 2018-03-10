// testList.c - testing DLList data type
// Written by John Shepherd, March 2013

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "DLList.h"

typedef struct DLListNode {
	char   *value;  // value of this list item (string)
	struct DLListNode *prev;
	               // pointer previous node in list
	struct DLListNode *next;
	               // pointer to next node in list
} DLListNode;

typedef struct DLListRep {
	int  nitems;      // count of items in list
	DLListNode *first; // first node in list
	DLListNode *curr;  // current node in list
	DLListNode *last;  // last node in list
} DLListRep;

void run(void);
void testInsertBeforeHead(DLList List);
DLList createList(void);
void displayList(DLList List);
void testInsertAfterHead(DLList List);
void testInsertAfterEmpty(DLList List);
void testInsertBeforeEmpty(DLList List);
void testInsertAfterEnd(DLList List);
void testInsertBeforeEnd(DLList List);
void testDeleteEmpty(DLList List);
void testDeleteHead(DLList List);
void testDeleteLast(DLList List);
void testDeleteMiddle(DLList List);
void testInsertAfterMiddle(DLList List);
void testInsertBeforeMiddle(DLList List);

int main(int argc, char *argv[]){
	run();
	return 0;
}

void run(void){

    DLList myList1 = createList();
	testInsertAfterEmpty(myList1);

	DLList myList2 = createList();	
	testInsertAfterHead(myList2);

	DLList myList3 = createList();
	testInsertAfterEnd(myList3);

	DLList myList4 = createList();
	testInsertAfterMiddle(myList4);

	DLList myList5 = createList();
	testInsertBeforeHead(myList5);

	DLList myList6 = createList();
	testInsertBeforeEnd(myList6);

	DLList myList7 = createList();
	testInsertBeforeMiddle(myList7);

	DLList myList8 = createList();
	testDeleteEmpty(myList8);

	DLList myList9 = createList();
	testDeleteHead(myList9);

	DLList myList10 = createList();
	testDeleteLast(myList10);

	DLList myList11 = createList();
	testDeleteMiddle(myList11);

}

DLList createList(void){
	printf("create a list to Test function on\n");	
	DLList myList;
	myList = getDLList(stdin);
	assert(validDLList(myList));
	return myList;
}

void displayList(DLList List){
   struct DLListNode * curr = List->first;
   while(curr != NULL){
      printf("%s\n",curr->value);
      curr = curr->next;
   }
   printf("\n");
}

void testInsertAfterEmpty(DLList List){
	printf("Test for insert after empty\n");	
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting after empty list\n");
	DLListAfter(List,"input");
	displayList(List);
	freeDLList(List);
}

void testInsertAfterHead(DLList List){
	printf("Test for insert after head\n");	
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting after head\n");
	DLListAfter(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testInsertAfterEnd(DLList List){
	printf("Test for insert after last node\n");	
	printf("moving current node to last node\n");
	DLListMove(List,List->nitems);
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting after last node\n");
	DLListAfter(List,"input");	
	displayList(List);
	freeDLList(List);	
}

void testInsertAfterMiddle(DLList List){
	printf("Test for insert after middle node\n");	
	printf("create a list to Test function on (please be over 5 characters to make this test work)\n");
	printf("moving current node to node 4\n");
	DLListMove(List,3);
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting after node 4\n");
	DLListAfter(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testInsertBeforeEmpty(DLList List){
	printf("Test for insert before empty\n");	
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting before empty list\n");
	DLListAfter(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testInsertBeforeHead(DLList List){
	printf("Test for insert before head\n");	
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting before head\n");
	DLListBefore(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testInsertBeforeEnd(DLList List){
	printf("Test for insert before the last node\n");	
	printf("moving current node to last node\n");
	DLListMove(List,List->nitems);
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting before last node\n");
	DLListBefore(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testInsertBeforeMiddle(DLList List){
	printf("Test for insert before a middle node\n");	
	printf("create a list to Test function on (please be over 5 characters to make this test work)\n");
	printf("moving current node to node 4\n");
	DLListMove(List,3);
	printf("This is your list before operation\n");
	displayList(List);
	printf("This is your list after inserting before node 4\n");
	DLListAfter(List,"input");
	displayList(List);
	freeDLList(List);	
}

void testDeleteEmpty(DLList List){
	printf("Test for deleting empty List\n");
    printf("This is your list before operation\n");	
    displayList(List);
    printf("this is your list after you delete an empty list\n");
    DLListDelete(List);
    displayList(List);
   	freeDLList(List); 
}

void testDeleteHead(DLList List){
	printf("Test for deleting head of the List\n");

    printf("This is your list before operation\n");	
    displayList(List);
    printf("this is your list after you delete the head of the list\n");
    DLListDelete(List);
    displayList(List);
   	freeDLList(List); 
}

void testDeleteLast(DLList List){
	printf("Test for deleting last node in List\n");
    printf("This is your list before operation\n");	
    displayList(List);
    printf("Moving current node to last node\n");
    DLListMove(List,List->nitems);
    printf("this is your list after you delete the end node of the list\n");
    DLListDelete(List);
    displayList(List);
   	freeDLList(List); 
}

void testDeleteMiddle(DLList List){
	printf("Test for deleting a node in the middle of the list List\n");
	printf("create a list to Test function on (please be over 5 characters to make this test work)\n");
    printf("This is your list before operation\n");	
    displayList(List);
    printf("Moving current node to node 4\n");
    DLListMove(List,3);
    printf("this is your list after you delete the node in position 3 of the list of the list\n");
    DLListDelete(List);
    displayList(List);
   	freeDLList(List); 
}
