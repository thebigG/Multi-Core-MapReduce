#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <unistd.h>
#include "io_api.h"
#include <assert.h>
#include "word_count.h"
#include "map_reduce.h"
#define WORDCOUNT "wordcount"
#define SORT "sort"
#define PATHSIZE 4096		//max linux path size (note max file nam size is 255)
#define ARGSIZE 12		//define the string argument size for routine & implementation

int main(int argc, char** argv)
{

 if (argc!=7)	 // check to make sure there are 7 arguments (otherwise send error & exit)
 {
   printf("Ivalid arguments: %d\n", argc);
   printf("Seven arguments required: ./mapred –-app [wordcount, sort] "
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile");
   exit(0);
 }

 // printf("Are you running??");
 // printf("file: %s\n", argv[9]);


/**** Store arguments into variables *****/

 char * routine = (char*) malloc(ARGSIZE * sizeof(char));   	// argument for routine is either -wordcount or -sort
 assert(routine != NULL);				      	// if malloc returns NULL, give error
 memset (routine, '\0', (ARGSIZE * sizeof(char)));		// clear malloced space
 strncpy(routine, argv[1], (ARGSIZE * sizeof(char)));		// copy argument 1 to routine
 routine++;							// increment by one to remove dash

 char * implementation = (char*) malloc(ARGSIZE * sizeof(char));  // argument for routine is either -procs or -threads
 assert(implementation != NULL);				  // if malloc returns NULL, give error
 memset (implementation, '\0', (ARGSIZE * sizeof(char)));	  // clear malloced space
 strncpy(implementation, argv[2], (ARGSIZE * sizeof(char)));	  // copy argument 2 to implementation
 implementation++;						 // increment by one to remove dash

 int num_maps  = abs(atoi(argv[3]));		// argument 3 - number of maps (absolute number of the string to integer)
 int num_reduces  = abs(atoi(argv[4]));		// argument 4 - number of reducers (absolute number of the string to integer)

 char * input_file = (char*)malloc(PATHSIZE * sizeof(char));	//malloc for input_file
 assert(input_file != NULL);					// if malloc returns NULL, give error
 memset (input_file, '\0', (PATHSIZE * sizeof(char)));	 	// clear malloced space
 strncpy(input_file, argv[5], (PATHSIZE * sizeof(char)));	// copy argument 5 to input_file
 input_file++;							// increment by one to remove dash

 char * output_file = (char*) malloc(PATHSIZE * sizeof(char));	// malloc for output_file
 assert(output_file != NULL);					// if malloc returns NULL, give error
 memset(output_file, '\0', (PATHSIZE * sizeof(char)));	 	// clear malloced space
 strncpy(output_file, argv[6], (PATHSIZE * sizeof(char)));	// copy argument 6 to output_file
 output_file++;							// increment by one to remove dash


//OLD** char* input_file = argv[10];
//OLD** char* routine  = argv[2];
//OLD** char* implementation  = argv[4];
//OLD** int num_maps  = atoi(argv[6]);
//OLD** int num_reduces  = atoi(argv[8]);
//OLD** char* output_file = argv[8];

/**** Read in File  ****/
 int input_file_size = get_filesize(input_file);			// call get_filesize to find size of the input file
 char * data = (char*) malloc((input_file_size + 1)*sizeof(char));	// malloc space for the size of the file
 assert(data != NULL);							// throw error
 memset (data, '\0', ((input_file_size +1)*sizeof(char)));		// clear out malloced space

 int input_fd  = open(input_file, O_RDONLY);		// open input_file / read only
 if(input_fd == -1)					// if -1, then throw error and exit
 {
   perror("open");
   return -1;
 }

 if(reader(input_fd, data, input_file_size, input_file_size) == -1)	// read file, and throw error if nothing returns
 {
   perror("reader");
   return -1;
 }

// ***** TESTING
// printf("num_maps: %d\n", num_maps);
// printf("program: %s\n", routine );
// ***** END Testing

 if(strncmp(WORDCOUNT, routine, (ARGSIZE*sizeof(char))) ==  0) 	//if routine = wordcount, then
 {
   data[input_file_size] = '\0';					// Ensure there is a null at the end of the string data

								// createstruct
   token_split_object * word_data = (token_split_object *) malloc(sizeof(token_split_object));
   assert(word_data != NULL);				      	// if malloc returns NULL, give error
 
   word_data->Data = data;					// assign the pointer of data to word_data->Data

// printf("Data:%s\n", word_data->Data);

   word_data->delimiter = " .,;:!-";				// Assign delimeters

// printf("delimiter: %s\n", word_data->delimiter);

   /**
   The next two lines are VERY important
   If you do not do this when using token_split_object, things will break!
   So just remember to malloc() AND NULL token_list before doing stuff with it :)
   **/

   (word_data->token_list) = malloc(sizeof(StringLinkedList*));		//Pointing Next link to next space
   *(word_data->token_list) = NULL;					//Setting the following link to NULL.


// word_data->token_range_list_size = num_maps;
// word_data->token_range_list = malloc(sizeof(range) * num_maps);
// map_index* maps = malloc(sizeof(map_index) * num_maps);
// maps->mapper_data = word_data;


   indexed_map* map_index =  map(token_split, word_data, count_words, num_maps);

   int i = 0;
   while(i<10000)
   i++;
// write_map(1,pairs, parse_string, strlen );

// printf("command line num_reduces: %d\n", num_reduces);

//  reduce(map_index->pairs, map_index->pairs_count,num_reduces, strcmp );
 }

 //free(data);
return 0;
}
