//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "helper.h"
#include "graph.h"
#include "queue.h"
#include "sort.h"
#include "BST.h"

int main(void){
	BST new = freshTree();									//create a new tree
	Queue q = collecturl();									//get the collection into a queue
	new = addurltoBST(new,q);								//use the helper.c funcion to create our binary search tree with linked list + words
	FILE *fp = fopen("invertedIndex.txt","w");				//create our output file to write too
	displayTree(new,fp);									//this will display the tree in alphabetical order will write to the file in above line
	displayTree(new,stdout);								//his will display the tree in alphabetical order will write to standard output
	eraseTree(new);											//free all the memory
	free(q);
	fclose(fp);
	return EXIT_SUCCESS;
}