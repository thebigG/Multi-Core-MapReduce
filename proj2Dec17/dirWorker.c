#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>
#include <getopt.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <assert.h>
#include <pthread.h>
#include "sorter.h"
#include "queue.h"


/*
functions to work on file path and process diretory information
*/

/************************************************************
CSV Finder will search through a directory to find all 
instances of subdirectories and files.  Upon finding a 
subdirectory it will enter that sub directory (through a
recursive call) and search that sub direcory.  Upon finding
a file it will check if it is a CSV.  If it is not a CSV
it will ignore it.  If it is a CSV it will pass the path
of the directory onto a new functino to process the file
************************************************************/
void *CSVFinder(void *args)                                             //(const char *directName, const char *columnName)
{	
	char* dir_path = (char*)args;
	DIR *dirPointer;                           //a pointer to a directory type which will be used to open directory
	struct dirent *directEntry;                //struct to store information about the directory we are working with
	char *fileExtension;                       //this variable wil be used to check the file etension passed in is a .CSV
	int i = 0,				   //for loops
	err, count = 0;				   //keeps track of the threads created

        pthread_t * threads = (pthread_t*) malloc (MAXTHREAD * sizeof(pthread_t));


	//strncpy(path, funcArgs->path, sizeof(path)); //the proper contents will be stored in path
	
//	printf("CSVFinder has been entered searching %s\n", dir_path);	

	if(!(dirPointer = opendir(dir_path)))    //open diretory and print error if failure occurs
	{
		printf("opening of directory has failed.  You have most likely requested to search a directory that does not exist. Try again\n");
		exit(1);
	}
		
	/***************************************************************
 	This block of code will search directories passed into this 
	function recursively. Upon finding a directory the process is
	forked so the parent directory will search an entire directory
	while the child processes will search subdiretories. When a 
	CSV file is found the path to the file is passed to a subsequent
	function for further processing.
	**************************************************************/
	while((directEntry = readdir(dirPointer)) != NULL)
	{
		if(strchr(directEntry->d_name, '.') == directEntry->d_name)
		{
			continue;
		}
		else if(directEntry->d_type == DT_DIR)        //if the pointer is pointing to a directory do this
		{
			if(!strcmp(directEntry->d_name, ".") || !strcmp(directEntry->d_name, ".."))     //if the found directory is current directory or parent directory skip this iteration of loop
			{
				continue;
			}
			char * path = (char*) malloc (MAXPATH * sizeof(char));
			assert(path != NULL);
			memset(path, '\0', MAXPATH);
			snprintf(path, (sizeof(path) + sizeof(directEntry->d_name)), "%s/%s", dir_path, directEntry->d_name);  //save the found sub directory to buffer


                        err = pthread_create(&(*(threads+count)), NULL, &CSVFinder, (void*)path);	//create a new thread and call the CSV finder function to search other directories
                        if(err != 0)
                        {
                                fprintf(stderr, "can't create thread");
                        }

			pthread_mutex_lock(&lockFour);
			dirTIDCount += 1;
			pthread_mutex_unlock(&lockFour);

	                pthread_join(threads[count], NULL);
	                fprintf(stdout, "%lu, ", (unsigned long)threads[i]);
	                fflush(stdout);

                        count++;
		
//			CSVFinder((void*)path);	//recursively call this function	
		}
		else if(directEntry->d_type == DT_REG)          //if the pointer is pointing to a file
		{
			if(strrchr(directEntry->d_name, '.') == NULL)    // find the file extension and save to a variable
//			if(fileExtension == NULL)        //if file Extension is NULL this file has no file extension, ignore this file by simply continuing with loop
			{
				continue;
			}
			else if(strcmp((strrchr(directEntry->d_name, '.')), ".csv") != 0 && strcmp((strrchr(directEntry->d_name, '.')), ".CSV") != 0)   //if the found file extension does not match .csv ignore by continuing with loop
//			else if(strcmp(fileExtension, ".csv") && strcmp(fileExtension, ".CSV"))   //if the found file extension does not match .csv ignore by continuing with loop
			{
				continue;
			}
			else                                //in all other cases it will be a .csv so we can process
			{									
				char filePath[MAXPATH];    //create an array to hold path to file
				memset(filePath, '\0', MAXPATH);
				snprintf(filePath, (sizeof(*(dir_path)) + sizeof(directEntry->d_name)), "%s/%s", dir_path, directEntry->d_name);   //save path of file to buffer	
				pthread_mutex_lock(&lock);
				enqueue(csv_queue, filePath);
				++csv_count;
				pthread_mutex_unlock(&lock);
			}
		}
	}
	/*********************************************************
	This ends the loop the searches for subdirectories and
	CSV files
	*********************************************************/

//	pthread_mutex_lock(&lockFour);
//        dirTIDCount += count;
//	pthread_mutex_unlock(&lockFour);
//
//	for(i = 0; i < count; i++)
//        {
//                pthread_join(threads[i], NULL);
//                fprintf(stdout, "%lu, ", (unsigned long)threads[i]);
//               fflush(stdout);
//       }

	closedir(dirPointer);
	return;
}


