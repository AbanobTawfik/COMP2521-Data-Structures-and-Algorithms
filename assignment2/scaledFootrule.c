//
// Created by Abs Tawfik on 21/09/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "helper.h"
#include "queue.h"
//time to make a smart ass algorithm

/*  MAKE IT SMART
 * step 1 - check conditions,
 * condition 1. no shared elements between sets,
 * condition 2.check position of url in both ranks, if its the same in document 1 as document 2, no need to change it.
 */

//make a struct for our final SFR with the urls and the value
typedef struct scale *SF;
typedef struct scale {
	char **arr;
	double SFR;
} scale;

//make a struct for our locked in array values
typedef struct prison *lock;
typedef struct prison {
	char *url;
	int position;
} prison;

int numofurlstore(char *file, char ***store);

void run(int argc, char *argv[]);

char ***resultarrays(int argc, char *argv[]);

void printSFR(char ***array, char **allwords, int length, int totalwords, int argc, int nurls[]);

double calculateSFR(char ***array, int length, int argc, int nurls[], char **final);

int findpos(char ***array, int column, int length, char *word);

SF makeSFSTRUCT(int length);

void
findpermutation(char **allwords, int i, int length, char ***array, int argc, int nurls[], lock *lockedvals, SF storage);

lock createCell(int position, char *url);

int inarray(char ***array, int column, int length, char *word);

int findindex(char **array, int length, char *word);

void lockarray(int length, lock *lockedvals, char **allwords);

int findpos2(char **array, int length, char *word);

void freethepeople(SF SFR, lock *prisoners, int n);

int main(int argc, char *argv[]) {
	run(argc, argv);
	return EXIT_SUCCESS;
}

void run(int argc, char *argv[]) {
	if (argc < 2)return;
	char **array[argc - 1];                             //big boy storing whats inside the file into an array index
	int nurls[argc - 1];                               //store the number of urls in file to a url index
	for (int i = 0; i < argc - 1; i++)
		nurls[i] = 0;
	//go through the files and store
	Queue unionset = newQueue();                             //index to get the UNION i.E add with no repetition of both sets
	for (int i = 1; i < argc; i++) {
		//gonna read through the files
		char *file = strdup(argv[i]);
		nurls[i] = numofurlstore(file, &array[i - 1]);
		free(file);
		//store the urls from the array matrix into a queue with no duplicatess
		for (int j = 0; j < nurls[i]; j++) {
			if (!inQueue(unionset, array[i - 1][j])) {
				enterQueue(unionset, array[i - 1][j]);
			}
		}
	}
	//number of unique urls is the size of our unique queue
	int unique = queuesize(unionset);
	int allwords = 0;
	for (int i = 1; i < argc; i++) allwords += nurls[i];
	//make an array which can hold our unique urls all of them
	char **urls = malloc(sizeof(char *) * queuesize(unionset));
	//make a string array from our queue
	urls = list(unionset, urls);
	printSFR(array, urls, unique, allwords, argc, nurls);
	//FREE ALL THE MEMORY
	free(urls);
	for (int i = 0; i < argc - 1; i++) {
		for (int j = 0; j < nurls[i]; j++) {
			free(array[i][j]);
		}
		free(array[i]);
	}
	disposeQueue(unionset);
	free(unionset);
}

//kill 2 birds with 1 stone do a store n save
//reads through stores into the arr the url maintaining position and through the file and returns number of urls
int numofurlstore(char *name, char ***store) {
	FILE *fp = fopen(name, "r");
	assert(fp != NULL);
	int numberofurls = 0;
	char *url = malloc(sizeof(char) * MAXURLLENGTH);
	//let this be our "string" we want to store into our input store
	char **words = malloc(sizeof(char *) * 1);
	//scan through the file storing into our 2d string array the file, we are literally storing the file into a 2d array
	//each column of this array is a file of ranking
	while (fscanf(fp, "%s", url) != EOF) {
		int stringlength = strlen(url);
		words = realloc(words, sizeof(char *) * (numberofurls + 1));
		words[numberofurls] = malloc(stringlength + 1);
		strcpy(words[numberofurls], url);
		numberofurls++;
	}
	fclose(fp);
	free(url);
	//set the pointer to the input of our matrix as the column we just made of string
	words = realloc(words, sizeof(char *) * (numberofurls + 1));
	words[numberofurls] = NULL;

	*store = words;
	return numberofurls;
}

