#include "file_openings.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


int add_file_descriptor(int fd, struct dynamic_int_array *fd_list)
{

  if (fd_list->num_of_elements == fd_list->array_size)
    {
      if (fd_list->array_size == 0) { fd_list->array_size = 10; }
      else { fd_list->array_size *= 2; }

      fd_list->array =
        (int *) realloc(fd_list->array, fd_list->array_size * sizeof(int));

      if(fd_list->array == NULL) { return -1; }
    }


  fd_list->array[fd_list->num_of_elements++] = fd;
  return 0;
}


int retrieve_file_descriptor(int index, struct dynamic_int_array *fd_list)
{
  if (-1 < index  &&  index < fd_list->num_of_elements)
    { return fd_list->array[index]; }
  else
    { return -2; }
}

void update_file_descriptor(int index, int new_fd, struct dynamic_int_array *fd_list)
{
  fd_list->array[index] = new_fd;
}

void close_file_descriptor(int index, struct dynamic_int_array *fd_list, int *exit_status)
{
  if (close(fd_list->array[index]) == -1)
    {
      fprintf(stderr, "Error closing file: %s\n", strerror(errno));
      *exit_status = 1;
    }
  fd_list->array[index] = -1;
}


void close_file_descriptors(struct dynamic_int_array *fd_list)
{
  for(int i = 0; i < fd_list->num_of_elements; i++)
    {
      if (fd_list->array[i] != -1)
        {
          if (close(fd_list->array[i]) == -1)
            {
              fprintf(stderr, "Error closing file: %s\n", strerror(errno));
            }
        }
    }
}


void free_file_descriptor_array(struct dynamic_int_array *fd_list)
{
  free(fd_list->array);
  fd_list->array = NULL;
  fd_list->num_of_elements = 0;
  fd_list->array_size = 0;
}


void dump_fd_array(struct dynamic_int_array *fd_list)
{
  for (int i = 0; i < fd_list->num_of_elements; i++)
    {
      fprintf(stdout, "Index %i fd: %i\n", i, fd_list->array[i]);
    }
}
