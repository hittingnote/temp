int main(int argc, char *argv[])
{
	client_addr.sin_port = htons(8880);

	/* Create a client TCP socket */
	client_socket = socket(AF_INET, SOCK_STREAM);

	/* Bind client_socket with local port */
	bind(client_socket, &client_addr, sizeof(client_addr));

	/* Connect to remote server */
	inet_aton("172.17.0.4", &server_addr.sin_addr);
	connect(client_socket, &server_addr, sizeof(server_addr));

	/* Send messages in sendstr to remote server */
	send(client_socket, sendstr);

	/* Receive messages from remote server and copy them to recvstr */
	recv(client_socket, recvstr);

	/* close client socket */
	close(client_socket);

	return 0;
}

