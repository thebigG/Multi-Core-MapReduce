#include<stdlib.h>
#include<string.h>
#include<stdio.h>
#include "io_api.h"
#define WORD_COUNT "wordcount"
#define SORT "sort"
int map();
int reduce();
int main(int argc, char** argv)
{

 if (argc!=12)
 {
   printf("Ivalid arguments\n");
   printf("mapred –-app [wordcount, sort] "
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile");
   exit(0);
 }
char data[9000];
int input_fd  = open("./word_input.txt", O_RDONLY);
if(input_fd == -1)
{
  perror("open:");
  return -1;
}
 data[reader(input_fd, data, get_filesize("./word_input.txt"), get_filesize("./word_input.txt"))] = '\0';
int  output_fd  = open("./output_word.txt", O_CREAT | O_RDWR);
 if (output_fd == -1)
 {
   perror("open");
   return -1;
 }
 writer(output_fd, data, strlen(data), strlen(data));
 return 0;

}
