#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/filter.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <stdlib.h>



int main(int argc, char *argv[])
{
	struct sockaddr_in client_addr;
	bzero(&client_addr, sizeof(client_addr));
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = htons(INADDR_ANY);
	client_addr.sin_port = htons(8880);

	/* Create a client TCP socket */
	int client_socket = socket(AF_INET, SOCK_STREAM, 0);
	if(client_socket < 0)
	{
		perror("socket");
		exit(1);
	}

	/* Bind client_socket with local port */
	if(bind(client_socket, (struct sockaddr*)(&client_addr), sizeof(client_addr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	/* Connect to remote server */
	struct sockaddr_in server_addr;
	bzero(&server_addr, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	inet_aton("172.17.0.4", &server_addr.sin_addr);
	server_addr.sin_port = htons(12345);
	if(connect(client_socket, (struct sockaddr*)(&server_addr), sizeof(server_addr)) < 0)
	{
		perror("connect");
		exit(1);
	}

	/* Send and receive messages */
	char sendstr[] = "I am client.";
	char recvstr[1024];
	send(client_socket, sendstr, strlen(sendstr)+1, 0);
	recv(client_socket, recvstr, 1024, 0);
	printf("%s\n", recvstr);

	close(client_socket);

	return 0;
}

