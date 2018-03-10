//
// Created by Abs Tawfik Mko Galoyan on 21/09/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "helper.h"
#include "set.h"
#include "graph.h"
#include "queue.h"
#include "sort.h"

//structure for the table (Really similair to the one in the assignment spec)
typedef struct table *tab;
typedef struct table {
	char *word;
	int occurence;
} table;
//structure for tfidf holds EVERYTHING
typedef struct tfidf *ti;
typedef struct tfidf {
	char *url;
	int occurences;
	float tf;
	float id;
	int totalwords;
	tab *table;
	float tfid;
	int uniquewords;
} tfidf;

void calculate(int argc, char *argv[]);

void id(ti *array, char *argv[], int argc, int n);

void tf(ti *array, char *argv[], int argc, int n);

void printtfidf(ti *array, int occurences);

int cmpOccurences(const void *a, const void *b);

ti newNodes(char *url);

tab tablerow(char *word);

int intable(tab *table, char *word, int n);

int findindex(tab *table, char *word, int n);

void deleteArr(ti *arr, int n);

void deleteTable(tab *table, int n);

void calculatefinalscore(ti *arr, int urls);

void freetable(tab *table, int collength);

int contained(int argc, char *argv[], char *check);

int main(int argc, char *argv[]) {
	calculate(argc, argv);
	return EXIT_SUCCESS;
}

void calculate(int argc, char *argv[]) {
	Queue size = collecturl();                        //collect the url to workout the size of our tfidf array
	int maxsize = queuesize(size);
	disposeQueue(size);                                //FREE ALL THE MMEMORII
	free(size);
	ti arr[maxsize];                                //create an array of tfidf structs size = # of urls
	tf(arr, argv, argc, maxsize);
	id(arr, argv, argc, maxsize);
	calculatefinalscore(arr, maxsize);
	printtfidf(arr, maxsize);
	deleteArr(arr, maxsize);
}
//search through each url, have a struct for each url which contains
//a table ->word/count, next

void tf(ti *array, char *argv[], int argc, int n) {
	int occurences = 0;
	//step 1 open all urls in collection.txt
	Queue q = collecturl();
	FILE *fp;
	char *filenosuffix;
	int uniquewords;
	//scan through every file in the url
	while (!emptyQueue(q)) {
		//make a tfidf for each
		int count = 0;
		//a set o keep track of each word want it unique words
		Set seen = newSet();
		char *open = leaveQueue(q);
		//keep the filename without the suffix and create a tfidf struct with it
		filenosuffix = strdup(open);
		array[occurences] = newNodes(filenosuffix);
		//allocate memory for the .txt suffix xD
		open = realloc(open, strlen(open) + strlen(".txt") + 1);
		strcat(open, ".txt");                                                            //add a .txt suffix
		fp = fopen(open, "r");
		assert(fp != NULL);
		//get to the keywords already for the file
		char temp[MAXURLLENGTH];                                                        //initialise the buffer
		memset(temp, 0, sizeof(temp));
		while (strcmp(temp, "Section-2") != 0)
			fscanf(fp, "%s", temp);                    //get to section2 with the WORDS
		fscanf(fp, "%s", temp);
		Queue allwords = newQueue();                                                    //create a queue for these words
		char **allwordstemp = malloc(sizeof(char *) * (1));                                //DyNaMiCaLlY AlLoCaTe
		//make this table yooooo

		while (strcmp(temp, "#end") !=
			   0) {                                                //scan till the #end of the file
			//add 1 to the total words for this url if its a unique word;
			//keep track of seen words
			/*
			 * we are adding each word into both a unique words array no duplicates
			 * and an all words array with duplicaes
			 */
			insertInto(seen, normalise(temp));
			allwordstemp = realloc(allwordstemp, sizeof(char *) * (count + 1));
			allwordstemp[count] = strdup(normalise(temp));
			enterQueue(allwords, allwordstemp[count]);
			//add 1 to the total number of words in the file each iteration
			array[occurences]->totalwords++;
			count++;
			fscanf(fp, "%s", temp);
		}
		//free the DyNaMiCaLlY AlLoCaTeD buffer
		free(allwordstemp);
		//get the number of UNIQUE words in the file
		uniquewords = nElems(seen);
		disposeSet(seen);                                                                //FREE ALL THE MEMORIII
		free(seen);
		//store the number of unique words in the file as uniquewords
		array[occurences]->uniquewords = uniquewords;
		//now make the table
		tab table[uniquewords];
		//step1  we have array allwords,
		//make a seen queue to see what we do to occurences
		int i = 0;
		int index;
		while (!emptyQueue(allwords)) {
			char *word = leaveQueue(allwords);//
			//if word is in table add 1 to the occurence of the word
			if (intable(table, word, i)) {
				index = findindex(table, word, i);
				table[index]->occurence++;
			} else {
				table[i] = tablerow(word);
				i++;
				//or else add it to the table
			}
			free(word);                                                                //FREE ALL THE MEMORY
		}
		array[occurences]->table = table;
		disposeQueue(allwords);
		free(allwords);                                                                //FREE ALL THE MEMORY
		//now that we have our table we use that to workout TFIDF VALUE
		//our term frequency =sum for each word term frequency which is word frequency/total words
		for (int i = 1; i < argc; i++) {
			float termfrequency = 0;
			//if the input arguements from our run are in the table
			if (intable(array[occurences]->table, argv[i], array[occurences]->uniquewords)) {

				//find the index of the word in the table
				int index = findindex(array[occurences]->table, argv[i], array[occurences]->uniquewords);
				//check how many times the arguement appears in the table
				int termfreq = array[occurences]->table[index]->occurence;
				//tf is just the frequency/total words
				termfrequency = termfreq / (float) array[occurences]->totalwords;
				//store it in our tfidf struct and add 1 to number of matches
				array[occurences]->tf += termfrequency;
				array[occurences]->occurences++;
			}
		}
		freetable(table, uniquewords);                //FREE ALL THE MEMORY
		occurences++;
		free(open);
		fclose(fp);
	}
	disposeQueue(q);
	free(q);
}

