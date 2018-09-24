#ifndef SORTER_H
#define SORTER_H

#include <pthread.h>

#define MAXCOL 30
#define MAXFIELDLEN 200
#define MAXBUF (MAXCOL * MAXFIELDLEN)
#define MAXROW 6000
#define MAXCSV 10000
#define MAXTHREAD 5000
#define MAXPATH 4100

enum varType {EMPTY, STRING, DOUBLE};	//Three Types - Empty (as a starting type), and then String & double
				        //All numbers can be sorted as a double
struct treeNode
{
	struct treeNode * left;
	struct treeNode * right;
//	double numData;
//	char stringData[MAXFIELDLEN];
//	char wholeRow[(MAXFIELDLEN+1)*MAXCOL];
	struct defineRow * row;
//	struct treeNode * parent;
};

//extern int headerBoolValue;		//bool value that will will notify if header information has already been pulled (we only want to pull one time)
//extern int colIndex;			//the index of the column that will be sorted
//extern int colCount;			//the total amount of columns that are in the CSV
extern int bstBool;				// this will check to see if a bst has been created already
extern pthread_mutex_t lock;		//create a lock to be used between in threading
extern pthread_mutex_t lockTwo;		//create a lock to be used for threading
extern pthread_mutex_t lockThree;		//create a lock to be used for threading
extern pthread_mutex_t lockFour;		//create a lock to be used for threading
extern struct Queue* csv_queue;
extern int csv_count;			//will keep track of of how many CSVs are found
extern int dirTIDCount;			//will keep track of of how many CSVs are found
extern struct treeNode *root;

struct defineRow
{
	double numData;
	char stringData[MAXFIELDLEN];
	char wholeRow[(MAXFIELDLEN+1)*MAXCOL];
};

//typedef
//struct _dir_proc
//{
//	char* path;
//	char* colName;
//} dir_proc;

struct readcsvStruct
{
	char * csv_path;
//	char csv_path[MAXPATH];
	int mov_tit_index;		//the index for the movie_title(if there is one)
	int colIndex;			//the index of the column to be sorted
	int colCount;			//the total amount of columns that will be in the CSV
};

void *CSVFinder(void *args);                              //(const char *directName, const char *columnName);
struct Queue*  headerChecker(const char *filePath, const char *columnName, int* colCount);

void *readcsv(void *args);

//Two Mergesorts - one for doubles / floats and one for strings
void fmergeSort(struct defineRow * myRow, int left, int right);
void smergeSort(struct defineRow * myRow, int left, int right);

struct treeNode * newNode (struct defineRow * myRow, int mid);
struct treeNode * sortedArrayToBST(struct defineRow * myRow, int start, int end);
struct treeNode * insert(struct defineRow * myRow, int i, enum varType coType, struct treeNode * node);
void preOrder(struct treeNode * node);
void inOrder(struct treeNode * node, FILE * file);



#endif

