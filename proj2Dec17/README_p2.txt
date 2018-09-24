Systems Programming CS214
Class Project Project 2
Basic Data Sorter (multithreaded)

Group Name: ME

Student 1:				Student
Eric Fiore				Michelle Dry-Moran
NetID: EJF96				NetID: MDRY
RUID: 181008642				RUID: 023007716		


Tar:
 - sorter.c
 - sorter.h
 - dirWorker.c
 - queue.c
 - queue.h
 - readcsv.c
 - mergesort.c
 - bst.c
 - forking chart.pdf
 - analysis.pdf
(As well as this README.txt)


Complier, used the following:
gcc -pthread -o sorter ./sorter.c ./dirWorker.c ./queue.c ./readcsv.c ./mergesort.c ./bst.c -I.


******************************************

This is the third part of the class project for CS 214 Systems Programming.  The purpose is search a directory (either given or default) and to find all acceptable csv files (based on the inputed column to sort existing in the header of the csv file) and to sort the csv file based on user input from the command line.  All sorted files will be merged together, and will be outputted to either the current default directory, or a directory that the user specifies.  The program also threads in the main directory for multiple directories and also spawns a new thread for each file it has to sort.  The program will print the parent tid along with all child tids.  It will then add up all tids, and give the total # of processes

*** >>> NOTE <<< ***
Assumptions: For sorting - we removed ALL special characters.  It only keeps 


*****************************************
**** Sorter.c file / main() function ****
*****************************************
Our implementation starts at main which is in “sorter.c”.
Argc is initially tested for having more than 2 arguments (if less - an error is printed and the program exits), as the program requires the executable followed by a -c, and a column header that will be sorted on.  Argv is then tested specifically tested for -c, followed by an argument, -d, followed by an argument, and -o followed by an argument.  Putting in the incorrect arguments will throw an error if it is not there. 

