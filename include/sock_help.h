#ifndef SOCK_HELP_H
#define SOCK_HELP_H

#ifdef __linux__ 
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#elif _WIN32
#include <Ws2tcpip.h>
#include <winsock2.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "poorIRC_proto.h"

#define SOCKET_BIND 0x01
#define SOCKET_CONN 0x02
#define SOCKET_REUS 0x04
#define SOCKET_NOBL 0x08

void *get_in_addr(struct sockaddr *sa);
int get_tcp_socket(const char *port, char *hostname, char flags);
int poor_send(const int socket, const char *message);
int poor_recv(const int socket, struct poorIRC_message_srv *rsp );

#ifdef __linux__
void modify_tcp_socket(int fd, char flags);
#endif /* __linux */

#endif
