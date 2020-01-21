CC=gcc
CFLAGS=-I -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE -std=gnu99

simpsh: source_files/simpsh.c
	$(CC) $(CFLAGS) -o  simpsh  dynamic_int_array.h  dynamic_string_array.h  dynamic_pid_array.h  simpsh.c  file_openings.h  file_openings.c  subcommands.h  subcommands.c

.PHONY: clean 

clean:
	rm -f simpsh lab1-304964539.tar.gz

.PHONY: dist

dist:
	tar -cvzf lab1-ID_NUMBER.tar.gz  README  Makefile dynamic_int_array.h  dynamic_string_array.h  dynamic_pid_array.h  simpsh.c  file_openings.h  file_openings.c  subcommands.h  subcommands.c

# add test cases to add functionality for check command
.PHONY: check

check:
