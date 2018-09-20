#include<pthread.h>
#include<stdlib.h>
#include<stdio.h>

int shared_memory[] = {1,2,3,4,5,6,7,8,9,10};
int shared_index = 0;
pthread_mutex_t mutex;
void do_something(void* data)
{
    while(shared_index<10)
    {
      pthread_mutex_lock(&mutex);
      printf("%d:%d\n", data ,shared_memory[shared_index]);
    //  printf("");
      shared_index++;
      pthread_mutex_unlock(&mutex);
    }

}

int main(int argc, char** argv)
{
  pthread_mutex_init(&mutex, NULL);
  printf("Ok, so far\n");
  char* message1 = "Hi! This is thread x1\n";
  char* message2 = "Hi! This is thread x2\n";
 pthread_t x1, x2;
 // pthread_t** threads;
 // *threads = 4;
 // *(threads + 1) = &x2;
 pthread_create(&x1 , NULL, do_something, 1);
 pthread_create(&x2, NULL, do_something, 2);
 // pthread_detach(x1);
 // pthread_detach(x2);
 pthread_join(x1, NULL);
 pthread_join(x2, NULL);
 // printf("\n");
}
