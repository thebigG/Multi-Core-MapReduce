#ifndef CORE_UTILITY
#define CORE_UTILITY
#include <stdlib.h>
#include "word_count.h"

/*
DO NOT delete these forward declarations
*/
typedef struct StringLinkedList StringLinkedList; //forward declaration to tell the compiler that StringLinkedList type exists

typedef struct range
{
int start;
int end;
}range;

void print_strings(StringLinkedList*);
void print_strings_at(StringLinkedList* , int , int );
void init_distribute_data(range* , int , int );
#endif
