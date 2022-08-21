#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>

#include "measure.h"

void help() {
    printf(
    "measure - Run command and print used system resources\n"
    "          similar to time or GNU time utils. See getrusage(2)\n"
    "          for info about the output of this program\n"
    "\n"
    "Usage:\n"
    "       measure command [arguments...]\n"
    "\n"
    "Output:\n"
    " If command was executed succesfully, then output of the\n"
    " command is followed by a line of numbers showing usage\n"
    " of these resources:\n"
    " - utime stime maxrss minflt majflt inblock oublock nvcsw nivcsw\n"
    "\n"
    " If command failed, then program exits with the exit status\n"
    " of the command.\n"
    "\n"
    "Resources:\n"
    " - ttime         Total CPU time used (utime + stime)\n"
    " - utime         User CPU time in seconds (microsecond precision)\n"
    " - stime         System CPU time in seconds (microsecond precision)\n"
    " - maxrss        Maximum Resident Set Size in kilobytes\n"
    " - minflt        Soft page faults\n"
    " - majflt        Hard page faults\n"
    " - inblock       Block input operations via file system\n"
    " - oublock       Block output operations via file system\n"
    " - nvcsw         Voluntary context switches\n"
    " - nivcsw        Involuntary context switches\n"
    "\n"
    "Examples:\n"
    "  $ measure -h (prints this message)\n"
    "  $ measure ls -la\n"
    "  $ measure expr 365 \\* 24 \\* 60 \\* 60\n");
}

void print_result(struct rusage *r) {

    double total_t, user_t, system_t;

    user_t = r->ru_utime.tv_sec + (r->ru_utime.tv_usec / 1000000.0);
    system_t = r->ru_stime.tv_sec + (r->ru_stime.tv_usec / 1000000.0);
    total_t = user_t + system_t;
    printf("%.6f", total_t);
    printf(" %.6f", user_t);
    printf(" %.6f", system_t);
    printf(" %ld", r->ru_maxrss);
    printf(" %ld", r->ru_minflt);
    printf(" %ld", r->ru_majflt);
    printf(" %ld", r->ru_inblock);
    printf(" %ld", r->ru_oublock);
    printf(" %ld", r->ru_nvcsw);
    printf(" %ld", r->ru_nivcsw);
	printf("\n");
}

int main(int argc, char *argv[]) {

    char *cmd[argc];
	struct rusage *r;
    int i;

    // check arguments, at least one argument should be provided
    if (argc == 1) {
        printf("Usage: %s command [arguments...]\n", argv[0]);
        return 1;
    }

	if ( strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
        help();
        return 0;
    }

    // new argument vector for the command to measure
    for (i=0; i<argc-1; i++) {
        cmd[i] = argv[i+1];
    }
    cmd[i] = NULL; // should be NULL-terminated

	r = measure(cmd);
	if ( r == NULL ) {
		return 1;
	}

	print_result(r);
	free(r);

    return 0;
}
