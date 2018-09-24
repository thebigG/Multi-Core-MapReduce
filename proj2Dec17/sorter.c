#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include "sorter.h"
#include "queue.h"

//#define CAPACITY 400

struct treeNode * root = NULL;
int bstBool = 0;
int csv_count = 0;
int dirTIDCount = 0;
struct Queue* csv_queue;		//to hold the values of found CSVs
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;	//this lock will be used in the CSV Finder function to lock updating of path information
pthread_mutex_t lockTwo = PTHREAD_MUTEX_INITIALIZER;		//this lock will be used in sorter.c and CSV FInder to lock csv_count
pthread_mutex_t lockFour = PTHREAD_MUTEX_INITIALIZER;		//this lock will be used in sorter.c and CSV FInder to lock csv_count
pthread_mutex_t lockThree = PTHREAD_MUTEX_INITIALIZER;		//this lock will be used in sorter.c and CSV FInder to lock csv_count

int main(int argc, char* argv[])
{
	int commandOption = 0,		//holds the arguments coming from the command line using the getopt function
	cflag = 0,			//ensures the c argument can only be input once
	dflag = 0,			//ensures the d argument can only be input once
	oflag = 0,			//ensures the o orgument can only be input once
	err,				//to print out errors in case thread creation fails
	count = 0,			//for use in array for creating threads, will keep track of how many threads are created
	filecount = 0;		//for use in array for creating file threads, will keep track of how many threads are created
	int totTID = 1;		// total number of TIDs
	int * colInfo = (int*) malloc(3 * (sizeof(int)));			//this will store the amount of columns in the CSV that is being processed
	assert(colInfo != NULL);

	struct readcsvStruct * fileProc = (struct readcsvStruct *) malloc (MAXTHREAD * sizeof(struct readcsvStruct)); 
	assert(fileProc != NULL);

//	char* headerTitle = NULL;	//holds information about the column title being passed in from the command line

	char * headerTitle = (char*) malloc(MAXFIELDLEN*sizeof(char)); //  max # of characters in a field
	assert(headerTitle != NULL);
	
	char * searchedDirectory = (char*) malloc(MAXPATH*sizeof(char)); //4096 is max # of characters in linux path name
	assert(searchedDirectory != NULL);
	sprintf(searchedDirectory, "%c%c",'.','\0');	// inialize searched directory to '.' for default

	char * outputDirectory = (char*) malloc(MAXPATH*sizeof(char)); //4096 is max # of characters in linux path name
	assert(outputDirectory != NULL);
	sprintf(outputDirectory, "%c%c",'.','\0');	// inialize searched directory to '.' for default


//	char* searchedDirectory = ".";	//holds information about the directory end user wants to search
//	char* outputDirectory = NULL;	//holds Iinformation about the directory end user wants to use for output
	pthread_t tid;			//holds the thread id
	DIR *dirPointer;		//a pointer to the directory

	pthread_t * threads = (pthread_t*) malloc (MAXTHREAD * sizeof(pthread_t));
//	pthread_t threads[MAXTHREAD];	//to keep track of thread ID when creating threads

	pthread_t * filethreads = (pthread_t*) malloc (MAXTHREAD * sizeof(pthread_t)); 
	struct dirent *directEntry;	//a struct that helps to identify directories
	csv_queue = createQueue(MAXCSV);

	/***********************************************************
	the below below block of code will determine the arguments
	being passed in. If no options are passed in argc will be
	be 1 and an error code will be printed. If argc is equal or
	greater tehn two then on or more arguments is passed in one
	of which is required to be "C". Two other options are optional
	one is allowd to be "d" which indicates the directory that 
	will be searched for a .CSV, the other can be "o" which will
	indicate the directory to which the output will be directed
	***********************************************************/
	if(argc < 2)			//if argc is less than two then the required amount of arguments have not been entered
	{
		printf("You must enter an argument at start of application. Try -c\n");
		exit(1);
	}

	while((commandOption = getopt(argc, argv, "c:d:o:")) != -1)
	{
		switch(commandOption)
		{
			case 'c' :			//checking for the "c" argument
				if(cflag > 1)
				{
					fprintf(stderr, "You have chosen an invalid argument. Please type 'C' only one time \n");	//if c in input more than once print error and exit
					exit(1);
				}
				else
				{
					cflag++;		//when "c" is detected increase c flag which allows us to check if "c" is input more than once
					strncpy(headerTitle, optarg, (MAXFIELDLEN * sizeof(char)));    //save headerTitle to be searched
//					headerTitle = optarg;	//save the column title to be searched
					break;
				}
			case 'd' :
				if(dflag > 1)
				{
					fprintf(stderr, "You have chosen an invalid argument. Please type 'd' only one time \n");	//if d is input more than once print error and exit
					exit(1);
				}
				else
				{
					dflag++;			//when d is detected increase dflag which allows us to check is d is input more than once
					strncpy(searchedDirectory, optarg, (MAXPATH * sizeof(char)));    //save directory name to be searched
//					searchedDirectory = optarg;	//save directory name to be searched
					break;
				}
			case 'o' :
				if(oflag > 1)
				{
					fprintf(stderr, "You have chosen an invalid argument. Please type 'O' only one time\n");	//if o is input more tahn once print error and exit
					exit(1);
				}
				else
				{
					oflag++;			//when o is detected increase oflag which allows us to check if o is input more than once
					strncpy(outputDirectory, optarg, (MAXPATH * sizeof(char)));    //save directory where output will be directed
//					outputDirectory = optarg;	//save directory wehre output will be directed
					break;
				}
			case '?' :
				fprintf(stderr, "Not a valid option -%c\n", optopt);
				exit(1);
			default	 :
				fprintf(stderr, "You have chosen a valid argument please try again\n");
				exit(1);
		}
	}
		if(cflag == 0)
		{
			fprintf(stderr, "You must pass an argument in while starting an application try -c\n");
			exit(1);
		}
	/******************************************************************
 	This ends the block of code where incoming arguments are deciphered
	******************************************************************/

//printf("%s\n",searchedDirectory);
//printf("%s\n",outputDirectory);
//printf("%s\n",headerTitle);
	

	/****************************************************************
	This block of code will test the arguments searchedDirectory and 
	outputDirectory to ensure these directories exist.  If they do 
	not exist, the program will give an error and exit the program.
	This will also set the defaults to these arguments.
	*****************************************************************/
	if(oflag != 0)
	{
		if(!(dirPointer = opendir(outputDirectory)))
		{
			printf("Opening output directory %s has failed.\n",outputDirectory);
			printf("You most likely requested to save files in a directory that does not exist. Try again.\n");
			exit(1);
		}
		closedir(dirPointer);
	}

	if(dflag == 0)
	{
		strncpy(searchedDirectory,".",sizeof(char));
	}
	
	if(!(dirPointer = opendir(searchedDirectory)))
	{
		printf("Opening directory %s has failed.\n",searchedDirectory);
		printf("You most likely requested to search a directory that does not exist. Try again.\n");
		exit(1);
	}
		
	/****************************************************************
	This ends the block of code which tests the directories being
	read in as arguments.
	*****************************************************************/






	tid = pthread_self();
	fprintf(stdout, "Initial PID %lu\n", (unsigned long)tid);	
	fprintf(stdout, "TIDs of all children threads: ");
	fflush(stdout);

	/*****************************************************************
 	set dirPointer to current directory and print error message if
	opening the diretory has failed
	*****************************************************************/	

	while((directEntry = readdir(dirPointer)) != NULL)	//while the end of the directory has not been reached
	{	
		if (strchr(directEntry->d_name,'.') == directEntry->d_name)  	// if the first character is a '.', then it's the current dir, parent dir, or hidden file/dir - so ignore it
		{
			continue;
		}
		else if(directEntry->d_type == DT_DIR)
		{
//			if(!strcmp(directEntry->d_name, ".") || !strcmp(directEntry->d_name, ".."))	//if the diretory pointer is pointed at the current or parent directory skip and continue with loop
//			{
//				continue;
//			}			
			char* path = (char*) malloc(MAXPATH * sizeof(char));
			assert(path != NULL);
			memset(path, '\0', MAXPATH);
			fflush(stdout);
			snprintf(path, (sizeof(searchedDirectory) + sizeof(directEntry->d_name)), "%s/%s", searchedDirectory, directEntry->d_name);	//copy the name of the found directory
//			printf("the path found in main is %s\n", path);			
			err = pthread_create(&(*(threads+count)), NULL, &CSVFinder, (void*)path);	//create a new thread and call the CSV finder function to search other directories
			if(err != 0)
			{
				fprintf(stderr, "can't create thread");
			}
			count++;
		}
		else if(directEntry->d_type == DT_REG)		//what is being pointed at is a file
		{
			if(strrchr(directEntry->d_name, '.') == NULL)    //if file Extension is NULL this file has no file extension, ignore this file by simply continuing with loop
			{
				continue;
			}
			else if(strcmp((strrchr(directEntry->d_name, '.')), ".csv") != 0 && strcmp((strrchr(directEntry->d_name, '.')), ".CSV") != 0)   //if the found file extension does not match .csv ignore by continuing with loop
			{
				continue;
			}
			else				//the file being looked at is a .csv
			{
				char filePath[MAXPATH];		//create an array to hold the path to the file
				memset(filePath, '\0',MAXPATH);
				fflush (stdout);
				snprintf(filePath, (sizeof(searchedDirectory) + sizeof(directEntry->d_name)), "%s/%s", searchedDirectory, directEntry->d_name);	//concatonate the file name to directory name so we can process
				pthread_mutex_lock(&lock);
				enqueue(csv_queue, filePath);	//save the path to the CSV file in a queue
				++csv_count;			//increase csv_vount to keep track of how many files have been found
				pthread_mutex_unlock(&lock);
			}	
		}
	}

	int i;
	for(i = 0; i < count; ++i)
	{
		pthread_join(threads[i], NULL);
		fprintf(stdout, "%lu, ", (unsigned long)threads[i]);
		fflush(stdout);
	}
	closedir(dirPointer);
	free(searchedDirectory);
	totTID += count;

//	pthread_mutex_lock(&lockFour);
	totTID+=dirTIDCount;
//	pthread_mutex_unlock(&lockFour);


//	printf("csv_count is %d\n", csv_count);
//	pthread_mutex_lock(&lockTwo);
	char* csv_path = (char*) malloc(MAXPATH * sizeof(char));
	assert(csv_path != NULL);
	dequeue(csv_queue, csv_path);
//	printf("csv_path: %s\n", csv_path);
	struct Queue* queue = headerChecker(csv_path, headerTitle, colInfo);
//	pthread_mutex_unlock(&lockTwo);

	fileProc[0].colCount = *(colInfo + 0);
	fileProc[0].colIndex = *(colInfo + 1) - 1;
	fileProc[0].mov_tit_index = *(colInfo + 2) - 1;
	fileProc[0].csv_path = csv_path;

//	printf("colCount: %d, colIndex: %d, movTitle: %d, csvpath %s\n",fileProc[0].colCount, fileProc[0].colIndex, fileProc[0].mov_tit_index, fileProc[0].csv_path);


	err = pthread_create(&(*(filethreads+0)), NULL, &readcsv, (void*)(fileProc+0));	//create a new thread and call the CSV finder function to search other directories
	if(err != 0)
	{
		fprintf(stderr, "can't create thread");
	}

	for(i = 1; i < csv_count; i++)
	{
//		printf("Am I spawning??\n");
//		struct readcsvStruct * fileProc = (struct readcsvStruct *) malloc (sizeof(struct readcsvStruct)); 
//		assert(fileProc != NULL);
		char *csv_path = (char*) malloc(MAXPATH * sizeof(char));
		
		assert(csv_path != NULL);
		dequeue(csv_queue, csv_path);
		fileProc[i].colCount = *(colInfo + 0);
		fileProc[i].colIndex = *(colInfo + 1) - 1;
		fileProc[i].mov_tit_index = *(colInfo + 2) - 1;
		fileProc[i].csv_path = csv_path;
//		printf("colCount: %d, colIndex: %d, movTitle: %d, csvpath %s\n",fileProc[i].colCount, fileProc[i].colIndex, fileProc[i].mov_tit_index, fileProc[i].csv_path);

		err = pthread_create(&(*(filethreads+i)), NULL, &readcsv, (void*)(fileProc+i));	//create a new thread and call the CSV finder function to search other directories
		if(err != 0)
		{
			fprintf(stderr, "can't create thread");
		}
//		readcsv(csv_path);
	}
	filecount = i;
//	printf("filecount = %d\n", filecount);

	for(i = 0; i < filecount; ++i)
	{
		pthread_join(filethreads[i], NULL);
		fprintf(stdout, "%lu, ", (unsigned long)filethreads[i]);
	//	fflush(stdout);
	}
	totTID+=filecount;

	printf("\nTotal number of threads: %d\n",totTID);
	
	int z;
	char * tempHeader = (char *) malloc (200 * sizeof(char));

	FILE * newfile;
	char * newfilepath = (char*) malloc (MAXPATH * sizeof(char));
	assert (newfilepath !=NULL);
	memset(newfilepath, '\0', MAXPATH);
	newfilepath = strcat(newfilepath, outputDirectory);
	newfilepath = strcat(newfilepath, "/AllFiles-sorted-");
	newfilepath = strcat(newfilepath, headerTitle);
	newfilepath = strcat(newfilepath, ".csv");


	if ((newfile = fopen(newfilepath,"r")) != NULL)
	{
		remove(newfilepath);
	}



	newfile = fopen(newfilepath, "w+");
	for (i=0; i < (*(colInfo + 0)-1); i++)
	{
		dequeue(queue, tempHeader);
		
		if((totTID-filecount) > 1)
		{
			if(i == 0 || i == 1)
			{
				z = strlen(tempHeader);
				*(tempHeader + z-1) = '\0'; 
			}
		}
		fprintf(newfile, "%s,",tempHeader);
		memset (tempHeader, '\0', 200);
	}
	dequeue(queue, tempHeader);	
	fprintf(newfile, "%s\n", tempHeader);


	inOrder(root, newfile);

	fclose(newfile);
	free(tempHeader);
	free (newfilepath);
	free (colInfo);
	free(outputDirectory);
	free (queue);
	free(csv_queue);
	pthread_mutex_destroy(&lock);
//	pthread_mutex_destroy(&lockTwo);
//	pthread_mutex_destroy(&lockThree);
//	pthread_mutex_destroy(&lockFour);
	return (0);
}
