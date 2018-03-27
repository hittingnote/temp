#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/filter.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <linux/if_ether.h>
#include <stdlib.h>


struct sock_filter code[] = {
	{ 0x28,  0,  0, 0x00000000 },
	{ 0x15,  1,  0, 0x000022b0 },
	{ 0x06,  0,  0, 0x00000000 },
	{ 0x06,  0,  0, 0xffffffff },
};


struct sock_fprog bpf;


int main(int argc, char *argv[])
{
	struct sockaddr_in server_addr;
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htons(INADDR_ANY);
	server_addr.sin_port = htons(12345);

	bpf.len = sizeof(code) / sizeof(code[0]);
	bpf.filter = code;

	/* Create a server TCP socket */
	int server_socket = socket(PF_INET, SOCK_STREAM, 0);
	if(server_socket < 0)
	{
		perror("socket");
		exit(1);
	}

	/* Attach cBPF program to server_socket */
	if(setsockopt(server_socket, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	/* Bind server_socket with local port */
	if(bind(server_socket, (struct sockaddr*)(&server_addr), sizeof(server_addr)) < 0)
	{
		perror("bind");
		exit(1);
	}

	/* Listen to the requests from clients */
	if(listen(server_socket, 50) < 0)
	{
		perror("listen");
		exit(1);
	}

	/* Accept the arrived request from the client */
	struct sockaddr_in client_addr;
	socklen_t length = sizeof(client_addr);
	int client_socket = accept(server_socket, (struct sockaddr*)(&client_addr), &length);
	if(client_socket < 0)
	{
		perror("accept");
		exit(1);
	}

	/* Send and receive messages */
	char sendstr[] = "I am server.";
	char recvstr[1024];
	bzero(recvstr, 1024);
	if(recv(client_socket, recvstr, 1024, 0) < 0)
	{
		perror("recv");
		exit(1);
	}

	printf("%s\n", recvstr);

	if(send(client_socket, sendstr, strlen(sendstr)+1, 0) < 0)
	{
		perror("send");
		exit(1);
	}

	close(client_socket);
	close(server_socket);

	return 0;
}


