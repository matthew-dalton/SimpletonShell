#ifndef DYNAMIC_STRING_ARRAY
#define DYNAMIC_STRING_ARRAY

struct dynamic_string_array
{
  char** array;
  int array_size;
  int num_of_elements;
};

#endif /*DYNAMIC_STRING_ARRAY*/
