#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>

int parent_to_child[2];
int child_to_parent[2];
static char message[BUFSIZ];
int child_status, size;

#define INP 1
#define OUTP 0


// char* toUpper(char* string_in) {
//     char* string_out;
//     int i = 0; 
//     while (string_in + i != NULL)
//     {
//         string_out + i = string_in[i];
//     }
// }

void to_uppercase(char* str) {
    int i = 0;
    while (*str != NULL)
    {
        i++;
        str[i] = toupper(str[i]);
    }   
}


int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "Usage: %s message\n", argv[0]);
        exit(1);
    }

    /* Create pipes */
    if (pipe(parent_to_child) == -1) {
        perror("Pipe from");
        exit(2);
    }
    if (pipe(child_to_parent) == -1) {
        perror("Pipe - to");
        exit(2);
    }

    switch (fork()) { /* Fork a new process */

    case -1: /* Error forking */
        perror("Fork");
        exit(3);
        break; // Not needed, just good practice. 

    case 0: /* Child code */
        close(parent_to_child[INP]);
        close(child_to_parent[OUTP]);
        to_uppercase(message);

        /* Read from parent */
        if (read(parent_to_child[OUTP], message, BUFSIZ) != -1) {
            printf("CHILD: Recieved %s\n", toUpper(message));
        }
        else {
            perror("Read");
            exit(4);
        }

        /* Close pipes */
        close(parent_to_child[OUTP]);
        close(child_to_parent[INP]);
        break;

    default: /* Parent code */
        close(parent_to_child[OUTP]);
        close(child_to_parent[INP]);

        if (write(parent_to_child[INP], argv[1], strlen(argv[1])) != -1) {
            printf("PARENT: Sent %s\n", argv[1]);
        }
        else {
            perror("Write");
            exit(5);
        }

        // Wait for the child to exit - don't worry too much about the details of this yet. 
        wait(&child_status);

        /* Close pipes */
        close(parent_to_child[INP]);
        close(child_to_parent[OUTP]);
    }

    return EXIT_SUCCESS;
}
