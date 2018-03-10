//
// Created by Abs Tawfik on 21/09/2017.
//
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <math.h>


#include "graph.h"
#include "queue.h"
#include "sort.h"
#include "helper.h"

//data structure for our page rank add this to each node in our graph
typedef struct pageRank *PR;
typedef struct pageRank {
	double rank;
	int outbound;
	char *url;
} pageRank;

void CalculatePR(Graph graph, double damp, double diff, int maxurl);

void printToFile(Graph graph, double *array);

PR createPRNode(Graph g, double rank, int index);

void delNode(PR node);

int cmpRank(const void *a, const void *b);

int main(int argc, char *argv[]) {
	if (argc < 4)return EXIT_FAILURE;
	/*
	 * turn our program arguements into function arguements
	 */
	double damp = atof(argv[1]);
	double diffpr = atof(argv[2]);
	int maxiterations = atoi(argv[3]);
	Queue data = collecturl();                                    //create our queue with the url collection
	int numberOfUrls = queuesize(data);
	Graph newgraph = newGraph(numberOfUrls);                    //make a new graph
	addurltograph(newgraph, data);                                //add the urls to the graph vertex's
	CalculatePR(newgraph, damp, diffpr, maxiterations);            //calculate the pagerank
	disposeGraph(newgraph);
	free(newgraph);                                                //free all the memoryy
	free(data);
	return EXIT_SUCCESS;
}

void CalculatePR(Graph graph, double damp, double diffpr, int maxiterations) {
	//all initialisationsss
	int n = nVertices(graph), iterations = 0;
	double compare, sum;
	double initialrank = 1 / (double) n;
	double rank[n];
	double diff = diffpr;

	double inlinks[n];
	double outlinks[n];
	for (int i = 0; i < n; i++) {
		inlinks[i] = 0;                        //initialise our array which has the number of incoming links and outcoming links
		outlinks[i] = 0;                    //index matches graph vertice index
	}
	/*
	 * calculate the number of outgoing links at each graph vertice
	 */
	for (int i = 0; i < n; i++) {
		if (outgoing(graph, i) == 0) {            //if there are no outgoing links store 0.5 to avoid dividing by 0
			outlinks[i] = 0.5;
			continue;
		}
		outlinks[i] = outgoing(graph, i);
	}
	/*
	 * calculate the number of incoming links
	 * checks if isconnected at teach array index with each graph vertice, add to inlinks for each connection at index
	 */
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (isConnected(graph, vertexName(graph, j), vertexName(graph, i))) {
				inlinks[i]++;
			}
		}
	}
	//store each pagerank as initial 1/N
	for (int i = 0; i < n; i++)rank[i] = initialrank;

	//2d matrux fir weighted in and out
	double win[n * n];
	double wout[n * n];
	int count = 0;
	//make an array that stores the sum of win/wout for the vertice
	double winsum[n];
	double woutsum[n];
	//initialise the array for winsum and woutsum
	for (int i = 0; i < n; i++) {
		winsum[i] = 0;
		woutsum[i] = 0;
	}
	//make the array allign with the winout values
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (outbound(graph, i, j)) {
				win[count] = inlinks[j];                                //stores into Win values the inlink at index
				wout[count] = outlinks[j];                                //stores ino Wout values the outlink at index j if vertex is outbound
				count++;
				winsum[i] += inlinks[j];                                    //add to the winsum and woutsum in the index i the running some of inlinks outlinks j
				woutsum[i] += outlinks[j];
			}
		}
	}
	//now for the magic we have now the sum and the array of wins and wouts
	//2d arrays of finalWin finalWout
	double finalwin[n][n];
	double finalwout[n][n];
	//initialise the 2d array with 0's
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			finalwin[i][j] = 0;
			finalwout[i][j] = 0;
		}
	}
	count = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (outbound(graph, i, j)) {
				finalwin[i][j] = win[count] /
								 winsum[i];                //if vertex i is outbound for j, then set the 2d index as the Win for the count over sum at index i
				finalwout[i][j] = wout[count] / woutsum[i];                //same thing for Wout
				count++;
			}
		}
	}
	//NOW FOR THE REAL MAGIC
	while (iterations < maxiterations && diff >= diffpr) {
		iterations++;
		diff = 0;
		//THE STANDARD ALGORITHM FOR SUM
		//for each pagerank we update it every iteration
		for (int j = 0; j < n; j++) {
			//reset sum for each pagerank
			sum = 0;
			//keep track of our old peagrank value
			compare = rank[j];
			for (int k = 0; k < n; k++) {
				//if the two vertex's on the graph are connected
				if (isConnected(graph, vertexName(graph, k), vertexName(graph, j))) {
					//add to the sum the pagerank of the pagerank for the url at vertex k
					//multiply that with the coorect weight
					sum += (rank[k]) * finalwin[k][j] * finalwout[k][j];
				}
			}
			//the algorithm which was given
			//rank = (1-d)/n + (d*sum) -> sum from above
			rank[j] = ((1 - damp) / n) + damp * sum;
			//check the difference between the pagerank values from the updated,
			//	|PR(new)-PR(old|
			diff = diff + fabs(rank[j] - compare);
		}
	}
	//print out the pagerank values and store them in the file aswell
	printToFile(graph, rank);
}

void printToFile(Graph graph, double *array) {
	int n = nVertices(graph);
	//create a array of pagerank nodes from our struct
	PR rank[n];
	//the double *array has our pr for the nodes
	for (int i = 0; i < n; i++) {
		//create the node using
		/*
		 * graph
		 * pagerank at vertex i
		 * index i
		 */
		rank[i] = createPRNode(graph, array[i], i);
	}
	//sort the nodes for our pagerank by their pagerank
	quicksort((void *) rank, n, sizeof(pageRank *), &cmpRank, &swap);
	FILE *fp;
	fp = fopen("pagerankList.txt", "w");
	assert(fp != NULL);
	//print out the final array which is sorted by pagerank
	//write to file and print to standard output
	//free the memory for the node after we use it
	for (int i = 0; i < n; i++) {
		printf("%s, %d, %.7lf\n", rank[i]->url, rank[i]->outbound, rank[i]->rank);
		fprintf(fp, "%s, %d, %.7lf\n", rank[i]->url, rank[i]->outbound, rank[i]->rank);
		delNode(rank[i]);
	}
	fclose(fp);

}

/*
 * creating nodes for our pagerank
 * stores pagerank/url/outbound links
 */
PR createPRNode(Graph g, double rank, int index) {
	PR new = malloc(sizeof(struct pageRank));
	new->rank = rank;
	new->outbound = outgoing(g, index);
	new->url = strdup(vertexName(g, index));
	return new;
}

/*
 * comparison function for comparing the pageranks useful in our quicksort
 */
int cmpRank(const void *a, const void *b) {
	const pageRank *aa = *((pageRank **) a);
	const pageRank *bb = *((pageRank **) b);
	double aval = aa->rank;
	double bval = bb->rank;
	if (aval > bval)return -1;
	if (aval < bval)return 1;
	return 0;
}

/*
 * FREE ALL THE MEMORIES
 */
void delNode(PR node) {
	free(node->url);
	free(node);
}
