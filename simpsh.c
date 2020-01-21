/*
NAME: Matthew Dalton
EMAIL: mattdalton@g.ucla.edu
UID: 304964539
*/

#define _GNU_SOURCE

/*My Header Files*/
#include "subcommands.h"
#include "file_openings.h"
#include "dynamic_int_array.h"
#include "dynamic_string_array.h"
#include "dynamic_pid_array.h"

/*GNU Header Files*/
#include <errno.h>
#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>


/********************************************************************/
/*Auxillary Declarations*/

void open_file(int flags, int mode, struct dynamic_int_array *fd_list, int *exit_status);

void open_pipe(struct dynamic_int_array *fd_list, int *exit_status);

void verbose_flag_check(char *option_string, int flag, int optarg_flag);

void sig_handler(int N);

void execute_command(int fd_input, int fd_output, int fd_error, struct dynamic_string_array *cmd_list, int *exit_status, struct dynamic_int_array *fd_list, struct dynamic_pid_array *pid_list);

void wait_for_children(struct dynamic_pid_array *pid_list, int *exit_status, int *signal_status);

void profile_check(int profile_flag, int who, char *option, double *total_user_time, double *total_system_time);

/********************************************************************/
/*Main Function*/

int main(int argc, char **argv)
{

  struct option long_options[] = 
    {
      {"append", no_argument, 0, 1},
      {"cloexec", no_argument, 0, 2},
      {"creat", no_argument, 0, 3},
      {"directory", no_argument, 0, 4},
      {"dsync", no_argument, 0, 5},
      {"excl", no_argument, 0, 6},
      {"nofollow", no_argument, 0, 7},
      {"nonblock", no_argument, 0, 8},
      {"rsync", no_argument, 0, 9},
      {"sync", no_argument, 0, 10},
      {"trunc", no_argument, 0, 11},
      {"rdonly", required_argument, 0, 12},
      {"rdwr", required_argument, 0, 13},
      {"wronly", required_argument, 0, 14},
      {"pipe", no_argument, 0, 15},
      {"command", required_argument, 0, 16},
      {"wait", no_argument, 0, 17},
      {"close", required_argument, 0, 18},
      {"verbose", no_argument, 0, 19},
      {"profile", no_argument, 0, 20},
      {"abort", no_argument, 0, 21},
      {"catch", required_argument, 0, 22},
      {"ignore", required_argument, 0, 23},
      {"default", required_argument, 0, 24},
      {"pause", no_argument, 0, 25}
    };

  struct dynamic_int_array fd_list = {NULL, 0, 0};
  struct dynamic_string_array cmd_list = {NULL, 0, 0};
  struct dynamic_pid_array pid_list = {NULL, NULL, 0, 0};

  int exit_status = 0;
  int child_status = 0;
  int signal_status = 0;
  int file_opener_flags = 0;
  int create_flags = 0;
  int verbose_flag = 0;
  int profile_flag = 0;
  int profile_children_flag = 0;
  double total_user_time = 0;
  double total_system_time = 0;
  int input_fd, output_fd, error_fd, input_fd_index, output_fd_index, error_fd_index;
  char* verbose_spacer = " ";
  char* verbose_ender = "\n";
  

  int option_received;
  while ((option_received = getopt_long(argc, argv, "", long_options, NULL)) != -1)
    {
      /*Add verbose flag check to all options*/
      switch(option_received)
	{
	case 1: /*append*/
	  verbose_flag_check("--append ", verbose_flag, 0);
	  file_opener_flags |= O_APPEND;
	  profile_check(profile_flag, RUSAGE_SELF, "append", &total_user_time, &total_system_time);
	  break;

	case 2: /*cloexec*/
	  verbose_flag_check("--cloexec ", verbose_flag, 0);
	  file_opener_flags |= O_CLOEXEC;
	  profile_check(profile_flag, RUSAGE_SELF, "cloexec", &total_user_time, &total_system_time);
	  break;

	case 3: /*creat*/
	  verbose_flag_check("--creat ", verbose_flag, 0);
	  file_opener_flags |= O_CREAT;
	  profile_check(profile_flag, RUSAGE_SELF, "creat", &total_user_time, &total_system_time);
	  break;

	case 4: /*directory*/
	  verbose_flag_check("--directory ", verbose_flag, 0);
	  file_opener_flags |= O_DIRECTORY;
	  profile_check(profile_flag, RUSAGE_SELF, "directory", &total_user_time, &total_system_time);
	  break;

	case 5: /*dsync*/
	  verbose_flag_check("--dsync ", verbose_flag, 0);
	  file_opener_flags |= O_DSYNC;
	  profile_check(profile_flag, RUSAGE_SELF, "dsync", &total_user_time, &total_system_time);
	  break;

	case 6: /*excl*/
	  verbose_flag_check("--excl ", verbose_flag, 0);
	  file_opener_flags |= O_EXCL;
	  profile_check(profile_flag, RUSAGE_SELF, "excl", &total_user_time, &total_system_time);
	  break;

	case 7: /*nofollow*/
	  verbose_flag_check("--nofollow ", verbose_flag, 0);
	  file_opener_flags |= O_NOFOLLOW;
	  profile_check(profile_flag, RUSAGE_SELF, "nofollow", &total_user_time, &total_system_time);
	  break;

	case 8: /*nonblock*/
	  verbose_flag_check("--nonblock ", verbose_flag, 0);
	  file_opener_flags |= O_NONBLOCK;
	  profile_check(profile_flag, RUSAGE_SELF, "nonblock", &total_user_time, &total_system_time);
	  break;

	case 9: /*rsync*/
	  verbose_flag_check("--rsync ", verbose_flag, 0);
	  file_opener_flags |= O_RSYNC;
	  profile_check(profile_flag, RUSAGE_SELF, "rsync", &total_user_time, &total_system_time);
	  break;

	case 10: /*sync*/
	  verbose_flag_check("--sync ", verbose_flag, 0);
	  file_opener_flags |= O_SYNC;
	  profile_check(profile_flag, RUSAGE_SELF, "sync", &total_user_time, &total_system_time);
	  break;

	case 11: /*trunc*/
	  verbose_flag_check("--trunc ", verbose_flag, 0);
	  file_opener_flags |= O_TRUNC;
	  profile_check(profile_flag, RUSAGE_SELF, "trunc", &total_user_time, &total_system_time);
	  break;

	case 12: /*rdonly*/
	  file_opener_flags |= O_RDONLY;
	  create_flags |= (S_IRUSR | S_IRGRP | S_IROTH);
	  verbose_flag_check("--rdonly ", verbose_flag, 1);
	  open_file(file_opener_flags, create_flags, &fd_list, &exit_status);
	  file_opener_flags = 0;
	  create_flags = 0;
	  profile_check(profile_flag, RUSAGE_SELF, "rdonly", &total_user_time, &total_system_time);
	  break;

	case 13: /*rdwr*/
	  file_opener_flags |= O_RDWR;
	  create_flags |= (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH);
	  verbose_flag_check("--rdwr ", verbose_flag, 1);
	  open_file(file_opener_flags, create_flags, &fd_list, &exit_status);
	  file_opener_flags = 0;
	  create_flags = 0;
	  profile_check(profile_flag, RUSAGE_SELF, "rdwr", &total_user_time, &total_system_time);
	  break;

	case 14: /*wronly*/
	  file_opener_flags |= O_WRONLY;
	  create_flags |= (S_IWUSR | S_IWGRP | S_IWOTH | S_IRUSR | S_IRGRP | S_IROTH);
	  verbose_flag_check("--wronly ", verbose_flag, 1);
	  open_file(file_opener_flags, create_flags, &fd_list, &exit_status);
	  file_opener_flags = 0;
	  create_flags = 0;
	  profile_check(profile_flag, RUSAGE_SELF, "wronly", &total_user_time, &total_system_time);
	  break;

	case 15: /*pipe*/
	  verbose_flag_check("--pipe ", verbose_flag, 0);
	  open_pipe(&fd_list, &exit_status);
	  profile_check(profile_flag, RUSAGE_SELF, "pipe", &total_user_time, &total_system_time);
	  break;

	case 16: /*command*/

	  verbose_flag_check("--command ", verbose_flag, 1);
	  
	  input_fd_index = atoi(optarg);

	  verbose_flag_check(argv[optind], verbose_flag, 0);
	  verbose_flag_check(verbose_spacer, verbose_flag, 0);

	  output_fd_index = atoi(argv[optind++]);

	  verbose_flag_check(argv[optind], verbose_flag, 0);

	  error_fd_index = atoi(argv[optind++]);


	  while(optind < argc  &&  argv[optind][1] != '-')
	    {
	      verbose_flag_check(verbose_spacer, verbose_flag, 0);
	      verbose_flag_check(argv[optind], verbose_flag, 0);
	      add_command_arg(argv[optind], &cmd_list);
	      optind++;
	    }

	  if (return_arg_count(&cmd_list) < 4)
	    {
	      fprintf(stderr, "Missing command in \"--command\" option\n");
	      exit_status = 1;
	      free_command_array(&cmd_list);
	      break;
	    }

	  verbose_flag_check(verbose_ender, verbose_flag, 0);
	  
	  /*Array to execvp needs to be null-terminated*/
	  add_command_arg(NULL, &cmd_list);

	  input_fd = retrieve_file_descriptor(input_fd_index, &fd_list);
	  output_fd = retrieve_file_descriptor(output_fd_index, &fd_list);
	  error_fd = retrieve_file_descriptor(error_fd_index, &fd_list);

	  execute_command(input_fd, output_fd, error_fd, &cmd_list, &exit_status, &fd_list, &pid_list);

	  profile_check(profile_flag, RUSAGE_SELF, "command", &total_user_time, &total_system_time);
	  break;
  
	case 17: /*wait*/
	  verbose_flag_check("--wait\n", verbose_flag, 0);
	  wait_for_children(&pid_list, &child_status, &signal_status);
	  profile_check(profile_flag, RUSAGE_SELF, "wait", &total_user_time, &total_system_time);
	  if (profile_flag) { profile_children_flag = 1; }
	  break;

	case 18: /*close*/
	  verbose_flag_check("--close ", verbose_flag, 1);
	  close_file_descriptor(atoi(optarg), &fd_list, &exit_status);
	  profile_check(profile_flag, RUSAGE_SELF, "close", &total_user_time, &total_system_time);
	  break;

	case 19: /*verbose*/
	  verbose_flag_check("--verbose ", verbose_flag, 0);
	  verbose_flag = 1;
	  profile_check(profile_flag, RUSAGE_SELF, "verbose", &total_user_time, &total_system_time);
	  break;

	case 20: /*profile*/
	  verbose_flag_check("--profile ", verbose_flag, 0);
	  profile_check(profile_flag, RUSAGE_SELF, "profile", &total_user_time, &total_system_time);
	  profile_flag = 1;
	  break;

	case 21: /*abort*/
	  verbose_flag_check("--abort", verbose_flag, 0);
	  {
	    int *mistake = NULL;
	    *mistake = 'm';
	  }
	  profile_check(profile_flag, RUSAGE_SELF, "abort", &total_user_time, &total_system_time);
	  break;

	case 22: /*catch*/
	  verbose_flag_check("--catch ", verbose_flag, 1);
	  signal(atoi(optarg), sig_handler);
	  profile_check(profile_flag, RUSAGE_SELF, "catch", &total_user_time, &total_system_time);
	  break;

	case 23: /*ignore*/
	  verbose_flag_check("--ignore ", verbose_flag, 1);
	  signal(atoi(optarg), SIG_IGN);
	  profile_check(profile_flag, RUSAGE_SELF, "ignore", &total_user_time, &total_system_time);
	  break;

	case 24: /*default*/
	  verbose_flag_check("--default ", verbose_flag, 1);
	  signal(atoi(optarg), SIG_DFL);
	  profile_check(profile_flag, RUSAGE_SELF, "default", &total_user_time, &total_system_time);
	  break;

	case 25: /*pause*/
	  verbose_flag_check("--pause ", verbose_flag, 0);
	  pause();
	  profile_check(profile_flag, RUSAGE_SELF, "pause", &total_user_time, &total_system_time);
	  break;

	default:
	  fprintf(stderr, "./simpsh: invalid argument. Usage: refer to https://web.cs.ucla.edu/classes/winter19/cs111/assign/lab1.html for usage details.\n");
	  exit_status = 1;
	  break;
	}
    }

  if (profile_children_flag)
    {
      profile_check(profile_flag, RUSAGE_CHILDREN, NULL, &total_user_time, &total_system_time);
    }

  free_file_descriptor_array(&fd_list);
  free_pid_array(&pid_list);
  if (signal_status)
    {
      signal(signal_status, SIG_DFL);
      fflush(NULL);
      raise(signal_status);
    }

  if (child_status > exit_status)
    {
      exit(child_status);
    }
  exit(exit_status);
}


