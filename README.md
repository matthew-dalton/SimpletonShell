# SimpletonShell

SimpletonShell is a robust, stripped down shell that allows for arbitrary directed pipe graphs (rather than only a pipeline), the opening of files using the flags available at the syscall level (e.g. the O_DSYNC flag), and simpler invocation through other programs and scripts (useful for automated processes).

### Example of Use

```
simpsh \
  --rdonly a \
  --pipe \
  --pipe \
  --creat --trunc --wronly c \
  --creat --append --wronly d \
  --command 3 5 6 tr A-Z a-z \
  --command 0 2 6 sort \
  --command 1 4 6 cat b - \
  --close 2 \
  --close 4 \
  --wait
```

This examples creates seven file descriptors:

(1) A read only descriptor for the file a, created by the --rdonly option.
(2) The read end of the first pipe, created by the first --pipe option.
(3) The write end of the first pipe, also created by the first --pipe option.
(4) The read end of the second pipe, created by the second --pipe option.
(5) The write end of the second pipe, also created by the second --pipe option.
(6) A write only descriptor for the file c, created by the first --wronly option as modified by the preceding --creat and --trunc.
(7) A write only, append only descriptor for the file d, created by the --wronly option as modified by the preceding --creat and --append options.

It then creates three subprocesses:

- A subprocess with standard input, output, and error being the file descriptors numbered 3, 5, and 6 above. This subprocess runs the command tr with the two arguments A-Z and a-z.
- A subprocess with standard input, standard output, and standard error being the file descriptors numbered 0, 2, and 6 above, respectively. This subprocess runs the command sort with no arguments
- A subprocess with standard input, output, and error being the file descriptors numbered 1, 4, and 6 above. This subprocess runs the command cat with the two arguments b and -.

### Description of Options

#### Flags for file openings:

`--append`
O_APPEND

`--cloexec`
O_CLOEXEC

`--creat`
O_CREAT

`--directory`
O_DIRECTORY

`--dsync`
O_DSYNC

`--excl`
O_EXCL

`--nofollow`
O_NOFOLLOW

`--nonblock`
O_NONBLOCK

`--rsync`
O_RSYNC

`--sync`
O_SYNC

`--trunc`
O_TRUNC


#### File-opening Options:

`--rdonly f`

O_RDONLY. Open the file f for reading only.

`--rdwr f`

O_RDWR. Open the file f for reading and writing.

`--wronly f`

O_WRONLY. Open the file f for writing only.

`--pipe`

Open a pipe. Unlike the other file options, this option does not take an argument. Also, it consumes two file numbers, not just one.

#### Subcommand Options:

`--command i o e cmd args`

Execute a command with standard input i, standard output o and standard error e; these values should correspond to earlier file or pipe options. The executable for the command is cmd and it has zero or more arguments args. None of the cmd and args operands begin with the two characters "--".

`--wait`

Wait for all commands to finish. As each finishes, output its exit status or signal number as described above, and a copy of the command (with spaces separating arguments) to standard output.

#### Miscellaneous Options:

`--close N`

Close the Nth file that was opened by a file-opening option. For a pipe, this closes just one end of the pipe. Once file N is closed, it is an error to access it, just as it is an error to access any file number that has never been opened. File numbers are not reused by later file-opening options.

`--verbose`

Just before executing an option, output a line to standard output containing the option. If the option has operands, list them separated by spaces. Ensure that the line is actually output, and is not merely sitting in a buffer somewhere.

`--profile`

Just after executing an option, output a line to standard output containing the resources used. Use getrusage and output a line containing as much useful information as you can glean from it.

`--abort`

Crash the shell. The shell itself should immediately dump core, via a segmentation violation.

`--catch N`

Catch signal N, where N is a decimal integer, with a handler that outputs the diagnostic N caught to stderr, and exits with status N. This exits the entire shell. N uses the same numbering as your system; for example, on GNU/Linux, a segmentation violation is signal 11.

`--ignore N`

Ignore signal N.

`--default N`

Use the default behavior for signal N.

`--pause`

Pause, waiting for a signal to arrive.


***Project Specification Provided by [Paul Eggert](http://web.cs.ucla.edu/classes/winter19/cs111/assign/lab1.html)***
