#include <stdlib.h>
#include <stdio.h>
#include "core_utility.h"
#include "word_count.h"

/*
Prints all of the strings linked to head
*/
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
while(start<end)
{
  printf("%s\n", current->String);
  current = current->next;
  start++;
}
return;
}



void init_distribute_data(token_split_data* word_data, int data_tally, int bins )
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
      word_data->token_range_list[i].start = start;
      // printf("start on init:%d\n", word_data->token_range_list[i].start);
      word_data->token_range_list[i].end = (start+ interval) -(1);
      // printf("end on init:%d\n", word_data->token_range_list[i].end);
      start += interval;
        i++;
    }

  }
  return;
}
