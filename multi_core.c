#include<stdlib.h>
#include<string.h>
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
   "–-impl [procs, threads] --maps num_maps –-reduces num_reduces --input infile –-output outfile")
   exit(0);
 }


 void()
 if (strcmp(argv[2], WORD_COUNT ) == 0)
 {

 }
}
