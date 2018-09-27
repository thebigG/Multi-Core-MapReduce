#ifndef MAP_REDUCE
#define MAP_REDUCE
#include "word_count.h"
typedef struct value_key_link
{
void* key;
int value;
struct key_value_link* next;
} key_value_link;

typedef struct
{
  int tag_index;
  int thread_id;
} map_tag;
typedef struct
{
void* data;
key_value_link* pairs;
int index;
} map_index;
void* map(void *(data_parser) (void*), void* data, void* (data_routine) (void*), int );
int reduce();
#endif
