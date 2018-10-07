/*
map dispatches num_maps threads which call mapper.
*/
// New File
#include"map_reduce.h"
#include "core_utility.h"
#include<pthread.h>
#include"io_api.h"
#include "word_count.h"
pthread_mutex_t count_mutex;
pthread_cond_t  condition_var = PTHREAD_COND_INITIALIZER;
key_value_link* reduced_links = NULL;
//NOTE : lowercase everthing
int map_count = 0;
int reduce_count = 0;
void* global_current_key = NULL;

key_value_link* goto_end_link(key_value_link* head )
{
  while(head->next!=NULL)
  {
    head = head->next;
  }
  return head;
}
void link_heads(key_value_link** heads, int num_heads  )
{
int i = 0;
while(i<num_heads)
  {
    if(i+1<num_heads)
      {
        goto_end_link(heads[i])->next = heads[i+1];
      }
      printf("last link: %s\n",goto_end_link(heads[i])->key );
    i++;
  }
}
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
}
key_value_link* map(void *(context_parser) (void*), void* context, void* (mapper) (void*), int num_maps)
{
  pthread_t map_threads[num_maps];
pthread_mutex_init(&count_mutex, NULL);
  map_index* maps  = malloc(sizeof(map_index) * num_maps);
  // int i = 0;
  // while(i<num_maps)
  // {
  //
  // }
  int context_list_count = context_parser(context);
  int  i  =0;
  range* ranges = malloc(sizeof(range) * context_list_count);
  init_distribute_data(ranges, context_list_count, num_maps);
  while(i<num_maps)
  {
    printf("i value:%d\n", i);
    // pthread_detach(map_threads[i]);
    maps[i].context_data = context;
    maps[i].index  = i;
    maps[i].context_range = &ranges[i];
    maps[i].are_you_done = FALSE;
    printf("bin#%d: {%d, %d}\n", i+1, maps[i].context_range->start, maps[i].context_range->end);
    print_strings_at( *(((token_split_object*)  maps[i].context_data)->token_list ), maps[i].context_range->start, maps[i].context_range->end);
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
  //block until all map routines are done
  while(map_count<num_maps);
  printf("done with threads!\n");
  i = 0;
  int final_pair_count = 0;
  while(i<num_maps)
  {
    // printf("are you running?");
    final_pair_count += maps[i].pairs_count;
  i++;
  }
  i = 0;
  key_value_link* links[num_maps];
  while(i<num_maps)
  {
    links[i] = maps[i].pairs;
    // printf("new pair: ");
    // write_map(1,links[i], parse_string, strlen);
    // printf("\n");
  i++;
  }
  i = 0;
  // free(maps);
  // while(i<num_maps)
  // {
    // printf("New pair:\n");
  // i++;
  // }
  link_heads(links ,num_maps);
  // printf("final pairs count: %d\n", final_pair_count);

  // printf("done!");
  indexed_map* return_map  = malloc(sizeof(indexed_map));
  return_map->pairs = links[0];
  return_map->pairs_count = final_pair_count;

  return return_map;
}
// void update_key_value(void(key_compare) (void*), void* key)
// {
//
// }
key_value_link* go_to_link(key_value_link* head, int offset)
{
int i =0;

printf("offset: %d head passed: %s\n", offset, head->key);
while(i<offset)
{
  head = head->next;
  printf("i: %d\n", i);
  i++;
}
return head;
}
// void add_link(key_value_link* head, key_value_link* new_link )
// {
// key_value_link*  current  = head;
// if (current == NULL)
// {
//   current  = malloc(sizeof(key_value_link));
//   return;
// }
// while()
// {
//
// }
//
// }
// /*
// get_key_count assumes that the key_value_link is sorted/shuffled
// */
// int get_key_count(key_value_link* starting_link, void* key, int (key_compare) (void*, void*) )
// {
// int redundant_count = 0;
// key_value_link* current = starting_link;
// while(current!= NULL)
// {
// if (key_compare(current->key, key) == TRUE )
// {
//   reduce_count += current->value;
// }
// else
// {
//   break;
// }
// current = current->next;
// }
// return redundant_count;
// }
// //iterate through the key_value_links inside map and add them to reduced_links
// void reduce_helper(void* reduced_data )
// {
// reduce_index* reduce_context =  (reduce_index*) reduced_data;
// // printf("Tid: %d start: %d\nend:%d\n", pthread_self(), reduce_context->reduced_range->start, reduce_context->reduced_range->end);
// int i = 0;
// int pairs_count = reduce_context->reduced_range->end - reduce_context->reduced_range->start;
// key_value_link* head_link = reduce_context->pairs;
// key_value_link* starting_link  =  go_to_link(head_link, reduce_context->reduced_range->start);
// key_value_link*  current_link = starting_link;
// int j = 0;
// while(i<pairs_count)
// {
//   if  head_link->key_compare(global_current_key,current_link->key )
//   {
//     continue;
//   }
//   if ( has_key(current_link ,reduced_links,head_link->key_compare ) == FALSE)
//   {
//     global_current_key = current_link->key;
//     current_link->value  =  get_key_count(current_link, global_current_key, head_link->key_compare );
//   }
// current_key = current_key->next;
// i++;
// }
// // printf("");
// reduce_count++;
// return;
// }
// int has_key(void* key,key_value_link* head, int (key_compare) (void*, void*) )
// {
// if (head== NULL)
// {
//   return FALSE;
// }
// while(head->key != NULL)
// {
//   printf("has_key: {%s, %s}\n" , head->key ,key );
//   if( key_compare(head->key, key) == 0)
//   {
//     return TRUE;
//   }
//   head = head->next;
// }
// return FALSE;
// }
// void* reduce(key_value_link* links, int links_list_size, int num_reduces, int (key_compare) (void*))
// {
// range* reduce_ranges = malloc(sizeof(range) * num_reduces)  ;
// reduced_links = malloc(sizeof(key_value_link));
// pthread_t reduce_threads[num_reduces];
// init_distribute_data(reduce_ranges, links_list_size, num_reduces);
// reduce_index* reduce_indecies  = malloc(sizeof(reduce_index) * num_reduces);
// int i = 0;
// while(i<num_reduces)
//   {
//     reduce_indecies[i].pairs = links;
//     reduce_indecies[i].reduced_range = &reduce_ranges[i];
//     // pthread_detach(reduce_threads[i]);
//     pthread_create(&reduce_threads[i] ,NULL,reduce_helper, &reduce_indecies[i] );
//     pthread_join(reduce_threads[i], NULL);
//     i++;
//   }
//   while(reduce_count<num_reduces)
//   i++;
//   printf("done with reduce!\n");
//
// }
