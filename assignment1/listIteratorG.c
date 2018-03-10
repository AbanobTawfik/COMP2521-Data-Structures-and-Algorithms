/* listIteratorG.c ... Generic List Iterator implementation
     Written by Abanob Tawfik
     z5075490
     z5075490@ad.unsw.edu.au

  */


  #include <stdlib.h>
  #include <stdio.h>
  #include <assert.h>
  #include "listIteratorG.h"

  typedef struct Node *node;
  typedef struct Node {
          void *data;
          node next;
          node prev;
  } Node;

  typedef struct IteratorGRep {
          int size;
          node first;
          node last;
          //this is used to keep track of what is the current node
          node current;
          //this is used to show returns from previous and next or find previous and find next
          node returnNode;
          //this is used to keep track of the cursor
          int cursorPosition;
          //just to fix my set to work properly
          int previouscall;
          //functions given (free function was not used as i implemented a free function in my delete)
          ElmFreeFp freeFunction;
          ElmNewFp newelm;
          ElmCompareFp cmp;
  } IteratorGRep;


  IteratorG IteratorGNew(ElmCompareFp cmpFp, ElmNewFp newFp, ElmFreeFp freeFp) {
          IteratorG new;
          //allocate size for list
          new = malloc(sizeof(struct IteratorGRep));
          //next 3 lines are to assign the given functions to list
          new->freeFunction = freeFp;
          new->newelm = newFp;
          new->cmp = cmpFp;
          //size of list + cursor position = 0, and all returns are NULL
          new->size = 0;
          new->returnNode = NULL;
          new->cursorPosition = 0;
          new->previouscall = 0;
          new->first = new->last = new->current = NULL;
          return new;
  }

  //added function for creating a node using supplied function
  node newNode(void *vp, void *(*ElmNewFp)(void const *e1)) {
          assert(vp != NULL);
          //allocate size for the node
          node new = malloc(sizeof(struct Node));
          new->prev = NULL;
          new->next = NULL;
          //using given function to copy the contents in the memory of vp  into our new node data
          new->data = ElmNewFp(vp);
          return new;
  }

  int add(IteratorG it, void *vp) {
          //NOTE ALL CALLS TO ADD WILL CHANGE RETURNNODE TO NULL SO ANY CALL TO DELETE/SET AFTER ADD WILL BE INVALID
          //asserting a list exists
          assert(it != NULL);
          //asserting to make sure the input to the list is not a null input
          assert(vp != NULL);
          //creating a node given data vp to insert using given function in list
          node new = newNode(vp, it->newelm);
  // this is the case for an empty list
          if (it->size == 0) {
                  //first and last nodes are the same and = to new node
                  //make the current node the last node
                  //add 1 to list size
                  it->first = it->last = new;
                  it->current = it->last;
                  it->size++;
                  //in the case your cursor is at the start of the list (cursor position = 0) you want to put
                  //your cursor INFRONT of the new node, so add 2 to cursor position.
                  // for example |.. cursor position = 0, 2..| cursor position = 2
                  if (it->cursorPosition != 1)it->cursorPosition = it->cursorPosition + 2;
                          //however in the case that you called a delete to first node cursor position will be 1
                          //but it will be an empty list so add only 1 to cursor position to achieve same result as above
                  else it->cursorPosition++;
                  it->returnNode = NULL;
                  return 1;

                  //case if you are trying to insert before the first element i.e cursor in position 1 so for example
                  //(|..2..)->(vp..|..2..)
          } else if (hasPrevious(it) == 0) {
                  //make the new->next point to the first element in list (for our example 2)
                  new->next = it->first;
                  //make the first element previous point to the new element (so 2 points to our new node in our example)
                  it->first->prev = new;
                  //update first node in the list to be our new node (vp)
                  it->first = new;
                  //the current node is now the new node (vp)
                  it->current = new;
                  //increment size and cursor position
                  it->size++;
                  it->cursorPosition++;
                  it->returnNode = NULL;
                  return 1;
                  //case if you are trying to insert before the last element i.e cursor in position list->size+1 for example
                  //(2..|)-> (2..vp..|..)
          } else if (hasNext(it) == 0) {
                  //make new node previous point to the last node in the list
                  new->prev = it->last;
                  //make the last->next (2->next in our example) point to new node
                  it->last->next = new;
                  //update the last node in the list to be the new node
                  it->last = new;
                  //make the current node in the list the new node
                  it->current = new;
                  //increment size and cursor position
                  it->size++;
                  it->cursorPosition++;
                  it->returnNode = NULL;
                  return 1;
                  //else in the case you are inserting in the middle of list so for example
                  //(1..2..|..4)-> (1..2..vp..|..4)
          } else if (hasNext(it) == 1 && hasPrevious(it) == 1) {
                  //update current item in the list to be the one after current
                  it->current = it->current->next;
                  //the new node previous points to the current node of list
                  new->prev = it->current->prev;
                  //the new node next points to current->next;
                  new->next = it->current;
                  //make current previous next (2->next in our example) point to the new node
                  it->current->prev->next = new;
                  //make the current->next->previous point to the new node (so 4->prev)
                  it->current->prev = new;
                  //update the current node to be the new node
                  it->current = new;
                  //increment cursor + list size;
                  it->size++;
                  it->cursorPosition++;
                  it->returnNode = NULL;
                  return 1;
          } else return 0;
  }

  int hasNext(IteratorG it) {
          //asserting that this is a valid list
          assert(it != NULL);
          //this implies that the cursor is at an element AFTER list size for example
          //(1..2..3..4..|) list->cursorposition = 5 but elements in list 4
          //if the case the cursor position is infront of last element in list return 0
          //else return 1
          if (it->cursorPosition > it->size) return 0;
          else return 1;

  }

  int hasPrevious(IteratorG it) {
          //asserting that this is a valid list
          assert(it != NULL);
          //this implies that the cursor is BEHIND the first element in the list for example
          //(|..1..2..3..4..) list->cursorposition = 1
          //if the case the cursor is behind the first element of list return 0
          //else return 1
          if (it->cursorPosition > 1) return 1;
          else return 0;

  }

  //this function will advance cursor position by 1 if possible and return the value behind the cursor
  void *next(IteratorG it) {
          //asserting that the list is valid
          assert(it != NULL);
          //if it has no next element the function failed return NULL
          if (hasNext(it) == 0) return NULL;
          //in the case the cursor is right behind the last element of the list (it->cursorposition = it->size) for example
          //(1..|..2)->(1..2..|)
          if (it->cursorPosition == it->size) {
                  //you want the return node to be the last node of the list
                  it->returnNode = it->last;
                  //made the current node the last node of list
                  it->current = it->last;
                  //increment cursor position by 1
                  it->cursorPosition++;
                  return it->returnNode->data;
          }
          //in the case the cursor is behind the first element of the list for example
          //(|..1..2..)->(1..|..2..)
          if (it->cursorPosition == 1) {
                  //you want to make the return node the first node in the list
                  it->returnNode = it->first;
                  //you want to make the current node the first node in the list
                  it->current = it->first;
                  //increment cursor position by 1
                  it->cursorPosition++;
                  return it->returnNode->data;
                  //general case in the middle of the list so for example
                  //(1..2..|..3..4)->(1..2..3..|..4)
          } else {
                  //make the current node = current ->next  updating the current node
                  it->current = it->current->next;
                  //the return node will be current->next
                  it->returnNode = it->current;
                  //incremenet cursor position by 1
                  it->cursorPosition++;
                  //this is to make sure set works with both prev/next
                  it->previouscall = 0;
                  return it->returnNode->data;
          }
  }

  //this function will move back cursor position by 1 if possible and return the value infront of the cursor
  void *previous(IteratorG it) {
          //asserting that the list is valid
          assert(it != NULL);
          //if the list has no previous elements return NULL
          if (hasPrevious(it) == 0) return NULL;
          //if the cursor is INFRONT of the first element for example
          //(1..|..2..)->(|..1..2..)
          if (it->cursorPosition == 2) {
                  //the return node and current node will be the first node in the list
                  it->returnNode = it->first;
                  it->current = it->first;
                  //decrement the cursor position by 1
                  it->cursorPosition--;
                  //return the data stored in the return node
                  return it->returnNode->data;
                  //general case for example
                  //(1..2..|..3..4..)-> (1..|..2..3..4..)
          } else {
                  //the current node becomes the node behind current (curr->prev)
                  //the return node however is INFRONT of the cursor so it what is infront of the new current node
                  node curr = it->current;
                  it->current = curr->prev;
                  it->returnNode = it->current->next;
                  //decrement the cursor position by 1
                  it->cursorPosition--;
                  //this is to make sure set works with both prev/next
                  it->previouscall = 1;
                  //return the data stored in the return node
                  return it->returnNode->data;
          }
  }

  //this function deletes the value returned by either next/previous/findNext/findPrevious
  //returns 1 if successful delete or 0 otherwise
  int delete(IteratorG it) {
          //assert this is a valid list
          assert(it != NULL);
          //creating a placeholder node for freeing allocated data
          node temp;
          //if the returnNode = NULL delete failed, this means that there was not a call to next/previous/findNext/findPrevious
          //or that it was overwritten by an add
          if (it->returnNode == NULL)return 0;
          //if the list is empty delete failed
          if (it->size == 0)return 0;
                  //case if the list only has 1 element in the list
          else if (it->size == 1) {
                  //set the placeholder node as the first node
                  temp = it->first;
                  //overwrite ALL nodes to null
                  it->first = it->last = it->current = NULL;
                  //decrement list size
                  it->size--;
                  //free the allocated memory in the placeholder node
                  free(temp->data);
                  free(temp);
                  //change return node to null so u cant call multiple deletes so another call to next/previous/findNext/findPrevious
                  //will be needed
                  it->returnNode = NULL;
                  //in the case the cursor is INFRONT of the first element (it->cursorposition = 2) so for example
                  //(1..|..)-> (|..)
                  //decrement cursor position by 1 (so no bugs with add/next/prev functions
                  //other case is where you do nothing and the cursor is behind the first element so it->cursorposition = 1
                  if (it->cursorPosition != 1)it->cursorPosition--;
                  return 1;
                  //case if the return node is the first node in the list
                  //example (|..1..2..3..4..)->(|..2..3..4..)
          } else if (it->returnNode == it->first) {
                  //set placeholder node as the first node in the list
                  temp = it->first;
                  //make the pointer to previous of the node after first node (2 in this case) point to null
                  it->first->next->prev = NULL;
                  //update first element to be the element after first node (2 in this case)
                  it->first = it->first->next;
                  //new current node will be the updated first node
                  it->current = it->first;
                  //decrement list size
                  it->size--;
                  //free allocated memory to the data AND the node
                  free(temp->data);
                  free(temp);
                  //in the case the cursor is in position AFTER first element decrement by 1 or else do nothing
                  //example if the cursor is like this (1..|..2..3..) must decrement cursor size by 1 or it will
                  //mess up next/previous/findNext/findPrevious by invalid cursor position
                  if (it->cursorPosition == 2)it->cursorPosition--;
                  it->returnNode = NULL;
                  return 1;
                  //case if the return node is the last element in the list
          } else if (it->returnNode == it->last) {
                  //make placeholder node be the last node in the list
                  temp = it->last;
                  //make the pointer to the next node before the last point to null
                  it->last->prev->next = NULL;
                  //update last node in the list to be the previous node before last
                  it->last = it->last->prev;
                  //update the current node to be the updated last node in the list
                  it->current = it->last;
                  //in the case the cursor is INFRONT of the last element in the list
                  //example (1..2..3..4..|..)->(1..2..3..|..) you have to decrement the cursorposition by 1
                  //else do nothing
                  if (it->cursorPosition == it->size + 1) it->cursorPosition--;
                  //decrement list size
                  it->size--;
                  //free allocated memory in the placeholder node and the data
                  free(temp->data);
                  free(temp);
                  it->returnNode = NULL;
                  return 1;
                  //general case deleting in the middle
                  //(1..2..|..3..4..) assuming return is 3-> (1..2..|..4..)
          } else {
                  //place holder node is = return node of the list
                  temp = it->returnNode;
                  //the new current node will be the node BEHIND the return node
                  it->current = it->returnNode->prev;
                  //to delete the node inbetween you have to rearrange the links from the node behind and infront of returnnode
                  //return node previous->next has to point to next so it deletes the link inbetween for previous next
                  //return node next->previous points to the prev so it deletes the link inbetween for next previous
                  it->returnNode->prev->next = it->returnNode->next;
                  it->returnNode->next->prev = it->returnNode->prev;
                  //decrement list size by 1
                  it->size--;
                  //if the cursor is NOT infront of the first element decrement list size by 1 so example
                  //(1..|..2..3..) and return is 2, ->(1..|..3..) since the cursorposition is 2 it will not move cursor back
                  if (it->cursorPosition != 2)it->cursorPosition--;
                  //free allocated memory for the placeholder node and the data inside the node
                  free(temp->data);
                  free(temp);
                  it->returnNode = NULL;
                  return 1;
          }
  }

  //replaces the element returned by prev/next/findprev/findnext with the value vp
  //returns 1 if successful or 0 if unsuccessful
  int set(IteratorG it, void *vp) {
          //assert that this is a valid list
          assert(it != NULL);
          assert(vp != NULL);
          //if the return node is NULL i.e no call to prev/next/findprev/findnext return failed set
          if (it->returnNode == NULL)return 0;
          //create a new node containing the data provided by vp
          node new = newNode(vp, it->newelm);
          //if there is only 1 element in the list then
          if (it->size == 1) {
                  //set placeholder node as the first node in the list
                  node temp = it->first;
                  //set all nodes to the new node
                  it->first = it->last = it->current = new;
                  //set returnNode to NULL
                  it->returnNode = NULL;
                  //free the placeholder node
                  free(temp->data);
                  free(temp);
                  return 1;;
          }
          //if the returnNode is the first node in the list
          if (it->returnNode == it->first) {
                  //set placeholder node as the first node in the list
                  node temp = it->first;
                  //make the previous link for the node after the first node point to the new node
                  it->first->next->prev = new;
                  //make new next point to first->next
                  new->next = it->first->next;
                  //make the first node in the list be the new node
                  it->first = new;
                  //make the current node in the list be the new node
                  it->current = new;
                  //make the return node null
                  it->returnNode = NULL;
                  //free all allocated memory for the temporary node and the data in the node
                  free(temp->data);
                  free(temp);
                  return 1;
          }
          //if the return node is the last node in the list
          if (it->returnNode == it->last) {
                  //set the temporary node as the last node
                  node temp = it->last;
                  //make the next node of the previous from last point to the new node
                  it->last->prev->next = new;
                  //make the previous node of the new node point to last->previous
                  new->prev = it->last->prev;
                  //update the last node of the list to be the new node
                  it->last = new;
                  //update the current node of the list to be the previous node of the last node
                  it->current = new->prev;
                  //returnNode = NULL
                  it->returnNode = NULL;
                  //free allocated memory for the placeholder node and data
                  free(temp->data);
                  free(temp);
                  return 1;
                  //general case in the middle
          } else {
                  //make the placeholder node be the return node
                  node temp = it->returnNode;
                  //the new next and previous should have the same links as returnNode since you are replacing return NOde
                  //make new->next = returnnode->next
                  new->next = it->returnNode->next;
                  //make new->prev = returnnode->prev
                  new->prev = it->returnNode->prev;
                  //update the links for the node infront and behind the return node to point to the new node
                  //for the node BEHIND newnode make its next point to the new node
                  it->returnNode->prev->next = new;
                  //for the node INFRONT newnode make its prev point to the new node
                  it->returnNode->next->prev = new;
                  //make the current node of the list be the new node
                  if(it->previouscall==1) it->current = it->returnNode->prev;
                  else it->current = new;
                  //free allocated memory for the temporary node and the data inside the node
                  free(temp->data);
                  free(temp);
                  //returnnode = NULL
                  it->returnNode = NULL;
                  return 1;
          }
  }

  void *findNext(IteratorG it, void *vp) {
          assert(it != NULL);
          assert(vp!=NULL);
          //using given comparison function since you are comparing two void pointers you need to cast
          //the pointers to the data type -> *(int *) vp for integers etc
          //given function which is supplied in the create list does this for us compares 2 values inv oid pointer and returns
          //0 if equal or -1 otherwise.
          int (*ElmCompareFp)(
                          void const *e1,
                          void const *e2) = it->cmp;
          //if your current node is the last node in the list
          if (it->current == it->last) return NULL;
          //set a comparison integer for comparing vp and the current node
          int comparison;
          //cursor move counter = 1 which is used to update the cursor position iterates cursor forward by 1 at start so
          //the cursor is INFRONT of the return
          //if the cursor is behind the first node like this (|..1..2..3..4..)
          if(it->cursorPosition == 1 && ElmCompareFp(it->first->data,vp)==0){
                  //increment cursor position by 1
                  it->cursorPosition++;
                  //the current node is still the first node
                  it->current = it->first;
                  //return the data in the first node
                  return it->current->data;
          }
          int cursormovecounter = 1;
          //if the cursor is in position 1 i.e behind first element increment by an extra 1 to account for cursor
          //being BEHIND the first element
          if (it->cursorPosition == 1)cursormovecounter++;
          //make the counter node for the list start at the element AFTER the current elemenet
          node curr = it->current->next;
          //iterate through the entire list from it->current->next to it->last->next
          while (curr != NULL) {
                  //compare value of vp and the data in the current iteration and store it in comparison
                  comparison = ElmCompareFp(curr->data, vp);
                  //if there is a match
                  if (comparison == 0) {
                          //set the current node as the current iteration
                          it->current = curr;
                          //set the return node as the current iteration
                          it->returnNode = curr;
                          //move the cursor by the amount of iterations +1/2 depending on if the cursor was before first element
                          it->cursorPosition += cursormovecounter;
                          //return the data in the return node
                          //this is to make sure set works with both prev/next
                          it->previouscall = 0;
                          return it->returnNode->data;
                  }
                  //incremement counter for how much to move cursor by 1 each iteration
                  cursormovecounter++;
                  //move the counter node to the next node each iteration
                  curr = curr->next;
          }
          return NULL;
  }

  void *findPrevious(IteratorG it, void *vp) {
          assert(it != NULL);
          assert(vp != NULL);
          //using given comparison function since you are comparing two void pointers you need to cast
          //the pointers to the data type -> *(int *) vp for integers etc
          //given function which is supplied in the create list does this for us compares 2 values inv oid pointer and returns
          //0 if equal or -1 otherwise.
          int (*ElmCompareFp)(
                          void const *e1,
                          void const *e2) = it->cmp;
          //if the cursor is in position 1 i.e (|..1..2..3..4..) return null do nothing
          if (it->cursorPosition == 1) return NULL;
          //if the cursor is infront of the first element i.e (1..|..2..3..4..)
          if(it->current == it->first && it->cursorPosition == 2 && ElmCompareFp(it->first->data,vp) == 0){
                  //keep the current element as the first element however
                  it->current = it->first;
                  //return the first node but decrement the cursor by 1
                  it->returnNode = it->first;
                  it->cursorPosition--;
                  return it->returnNode->data;
          }
          //if the cursor is at the end of the list i.e (1..2..3..4..|..)
          if(it->current == it->last && ElmCompareFp(it->last->data,vp) == 0){
                  //decrement cursor by 1
                  it->cursorPosition--;
                  //make the current node BEHIND the cursor
                  it->current = it->current->prev;
                  //return the date INFRONT of the cursor because it is our return
                  return it->current->next->data;
          }
          //set a comparison integer for comparing the data in counter node and vp
          int comparison;
          //set a counter for moving cursor starting at 1, moving in backwards direction starts at 1 because it moves the cursor
          //BEHIND the return node
          int cursormovecounter = 1;
          //if the cursor is INFRONT of the last node incremenet cursor move counter by 1 to move it backwards an extra time
          //it moves the cursor BEHIND the return node
          //starts node iteration at the node BEHIND the current node
          node curr = it->current;
          //iterate through the list BACKWARDS from current->prev to first->prev
          while (curr != NULL) {
                  //compare with each iteration the data in curr and vp
                  comparison = ElmCompareFp(curr->data, vp);
                  //if there is a match
                  if (comparison == 0) {
                          //set the current node as the node in the iteration
                          if(curr == it->first) it->current = it->first;
                          else it->current = curr->prev;
                          //set the return node as the node in the iteration
                          it->returnNode = curr;
                          //DECREMENT AKA move the cursor backwards cursormoveocunter amount of times +1 depending on if the current node was
                          //the last node
                          it->cursorPosition -= cursormovecounter;
                          //this is to make sure set works with both prev/next
                          it->previouscall = 1;
                          //return the data in the returnNode
                          return it->returnNode->data;
                  }
                  //incremenet the cursormovecounter each iteration
                  cursormovecounter++;
                  //decrement the node counter each iteration ->prev
                  curr = curr->prev;
          }
          return NULL;
  }

  //move cursor to position 1
  void reset(IteratorG it) {
          assert(it != NULL);
          //change current node to be first node in the list
          it->current = it->first;
          //reset cursor position to 1
          it->cursorPosition = 1;
  }

  //empty list
  void freeIt(IteratorG it) {
          assert(it != NULL);
          //counter system using node start at FIRST
          node curr = it->first;
          //placeholder node
          node temp;
          //loop through list so while curr ! = NULL advance forward in list till u get to null which will be last->next
          while (curr != NULL) {
                  //set the placeholder node as the counter node
                  temp = curr;
                  //free the data and node for the placeholder node
                  free(temp->data);
                  free(temp);
                  //increment counter by moving forward in list by 1
                  curr = curr->next;
          }
  }
