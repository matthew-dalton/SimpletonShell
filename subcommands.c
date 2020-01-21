#include "subcommands.h"
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


int add_command_arg(char* arg, struct dynamic_string_array *command_array)
{
  if (command_array->num_of_elements == command_array->array_size)
    {
      if (command_array->array_size == 0) { command_array->array_size = 10; }
      else { command_array->array_size *= 2; }

      command_array->array = (char**) realloc(command_array->array,
                                       command_array->array_size * sizeof(char*));
      if (command_array->array == NULL) { return -1; }
    }

  command_array->array[command_array->num_of_elements++] = arg;
  return 0;
}

char* retrieve_arg(int index, struct dynamic_string_array *command_array)
{
  if (-1 < index < command_array->num_of_elements)
    { return command_array->array[index]; }
  else
    { return NULL; }
}

char** retrieve_command_array(struct dynamic_string_array *command_array)
{
  return command_array->array;
}

void free_command_array(struct dynamic_string_array *command_array)
{
  free(command_array->array);
  command_array->array = NULL;
  command_array->num_of_elements = 0;
  command_array->array_size = 0;
}

void dump_command_array(struct dynamic_string_array *command_array)
{
  for (int i = 0; i < command_array->num_of_elements; i++)
    {
      fprintf(stdout, "%s\n", command_array->array[i]);
    }
}


int return_arg_count(struct dynamic_string_array *command_array)
{
  return command_array->array_size;
}

char* command_string(struct dynamic_string_array *command_array)
{
  char* cmd_string = NULL;
  size_t size = 0;
  

  /*The following for-loops use (num_of_elements - 1) because the arrays are null-terminated*/

  for (int i = 0; i < command_array->num_of_elements - 1; i++)
    {
      size += strlen(command_array->array[i]);
      size += 2;
    }
  
  cmd_string = (char*) malloc(size * sizeof(char));
  
  strcpy(cmd_string, command_array->array[0]);

  for (int i = 1; i < command_array->num_of_elements - 1; i++)
    {
      strcat(cmd_string, " ");
      strcat(cmd_string, command_array->array[i]);
    }

  return cmd_string;
}

int add_pid(pid_t new_child, struct dynamic_string_array *command_array, struct dynamic_pid_array *pid_array)
{
  if (pid_array->num_of_elements == pid_array->array_size)
    {
      if (pid_array->array_size == 0) { pid_array->array_size = 10; }
      else { pid_array->array_size *= 2; }
      
      pid_array->array = (pid_t*) realloc(pid_array->array, pid_array->array_size * sizeof(pid_t));
      pid_array->commands = (char**) realloc(pid_array->commands, pid_array->array_size * sizeof(char*));

      if (pid_array->array == NULL) { return -1; }
      if (pid_array->commands == NULL) { return -1; }
    }
  
  pid_array->commands[pid_array->num_of_elements] = command_string(command_array);
  pid_array->array[pid_array->num_of_elements++] = new_child;
  return 0;
}

void free_pid_array(struct dynamic_pid_array *pid_array)
{
  for (int i = 0; i < pid_array->num_of_elements; i++)
    {
      free(pid_array->commands[i]);
    }
  free(pid_array->commands);
  free(pid_array->array);
  pid_array->commands = NULL;
  pid_array->array = NULL;
  pid_array->num_of_elements = 0;
  pid_array->array_size = 0;
}
