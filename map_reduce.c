void* map(void *(data_parser) (void*), void* data, void* (data_routine) (void*)  ,int num_maps)
{
  data_parser(data);
  printf("String count:%d", data_routine(data));
return 0;
}
