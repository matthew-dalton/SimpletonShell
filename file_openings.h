#ifndef FILE_OPENINGS
#define FILE_OPENINGS

#include "dynamic_int_array.h"

/*Returns -1 if unable to allocate enough memory to add file descriptor*/
int add_file_descriptor(int fd, struct dynamic_int_array *fd_list);

/*Returns -2 if that index is not being used*/
/*Returns -1 if that file could not be opened*/
int retrieve_file_descriptor(int index, struct dynamic_int_array *fd_list);

/*Updates an index*/
void update_file_descriptor(int index, int new_fd, struct dynamic_int_array *fd_list);

/*Closes an index*/
void close_file_descriptor(int index, struct dynamic_int_array *fd_list, int *exit_status);

/*Closes all of the file descriptors*/
void close_file_descriptors(struct dynamic_int_array *fd_list);

/*Frees the dynamically allocated memory used by the array*/
void free_file_descriptor_array(struct dynamic_int_array *fd_list);

/*For Debugging Purposes*/
void dump_fd_array(struct dynamic_int_array *fd_list);

#endif /*FILE_OPENINGS*/
