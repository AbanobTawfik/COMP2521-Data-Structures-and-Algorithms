//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//
#ifndef SEARCHENGINE_BST_H
#define SEARCHENGINE_BST_H
#include <stdio.h>
typedef struct BSTree *BST;

BST freshTree(void);
void eraseTree(BST tree);
BST insertTree(BST tree,char *word,char *url);
void displayTree(BST tree, FILE *fp);

#endif //SEARCHENGINE_BST_H
