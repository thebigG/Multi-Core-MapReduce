#ifndef WORD_COUNT
#define WORD_COUNT
typedef struct
{
char* String;
struct StringLinkedList* next;
} StringLinkedList;
typedef struct
{
  StringLinkedList* words;
  int num_words;

} word_count_data;
typedef struct
{
char* Data;
StringLinkedList** token_list;
char* delimeter;
}token_split_data;
void initStringLinkedList(char* , StringLinkedList** );
void insertString(char*, StringLinkedList** );
char* getSubStr(char* , int , int );
void token_split(void*);
int count_words(void*  );
int word_compare(const void* , const void* );
int count_strings(StringLinkedList**);
#endif
