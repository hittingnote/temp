int main(int argc, char *argv[])
{
	/* Create a server TCP socket */
	server_socket = socket(PF_INET, SOCK_STREAM);

	/* Attach cBPF program to server_socket */
	setsockopt(server_socket, SOL_SOCKET, SO_ATTACH_FILTER, &bpf, sizeof(bpf));

	/* Bind server_socket with local port */
	bind(server_socket, &server_addr, sizeof(server_addr));

	/* Listen to the requests from clients */
	listen(server_socket);

	/* Accept the arrived request from the client */
	client_socket = accept(server_socket, &client_addr, &length);

	/* Receive messages from client and copy them to recvstr */
	recv(client_socket, recvstr);

	/* Send messages in sendstr to client */
	send(client_socket, sendstr);

	/* close sockets */
	close(client_socket);
	close(server_socket);

	return 0;
}


