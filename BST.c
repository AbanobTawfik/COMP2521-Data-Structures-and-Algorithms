//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//

/*
 * This file was designed for use in inverted.c
 * special type of Binary Search Tree
 * the nodes identifier are words from files and inside those nodes is a linked list with all urls that contain the word
 * for this assignment in inverted.c it will open a file, store the word in tree and add the url to the linked list of that word
 * if word exists in another file will add that url to the linked list with the word
 * BST was a very very easy implementation of this
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "helper.h"
#include "BST.h"

/*
 * structure for our linked list
 */
typedef struct Data *data;
typedef struct Data {
	char *url;
	data next;
} Data;

/*
 * structure for our binary search tree
 */
typedef struct BSTree {
	char *keyword;
	data pages;
	BST left;
	BST right;
} BSTree;

/*
 * function delarations
 */
BST insertTree(BST tree, char *word, char *url);

void displayTree(BST tree, FILE *output);

void displayTreeNodes(BST tree, FILE *output);

void displayURL(data nodeurl, FILE *output);

BST newTreeLink(char *input, char *list);

void addurltoword(BST tree, data urls, char *add);

//////////////////////////////////////////////////////////////
//creates a fresh new tree
BST freshTree(void) {
	return NULL;
}

//creates a linked list for the node with the urls to add
data newNode(char *input) {
	data new = malloc(sizeof(struct Data));
	assert(new != NULL);
	new->url = strdup(input);
	new->next = NULL;
	return new;
}

//when creating a new node for our linked list we take our keyword as input and then our urls for our linked list as input
BST newTreeLink(char *input, char *list) {
	BST new = malloc(sizeof(struct BSTree));
	assert(new != NULL);
	new->keyword = strdup(input);
	new->pages = newNode(list);
	new->left = NULL;
	new->right = NULL;
	return new;
}

/*
 * adds a node to our BST keyword, it will find where the word is in the tree if it exists already and will add url to the linked list
 * otherwise it will create new tree node if it doens't exist
 */
 BST insertTree(BST tree, char *word, char *url) {
	if (tree == NULL)return newTreeLink(word, url);													//if the word isn't in the tree already create a new node
	if (strcmp(tree->keyword, word) == 0)addurltoword(tree, tree->pages, url);						//if the word exists add the url to the linked list
	if (strcmp(tree->keyword, word) > 0) tree->right = insertTree(tree->right, word, url);			//recursive calls
	if (strcmp(tree->keyword, word) < 0) tree->left = insertTree(tree->left, word, url);
	return tree;
}

/*
 * adds a url to the linked list with urls,
 * inserts alphabetically using sorted insert
 */
 void addurltoword(BST tree, data urls, char *add) {
	if (urls == NULL)tree->pages = newNode(add);
	else {
		//if url already exists return
		if (strcmp(urls->url, add) == 0)return;
		if (strcmp(urls->url, add) < 0) {
			//to preserve order if the url value is lower insert before
			data new = newNode(add);
			new->next = urls;
			tree->pages = new;
			return;
		}
		while (urls->next != NULL) {
			if (strcmp(urls->url, add) > 0 && (strcmp(urls->next->url, add) < 0)) {
				data new = newNode(add);					//insertion sort
				new->next = urls->next;
				urls->next = new;
				return;
			}
			urls = urls->next;
		}
		//adding to end if other 2 cases failed
		data new = newNode(add);
		urls->next = new;
	}

}

/*
 * frees all the memory in the tree including the memory in the linked list
 */
 void eraseList(data List) {
	if (List == NULL)return;
	eraseList(List->next);
	free(List->url);
	free(List);
}

/*
 * deletes everything inside that tree utilising the eraseList function
 * recursive delete till entire tree is freed
 */
void eraseTree(BST tree) {
	if (tree == NULL)return;
	eraseTree(tree->left);										//recursive calls to erase the left and right of he tree
	eraseTree(tree->right);
	eraseList(tree->pages);
	free(tree->keyword);										//free the memory
	free(tree);
}

/*
 * prints a keywords url list to a output file
 * printing linked list to an output file (could be stdout or a file)
 */
void displayURL(data nodeurl, FILE *output) {
	if (nodeurl == NULL)return;
	displayURL(nodeurl->next, output);							//recursively print the linked list
	fprintf(output, " %s", nodeurl->url);
}

/*
 * prints each word with their url list to a output file in ascending order
 */
void displayTreeNodes(BST tree, FILE *output) {
	if (tree == NULL)return;
	fprintf(output, "%s", tree->keyword);
	displayURL(tree->pages, output);							//display the linked list
	fprintf(output, "\n");
}

/*
 * displays the entire tree using both displays of linked list and words
 * recursive call to display every value in the tree
 */
void displayTree(BST tree, FILE *output) {
	if (tree == NULL)return;
	displayTree(tree->right, output);
	displayTreeNodes(tree, output);								//display the word -> linked list
	displayTree(tree->left, output);
}