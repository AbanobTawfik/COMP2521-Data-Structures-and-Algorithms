// set.h ... interface to simple Set of Strings
// Written by John Shepherd, September 2015

#ifndef SEARCHENGINE_SET_H
#define SEARCHENGINE_SET_H

typedef struct SetRep *Set;

// Function signatures

Set newSet();
void disposeSet(Set);
void insertInto(Set,char *);
void dropFrom(Set,char *);
int  isElem(Set, char*);
int  nElems(Set);
void showSet(Set);

#endif //SEARCHENGINE_SET_H
