// graph.c ... Graph of strings (adjacency matrix)
// Written by John Shepherd, September 2015

/*
 * added in our own functions
 * we added in vertexname,outgoing,outbound functions to allow pagerank to be completed easily
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "helper.h"
#include "graph.h"

#define strEQ(g, t) (strcmp((g),(t)) == 0)

typedef unsigned char Num;

typedef struct GraphRep {
	int nV;
	int maxV;
	char **vertex;
	Num **edges;
} GraphRep;

// Function signatures

Graph newGraph();

void disposeGraph(Graph);

int addEdge(Graph, char *, char *);

int nVertices(Graph);

int isConnected(Graph, char *, char *);

void showGraph(Graph, int);

static int vertexID(char *, char **, int);

int addVertex(char *, char **, int);

// newGraph()
// - create an initially empty Graph
Graph newGraph(int maxV) {
	Graph new = malloc(sizeof(GraphRep));
	assert(new != NULL);
	int i, j;
	new->nV = 0;
	new->maxV = maxV;
	new->vertex = malloc(maxV * sizeof(char *));
	assert(new->vertex != NULL);
	new->edges = malloc(maxV * sizeof(Num *));
	assert(new->edges != NULL);
	for (i = 0; i < maxV; i++) {
		new->vertex[i] = NULL;
		new->edges[i] = malloc(maxV * sizeof(Num));
		assert(new->edges[i] != NULL);
		for (j = 0; j < maxV; j++)
			new->edges[i][j] = 0;
	}
	return new;
}

// disposeGraph(Graph)
// - clean up memory associated with Graph
void disposeGraph(Graph g) {
	if (g == NULL) return;
	int i;
	for (i = 0; i < g->nV; i++) {
		free(g->vertex[i]);
	}
	free(g->vertex);
	for (i = 0; i < g->maxV; i++) {
		free(g->edges[i]);
	}
	free(g->edges);
}

// addEdge(Graph,Src,Dest)
// - add an edge from Src to Dest
// - returns 1 if edge successfully added
// - returns 0 if unable to add edge
//   (usually because nV exceeds maxV)
int addEdge(Graph g, char *src, char *dest) {
	assert(g != NULL);
	int v = vertexID(src, g->vertex, g->nV);
	if (v < 0) {
		if (g->nV >= g->maxV) return 0;
		v = addVertex(src, g->vertex, g->nV);
		g->nV++;
	}
	int w = vertexID(dest, g->vertex, g->nV);
	if (w < 0) {
		if (g->nV >= g->maxV) return 0;
		w = addVertex(dest, g->vertex, g->nV);
		g->nV++;
	}
	g->edges[v][w] = 1;
	return 1;
}

// isConnected(Graph,Src,Dest)
// - check whether there is an edge from Src->Dest
int isConnected(Graph g, char *src, char *dest) {
	assert(g != NULL);
	int v = vertexID(src, g->vertex, g->nV);
	int w = vertexID(dest, g->vertex, g->nV);
	if (v == w)return 0;    //ignoring self loops
	if (v < 0 || w < 0)
		return 0;
	else
		return g->edges[v][w];
}

// nVertices(Graph)
// - return # vertices currently in Graph
int nVertices(Graph g) {
	assert(g != NULL);
	return (g->nV);
}

// showGraph(Graph)
// - display Graph
void showGraph(Graph g, int mode) {
	assert(g != NULL);
	if (g->nV == 0)
		printf("Graph is empty\n");
	else {
		printf("Graph has %d vertices:\n", g->nV);
		int i, j;
		if (mode == 1) {
			for (i = 0; i < g->nV; i++) {
				for (j = 0; j < g->nV; j++)
					printf("%d", g->edges[i][j]);
				putchar('\n');
			}
		} else {
			for (i = 0; i < g->nV; i++) {
				printf("Vertex: %s\n", g->vertex[i]);
				printf("connects to\n");
				for (j = 0; j < g->nV; j++) {
					if (g->edges[i][j])
						printf("   %s\n", g->vertex[j]);
				}
			}
		}
	}
}

// Helper functions

// vertexID(Str,Names,N)
// - searches for Str in array of Names[N]
// - returns index of Str if found, -1 if not
static int vertexID(char *str, char **names, int N) {
	int i;
	for (i = 0; i < N; i++)
		if (strEQ(str, names[i])) return i;
	return -1;
}

// addVertex(Str,Names,N)
// - add Str at end of Names
int addVertex(char *str, char **names, int N) {
	names[N] = strdup(str);
	return N;
}

/*
 * OUR OWN ADDED IN FUNCTIONS HERE
 */

/*
 * this is so we can access our vertex element without needing the struct in the header file
 * takes in index of vertex and the graph name
 * returns the name of the vertex from the structure (store in g->vertex[index]))
 */
char *vertexName(Graph g, int vertexindex) {
	if (vertexindex > g->nV - 1 || vertexindex < 0)return NULL;
	else return g->vertex[vertexindex];
}

/*
 * this function allows to see how many outgoing links there are in the graph
 * takes in the graph and the vertex
 * scans through the entire graph and checks the value of g->edges[vertexindex][count]
 * if the value of the edge is >0 then add 1 to the count
 */
int outgoing(Graph g, int vertexIndex) {
	if (vertexIndex > g->nV - 1 || vertexIndex < 0)return 0;
	int counter = 0;
	for (int i = 0; i < g->nV; i++) {
		if (i == vertexIndex)continue; 				 //no self loops
		if (g->edges[vertexIndex][i] != 0)counter++; //if link add to counter 1
	}
	return counter;
}

/*
 * This function checks if a vertice is an outbound of another vertice or linked to from another verice
 * checks the edge of the first vertice with second, ORDER IS EXTREMELY IMPORTANT FOR THIS
 * returns 1 if its outbound else 0
 * a true false type function
 */

int outbound(Graph g, int i, int j) {
	if (i == j) return 0;                //NO SELF LOOPS
	if (i > nVertices(g)) return 0;      //checking if the vertices are in boundaries
	if (i < 0) return 0;
	if (g->edges[i][j]) return 1;
	return 0;
}