struct Queue* headerChecker(const char *filePath, const char *columnName, int* colInfo)
{
	FILE *fp;                 //create a file pointer
	size_t numChars = 0,	 //to store the number of charachters in the header column
	bufferSize = MAXBUF * sizeof(char);		 //the size of the buffer where header data will be stored
	size_t lenColName = strlen(columnName);
	int j;
	int i,					//used for loops
	charCount= 0,				//used to advance through charachters in header
	numComma = 0,				//used to save the number of commas in header
	numCol = 0,				//to save the amount of columns in header			
	ifield,					//to keep track of charachter position while putting information into array
	colI = -1,
	colIndex = -1,				//keeps track of if the index of the requested column has been found
	mov_tit_index = -1;			//keep track of if the index of the movie_title column has been found
	char* stringBuffer;			//will be used a tempory holding buffer where the string information of each string in the header will be stored until that information can be passed onto the queue
	char* buffer = (char*) malloc(bufferSize);     //buffer where header data will be stored		
	assert(buffer != NULL);			//verify memory allocation of buffer was succesfull
	fp = fopen(filePath, "r");              //open a file pointer in read mode
	if(fp == NULL)				//if the file pointer is NULL (didn't work) print error and exit
	{
		printf("File cannot be opened. File %s will not be process. You may have entered an incorrect file path at startup ", filePath);
		return NULL;
	}

	numChars = getline(&buffer, &bufferSize, fp);     //put the first line of the file (header) into a buffer and record the number of charachter in the first line
	
	i = 0;           //reset i to zero for loop
	while(*(buffer + i) != '\n')                     //count the number of commas in buffer (header is in buffer)
	{
		if(*(buffer + i) == ',')
		{
			numComma++;
		}
		i++;
	}
	numCol = ++numComma;                          //store the number of columns in header which is num commas + 1
	
	/***************************************************************
	This block of code will take the header information that is
	stored in the buffer and put it into a queue.
	***************************************************************/
	i = 1;						//reset i for the loop
	stringBuffer = (char*) malloc(numChars);
	assert(stringBuffer != NULL);			//verify memory allocation was succesfull for stringBuffer
	struct Queue* queue = createQueue(numCol);	//create a queue to save the header information
	for (i; i <= numCol; i++)
	{
		ifield = 0;
		while(*(buffer + charCount) != ',' && *(buffer + charCount) != '\0')	//while buffer is not a comma nor a null terminating charachter
		{
			if(isblank(*(buffer + charCount)))		//if a blank is encountered skip over that position
				charCount++;
			else						//if it is not a blank copy the information over to stringBuffer
			{
				*(stringBuffer + ifield++) = *(buffer + charCount++); 
			}
		}
		if(*(stringBuffer + (ifield - 1)) == '\n')		//if the last charachter is a newline charachter print over the last two positions with a null terminating charachter
		{
			*(stringBuffer + (ifield - 2)) = '\0';
			*(stringBuffer + (ifield - 1)) = '\0';
		}

		if(i == 1 || i == 2)
		{
			int z = 0;
			z = strlen(stringBuffer);
			*(stringBuffer+z) = '\0';
		}
		
		if(colI == -1)
		{
//		printf("stringBuff: %s ... ColumnName: %s\n", stringBuffer, columnName);
			for(j=0; j<lenColName; j++)
			{
				if(*(stringBuffer+j) == *(columnName+j))
					continue;
				else
					break;
			}
			if(j == lenColName)
				colI = i;
		}
			
//		if(!strcmp(stringBuffer, columnName))		//if we have not found the column yet and the string compare is true then write the current column to colIndex
//			colI = i;
		if(mov_tit_index == -1 && !strcmp(stringBuffer, "movie_title"))
			mov_tit_index = i;

//		printf("stringBuffer: %s\n", stringBuffer);
		enqueue(queue, stringBuffer);				//save this string to the queue for later printing
		memset(stringBuffer, '\0', ifield);	//reset stringBuffer to null so new charachters can be saved in next iteration
		charCount++;
	}
	if(colI == -1)		//if colIndex is -1 at end of loop the column was not found.  print error and exit
	{
		fprintf(stderr, "The column you entered has not been found. Please try again by entering a correct column name\n");
		exit(1);
	}
	int* tempInt;
	tempInt = &numCol;
	*(colInfo + 0) = *(tempInt);		//to store the number of columns derereference the temp int pointer
	tempInt = &colI;
	*(colInfo + 1) = *(tempInt);	//store the inde of the column to be sorted 
	tempInt = &mov_tit_index;
	*(colInfo + 2) = *(tempInt);
	free(stringBuffer);		//free malloced string buffer
	free(buffer);			//free malloced buffer
	/*************************************************************
 	this ends the block of code where the header information is 
	stored
	**************************************************************/
	
	fclose(fp);			//close file pointer
	return (queue);
}
