#ifndef WORD_COUNT
#define WORD_COUNT
#include "map_reduce.h"
typedef struct
{
char* String;
struct StringLinkedList* next;
} StringLinkedList;
typedef struct
{
  StringLinkedList* words;
  int num_words;

} word_count_data;
typedef struct
{
int start;
int end;
}range;

// typedef struct
// {
// // StringLinkedList* token_data;
// int start;
// int end;
// }token_bin;
/*
I should call this struct something else because it is used throughout
entirety of word_count.c
 */
typedef struct
{
char* Data;
StringLinkedList** token_list;
// range data_distribute[512];
int token_range_list_size;
range token_range_list[512];
char* delimeter;
// int map_tag;
}token_split_data;



void initStringLinkedList(char* , StringLinkedList** );
void insertString(char*, StringLinkedList** );
char* getSubStr(char* , int , int );
void token_split(void*);
int count_words(void*  );
int word_compare(const void* , const void* );
int count_strings(StringLinkedList*);
void map_tagger(void* , int, int , int  );
#endif
