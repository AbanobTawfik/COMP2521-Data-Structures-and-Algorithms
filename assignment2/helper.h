//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//

#include "set.h"
#include "graph.h"
#include "queue.h"
#include "BST.h"

#ifndef SEARCHENGINE_CRAWL_H
#define SEARCHENGINE_CRAWL_H

#define MAXURLS 30
#define MAXURLLENGTH 30
#define MAXLINE 1000
Queue collecturl(void);
Queue urlList(char **array);
void addurltograph(Graph new, Queue q);
char *normalise(char *s);
BST addurltoBST(BST tree, Queue q);
char *strdup(char *input);
#endif //SEARCHENGINE_CRAWL_H