//given the array containing both pages, unique urls compute the
void printSFR(char ***array, char **allwords, int length, int totalwords, int argc, int nurls[]) {
	int max = 0;
	for (int i = 0; i < argc - 1; i++) {
		if (nurls[i] > max) max = nurls[i];
	}
	//if all the elements in every file is non unique i.e unique = total then the order would be
	/*
	 * rank 1 of file 1, rank 1 of file 2.... rank 1 of file n, etc all the way up to rank n of file n
	 * cut the need out for working out permutations SMARTER ALGORITHM and instantly get answer eyy lmao
	 */
	//calculate SFR
	if (length == totalwords) {
		//workout which array has the maximum length
		//store the final into the array we calculate sfr for
		int count = 0;
		char **final = malloc(sizeof(char *) * length);
		for (int i = 0; i < max; i++) {
			for (int j = 0; j < argc - 1; j++) {
				if (array[j][i] == NULL)continue;
				else {
					final[count] = array[j][i];
					count++;
				}
			}
		}
		//calculate SFR
		double SFR = calculateSFR(array, length, argc, nurls, final);
		//print the scaled foot rule
		printf("%.6lf\n", SFR);
		int i;
		//now print the bad boii
		for (i = 0; i < length; i++) printf("%s\n", final[i]);
		for (int i = 1; i < argc; i++)free(array[i - 1]);
		return;
	}

	/*
	 * for all other cases want to make permutations (will make the algo smarter tho hehe
	 */

	lock prisoncell[length];
	int count = 0;
	int j, i;
	int check = 0;
	for (i = 0; i < length; i++) {
		for (j = 0; j < argc - 2; j++) {
			check = 0;
			//if an entire row is equal i.e same rank then store it in the prison cell
			if (array[j][i] == NULL || array[j + 1][i] == NULL) {
				check = 1;
				continue;
			} else {
				//if 1 row doesnt match set the check to be FALSE and break;
				if (strcmp(array[j][i], array[j + 1][i]) != 0) {
					check = 1;
					break;
				}
			}
		}
		//now
		//if the condition was met store it into the lock with the position
		if (check == 0) {
			prisoncell[count] = createCell(i, array[j][i]);
			count++;
		}
	}
	prisoncell[count] = NULL;
	SF scaledfootrule = makeSFSTRUCT(length);
	findpermutation(allwords, 0, length, array, argc, nurls, prisoncell, scaledfootrule);
	printf("%.6lf\n", scaledfootrule->SFR);
	for (int i = 0; i < length; i++)printf("%s\n", scaledfootrule->arr[i]);
	freethepeople(scaledfootrule, prisoncell, count);
	//omg we actually finished
}

//if elements are in the same position store them into a struct locked which has both array and position
//check for these by going through all words and if on a row they match store it into the lock
/*
 * find the permutation with the LOWEST SFR
 * if an element is the same rank accross all pages, then keep it in place and just make permutaitons of others
 * reduces our input from n! -> (n-a)! where a = number of locked values
 * SMART YO
 * return the final array with the SFR
 */
void findpermutation(char **allwords, int i, int length, char ***array, int argc, int nurls[], lock *lockedvals,
					 SF storage) {
	double SFR = calculateSFR(array, length, argc, nurls, allwords);
	//updates the scaled foot rule to be the MINIMUM scaled foot rule
	if (SFR < storage->SFR) {
		storage->SFR = SFR;
		for (int i = 0; i < length; i++) {
			storage->arr[i] = allwords[i];
		}
	}

	//base case for our recursion
	if (length == i) {
		return;
	}
	//make a random permutation of the original array
	int k;
	for (k = i; k < length - 1; k++) {
		char *temp = allwords[k];
		allwords[k] = allwords[k + 1];
		allwords[k] = temp;
		//always locking to make sure less permutations to go through
		lockarray(length, lockedvals, allwords);
		findpermutation(allwords, i + 1, length, array, argc, nurls, lockedvals, storage);
		temp = allwords[k];
		allwords[k] = allwords[i];
		allwords[i] = temp;
		lockarray(length, lockedvals, allwords);
	}
}

