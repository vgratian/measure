#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "measure.h"
#include "print.h"

void help() {
    printf(
    "measure - run command and report used system resources\n"
    "\n"
    "Description\n"
    " This program is similar to the bash builtin time or GNU time.\n"
    " It reports resources used by a program (including its children\n"
    " that were waited for), such as time and memory.\n"
    "\n"
    "Usage:\n"
    "       measure [options] command [arguments...]\n"
    "\n"
    "Options:\n"
    "   -p     pretty print, i.e. print each metric name and value on\n"
    "          a new line. Default is to print space-separated values\n"
    "          only.\n"
    "\n"
    "Metrics:\n"
    " The output of the program comprises the metrics listed below (in\n"
    " the same order). For detailed info about them, see getrusage(2)\n"
    "\n"
    "Resources:\n"
    "   total      Total CPU time (user + sys time)\n"
    "   user       User CPU time in seconds (microsecond precision)\n"
    "   sys        System CPU time in seconds (microsecond precision)\n"
    "   maxrss     Maximum Resident Set Size in kilobytes\n"
    "   minflt     Soft page faults\n"
    "   majflt     Hard page faults\n"
    "   inblock    Block input operations via file system\n"
    "   oublock    Block output operations via file system\n"
    "   nvcsw      Voluntary context switches\n"
    "   nivcsw     Involuntary context switches\n"
    "\n"
    "Examples:\n"
    "  $ measure sleep 1\n"
    "  $ measure -p sleep 1\n"
    "  $ measure expr 365 \\* 24 \\* 60 \\* 60\n");
}

int main(int argc, char *argv[]) {

    char *cmd[argc];
	struct rusage *r;
    int pprint=0, i=0, k=1;

    // check arguments, at least one argument should be provided
    if (argc == 1) {
        printf("Usage: %s command [arguments...]\n", argv[0]);
        return 1;
    }

	if ( strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
    }

	if ( strcmp(argv[1], "-p") == 0 ) {
		pprint=1;
		k++;
	}

    // new argument vector for the command to measure
    while (i<argc-1) {
        cmd[i++] = argv[k++];
    }
    cmd[i] = NULL; // should be NULL-terminated

	r = measure(cmd);
	if ( r == NULL ) {
		return 1;
	}

	rusage_print(r, pprint);
	free(r);

    return 0;
}
