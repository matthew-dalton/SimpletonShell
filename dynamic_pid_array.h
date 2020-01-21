#ifndef DYNAMIC_PID_ARRAY
#define DYNAMIC_PID_ARRAY

#include "dynamic_string_array.h"
#include <sys/types.h>

struct dynamic_pid_array
{
  pid_t *array;
  char** commands;
  int array_size;
  int num_of_elements;
};

#endif /*DYNAMIC_PID_ARRAY*/
