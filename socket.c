#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "socket.h"

int serversocket_new(int port, int socket_queue_size) {
	int listenfd = socket(AF_INET, SOCK_STREAM, 0);
	if (listenfd == -1) {
		return -1;
	}
	struct sockaddr_in serv_addr;
	memset(&serv_addr, '0', sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);
	if (bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) == -1) {
		close(listenfd);
		return -1;
	}
	if (listen(listenfd, socket_queue_size) == -1) {
		close(listenfd);
		return -1;
	}
	return listenfd;
}

int serversocket_accept(int self) {
	int socket = accept(self, (struct sockaddr*) NULL, NULL);
	if (socket < 0) {
		return -1;
	}
	struct timeval timeout;
	timeout.tv_sec = 10;
	timeout.tv_usec = 0;
	if (setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
		close(socket);
		return -1;
	}
	if (setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char *) &timeout, sizeof(timeout)) < 0) {
		close(socket);
		return -1;
	}
	return socket;
}

void serversocket_close(int self) {
	close(self);
}

long socket_write(int self, const char* message, long length) {
	long count = write(self, message, length);
	if (count < 0 || count != length) {
		return -1;
	}
	return count;
}

long socket_read(int self, char* buffer, long length) {
	return read(self, buffer, length);
}

void socket_close(int self) {
	close(self);
}
