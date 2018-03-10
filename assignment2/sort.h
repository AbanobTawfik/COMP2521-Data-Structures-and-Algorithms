//
// Created by Abs Tawfik on 21/09/2017.
//

#ifndef SEARCHENGINE_SORT_H
#define SEARCHENGINE_SORT_H
void swap(void *a, void *b);
int cmp(const void *a, const void *b);
void quicksort(void *arr, int arraylength, int size, int(*cmp)(const void *x, const void *y),void(*swap)(void *a, void *b));


#endif //SEARCHENGINE_SORT_H
