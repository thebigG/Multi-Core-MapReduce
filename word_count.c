#include<string.h>
#include <stdlib.h>
#include<stdio.h>
#include "word_count.h"
#include "core_utility.h"
#include "map_reduce.h"
extern int map_count;
extern pthread_mutex_t count_mutex;
void parse_string(char* string)
{
  return;
}
int word_count_strcmp(void* key1, void* key2)
{
  printf("word_count_strcmp: {%s, %s}\n", (char*)key1, (char*) key2);
  perror("strcmp");
  return strcmp((char*)key1, (char*) key2);
}
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
/*
compare function for quick sort
*/
int word_compare(const void* a, const void* b)
{
   char* const  * word_a = ( char *const*)a;
  char *  const * word_b = (char* const*)b;
  // printf("word_a:%s\n", *word_a);
  return strcmp(*word_a, *word_b);

}
/*
sorts all of the StringLinkedList stricts linked to head
This uses the qsort() function from the C-Standard Library
*/
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

// void word_sort(void* word_map)
// {
//   map_index* map = (map_index*) word_map;
//   token_split_object* token_data = (token_split_object*)word_map->data;
//   StringLinkedList token_head = token_data->token_list;
//   int range_index =   map->index;
//   int start = token_data->token_range_list[range_index]->start;
//   int end = token_data->token_range_list[range_index]->end;
//   int token_count = end-start;
//   char* list[token_count];
//   StringLinkedList*  temp_pointer = go_to_string(token_head, start);
//   int i = 0;
//   while( i< end)
//   {
//     list[i] = temp_pointer->String;
//     // printf("list:%s\n", list[i]);
//     temp_pointer = temp_pointer->next;
//     i++;
//   }
//
//    qsort(list, token_count ,sizeof(char*), word_compare );
//    i =0;
//    temp_pointer = head;
//    while(temp_pointer!= NULL)
//    {
//      temp_pointer->String = list[i];
//      temp_pointer = temp_pointer->next;
//      i++;
//    }
//    map_data->are_you_done = TRUE;
//
// }


StringLinkedList* go_to_string(StringLinkedList*  head, int offset)
{
  StringLinkedList* current = head;
  int i = 0;
  while(i<=offset)
  {
    current = current->next;
    i++;
  }
  // printf("FETCHING: %S\n");
  return current;
}
//This assumes that the StringLinkedList is sorted
int count_word(char* word ,StringLinkedList* head, int start, int end)
{
 int  i = 0;
int word_count = 0;
StringLinkedList* current = head;
while(i<start)
{
  current = current->next;
  i++;
}
printf("jumpstart done!\n");
while(start<=end)
{
  // printf("Stuck in a loop? {i:%d end:%d}\n current_string:%s, word:%s\n", i2 + 1, end, current->String, word);
printf("looking at %s and %s\n", word, current->String );
if(strcmp(word, current->String) == 0)
{
word_count++;
}
else
{
  printf("breaking point-->");
  if(word_count == 0)
  {
    printf("ZERO:{%s,%s}\n", word, current->String );
    word_count++;
    // word
  }
  break;
}
start++;
current = current->next;
}
// printf("looking at (before) word:%s\n", );
// printf("**\nTid:%d\n offset: %d\n i:%d \n word: %s \n**", pthread_self(), offset, i,current->String );
return word_count;
}


int count_words(void* count_words_data )
{
  // pthread_mutex_lock(&count_mutex);
  map_index* map_data = (map_index* )count_words_data;
  // int range_index  = map_data->index;
  token_split_object* token_data   = (token_split_object*)(map_data->context_data);
  StringLinkedList* words  = *token_data->token_list;
  int start = map_data->context_range->start;
  int end = map_data->context_range->end;
  int token_count =  (end -start);
   printf("{start:%d end:%d}\n", start, end);
  StringLinkedList* current = words;
  int  j = 0;
 while(j<start)
 {
   current = current->next;
   j++;
 }
   map_data->pairs = malloc(sizeof(key_value_link));
   map_data->pairs_count = 0;
   map_data->pairs->key = NULL;
    map_data->pairs->value = 0;
   key_value_link* current_pair = map_data->pairs;
   key_value_link* head_pair  = current_pair;
   int i = 0;
   j =0;
  int current_word_count = 0;
  int offset  = token_count;
  // printf("starting word: %s\n", current->String);
  printf("starting offset: %d\n", offset);
  while(i<=token_count)
  {
    current_word_count = count_word(current->String, words , start + i, end ) ;
    current_pair->key = malloc(sizeof(char) * (strlen(current->String) + 1 ) );
    strcpy( current_pair->key, current->String);
    current_pair->value = current_word_count;
    map_data->pairs_count += 1;
    printf("new link: {%s, %d}", current_pair->key, current_pair->value );
    // printf("Tid:%d \n{start:%d end:%d}  \n  word: %s count:%d \n",pthread_self(), start, end ,current_pair->key, current_word_count  );
     // printf("{start:%d end:%d}\n", start, end);


    // offset = current_word_count;
    j = 0;
    while(j<current_word_count)
    {
      current = current->next;
      j++;
    }
     // printf("before i: %d\n", i);
    i += current_word_count;

    printf("{token_count:%d, i:%d}\n", token_count, i);
    if(i>token_count)
    {
      current_pair->next = NULL;
      printf("NULL next for %s\n", current_pair->key);
    }
    else
    {
    current_pair->next = malloc(sizeof(key_value_link));
    current_pair = current_pair->next;
    }
     printf("after i: %d\n", i);

  }

  // write_map(1, map_data->pairs, parse_string, strlen );
  map_data->are_you_done = TRUE;
  printf("current map_count:%d\n" ,map_count);
  printf("token_count: %d\n", token_count);
  // map_data->pairs_count = token_count + 1;
  printf("pair_count: %d\n",map_data->pairs_count);
  map_count++;
  // pthread_mutex_unlock(&count_mutex);
  // if map_count>
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
This also indexes the tokens into different ranges
These range structs is what every thread in map looks at -- every thread gets an index for the specific range it is supposed to look at
*/
int token_split(void* data)
{
   token_split_object* this_data =   (token_split_object*) data;
   char* token = strtok(this_data->Data, this_data->delimiter);
   while (token!= NULL)
   {
     printf("token:%s:\n", token);
     insertString(token, this_data->token_list);
     token  = strtok(NULL, this_data->delimiter);
   }
    word_sort(*(this_data->token_list));
    int token_count = count_strings(*this_data->token_list);
   //  printf("token_count: %d\n", token_count);
   // init_distribute_data(this_data->token_range_list, token_count,this_data->token_range_list_size );
   // int  i = 0;
   // printf("string_count: %d\n", token_count);
   // while(i<this_data->token_range_list_size)
   // {
       // print_strings_at(*(this_data->token_list), this_data->token_range_list[i].start, this_data->token_range_list[i].end);
   //   i++;
   // }
   return token_count;
}
