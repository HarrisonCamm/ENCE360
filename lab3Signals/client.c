#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCK_PATH "./mySocket"

int main(void)
{
	// Make single connection socket. 
	int sockfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);

	// Configuration struct
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCK_PATH, strlen(SOCK_PATH)+1);
	

	// Use this config struct to connect to the server. 
	connect(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr));
	

	// Now we can communicate. 
	char data[512] = {'\0'};
	ssize_t numBytes = 511;
	while (numBytes > 0) { // Loop while we get a response. 
		numBytes = sprintf(data, "Hello server!");
		write(sockfd, data, numBytes);
		numBytes = read(sockfd, data, 511);
		data[numBytes] = '\0';
		printf("From server: %s\n", data);
	}


	// We close the socket. 
	close(sockfd);
}