We check for the existence of the arguments using a while loop. The while loop checks for the existence of arguments using the getopt function which returns a value to the variable "commandOption".  In each iteration of the loop commandOption is checked, using a switch statement, for the
existence of the '-c' argument, ‘-d’ argument, and ‘-o’ argument.  If the -c, d or o’ arguments are found the variable "cflag”, “dflag”, and “oflag" are incremented.  On subsequent iterations of the while loop if "cflag”, “dflag”, or “oflag” are found to be anything other than 0 (equal to boolean true) an error is reported to the user. If only one -c, d or o are input the program will run normally. If an unknown character is input (that is anything besides -c, -d or -o an error is printed.  If no argument (after the -c, d or o) is input an error is also output.  The arguments following the -c, d and o are as follows:
-c argument : headerTitle (the column or columns (separated by a comma) to sort (and subsort) on.
-d argument : searchedDirectory (the directory to start searching from specified by the user (the default is the “.” directory). If dflag = 1, then the program tries to open the directory.  If it cannot, an error is given, and the program exits.  If the directory opens, then we close it and continue on our program. 
-o argument : outputDirectory (the directory in which to save the outputted csv files to specified by the user (the default is the csv source file directory). If oflag = 1, then the program tries to open the directory.  If it cannot, an error is given, and the program exits.  If the directory opens, then we close it and continue on our program.

The program then gets the parent TID and prints out the parent TID along with the statement to call out Child IDs.

The program then searches the main directory, and if it find a subdirectory, it threads into that directory using CSVFinder (which is in dirWorker.c)

If it finds a file - in any directory, it saves it to a csv_queue.

We call join to wait for the threads that get spawned (and print those IDs)

Then, we take the first file (doesn't matter which one), and send it to headerChecker which checks to find how many columns, where the column is that we want to sort, and where the movie_title column is - because we want to treat that separately.

Then we create a struct for the column count, the column index number of the column we want to sort and of the movie_title column, and the file path that we are going to sort.

For every file, we spawn a thread, and send it out to readcsv, and pass the struct.

We call join to wait on those threads - and then to print the tids.  We also print the total number of kids.

Next we create a new file, and print out the header (from a queue that was returned from headerChecker).

We also are going to use inOrder to print out the binary search tree which is where we stored all of the sorted data.

***********************************************
**** END : Sorter.c file / main() function ****
***********************************************

***********************
**** Sorter.h file ****
***********************
In sorter.h we define all of our macros, global variables, structs and prototypes.  sorter.h is a global header that is used for all the .c files (except queue.c)

*****************************
**** END : Sorter.h file ****
*****************************


**********************
**** Queue.c file ****
**********************
Overall:
This creates an array of chars.  You initialize the capacity, and it allows you to put stuff (chars) into the queue, and then it will return whatever was first put in. (FIFO).

createQueue() - creates the queue
Arguments:
unsigned capacity - which is the number of items you have in the queue
Returns:
Struct Queue * - Which is the string struct for storing data


isFull() - checks to see if the queue is full
Arguments:
Struct Queue* - Which is the string struct for storing data
Returns: int (1/0 for yes/no)

isEmpty() - checks to see if the queue is empty
Arguments:
Struct Queue* - Which is the string struct for storing data
Returns: int (1/0 for yes/no)

enqueue() - adds strings to the queue
Arguments:
Struct Queue* - Which is the string struct for storing data
char* string - the string you want to store
Returns int (for working or not)


dequeue() - removes strings to the queue
Arguments:
Struct Queue* - Which is the string struct for storing data
char* string - the string you want to take off the queue
Returns an int (for working or not)


front() - front of the queue
Arguments:
Struct Queue* - Which is the string struct for storing data
char* string - the string you want are storing/removing
Returns an int (for working or not)

rear() - back of the queue
Arguments:
Struct Queue* - Which is the string struct for storing data
char* string - the string you want are storing/removing
Returns an int (for working or not)


********************************************
**** END : Queue.c file / createQueue() ****
********************************************


**********************
**** Queue.h file ****
**********************

In queue.h we define all of our macros, structs and prototypes that are specific to queue.c

****************************
**** END : Queue.h file ****
****************************


*************************************************
**** dirWorker.c file / CSVFinder() function ****
*************************************************
Arguments:
 - cost char * directName: the directory to start searching in

Returns: nothing

CSV Finder will search through a directory to find all instances of subdirectories and files.  The while loop will test to see if the first character is a ‘.’ And ignore all of those files (i.e. hidden files and current dir & parent).  Upon finding a subdirectory it recursively call CSVfinder. Upon finding a file, it store the file in an array to be used later.

*******************************************************
**** END : dirWorker.c file / CSVFinder() function ****
*******************************************************

*****************************************************
**** dirWorker.c file / headerChecker() function ****
*****************************************************
Arguments:
 - cost char * filePath: the path (including file name) of the file to check it’s header (first row of csv)
 - const char * columnName: The name of the column (or columns) to check against the header
 - int* colInfo: a global int array that returns how many columns there are, and the column indexes of the column to sort and movie_title column
Returns: struct Queue* - which we stored the header into

This function searches the filePath and ensures the file is ok to be sorted and collects data about the columns as we go.

***********************************************************
**** END : dirWorker.c file / headerChecker() function ****
***********************************************************

*********************************************
**** readcsv.c file / readcsv() function ****
*********************************************
Arguments:
void * args (which we turn into a readcsvStruct *)
Essentially we pass in through a struct, the file path, the numCol, colIndex and movieCol

Returns nothing.

Readcsv parses in the data, and stores it into a struct defineRow, which stores the column stringData, the numData (if it's a number), and the wholeRow data. 

It takes this information and pushes it to mergesort.

Then once back from mergesort - if this is the first file, we create a balanced binary search tree.  Otherwise we just search the tree and add files to it.

***************************************************
**** END : readcsv.c file / readcsv() function ****
***************************************************

**********************************************************
**** mergesort.c file / fmerge() & smerge() functions ****
**********************************************************
Arguments:
Struct defineRow myRow : the array of data to be sorted 
int rowNum[] : The row numbers that get swapped when data gets sorted
int left : the left number of the array to start looking at
int middle : the middle of the array 
int right : the right number of the array to end at
Returns: void - since the string/double dataArrays and row array are changed

The merge functions uses the left, middle and right #s to break the array dataArray up into two arrays.  It then compares the arrays in two temporary arrays called Left_Arr and Right_Arr (with similar left/right row arrays).  The data then compares the arrays and puts them in order.  It back fills with any remaining left or right array that hasn’t been looked at.  Anytime the data is swapped or moved, it’s corresponding row number is swapped and moved.  This way, we can use the row number for printing the rest of the data.
****************************************************************
**** END : mergesort.c file / fmerge() & smerge() functions ****
****************************************************************

******************************************************************
**** mergesort.c file / fmergesort() & smergesort() functions ****
******************************************************************
double doubleDataArray[] or char stringDataArray[MAXROWS][MAXFIELDLEN] : the array of data to be sorted 
int rowNum[] : The row numbers that get swapped when data gets sorted
int left : the left number of the array to start looking at
int right : the right number of the array to end at
Returns: void - since the string/double dataArrays and row array are changed

Mergesort will find the middle of the array by dividing (left+right-1) / 2.  Mergesort is then then recursively called, calling merge sort on the left & middle numbers and then on the middle and right numbers.  This ultimately breaks down the array into small arrays of 2.  Once at it’s smallest size - the array gets sent to the appropriate merge function, which will sort it.  The recursiveness of the function will sort all pairs, then groups of 4, then groups of 8, etc…. until the whole list has been sorted.
******************************************************************
**** END : mergesort.c file / fmergesort() & smergesort() functions ****
******************************************************************

******************************************
**** TESTING AND ISSUES WE RAN ACROSS ****
******************************************

Yeah - we had issues.

Please see the attached file for analysis and the chart.

********************************
********************************
**** END OF README.TXT FILE ****
********************************
********************************