/*
 * lock the array i.e keeping locked positions in place
 * allows us to find less permutations because we find permutations AROUND the locked values
 */
void lockarray(int length, lock *lockedvals, char **allwords) {
	if (lockedvals[0] == NULL)return;
	int j = 0;
	//while there are locked values
	while (lockedvals[j] != NULL) {
		//get the index of the locked url
		int index = lockedvals[j]->position;
		//find the index which the locked url is at
		int index2 = findindex(allwords, length, allwords[index]);
		//swap them around
		char *temp = allwords[index2];
		allwords[index] = lockedvals[j]->url;
		allwords[index2] = temp;
		j++;
	}
}

SF makeSFSTRUCT(int length) {
	SF new = malloc(sizeof(struct scale));
	//make it a rediculous value initialising to 0 will just meme me since it will never update unless negative SFR
	new->SFR = 50000;
	new->arr = malloc(sizeof(char *) * length);
	return new;
}

double calculateSFR(char ***array, int length, int argc, int nurls[], char **final) {
	double retval = 0;
	for (int i = 0; i < length; i++) {
		for (int j = 0; j < argc - 1; j++) {
			/*
			 * find the position in the original array
			 * find the position in the final array output
			 * divide both by respective length of arrays
			 * add abs of the difference to the final
			 */
			if (!inarray(array, j, nurls[j + 1], final[i]))continue;
			int pos = findpos(array, j, nurls[j + 1], final[i]);
			int temppos = findpos2(final, nurls[j + 1], final[i]);
			if (pos == 0)continue;
			/*
			 * now do the calculation as given in the spreadsheet
			 */
			double pos1 = (double) pos / (double) nurls[j + 1];
			double pos2 = (double) temppos / (double) length;
			double addtosum = fabs(pos1 - pos2);
			retval += addtosum;
		}
	}
	return retval;
}

/*
 * find position of the url in the original position with its rank file
 * scans through through the 2d array column and finds where the url is
 */
int findpos(char ***array, int column, int length, char *word) {
	for (int i = 0; i < length; i++) {
		if (array[column][i] == NULL) return 0;
		if (strcmp(array[column][i], word) == 0)return i + 1;
	}
	return 0;
}

/*
 * similair to above except with 1d array
 */
int findpos2(char **array, int length, char *word) {
	for (int i = 0; i < length; i++) {
		if (array[i] == NULL) return 0;
		if (strcmp(array[i], word) == 0)return i + 1;
	}
	return 0;
}

/*
 * checks if a given word is in the array column
 * true false type system
 */
int inarray(char ***array, int column, int length, char *word) {
	for (int i = 0; i < length; i++) {
		if (strcmp(array[column][i], word) == 0)return 1;    //scan through column if match return 1/true
	}
	return 0;                                                //else return 0/false
}

/*
 * locks the url in given position
 * "creating a prison cell for prisoner url"
 */
lock createCell(int position, char *url) {
	lock new = malloc(sizeof(struct prison));
	new->url = url;
	new->position = position;
	return new;
}

/*
 * find the index in which the word is located in
 */
int findindex(char **array, int length, char *word) {
	for (int i = 0; i < length; i++) {
		if (strcmp(array[i], word) == 0)return i;
	}
	return 0;
}

/*
 * FREE ALL THE MEMORY
 */
void freethepeople(SF SFR, lock *prisoners, int n) {
	int i = 0;
	while (i < n) {
		free(prisoners[i]->url);
		free(prisoners[i]);
		i++;
	}
	free(SFR->arr);
	free(SFR);
}
