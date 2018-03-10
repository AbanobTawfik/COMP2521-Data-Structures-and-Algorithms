// graph.h ... Interface to Graph of strings
// Written by John Shepherd, September 2015
 
#ifndef SEARCHENGINE_GRAPH_H
#define SEARCHENGINE_GRAPH_H

typedef struct GraphRep *Graph;

// Function signatures

Graph newGraph(int);
void  disposeGraph(Graph);
int   addEdge(Graph,char *,char *);
int   nVertices(Graph);
int   isConnected(Graph, char *, char *);
void  showGraph(Graph,int);
char *vertexName(Graph g,int vertexindex);
int outgoing(Graph g, int vertexIndex);
int outbound(Graph g, int i,int j);
#endif //SEARCHENGINE_GRAPH_H
