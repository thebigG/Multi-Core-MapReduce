#ifndef WORD_COUNT
typedef struct
{
char* String;
struct StringLinkedList* next;
} StringLinkedList;
void initStringLinkedList(char* , StringLinkedList** );
void insertString(char*, StringLinkedList** );
char* getSubStr(char* , int , int );
int token_split(char* , StringLinkedList**, char*);
#endif
