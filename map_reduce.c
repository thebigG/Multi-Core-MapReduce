/*
map dispatches num_maps threads which call mapper.
*/
// New File
#include"map_reduce.h"
#include "core_utility.h"
#include<pthread.h>
#include"io_api.h"
pthread_mutex_t count_mutex   = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;
int map_count = 0;
int write_map(int file_descriptor, key_value_link* pairs,void (parse_key_routine)(void*) , int (get_sizeofkey_routine)(void*) )
{
 key_value_link*  current = pairs;
 char* temp  =NULL;
 while(current!= NULL)
 {
   temp = (current->key);

   // printf("key: %s\n",parsed_key );
   writer(file_descriptor, temp,get_sizeofkey_routine(temp), 1 );
   writer(file_descriptor, ",",strlen(","), 1 );
   temp = toString( current->value);
   writer(file_descriptor, temp, strlen(temp), 1 );
   printf("\n");
   current = current->next;
 }
 return 0;
}
void* map(void *(data_parser) (void*), void* map_data, void* (mapper) (void*), int num_maps)
{
  pthread_t map_threads[num_maps];
  data_parser(map_data);
  map_index* maps  = malloc(sizeof(map_index) *(num_maps + 1));
  int i = 0;
  while(i<num_maps)
  {
    printf("i value:%d\n", i);
    // pthread_detach(map_threads[i]);
    maps[i].mapper_data = map_data;
    maps[i].index  = i;
    maps[i].are_you_done = FALSE;
    i++;
  }
  // &(maps[i]) = NULL;
  i = 0;
  while(i<num_maps)
  {
    // printf("are you running?");
  pthread_create(&(map_threads[i]), NULL, mapper,  &maps[i] );
  i++;
  }
  // pthread_cond_wait( &condition_var, &count_mutex );
  //block until all map routines are done
  while(map_count<num_maps);
  printf("done!");


  return 0;
}

// void* reduce(map_index* maps, int num_reduces, int num_maps)
// {
// head = maps;
// int are_you_done  = 0;
// while(head!=NULL)
// {
//   if head.
// }
// }
