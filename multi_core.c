#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "io_api.h"
#include "word_count.h"
#include "map_reduce.h"
#define WORDCOUNT "wordcount"
#define SORT "sort"
int main(int argc, char** argv)
{

 if (argc!=13)
 {
   printf("Ivalid arguments\n");
   printf("mapred –-app [wordcount, sort] "
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile");
   exit(0);
 }
 printf("Are you running??");
 printf("file: %s", argv[8]);
char* input_file = argv[10];
char* routine  = argv[2];
char* implementation  = argv[4];
int num_maps  = atoi(argv[6]);
int  num_reduces  = atoi(argv[8]);
char* output_file = argv[10];
int input_file_szie =  get_filesize(input_file);
char* data = malloc(input_file_szie + 1);
int input_fd  = open(input_file, O_RDONLY);
if(input_fd == -1)
{
  perror("open");
  return -1;
}
if(reader(input_fd, data, input_file_szie, input_file_szie) == -1)
{
  perror("reader");
  return -1;
}
if(strcmp(WORDCOUNT, routine) ==  0)
{
  data[input_file_szie] = '\0';
  token_split_data* word_data = malloc(sizeof(token_split_data));
  word_data->Data = data;
  word_data->delimeter = " .,;:!-";

  /**
  The next two lines are VERY important
  If you do not do this when using token_split_data, things will break!
  So just remember to malloc() AND NULL token_list before doing stuff with it :)
  **/
  (word_data->token_list) = malloc(sizeof(StringLinkedList*));
  *(word_data->token_list) = NULL;
  map(token_split, word_data, count_words, num_maps);
}
free(data);
return 0;
}
