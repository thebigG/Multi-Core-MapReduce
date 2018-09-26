#include<string.h>
#include <stdlib.h>
#include<stdio.h>
#include "word_count.h"
#include "core_utility.h"
#include "map_reduce.h"
int count_strings(StringLinkedList* head)
{
StringLinkedList* current = head;
int counter = 0;
while(current != NULL)
{
  counter += 1;
  current = current->next;
}
return counter;
}
int word_compare(const void* a, const void* b)
{
   char* const  * word_a = ( char *const*)a;
  char *  const * word_b = (char* const*)b;
  // printf("word_a:%s\n", *word_a);
  return strcmp(*word_a, *word_b);

}
void word_sort(StringLinkedList* head)
{
  int token_count =   count_strings(head);
  char* list[token_count];
  StringLinkedList*  temp_pointer = head;
  int i = 0;
  while( temp_pointer!= NULL)
  {
    list[i] = temp_pointer->String;
    // printf("list:%s\n", list[i]);
    temp_pointer = temp_pointer->next;
    i++;
  }

   qsort(list, token_count ,sizeof(char*), word_compare );
   i =0;
   temp_pointer = head;
   while(temp_pointer!= NULL)
   {
     temp_pointer->String = list[i];
     temp_pointer = temp_pointer->next;
     i++;
   }

}

int count_words(void* count_words_data )
{
  map_index* map_data = (map_index* )count_words_data;
  int range_index  = map_data->index;
  token_split_data* token_data   = (token_split_data*)(map_data->data);
  StringLinkedList* words  = *token_data->token_list;
  int start = token_data->token_range_list[range_index].start;
  int end = token_data->token_range_list[range_index].end;
  int i = 0;
  while(i<3)
  {
    printf("ID:%d, start:%d, end:%d  \n",pthread_self(), start, end);
    i++;
  }
  return 0;
}


/*
start is inclusive
end is exlusive
*/

char* getSubStr(char* src, int start, int end)
{
  char* Sub = malloc((sizeof(char) * (end - start)) + 1);
  strncpy(Sub, (src+ start), end - start);
  Sub[end - start] = '\0';
  // printf("Substring: %s\n", Sub);
  return Sub;
}
/*
This function allocates memory for a string that is length characters long.
Although the string itself is length characters long, the location it lives on is length + 1 bytes long.
This is done to allocate space for the null-byte.
*/
void initStringLinkedList(char* String, StringLinkedList** head )
{
  *head = malloc(sizeof(StringLinkedList));
  (*head)->String = malloc( (sizeof(char) * (strlen(String))) + 1 ) ;
  (*head)->next = NULL;
  strcpy((*head)->String, String);
  if((*head)->String== NULL)
  {
    printf("String allocation was NULL\n");
  }
}
void insertString(char* String, StringLinkedList** head)
{
//  printf("Head: %d\n", &head);
  if(*head == NULL)
  {
    // printf("If is running");
    initStringLinkedList(String, head);
    return;
  }
  StringLinkedList* newItem   = NULL;
  StringLinkedList* current = *head;
  initStringLinkedList(String, &newItem);
  if(current->next ==NULL)
  {
    current->next = newItem;
  }
  else
  {
    while(current->next!=NULL)
    {
      current = current->next;
    }
    current->next = newItem;
    if(current ==NULL)
    {
      printf("The current was null :(\n");
    }
  }

}


/*
This splits Data into tokens using delimeter
Every token is stored in token_list
*/
void token_split(void* data)
{
   token_split_data* this_data =   (token_split_data*) data;
   // this_data->map_tag = map_tag;
   char* token = strtok(this_data->Data, this_data->delimeter);
   while (token!= NULL)
   {
     // printf("token:%s:\n", token);
     insertString(token, this_data->token_list);
     token  = strtok(NULL, this_data->delimeter);
   }
    word_sort(*(this_data->token_list));
    int token_count = count_strings(*this_data->token_list);
   init_distribute_data(this_data, token_count,this_data->token_range_list_size );
   int  i = 0;
   while(i<this_data->token_range_list_size)
   {
     printf("bin#%d: {%d, %d}", i+1, this_data->token_range_list[i].start, this_data->token_range_list[i].end );
     print_strigs_at(*(this_data->token_list), this_data->token_range_list[i].start, this_data->token_range_list[i].end);
     i++;
   }
}

// void map_tagger(void* data,int map_index , int thread_id, int num_maps )
// {
// int i = 0;
// token_split_data* map_data  = (token_split_data*)(data);
// // while(i<num_maps)
// map_data->data_distribute_maps[map_index].tag_index = map_index;
// map_data->data_distribute_maps[map_index].thread_id = thread_id;
// printf("tag:(thread: %d, tag:  %d)\n",map_data->data_distribute_maps[map_index].thread_id, map_data->data_distribute_maps[map_index].tag_index );
// // i++;
//
// return;
// }
