#ifndef SUBCOMMANDS
#define SUBCOMMANDS

#include "dynamic_string_array.h"
#include "dynamic_pid_array.h"

int add_command_arg(char *arg, struct dynamic_string_array *command_array);

char* retrieve_arg(int index, struct dynamic_string_array *command_array);

char** retrieve_command_array(struct dynamic_string_array *command_array);

void free_command_array(struct dynamic_string_array *command_array);

void dump_command_array(struct dynamic_string_array *command_array);

int return_arg_count(struct dynamic_string_array *command_array);

char* command_string(struct dynamic_string_array *command_array);

int add_pid(pid_t new_child, struct dynamic_string_array *command_array, struct dynamic_pid_array *pid_array);

void free_pid_array(struct dynamic_pid_array *pid_array);

#endif /*SUBCOMMANDS*/
