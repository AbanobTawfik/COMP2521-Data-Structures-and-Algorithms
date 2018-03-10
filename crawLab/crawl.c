// crawl.c ... build a graph of part of the web
// Written by John Shepherd, September 2015
// Uses the cURL library and functions by Vincent Sanders <vince@kyllikki.org>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <curl/curl.h>
#include "stack.h"
#include "set.h"
#include "graph.h"
#include "html.h"
#include "url_file.h"
#include "queue.h"
#include <assert.h>


#define BUFSIZE 1024

void setFirstURL(char *, char *);

void normalise(char *, char *, char *, char *, int);

int main(int argc, char **argv) {
	//URL_FILE *handle;
    char buffer[BUFSIZE];
	char baseURL[BUFSIZE];
	char firstURL[BUFSIZE];
	char next[BUFSIZE];
	int maxURLs;

	if (argc > 2) {
		strcpy(baseURL, argv[1]);
		setFirstURL(baseURL, firstURL);
		maxURLs = atoi(argv[2]);
		if (maxURLs < 40) maxURLs = 40;
	} else {
		fprintf(stderr, "Usage: %s BaseURL MaxURLs\n", argv[0]);
		exit(1);
	}
	Graph new = newGraph(maxURLs);                                   //making a new graph
	Queue List = newQueue();                                             //todo list
	Set seen = newSet();                                             //seen list
	enterQueue(List, baseURL);                                       //put the first url in the todolist
	while ((nVertices(new) != maxURLs) && !(emptyQueue(List))) {     //while the graph is not full and the list of
		char* crawl = leaveQueue(List);				//current url in the list to inspect
		if (strstr(crawl, "unsw") == NULL) {
			printf("Warning! Don't open Non UNSW Links\n");
			continue;
		}
		URL_FILE *curr_url = url_fopen(crawl, "r");                    //open the url returned from getNexturl
		while (!url_feof(curr_url)) {                           //scan through the entire fire looking for links line by line
			url_fgets(buffer, sizeof(buffer), curr_url);        //scan the line of url
			int i = 0;									//holding the lin
			while((i = GetNextURL(buffer,firstURL,next,i))>0) {
				if (nVertices(new) != maxURLs || isConnected(new, next, crawl)) addEdge(new, crawl, next);
				if (!isElem(seen, next)) {
						enterQueue(List, next);
						insertInto(seen, next);
					
				}
			}
		}
			url_fclose(curr_url);
			free(crawl);
			sleep(1);	
		}
		// You need to modify the code below to implement:
		//
		// add firstURL to the ToDo list
		// initialise Graph to hold up to maxURLs
		// initialise set of Seen URLs
		// while (ToDo list not empty and Graph not filled) {
		//    grab Next URL from ToDo list
		//    if (not allowed) continue
		//    foreach line in the opened URL {
		//       foreach URL on that line {
		//          if (Graph not filled or both URLs in Graph)
		//             add an edge from Next to this URL
		//          if (this URL not Seen already) {
		//             add it to the Seen set
		//             add it to the ToDo list
		//          }
		//       }
		//    }
		//    close the opened URL
		//    sleep(1)
		// }


		printf("SHOWING GRAPH: \n");
		showGraph(new,0);
		/*if (!(handle = url_fopen(firstURL, "r"))) {
			fprintf(stderr, "Couldn't open %s\n", next);
			exit(1);
		}*/
		/*while (!url_feof(handle)) {
			url_fgets(buffer, sizeof(buffer), handle);
			//fputs(buffer,stdout);
			int pos = 0;
			char result[BUFSIZE];
			memset(result, 0, BUFSIZE);
			while ((pos = GetNextURL(buffer, firstURL, result, pos)) > 0) {
				printf("Found: '%s'\n", result);
				memset(result, 0, BUFSIZE);
			}
		}
		url_fclose(handle);
		sleep(1);
		return 0;*/
	disposeQueue(List);
	disposeSet(seen);
	disposeGraph(new);
	return EXIT_SUCCESS;
}
// setFirstURL(Base,First)
// - sets a "normalised" version of Base as First
// - modifies Base to a "normalised" version of itself
	void setFirstURL(char *base, char *first) {
		char *c;
		if ((c = strstr(base, "/index.html")) != NULL) {
			strcpy(first, base);
			*c = '\0';
		} else if (base[strlen(base) - 1] == '/') {
			strcpy(first, base);
			strcat(first, "index.html");
			base[strlen(base) - 1] = '\0';
		} else {
			strcpy(first, base);
			strcat(first, "/index.html");
		}
	}
