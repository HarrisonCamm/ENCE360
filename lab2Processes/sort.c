#include <stdio.h>
#include <stdlib.h>

int main() {
    // Define the command to be executed
    char *command = "/bin/sort -k +7 < my.file";

    // Execute the command
    int result = system(command);

    // Check if the command was executed successfully
    if (result == -1) {
        perror("Error executing sort command");
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

