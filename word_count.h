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
int start;
int end;
}range;
/*
This struct used to:
gather data
Tokenize the data(into words)
Index the data with an array of range structs
These range structs are what tells every thread in map what to do
 */
typedef struct
{
char* Data;
StringLinkedList** token_list;
int token_range_list_size;
range token_range_list[512];
char* delimeter;
}token_split_object;



void initStringLinkedList(char* , StringLinkedList** );
void insertString(char*, StringLinkedList** );
char* getSubStr(char* , int , int );
void token_split(void*);
int count_words(void*  );
int word_compare(const void* , const void* );
int count_strings(StringLinkedList*);
void map_tagger(void* , int, int , int  );
#endif
