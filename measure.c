
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <sys/resource.h>

static int run_command(char**);

// Executes the command 'cmd' and returns resource usage data 'rusage', even
// when executing the command failed or it exited with errors. Returns NULL
// if argument is invalid or unexpected error occured before executing cmd.
//
// Argument:
//    - cmd     NULL-terminated array of strings. First element is the program
//              name or path followed by its arguments. 
struct rusage *measure(char **cmd) {

	struct rusage *r;

	if ( run_command(cmd) != 0 ) {
		return NULL;
	}

	r = malloc(sizeof(struct rusage));
	if ( r == NULL ) {
		perror("malloc");
		return NULL;
	}

	if ( getrusage(RUSAGE_CHILDREN, r) != 0 ) {
		perror("getrusage");
		return NULL;
	}

	return r;
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
