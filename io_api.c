#include"io_api.h"
int get_filesize(char* file_path)
{
  printf("file path:%s\n", file_path);
struct stat data;
if (stat(file_path, &data ) == -1)
{
  return -1;
}
return data.st_size;
}
int writer(int file_descriptor, char* buffer, int byte_goal, int write_rate)
{
  int total_bytes_written  = 0;
  int bytes_written = 0;
  while(total_bytes_written<byte_goal)
  {
    bytes_written = write(file_descriptor, buffer + total_bytes_written , write_rate);
    if(bytes_written == -1)
    {
      perror("write");
      return -1;
    }
    total_bytes_written += bytes_written;
  }
  return total_bytes_written;
}
int reader(int file_descriptor ,char* buffer, int byte_goal, int read_rate)
{
  int bytes_read  =0;
  int total_bytes_read = 0;
  while(total_bytes_read<byte_goal)
  {
    bytes_read = read(file_descriptor,buffer + total_bytes_read , read_rate);
    if(bytes_read == -1)
    {
      perror("reader:");
      return -1;
    }
    total_bytes_read += bytes_read;
    //printf("total_bytes_read on client side: %d\n", total_bytes_read);
  }
  return total_bytes_read;
}