/********************************************************************/
/*Auxillary Functions*/


void open_file(int flags, int mode, struct dynamic_int_array *fd_list, int *exit_status)
{
  int temp_file_descriptor = open(optarg, flags, mode);
      if (temp_file_descriptor == -1)
	{
	  fprintf(stderr, "Error opening file: %s\n", strerror(errno));
	  *exit_status = 1;
	}
      if (add_file_descriptor(temp_file_descriptor, fd_list) == -1)
	{
	  fprintf(stderr, "Could not add file descriptor.\n");
	  *exit_status = 1;
	}
}


void open_pipe(struct dynamic_int_array *fd_list, int *exit_status)
{
  int temp_pipe_fds[2];
  if (pipe(temp_pipe_fds) == -1)
    {
      fprintf(stderr, "Error using pipe(1): %s", strerror(errno));
      *exit_status = 1;
    }
  if (add_file_descriptor(temp_pipe_fds[0], fd_list) == -1)
    {
      fprintf(stderr, "Could not add file descriptor.\n");
      *exit_status = 1;
    }
  if (add_file_descriptor(temp_pipe_fds[1], fd_list) == -1)
    {
      fprintf(stderr, "Could not add file descriptor.\n");
      *exit_status = 1;
    }
}


void verbose_flag_check(char *option_string, int flag, int optarg_flag)
{
  if (flag)
    {
      int index = 0;
      char ch = option_string[index];
      char spacer = ' ';
      while (ch != '\0')
	{
	  if (write(1, &ch, 1) == -1)
	    {
	      fprintf(stderr, "Error writing --verbose: %s\n", strerror(errno));
	    }
	  ch = option_string[++index];
	}

      if (optarg_flag)
	{
	  index = 0;
	  ch = optarg[index];
	  while (ch != '\0')
	    {
	      if (write(1, &ch, 1) == -1)
		{
		  fprintf(stderr, "Error writing optarg: %s\n", strerror(errno));
		}
	      ch = optarg[++index];
	    }
	  if (write(1, &spacer, 1) == -1)
	    {
	      fprintf(stderr, "Error writing --verbose: %s\n", strerror(errno));
	    }
	} /*End of 'optarg_flag' if*/
    } /*End of 'flag' if*/
}


