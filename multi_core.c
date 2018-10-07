#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include <unistd.h>
#include "io_api.h"
#include "word_count.h"
#include "map_reduce.h"
#define WORDCOUNT "wordcount"
#define SORT "sort"
int main(int argc, char** argv)
{

 if (argc!=13)
 {
   printf("Ivalid arguments: %d\n", argc);
   printf("mapred –-app [wordcount, sort] "
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile");
   exit(0);
 }
 // printf("Are you running??");
 // printf("file: %s\n", argv[9]);
char* input_file = argv[10];
char* routine  = argv[2];
char* implementation  = argv[4];
int num_maps  = atoi(argv[6]);
int  num_reduces  = atoi(argv[8]);
char* output_file = argv[8];
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
printf("num_maps: %d\n",num_maps );
printf("program: %s\n",routine );
if(strcmp(WORDCOUNT, routine) ==  0)
{
  data[input_file_szie] = '\0';
  token_split_object* word_data = malloc(sizeof(token_split_object));
  word_data->Data = data;
  // printf("Data:%s\n", word_data->Data);
  word_data->delimiter = " .,;:!-";
  // printf("delimiter: %s\n", word_data->delimiter);
  /**
  The next two lines are VERY important
  If you do not do this when using token_split_object, things will break!
  So just remember to malloc() AND NULL token_list before doing stuff with it :)
  **/
  (word_data->token_list) = malloc(sizeof(StringLinkedList*));
  *(word_data->token_list) = NULL;
  // word_data->token_range_list_size = num_maps;
  // word_data->token_range_list = malloc(sizeof(range) * num_maps);
  // map_index* maps = malloc(sizeof(map_index) * num_maps);
  // maps->mapper_data = word_data;
  indexed_map* map_index =  map(token_split, word_data, count_words, num_maps);
  int i = 0;
  while(i<10000)
  i++;
  write_map(1,map_index->pairs, parse_string, strlen );
  printf("command line num_reduces: %d\n", num_reduces);
  // sleep(2);
  // reduce(map_index->pairs, map_index->pairs_count,num_reduces, strcmp );
}
 //free(data);
return 0;
}
