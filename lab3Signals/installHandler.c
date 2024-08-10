#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Static variable to count the number of SIGQUIT signals received
static int sigquit_count = 0;

// Signal handler function
void handle_sigquit(int signum) {
    // Print "SIGQUIT\n" using non-buffered I/O
    write(1, "SIGQUIT\n", 8);

    // Increment the counter
    sigquit_count++;

    // If this is the second SIGQUIT signal, exit the process
    if (sigquit_count == 2) {
        exit(0);
    }
}

// Function to install the signal handler
void installHandler(void) {
    // Install the signal handler for SIGQUIT
    signal(SIGQUIT, handle_sigquit);
}

int main() {
    // Install the signal handler
    installHandler();

    // Infinite loop to keep the program running
    while (1) {
        pause(); // Wait for signals
    }

    return 0;
}