void sig_handler(int N)
{
  fprintf(stderr, "%i caught\n", N);
  exit(N);
}


void execute_command(int input_fd, int output_fd, int error_fd, struct dynamic_string_array *cmd_list, int *exit_status, struct dynamic_int_array *fd_list, struct dynamic_pid_array *pid_list)
{
  pid_t child;

  if (input_fd < 0 || output_fd < 0 || error_fd < 0)
    {
      fprintf(stderr, "Bad input, output, or error operand for \"--command\"\n");
      *exit_status = 1;
      free_command_array(cmd_list);
      return;
    }

  child = fork();

  if (child == -1)
    {
      fprintf(stderr, "Error using fork(): %s", strerror(errno));
      *exit_status = 1;
      free_command_array(cmd_list);
      return;
    }
  
  if (child > 0)
    {
      if (add_pid(child, cmd_list, pid_list) == -1)
	{
	  fprintf(stderr, "Error adding pid to pid array\n");
	}
      free_command_array(cmd_list);
      return;
    }

  if (child == 0)
    {
      
      if (input_fd = dup2(input_fd, 0) == -1)
	{ fprintf(stderr, "Error: %s\n", strerror(errno)); }
      
      if (output_fd = dup2(output_fd, 1) == -1)
	{ fprintf(stderr, "Error: %s\n", strerror(errno)); }
      
      if (output_fd = dup2(error_fd, 2) == -1)
	{ fprintf(stderr, "Error: %s\n", strerror(errno)); }
      
      
      /*Close all file descriptors from 3 on*/
      close_file_descriptors(fd_list);
      
      
      if (execvp(retrieve_arg(0, cmd_list), retrieve_command_array(cmd_list)) == -1)
	{
	  fprintf(stderr, "Error running execvp: %s\n", strerror(errno));
	  exit(1);
	}
    }
}


