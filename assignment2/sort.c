//
// Created by Abs Tawfik on 21/09/2017.
// seen used in stackexchange will link the exact post aswell
// https://stackoverflow.com/questions/18507015/generic-quicksort
// implemented from my understanding of this post

// a generic quicksort useful helper function for sorting based on comparison function

/*
 * swaps two node positions
 */
void swap(void *a, void *b) {
	int temp = *(int *) a;
	*(int *) a = *(int *) b;
	*(int *) b = temp;
}

/*
 * this is a comparison between integers i make my own in other files
 */
int cmp(const void *a, const void *b) {
	int aa = *(const int *) a;
	int bb = *(const int *) b;
	if (aa > bb) return 1;
	if (aa < bb) return -1;
	return 0;
}

/*
 * the meat of the generic quicksort
 * it is quicksort using the middle as the pivot
 */
void quicksort(void *arr, int arraylength, int size, int(*cmp)(const void *x, const void *y),
			   void(*swap)(void *a, void *b)) {
	int pivot = arraylength / 2;
	int low = 0;
	int high = arraylength - 1;

	//base case for our sort
	if (arraylength < 2)return;
	while (high - 1 >= low) {
		//if the pivot is the low, store after the pivot
		if (low == pivot) {
			swap((char *) arr + pivot * size, (char *) arr + (pivot + 1) * size);
			pivot++;
		}
		//if the pivot is at the end store before the end
		if (high == pivot) {
			swap((char *) arr + pivot * size, (char *) arr + (pivot - 1) * size);
			pivot--;
		}
		//if the element in high is greater than the pivot, move the high index down
		if (cmp((char *) arr + high * size, (char *) arr + pivot * size) > 0) {
			high--;
			continue;
		}
		//if the element in low is greater than the pivot, move the pivot index up
		if (cmp((char *) arr + pivot * size, (char *) arr + low * size) > 0) {
			low++;
			continue;
		}
		//swap the elements in high and low
		swap((char *) arr + low * size, (char *) arr + high * size);
		low++;
		high--;

	}
	//recursively call our quicksort to sort the entire array
	quicksort(arr, low, size, cmp, swap);
	quicksort((char *) arr + low * size, arraylength - low, size, cmp, swap);
}
