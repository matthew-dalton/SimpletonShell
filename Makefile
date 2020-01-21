#NAME: Matthew Dalton
#EMAIL: mattdalton@g.ucla.edu
#ID: 304964539

CC=gcc
CFLAGS=-I -Wall -Wextra -g -std=c11 -D_POSIX_C_SOURCE -std=gnu99

simpsh: source_files/simpsh.c
	$(CC) $(CFLAGS) -o  simpsh  source_files/dynamic_int_array.h  source_files/dynamic_string_array.h  source_files/dynamic_pid_array.h  source_files/simpsh.c  source_files/file_openings.h  source_files/file_openings.c  source_files/subcommands.h  source_files/subcommands.c

.PHONY: clean

clean:
	rm -f simpsh lab1-304964539.tar.gz

.PHONY: dist

dist:
	tar -cvzf lab1-304964539.tar.gz  README  Makefile source_files/dynamic_int_array.h  source_files/dynamic_string_array.h  source_files/dynamic_pid_array.h  source_files/simpsh.c  source_files/file_openings.h  source_files/file_openings.c  source_files/subcommands.h  source_files/subcommands.c Report.pdf

.PHONY: check

check:
