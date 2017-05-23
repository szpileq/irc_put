#include "../include/sock_help.h"

void *get_in_addr(struct sockaddr *sa)
{
	if(sa->sa_family == AF_INET)
		return &(((struct sockaddr_in *) sa)->sin_addr);

	else
		return &(((struct sockaddr_in6 *) sa)->sin6_addr);

}

int get_tcp_socket(const char *port, char *hostname, char flags)
{

	int fd  = 0;
	int rv  = 0;
#ifdef __linux__
	int yes = 1;
#elif _WIN32
    char yes = 1;
#endif


#ifdef __linux__
	char addr_str[INET6_ADDRSTRLEN];
#endif

	struct addrinfo hints;
	struct addrinfo *address_list;
	struct addrinfo *p = NULL;

#ifdef _WIN32
	WSADATA wsaData;
	unsigned long mode = 1;
#endif

	memset(&hints, 0, sizeof(hints));
    hints.ai_family        = AF_INET;
	hints.ai_socktype      = SOCK_STREAM;

	if(hostname == NULL) {
		hints.ai_flags = AI_PASSIVE;
	}

#ifdef _WIN32

    if (WSAStartup(MAKEWORD(2,0), &wsaData) != 0) {

		fprintf(stderr, "WSAStartup failed.\n");
		return -1;

	}
#endif
	
printf("PORT %s\n", port);

    if((rv = getaddrinfo(hostname, "2222", &hints, &address_list)) != 0) {
#ifdef __linux__
		fprintf(stderr, "Error: getaddrinfo failed with status: %s\n",
				gai_strerror(rv));

#elif _WIN32
        fprintf(stderr, "Error: getaddrinfo failed with status: %d\n",
                rv);

#endif
		return -1;

	}

	for(p = address_list; p != NULL; p = p->ai_next) {

#ifdef __linux__
        inet_ntop(p->ai_family, get_in_addr((struct sockaddr *) p->ai_addr),
                    addr_str, sizeof(addr_str));

        printf("get_tcp_socket: Trying to connect to %s...\n", addr_str);
#endif


		if((fd = socket(p->ai_family, p->ai_socktype,
						p->ai_protocol)) == -1) {
			fprintf(stderr, "Error: socket() failed with status: "
					"%s\n", strerror(errno));
			continue;

		}

        if((flags & SOCKET_REUS) && setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes,
						sizeof(int)) != 0) {

			fprintf(stderr, "Error: setsockopt() failed with status: "
					"%s\n", strerror(errno));
			return -1;

		}

		if((flags & SOCKET_BIND) && bind(fd, p->ai_addr, p->ai_addrlen) == -1) {

			fprintf(stderr, "Error: bind() failed with status: "
					"%s\n", strerror(errno));
#ifdef __linux__ 
			close(fd);
#elif _WIN32
			closesocket(fd);
#endif
			continue;

		}

		if((flags & SOCKET_CONN) && connect(fd, p->ai_addr, p->ai_addrlen) == -1) {

			fprintf(stderr, "Error: connect() failed with status: "
					"%s\n", strerror(errno));
#ifdef __linux__ 
			close(fd);
#elif _WIN32
			closesocket(fd);
#endif
			continue;

		}

#ifdef __linux__
		if((flags & SOCKET_NOBL) && fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {

			fprintf(stderr, "Error: fcntl() failed with status: "
					"%s\n", strerror(errno));
			close(fd);
			continue;

		}
#elif _WIN32
		if((flags & SOCKET_NOBL) && ioctlsocket(fd, FIONBIO, &mode) != 0) {

			fprintf(stderr, "Error: fcntl() failed with status: "
					"%s\n", strerror(errno));
            closesocket(fd);
			continue;

		}

#endif
		break;

	}

	if(p == NULL) {

		fprintf(stderr, "Error: failed to bind to any socket\n");
		return -1;

	}

	freeaddrinfo(address_list);

	printf("get_tcp_socket: Connected!\n");
	return fd;

}
/*
int poor_send(const int socket, const char *message){

    struct poorIRC_message msg;

    msg.len = strlen(message) + 1;
    printf("len sent: %d\nlen originally: %d", msg.len, strlen(message)+1);
    if(0 == msg.len - 1)
        return -1;  you have to actually send something
    else if (POORIRC_MSG_MAX_LEN < msg.len){
        return -1;  too much
    }
    else{
        if (-1 == send(socket, (char *)&(msg.len), sizeof(msg.len), 0)){
            return -1;  error with sending
        }
        strncpy(msg.body,message, msg.len);
    }
    Sleep(1000);
    if(-1 == (send(socket, (char *)&(msg.body), msg.len, 0))) {
        return -1; /* error with sending actual message
    }
    printf("orginal: %s\nlength: %c\nsent: %s", message,msg.len, msg.body);
    return 0;
}

int poor_recv(const int socket, struct poorIRC_message_srv *rsp ){


    if(-1 == (recv(socket, (char *)&(rsp->len), sizeof(rsp->len), 0))) {
        return -1;
    }
    printf("received lenght: %d\n",rsp->len);
    if(-1 == (recv(socket, (char *)&rsp->body, rsp->len, 0))) {
        return -1;
    }
    printf("received string: %s\n", rsp->body );

    return 0;

}
*/

#ifdef __linux__
void modify_tcp_socket(int fd, char flags)
{
	int yes = 1;

	if((flags & SOCKET_REUS) && setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes,
					sizeof(int)) != 0) {

		fprintf(stderr, "Error: setsockopt() failed with status: "
				"%s\n", strerror(errno));

	}

	if((flags & SOCKET_NOBL) && fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {

		fprintf(stderr, "Error: fcntl() failed with status: "
				"%s\n", strerror(errno));

	}

}
#endif

