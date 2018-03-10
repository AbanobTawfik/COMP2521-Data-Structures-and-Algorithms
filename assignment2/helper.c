//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>

#include "helper.h"
#include "queue.h"
#include "graph.h"
#include "BST.h"


/*
 * just a bunch of helper functionms to make this assignment easier
 * 1. opens the collection and stores each url with the .txt suffix into a queue
 * 2. stores the url from queue into graph
 * 3. stores each files keyword with url linked list in BST for inverted.c
 * 4. a normalise function which removes all trailing chars punct, and whitespaces from word
 * 5. our own strdup (cant use from string.h since std=c11)
 * 6. a function which makes a queue out of an array of strings with the nodes as each string
 */

/*
 * opens up the collection.txt of urls and stores each url into a queue
 */
Queue collecturl(void) {
	Queue urlarrayqueue;								//our array for storing urls in collection
	char *urlarray[MAXURLS] = {0};						//initialising array to hold urls
	char currenturl[MAXURLLENGTH];						//buffer for string
	int count = 0;
	FILE *fp = fopen("collection.txt", "r");			//open our collection file
	assert(fp != NULL);
	while (fscanf(fp, "%s", currenturl) != EOF) {		//scan through the collection.txt file
		urlarray[count] = strdup(currenturl);			//add each url to the array of urls
		count++;
	}
	fclose(fp);
	urlarrayqueue = urlList(urlarray);					//store this into the queue
	return urlarrayqueue;
}

/*
 * takes in an array of strings
 * stores each string in a node for our queue
 */
Queue urlList(char **array) {
	Queue UrlList = newQueue();							//create the new queue
	int i = 0;
	while (array[i] != '\0') {							//scan through entire queue
		enterQueue(UrlList, array[i]);					//add each string to queue
		i++;
	}
	return UrlList;
}

/*
 * creates an adjacency matrix graph out of our collection of urls
 * each vertex will be a url and edges will be connecting urls
 */
void addurltograph(Graph new, Queue q) {
	char *open;
	char filenosuffix[MAXURLLENGTH];
	char temp[MAXURLLENGTH] = {0};
	FILE *fp;
	while (!emptyQueue(q)) {                                                        	 //scan through the collection
		open = leaveQueue(q);
		strcpy(filenosuffix, open);
		open = realloc(open,strlen(open)+strlen(".txt")+1);                              //allocate memory for the .txt suffix
		strcat(open, ".txt");                                                            //add a .txt suffix
		fp = fopen(open, "r");                                                           //reading through the opened file
		assert(fp != NULL);
		while (strcmp(temp, "Section-1") != 0)											 //skip section-1 from the graph only add urls
			fscanf(fp, "%s",temp);
		fscanf(fp, "%s",temp);															//get the first url to start looping with

		while (strcmp(temp, "#end") != 0) {												//now we are at section 1 scan to end
			if (strstr(temp, "url") == NULL)											//make sure we are inserting a url indeed
				continue;
			addEdge(new, filenosuffix, temp);                                      		//add the edge to the vertice
			fscanf(fp, "%s", temp);
		}
		fclose(fp);
		free(open);																		//freeing all the memory
	}
}

/*
 * this function takes in a string and removes all trailling spaces/punctuation
 */
char *normalise(char *buffer){
	int i = 0;
	char *temp;
	temp = buffer;
	while(buffer[i]){
		temp[i] = tolower(buffer[i]);													//this changes the entire string to lowercase
		i++;
	}
	int j = 0;
	for(int i = 0;i<strlen(temp);i++){
		if(!ispunct(temp[i])){															//remove all punctuation from the string
			temp[j] = temp[i];
			j++;
		}else temp[j] = temp[i+1];
	}
	char *backtrail;
	backtrail = temp + strlen(temp) - 1;												//go through the very end of the string
	while(isspace(*temp))temp++;														//remove leading spaces from start
	if(*temp=='\0')return temp;
	while(backtrail>buffer && (isspace(*backtrail) || ispunct(*backtrail) || !isalnum(*backtrail)))backtrail--;
	*(backtrail+1) = '\0';																//remove all backtrail and set end string to null terminator
	return temp;
}

/*
 * making our binary search tree with our key words and a list of urls containing the words
 * inverted.c is basically this binary search tree
 */
BST addurltoBST(BST tree, Queue q){
	char *open;
	char filenosuffix[MAXURLLENGTH];
	char temp[MAXURLLENGTH] = {0};
	FILE *fp;
	while(!emptyQueue(q)){															     //scan through the collection
		open = leaveQueue(q);
		strcpy(filenosuffix, open);                                                 	 //store a version of url without .txt
		open = realloc(open,strlen(open)+strlen(".txt")+1);
		strcat(open, ".txt");                                                            //add a .txt suffix
		fp = fopen(open, "r");
		while(strcmp(temp,"Section-2")!=0)fscanf(fp,"%s",temp);						     //skip the urls we want to start at keywords for our bst
		fscanf(fp,"%s",temp);
		while(strcmp(temp,"#end")!=0){													 //now scan till the end storing keywords and the url into the bst
			tree = insertTree(tree,normalise(temp),normalise(filenosuffix));
			fscanf(fp,"%s",temp);
		}
		fclose(fp);
		free(open);
	}
	return  tree;
}

/*
 * duplicates the string and allocates memory for it, only making this because No C99
 */
char *strdup(char *input){
	char *new = malloc(sizeof(char)*(strlen(input)+1));
	strcpy(new,input);
	return new;
}
