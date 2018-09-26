
/*
map dispatches num_maps threads which call data_routine.
*/

#include"map_reduce.h"
#include<pthread.h>
void* map(void *(data_parser) (void*), void* data, void* (data_routine) (void*), int num_maps)
{
  pthread_t map_threads[num_maps];

  data_parser(data);
  map_index maps[num_maps];
  int i = 0;
  while(i<num_maps)
  {
    pthread_detach(map_threads[i]);
    maps[i].data = data;
    maps[i].index  = i;
    i++;
  }
  i = 0;
  while(i<num_maps)
  {
  pthread_create(&(map_threads[i]), NULL, data_routine,  &maps[i] );
  i++;
  }


  return 0;
}
