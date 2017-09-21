#ifndef SOCKET_T
#define SOCKET_T

struct serversocket_t {
	int data;
};

int serversocket_new(int port, int socket_queue_size) ;

int serversocket_accept(int self) ;

void serversocket_close(int self) ;

long socket_write(int self, const char* message, long length) ;

long socket_read(int self, char* buffer, long length) ;

void socket_close(int self) ;


#endif
