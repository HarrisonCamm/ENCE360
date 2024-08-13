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

int main(void) {
	pid_t pid = 0;
	pid = fork();

	printf("pid is %i\n", pid);

	// Check for errors (we skip this in most examples, but you should always do it). 
	if (pid < 0) { 
		perror("fork"); // Print to stderr, "standard error". 
		exit(EXIT_FAILURE);

	} else if (pid == 0) {
		// Child code. 

		// Register signal handlers. 
		struct sigaction sa;

		sa.sa_handler = &sighup;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGHUP, &sa, NULL); 

		sa.sa_handler = &sigint;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGINT, &sa, NULL); 

		sa.sa_handler = &sigquit;
		sigemptyset(&sa.sa_mask);
		sigaction(SIGQUIT, &sa, NULL); 

		while (true) {
			sleep(1); // Block forever. 
		}

	} else {
		// Parent code. 
		printf("Parent processing starts\n");
		
		int childStatus = 0; // This will hold return code. 

		printf("\nPARENT: sending SIGHUP\n\n");
		sleep(1); // Give the child some time to set up its signal handlers. 
		kill(pid, SIGHUP);
		sleep(3); // Pause for 3 seconds
		
		printf("\nPARENT: sending SIGINT\n\n");
		kill(pid, SIGINT);
		sleep(3); // Pause for 3 seconds. 
		
		printf("\nPARENT: sending SIGQUIT\n\n");
		kill(pid, SIGQUIT);
		wait(&childStatus);
		
		printf("\nPARENT: doing something\n\n");
		sleep(3);
	}

	return EXIT_SUCCESS;
}

void sighup(int sigNum) {
	struct sigaction sa; 
	sa.sa_handler = &sighup;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGHUP, &sa, NULL); // Reset signal. 
	printf("CHILD: I have received a SIGHUP\n");
}

void sigint(int sigNum) {
	struct sigaction sa; 
	sa.sa_handler = &sigint;
	sigemptyset(&sa.sa_mask);
	sigaction(SIGINT, &sa, NULL); // Reset signal. 
	printf("CHILD: I have received a SIGINT\n");
}

void sigquit(int sigNum) {
	// We'll be exiting, so no need to reset signal in this case. 
	printf("CHILD: My DADDY has Killed me!!!\n");
	printf("CHILD: cleaning up...\n");
	sleep(2);
	exit(0);
}

