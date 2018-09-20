#include<string.h>
#include <stdlib.h>
#include<stdio.h>
#include "word_count.h"
/*
start is inclusive
end is exlusive
*/
char* getSubStr(char* src, int start, int end)
{
  char* Sub = malloc((sizeof(char) * (end - start)) + 1);
  strncpy(Sub, (src+ start), end - start);
  Sub[end - start] = '\0';
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
  printf("length of string: %d", strlen((*head)->String));
  printf("initiated:%d", *head);
  if((*head)->String== NULL)
  {
    printf("String allocation was NULL\n");
  }
}
void insertString(char* String, StringLinkedList* head)
{
  printf("Head: %s\n", head->String);
  StringLinkedList* newItem   = NULL;
  StringLinkedList* current = head;
  initStringLinkedList(String, &newItem);
  printf("insertString");
  if(current->next ==NULL)
  {
    current->next = newItem;
    printf("New item: %s", newItem->String);
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
      printf("The was null :(\n");
    }
  }

}
