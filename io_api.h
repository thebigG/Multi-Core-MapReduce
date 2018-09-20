#ifndef IO_API
#include<fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <sys/stat.h>
#include <unistd.h>
int get_filesize(char*);
int writer(int, char* , int, int);
int reader(int  ,char* , int , int);

#endif
