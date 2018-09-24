#include <stdlib.h>
#include <stdio.h>
#include "core_utility.h"
#include "word_count.h"
void print_strings(StringLinkedList* head)
{
  while(head != NULL)
  {
    printf("%s\n", head->String);
    head = head->next;
  }
  return;
}