/*
 * checks the "RaRiTy" of the word across all urls
 */
void id(ti *arr, char *argv[], int argc, int n) {
	//read through our file from part B
	FILE *fp = fopen("invertedIndex.txt", "r");
	assert(fp != NULL);
	char temp[MAXLINE];                                                    //BUFFER FOR STRING
	char *temp2;
	int numberofurls;
	//go through the file line by line and then calculate id if match and add it to the total id
	//CONSTANT ACCROSS ALL URLS
	while (fgets(temp, MAXLINE, (FILE *) fp)) {
		numberofurls = 0;
		//break the line down to tokens little mini words
		temp2 = strtok(temp, " \t\n");
		//if the word is a program arguement
		if (contained(argc, argv, temp2)) {
			//tokenise the urls
			temp2 = strtok(NULL, " \n");
			while (temp2 != NULL) {
				//add 1 for every url in the line, tokenise till null
				numberofurls++;
				temp2 = strtok(NULL, " \n");
			}
			//id = log(totalurls/contaisningurls)
			float id = log10((double) n / numberofurls);
			//since id is constant for all urls
			for (int i = 0; i < n; i++) {
				arr[i]->id += id;
			}
		}
	}
	fclose(fp);
}

/*
 * multiply the tf and idf value together to get tfidf
 */
void calculatefinalscore(ti *arr, int urls) {
	//for every url workout tfidf multiply tf and idf
	for (int i = 0; i < urls; i++) {
		arr[i]->tfid = arr[i]->tf * arr[i]->id;
	}
}

/*
 * make a tfidf struct node initalised for a url
 */
ti newNodes(char *url) {
	ti new = malloc(sizeof(struct tfidf));
	new->url = normalise(strdup(url));
	new->occurences = 0;
	new->id = 0;
	new->tf = 0;
	new->totalwords = 0;
	new->table = NULL;
	new->tfid = 0;
	free(url);
	return new;
}

/*
 * make a table row which has word, and a initalised number of occurences
 */
tab tablerow(char *word) {
	tab new = malloc(sizeof(struct table));
	new->word = normalise(strdup(word));
	new->occurence = 1;
	return new;
}

void printtfidf(ti *array, int occurences) {
	//sort the array by number of matches primary then tfidf
	quicksort((void *) array, occurences, sizeof(array[0]), &cmpOccurences, &swap);
	//now print the tfid values
	int count = 0;
	for (int i = 0; i < occurences; i++) {
		//if there is a keyword match
		if (count > 30)break;	//break if over 30 urls are printed
		if (array[i]->occurences > 0) {
			printf("%s  %.6lf\n", array[i]->url, array[i]->tfid);
			count++;
		}
	}
}

/*
 * comparison function, first sorting by occurences THEN by the tfidf value that is the conditional at the end of our return
 */
int cmpOccurences(const void *a, const void *b) {
	const tfidf *aa = *((tfidf **) a);
	const tfidf *bb = *((tfidf **) b);
	int aval = aa->occurences;
	int bval = bb->occurences;
	int ret = bval - aval;
	return (ret ? ret : (bb->tfid > aa->tfid));
}

/*
 * checks if a given word shows up in the table
 */
int intable(tab *table, char *word, int n) {
	for (int i = 0; i < n; i++) {
		if (strcmp(table[i]->word, word) == 0) return 1;
	}
	return 0;
}

/*
 * finds the index in the table for a given word
 */
int findindex(tab *table, char *word, int n) {
	for (int i = 0; i < n; i++) {
		//if the table index match, and the words match return the index i
		if (strcmp(table[i]->word, word) == 0)return i;
	}
	return 0;
}

/*
 * FREE ALL THE MEMORY
 */
void deleteArr(ti *arr, int n) {
	for (int i = 0; i < n; i++) {
		free(arr[i]->url);
		free(arr[i]);
	}
}
/*
 * check if the word we are comparing with is a program arguement
 */
int contained(int argc, char *argv[], char *check) {
	int count = 0;
	//loop through program arguement
	while (count < argc) {
		if (strcmp(argv[count], check) == 0) {		//if match then return true
			return 1;
		}
		count++;
	}
	return 0;										//else return false
}

/*
 * FREE ALL THE MEMORY
 */
void freetable(tab *table, int collength) {
	for (int i = 0; i < collength; i++) {
		free(table[i]->word);
		free(table[i]);
	}
}