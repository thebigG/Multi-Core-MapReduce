// FUNCTIONS FOR DOUBLES (AND SORTING INTS THIS WAY), Note - smergesort is below fmergesort

// Function fMerge - merges two subarrays of myRow.numData
// First (Left) subarray is myRow[left...middle].numData
// Second (Right) subarray is myRow[middle+1 ... right].numData

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sorter.h"

void fmerge(struct defineRow * myRow, int left, int middle, int right)
{

	int index_firstArr = 0;			// Index of first subarray
	int index_secondArr = 0; 		// Index of second subarray
	int index_mergedArr = left;		// Index of merged subarray (starts at the left)

	int length1 = middle - left + 1;	// length1 finds the length of the Left array
	int length2 = right - middle;		// length2 finds the length of the Right array

	double Left_Arr[length1];			// Initialize Left_Arr with it's length;
	double Right_Arr[length2];			// Initialize Right_Arr with it's length;

	struct defineRow * tempRowLeft = (struct defineRow *) malloc (MAXROW * sizeof(struct defineRow));
	if(tempRowLeft == NULL)
	{
		printf("Error: Cannot malloc for tempRowLeft");
		exit(1);
	}
	struct defineRow * tempRowRight = (struct defineRow *) malloc (MAXROW * sizeof(struct defineRow));
	if(tempRowRight == NULL)
	{
		printf("Error: Cannot malloc for tempRowRight");
		exit(1);
	}

	/* Copy the data from myRow to temp arrays Left_Arr/tempRowLeft[] and Right_Arr/tempRowRight[]   */
	
	for (index_firstArr = 0; index_firstArr < length1; index_firstArr++)
	{
		Left_Arr[index_firstArr] = myRow[left + index_firstArr].numData;
		tempRowLeft[index_firstArr] = myRow[left + index_firstArr];
	}

	for (index_secondArr = 0; index_secondArr < length2; index_secondArr++)
	{
		Right_Arr[index_secondArr] = myRow[middle + 1 + index_secondArr].numData;
		tempRowRight[index_secondArr] = myRow[middle + 1 + index_secondArr];
	}

	/*** Merge the temporary arrays back into myRow[left.... right]  ***/
	
	index_firstArr = 0;		// Initializing index of first subarray back to 0
	index_secondArr = 0;	 	// Initializing index of second subarray back to 0

	/*** loop while index of 1st array AND index of 2nd array have not reached the end ***/
	/*** If either Array reaches the end of the array - then the loop with stop ***/

	while (index_firstArr < length1 && index_secondArr < length2)
	{
		/*** if-else compares left vs right array, returns # that is <=, stores in myRow ***/

		if(Left_Arr[index_firstArr] <= Right_Arr[index_secondArr])
		{
			myRow[index_mergedArr] = tempRowLeft[index_firstArr];
			index_firstArr++;
		}
		else
		{
			myRow[index_mergedArr] = tempRowRight[index_secondArr];
			index_secondArr++;
		}
		index_mergedArr++;
	}

	/* Copy the remaining elements of Left_Arr[], if there are any */
	while (index_firstArr < length1)
	{
		myRow[index_mergedArr] = tempRowLeft[index_firstArr];
		index_firstArr++;
		index_mergedArr++;
	}

	/* Copy the remaining elements of Right_Arr[], if there are any */
	while (index_secondArr < length2)
	{
		myRow[index_mergedArr] = tempRowRight[index_secondArr];
		index_secondArr++;
		index_mergedArr++;
	}
	free (tempRowLeft);
	free (tempRowRight);

}


// fMerge sort calls a recurrsive algorithm to divide the arrays and sort

void fmergeSort(struct defineRow * myRow, int left, int right)
{
	if (left < right)
	{
		int middle = (left+right-1)/2;

		fmergeSort (myRow, left, middle);
		fmergeSort (myRow, middle+1, right);

		fmerge(myRow, left, middle, right);
	}
}

// **************************************************


// Function sMerge - merges two subarrays of myRow[].stringData
// First (Left) subarray is myRow[left...middle].stringData
// Second (Right) subarray is myRow[middle+1 ... right].stringData

