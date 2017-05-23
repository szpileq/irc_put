/*
 * Definitions of structures and functions used by poorIRC server
 */

#ifndef _POORIRC_H
#define _POORIRC_H

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <semaphore.h>

#include "../../include/poorIRC_proto.h"

#define POORIRC_MODE_DEBUG 0x01
#define POORIRC_MODE_BG    0x02

#define POORIRC_MAX_CLIENTS 32


/*
 * struct poorIRC_config
 * Structure holding configuration of poorIRC server.
 *
 * @max_connections - number of clients able to join at once.
 *
 * @mode - char representing operation mode (server can operate on different
 * modes - eg. daemon or not)
 *
 * @port_no - port on which TCP port should the server operate.
 */

struct poorIRC_config {

	unsigned short int max_connections;
	unsigned char      mode;
	char               port_no[6];
	/* ... */

};

struct poorIRC_server_shared_buffer {

	/* Shared between forked processes */
	sem_t buffer_mutex;

};

struct poorIRC_server_client_entry {

	pid_t pid;
	int active;
	char nickname[POORIRC_NICKNAME_MAX_LEN];

	struct poorIRC_message_srv buffer;
	int dirty;



};

struct poorIRC_server_client_lookup_table {

	struct poorIRC_server_client_entry lookup_table[POORIRC_MAX_CLIENTS];
	int clients_no;
	sem_t mutex;

};

/*
 * struct poorIRC_server
 * This structure represents an instance of a server. It contains data required
 * for server operation (socket file descriptors, IP configuration, server
 * config, etc.) 
 */

struct poorIRC_server {

	/* Server configuration */
	const struct poorIRC_config *config;

	/* Socket FD's */
	int listen_fd;
	int client_fd;

	/* Internet Address of current client */
	struct sockaddr_storage client_addr;

	/*
	struct poorIRC_server_shared_buffer *shared_buf;
	*/

	struct poorIRC_server_client_lookup_table *shared_lookup;
	int my_lookup_id;


};

/*
 * Initialization of server instance, returns 0 on success, and non-zero value
 * on failure. Allocates memory for struct poorIRC_server instance.
 */

int poorIRC_setup(int argc, char **argv, struct poorIRC_config **cfg);
int poorIRC_init(struct poorIRC_config *cfg, struct poorIRC_server **srv);
int poorIRC_wait_for_client(struct poorIRC_server *srv);
int poorIRC_serve(struct poorIRC_server *srv);
int poorIRC_process_message(struct poorIRC_message *msg, struct poorIRC_server *srv);
int poorIRC_process_command(struct poorIRC_message *msg, struct poorIRC_server *srv);
int poorIRC_broadcast_message(struct poorIRC_message *msg, struct poorIRC_server *srv);
int poorIRC_register_client(char *nickname, struct poorIRC_server *srv);
int poorIRC_is_nickname_taken(char *nickname, struct poorIRC_server *srv);


#endif /* _POORIRC_H */

