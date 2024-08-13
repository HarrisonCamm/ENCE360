#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "./mySocket.socket"

void manageFork(int sockfd)
{
	if (fork() == 0) {
		printf("Connection accepted\n");
		// Spawn child, loop until connection dropped. 
		char data[512] = {'\0'};
		ssize_t numBytes = read(sockfd, data, 511);
		while (numBytes > 0) {
			printf("From client: %s\n", data);
			numBytes = sprintf(data, "Hello to you too!");
			write(sockfd, data, numBytes); 
			numBytes = read(sockfd, data, 511);
			data[numBytes] = '\0';
		}
		exit(0);
	} else {
		// Either parent or error. Regardless, we don't want sockfd open in this process. 
		close(sockfd);
		// Immediate return to accept more connections. 
	}
}




int main(void)
{
	int sock = socket(AF_UNIX, SOCK_SEQPACKET, 0);

	// Configuration struct
	struct sockaddr_un name;
	memset(&name, 0, sizeof(struct sockaddr_un));
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCK_PATH, strlen(SOCK_PATH));

	// Use this config struct on the socket. 
	bind(sock, (struct sockaddr *)&name, sizeof(struct sockaddr));

	// Mark ready for connections. 
	listen(sock, 10); // 10 is how long our connection queue is. 


	// Now we start accepting connections. We need a new file descriptor for each connection. 
	while (true) {
		int sockfd = accept(sock, NULL, NULL); // Blocking. 
		manageFork(sockfd);
	}

	// We close the socket and the file descriptor we accepted via it. 
	close(sock);

	unlink(SOCK_PATH); // Lets the OS know it can clean up.
}