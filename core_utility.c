#include <stdlib.h>
#include <stdio.h>
#include "core_utility.h"
#include "word_count.h"

/*
Prints all of the strings linked to head
*/


char* toString(int Num)
{
  char* String  = malloc(sizeof(char) * 12);
  sprintf(String, "%d", Num);
  // printf("toString: %s\n", String);
  return String;
}
void print_strings(StringLinkedList* head)
{
  while(head != NULL)
  {
    printf("%s\n", head->String);
    head = head->next;
  }
  return;
}
/*
start is inclusive
end is inclusive
Prints strings from start to end linked to head
*/
void print_strings_at(StringLinkedList* head, int start, int end)
{
int i = 0;
StringLinkedList* current = head;
while(i<start)
{
  current = current->next;
  i++;
}
printf("in function range{%d,%d}\n", start, end);
while(start<=end)
{
  printf("%s\n", current->String);
  current = current->next;
  start++;
}
return;
}



void init_distribute_data(range* data_range, int data_tally, int bins )
{
  printf("init_distribute_data %d/%d\n", data_tally, bins);

  if (data_tally% bins == 0)
  {
    int interval = data_tally/bins;
    int i = 0;
    int start  = 0;
    int end = 0;
    printf("Interval: %d\n", interval);
    while(i<bins)
    {
      data_range[i].start = start;
      // printf("start on init:%d\n", word_data->token_range_list[i].start);
      data_range[i].end = (start+ interval) -(1);
      // printf("end on init:%d\n", word_data->token_range_list[i].end);
      start += interval;
        i++;
    }

  }
  else
  {
    int interval = data_tally/bins;
    int i = 0;
    int start  = 0;
    int end = 0;
    printf("Interval: %d\n", interval);
    while(i<bins)
    {
      data_range[i].start = start;
      // printf("start on init:%d\n", word_data->token_range_list[i].start);
      if (i+1 == bins)
      {
        data_range[i].end = data_tally-1;
      }
      else
      {
        data_range[i].end = (start+ interval) -(1);
      }

      // printf("end on init:%d\n", word_data->token_range_list[i].end);
      start += interval;
      i++;
    }

  }
  return;
}
