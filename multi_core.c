#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "io_api.h"
#include "word_count.h"
#include "map_reduce.h"
#define WORD_COUNT "wordcount"
#define SORT "sort"
int main(int argc, char** argv)
{

 if (argc!=12)
 {
   printf("Ivalid arguments\n");
   printf("mapred –-app [wordcount, sort] "
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile");
   exit(0);
 }
 printf("Are you running??");
 printf("file: %s", argv[8]);
char* input_file = argv[8];
int input_file_szie =  get_filesize(input_file);
char* data = malloc(input_file_szie);
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
StringLinkedList* head  = NULL;

initStringLinkedList("Tree",&head);
printf("Head: %s\n", head->String);
 StringLinkedList* current = head;
current = current->next;
insertString("Dirty", head);
printf("Next: %s\n", current->String);
return 0;
}
