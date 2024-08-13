#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <sys/types.h>  /* defines pid_t */
#include <unistd.h>	 /* defines fork() */
#include <sys/wait.h>   /* defines the wait() system call. */
#include <signal.h>

 /* Function prototypes */
void sighup(int sigNum); /* routines child will call upon sigtrap */
void sigint(int sigNum);
void sigquit(int sigNum);
void waitChild(int sigNum); /* routine parent will call upon SIGCHLD */

int main(void) {
    pid_t pid = 0;

    // Register signal handler for SIGCHLD
    signal(SIGCHLD, &waitChild);

    pid = fork();

    printf("pid is %i\n", pid);

    // Check for errors (we skip this in most examples, but you should always do it). 
    if (pid < 0) {
        perror("fork"); // Print to stderr, "standard error". 
        exit(EXIT_FAILURE);

    } else if (pid == 0) {
        // Child code. 

        // Register signal handlers. 
        signal(SIGHUP, &sighup);
        signal(SIGINT, &sigint);
        signal(SIGQUIT, &sigquit);

        while (true) {
            sleep(1); // Block forever. 
        }

    } else {
        // Parent code. 
        printf("Parent processing starts\n");

        printf("\nPARENT: sending SIGHUP\n\n");
        sleep(1); // Give the child some time to set up its signal handlers. 
        kill(pid, SIGHUP);
        sleep(3); // Pause for 3 seconds

        printf("\nPARENT: sending SIGINT\n\n");
        kill(pid, SIGINT);
        sleep(3); // Pause for 3 seconds. 

        printf("\nPARENT: sending SIGQUIT\n\n");
        kill(pid, SIGQUIT);

        // Now we wait for SIGCHLD to be caught, which will trigger waitChild()
        printf("\nPARENT: waiting for child to terminate\n\n");

        // Continue doing something else if needed
        while (true) {
            sleep(1); // Simulate doing other work
        }
    }

    return EXIT_SUCCESS;
}

void sighup(int sigNum) {
    signal(SIGHUP, &sighup); // Reset signal. 
    printf("CHILD: I have received a SIGHUP\n");
}

void sigint(int sigNum) {
    signal(SIGINT, &sigint); // Reset signal. 
    printf("CHILD: I have received a SIGINT\n");
}

void sigquit(int sigNum) {
    // We'll be exiting, so no need to reset signal in this case. 
    printf("CHILD: My DADDY has Killed me!!!\n");
    printf("CHILD: cleaning up...\n");
    sleep(2);
    exit(0);
}

void waitChild(int sigNum) {
    int childStatus;
    wait(&childStatus);
    printf("\nPARENT: Child terminated, status: %d\n", childStatus);
    exit(EXIT_SUCCESS);
}