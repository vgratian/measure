
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <time.h>

#define BILLION 1000000000.0

static int run_command(char**);

// Executes the command 'cmd' and returns resource usage data 'rusage', even
// when executing the command failed or it exited with errors. Returns non-0
// if argument is invalid or unexpected error occured before executing cmd.
//
// Arguments:
//    - cmd     NULL-terminated array of strings. First element is the program
//              name or path followed by its arguments. 
//	  - r		Pointer to struct that will contain resource usage stats
//    - t		Pointer to double that will contain wall time in seconds 
int measure(char **cmd, struct rusage *r, double *t) {

	struct timespec s, e;

	if ( clock_gettime(CLOCK_REALTIME, &s) != 0 ) {
		perror("clock_gettime");
		return 1;
	}

	if ( run_command(cmd) != 0 ) {
		return 1;
	}

	if ( clock_gettime(CLOCK_REALTIME, &e) != 0 ) {
		perror("clock_gettime");
		return 1;
	}

	r = malloc(sizeof(struct rusage));
	if ( r == NULL ) {
		perror("malloc");
		return 1;
	}

	if ( getrusage(RUSAGE_CHILDREN, r) != 0 ) {
		perror("getrusage");
		return 1;
	}

	*t = (double) (e.tv_sec - s.tv_sec ) + (double) (e.tv_nsec - s.tv_nsec) / BILLION;

	return 0;
}

// Execute command in background and wait for it to exit.
static int run_command(char **cmd) {
	int exit_code;
	pid_t pid;

    // fork and let the child run the command
    switch (fork()) {
    case -1: // fork failed
        perror("fork");
		return 1;
    case 0: // child execs command
        execvp(cmd[0], cmd);
        perror("execvp");
		return 1;
    } // parent continues

    // parent waits for child
    if ( (pid = wait(&exit_code)) == -1) {
        perror("wait");
        return 1;
    }

	// if child exited abnormally, print warning, but continue
	if (exit_code != 0) {
        fprintf(stderr, "command (PID=%d) exited with %d\n", pid, exit_code);
    }

	return 0;
}