void wait_for_children(struct dynamic_pid_array *pid_list, int *exit_status, int *signal_status)
{
  int status = 0;
  int result = 0;
  for (int i = 0; i < pid_list->num_of_elements; i++)
    {
      if (waitpid(pid_list->array[i], &status, 0) == -1)
	{
	  fprintf(stderr, "Error waiting for process %i", pid_list->array[i]);
	}

      if(WIFEXITED(status))
	{
	  result = WEXITSTATUS(status);
	  if (result > *exit_status) { *exit_status = result; }
	  fprintf(stdout, "exit %i %s\n", result, pid_list->commands[i]);	  
	}

      if(WIFSIGNALED(status))
	{
	  result = WTERMSIG(status);
	  if (result > *signal_status) { *signal_status = result; }
	  fprintf(stdout, "signal %i %s\n", result, pid_list->commands[i]);
	}
    }

  free_pid_array(pid_list);
}

void profile_check(int profile_flag, int who, char* option, double *total_user_time, double *total_system_time)
{
  if (!profile_flag) { return; }

  struct rusage usage;
  if (getrusage(who, &usage) == -1)
    {
      fprintf(stderr, "Error calling getrusage(): %s\n", strerror(errno));
      return;
    }

  double user_time = (double)usage.ru_utime.tv_sec + ((double)usage.ru_utime.tv_usec / 1000000.0) - *total_user_time;
  double system_time = (double)usage.ru_stime.tv_sec + ((double)usage.ru_stime.tv_usec / 1000000.0) - *total_system_time;
  double total_time = user_time + system_time;

  *total_user_time = (double)usage.ru_utime.tv_sec + ((double)usage.ru_utime.tv_usec / 1000000.0);
  *total_system_time = (double)usage.ru_stime.tv_sec + ((double)usage.ru_stime.tv_usec / 1000000.0);
  
  if (who == RUSAGE_SELF)
    {
      fprintf(stdout, "%s times -- user: %lfs, system: %lfs, total: %lfs\n", option, user_time, system_time, total_time);
    }
  else if (who == RUSAGE_CHILDREN)
    {
      fprintf(stdout, "Children Proccesses' times -- user: %lfs, system: %lfs, total: %lfs\n", user_time, system_time, total_time);
    }
}
