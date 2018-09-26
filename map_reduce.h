#ifndef MAP_REDUCE
#define MAP_REDUCE
#include "word_count.h"


typedef struct
{
  int tag_index;
  int thread_id;
} map_tag;
typedef struct
{
void* data; 
int index;
} map_index;
void* map(void *(data_parser) (void*), void* data, void* (data_routine) (void*), int );
int reduce();
#endif
