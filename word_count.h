#ifndef WORD_COUNT
#define WORD_COUNT
#include<stdlib.h>
#include "core_utility.h"

typedef struct range range; //forward declaration to tell the compiler that range type exists


typedef struct StringLinkedList
//DO NOT remove "StringLinkedList" from here as it
//will prevent you from making the forward declaration elsewhere
{
char* String;
struct StringLinkedList* next;
} StringLinkedList;
/*
This struct used to:
gather data
Tokenize the data(into words)
Index the data with an array of range structs
These range structs are what tells every thread in map what to do
 */
typedef struct token_split_object
{
char* Data;
StringLinkedList** token_list;
int token_range_list_size;
range* token_range_list;
char* delimiter;
}token_split_object;

void initStringLinkedList(char* , StringLinkedList** );
void insertString(char*, StringLinkedList** );
char* getSubStr(char* , int , int );
void token_split(void*);
int count_word(StringLinkedList* , int );
StringLinkedList* go_to_string(StringLinkedList*  , int );
int count_words(void*  );
int word_compare(const void* , const void* );
int count_strings(StringLinkedList*);
void map_tagger(void* , int, int , int  );
#endif
