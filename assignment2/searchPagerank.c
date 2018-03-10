//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "set.h"
#include "graph.h"
#include "sort.h"
#include "helper.h"
#include "queue.h"

//structure which has the url,matching searchwords in url and the pagerank of hat page
typedef struct occurences *occ;
typedef struct occurences {
	char *url;
	int occurence;
	float PR;
} occurences;

void Search(int argc, char *argv[]);

occ makeNode(char *url);

int contained(int argc, char *argv[], char *check);

void deletelist(occ *urls, int nodeletes);

void printURLS(occ *urList, int numberofurls);

int hasoccured(occ *urls, char *cmp, int size);

int cmpOccurences(const void *a, const void *b);

void addtooccurence(occ *urls, char *add, int size);

int main(int argc, char *argv[]) {
	Search(argc, argv);
	return EXIT_SUCCESS;
}

void Search(int argc, char *argv[]) {
	if (argc < 2) {
		printf("Enter search terms plz\n");
		return;
	}
	//scanning through the keywords from inverted.c and grabbing urls which contain keywords
	FILE *fp = fopen("invertedIndex.txt", "r");
	assert(fp != NULL);
	char temp[MAXLINE];
	char *temp2;
	int numberofnodes = 0;
	//struct of occurences with both the url + number of occurences
	//make function is in occurences
	Queue maxsize = collecturl();
	int n = queuesize(maxsize);								//Collect urls and workout size of that queue to count number of urls in collection
	disposeQueue(maxsize);									//FREE ALL THE MEMORY
	free(maxsize);
	occ urlist[n];
	//seen on stackexchange nice function that reads file till end of line, returns -1 at end of file
	while (fgets(temp,MAXLINE,(FILE *)fp)) {
		//splitting the string since u cant access indexs
		temp2 = strtok(temp, " \t\n");
		//if the word is a key word or used in search
		if (contained(argc, argv, temp2)) {
			//break the string down into tokens with urls
			temp2 = strtok(NULL, " \n");
			while (temp2 != NULL) {
				//tokenize till null
				//if the url hasnt occured already insert it into the struct
				if (!hasoccured(urlist, temp2, numberofnodes)) {
					urlist[numberofnodes] = makeNode(temp2);
					numberofnodes++;
				} else {
					//add 1 to the number of occurences for the url
					addtooccurence(urlist, temp2, numberofnodes);
				}
				temp2 = strtok(NULL, " \n");
			}
		}
	}
	fclose(fp);
	printURLS(urlist, numberofnodes);
}

/*
 * create a structure for our url
 * initialise it with values
 */
occ makeNode(char *url) {
	occ new = malloc(sizeof(struct occurences));
	new->occurence = 1;
	new->url = strdup(url);
	new->PR = 0;
	return new;
}

/*
 * similair to use in tfidf (should of probably put it in helper.c)
 * checks if a given word is a program arguement
 * true/false return system
 */

int contained(int argc, char *argv[], char *check) {
	int count = 0;
	//loop through all program arguements
	while (count < argc) {
		if (strcmp(argv[count], check) == 0) {	//if match return 1 aka true
			return 1;
		}
		count++;
	}
	return 0;									//else return 0 aka false
}

/*
 * comparison function which will first sor by number of occurences, and then by pagerank
 * the return conditional is the sort by pagerank 2ndary sort
 */
int cmpOccurences(const void *a, const void *b) {
	const occurences *aa = *((occurences **) a);
	const occurences *bb = *((occurences **) b);
	int aval = aa->occurence;
	int bval = bb->occurence;
	int ret = bval-aval;
	return(ret ? ret: (bb->PR > aa->PR));
}

void printURLS(occ *urList, int numberofurls) {
	FILE *fp = fopen("pagerankList.txt", "r");
	assert(fp != NULL);
	char temp[MAXLINE];
	occ finalarray[numberofurls];
	int count = 0;
	float pagerank;
	//sort by pagerank first, then do another sort with keyword occurence
	while (fgets(temp,MAXLINE,(FILE *)fp)) {
		if (count > 30)break; //if more than 30 urls break
		char *temp2;
		temp2 = strtok(temp, " ,\n");
		// check if url is in the occurence
		for (int i = 0; i < numberofurls; i++) {
			//scan till you find the corresponding url in list
			if (strcmp(temp2, urList[i]->url) == 0) {
				//store pagerank value into the struct or final arraycount
					char *placeholder = temp2;
					while(placeholder!=NULL){
					//since pagerank always last papa bless we can do this
						pagerank = atof(placeholder);
						placeholder = strtok(NULL,", ");
					}
				free(placeholder);
				//store the struct into our final struct
				finalarray[count] = urList[i];
				finalarray[count]->PR = pagerank;
				count++;
			}
		}
	}
	fclose(fp);

	//now do a quicksort to sort the ocurence array by occurences so its finally sorted
	quicksort((void *) finalarray, count, sizeof(finalarray[0]), &cmpOccurences, &swap);

	//now print the final urls;
	for (int i = 0; i < count; i++) {
		printf("%s\n", finalarray[i]->url);
	}
	deletelist(urList,numberofurls);
}

/*
 * checks if a url had already occurred
 */
int hasoccured(occ *urls, char *cmp, int size) {
	if (size == 0)return 0;
	for (int i = 0; i < size; i++) {
		if (strcmp(urls[i]->url, cmp) == 0)return 1;
	}
	return 0;
}

/*
 * adds 1 to occurence if the url is already occurred
 */
void addtooccurence(occ *urls, char *add, int size) {
	if (size == 0)return;
	for (int i = 0; i < size; i++) {
		if (strcmp(urls[i]->url, add) == 0) {
			urls[i]->occurence++;
			return;
		}
	}
	return;
}
/*
 * FREE ALL MEMORY
 */
void deletelist(occ *urls,int nodeletes) {
	for(int i = 0;i<nodeletes;i++){
		free(urls[i]->url);
		free(urls[i]);
	}
}