void smerge(struct defineRow * myRow, int left, int middle, int right)
{
	int index_firstArr = 0;			// Index of first subarray
	int index_secondArr = 0; 		// Index of second subarray
	int index_mergedArr = left;		// Index of merged subarray (starts at the left)

	int length1 = middle - left + 1;	// length1 finds the length of the Left array
	int length2 = right - middle;		// length2 finds the length of the Right array

	/*** Left_Arr[] and Right_Arr[] are temporary arrays for merge ***/
	char Left_Arr[length1][MAXFIELDLEN];			// Initialize Left_Arr with it's length;
	char Right_Arr[length2][MAXFIELDLEN];			// Initialize Right_Arr with it's length;

	struct defineRow * tempRowLeft = (struct defineRow *) malloc (MAXROW * sizeof(struct defineRow));
	if(tempRowLeft == NULL)
	{
		printf("Error: Cannot malloc for tempRowLeft");
		exit(1);
	}
	struct defineRow * tempRowRight = (struct defineRow *) malloc (MAXROW * sizeof(struct defineRow));
	if(tempRowRight == NULL)
	{
		printf("Error: Cannot malloc for tempRowRight");
		exit(1);
	}
	
	int i, j;			// to loop through the row/character field of array
	for (i=0; i<length1; i++)	// initializing Arrays with '\0'
	{
		for (j=0; j<MAXFIELDLEN; j++)
			*(Left_Arr[i]+j) = '\0';
	}

	for (i=0; i<length2; i++)
	{
		for (j=0; j<MAXFIELDLEN; j++)
			*(Right_Arr[i]+j) = '\0';
	}
	
	/* Copy the data from myRow to temp arrays Left_Arr/tempRowLeft[] and Right_Arr/tempRowRight[]   */
	
	for (index_firstArr = 0; index_firstArr < length1; index_firstArr++)
	{
		for(i=0; i<MAXFIELDLEN; i++)
		{
			*(Left_Arr[index_firstArr]+i) = *(myRow[left + index_firstArr].stringData+i);
		}
		tempRowLeft[index_firstArr] = myRow[left + index_firstArr];
	}

	for (index_secondArr = 0; index_secondArr < length2; index_secondArr++)
	{
		for(i=0; i<MAXFIELDLEN; i++)
		{
			*(Right_Arr[index_secondArr]+i) = *(myRow[middle + 1 + index_secondArr].stringData+i);
		}
		tempRowRight[index_secondArr] = myRow[middle + 1 + index_secondArr];
	}


	/*** Merge the temporary arrays back into myRow[left.... right]  ***/
	
	index_firstArr = 0;		// Initializing index of first subarray back to 0
	index_secondArr = 0;	 	// Initializing index of second subarray back to 0

	/*** loop while index of 1st array AND index of 2nd array have not reached the end ***/
	/*** If either Array reaches the end of the array - then the loop with stop ***/

	while (index_firstArr < length1 && index_secondArr < length2)
	{
		/*** if-else compares left vs right array, returns # that is <=, stores in myRow ***/

		if(strcmp(Left_Arr[index_firstArr], Right_Arr[index_secondArr]) <= 0)
		{
			myRow[index_mergedArr] = tempRowLeft[index_firstArr];
			index_firstArr++;
		}
		else
		{
			myRow[index_mergedArr] = tempRowRight[index_secondArr];
			index_secondArr++;
		}
		index_mergedArr++;
	}

	/* Copy the remaining elements of Left_Arr[], if there are any */
	while (index_firstArr < length1)
	{
		myRow[index_mergedArr] = tempRowLeft[index_firstArr];
		index_firstArr++;
		index_mergedArr++;
	}

	/* Copy the remaining elements of Right_Arr[], if there are any */
	while (index_secondArr < length2)
	{
		myRow[index_mergedArr] = tempRowRight[index_secondArr];
		index_secondArr++;
		index_mergedArr++;
	}

	free (tempRowLeft);
	free (tempRowRight);
}


// sMerge sort calls a recurrsive algorithm to divide the arrays and sort

void smergeSort(struct defineRow * myRow, int left, int right)
{
	if (left < right)
	{
		int middle = (left+right-1)/2;

		smergeSort (myRow, left, middle);
		smergeSort (myRow, middle+1, right);

		smerge (myRow, left, middle, right);
	}
}
