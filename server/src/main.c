/*
 * This is main file of the IRC-like server.
 */

#include "poorIRC_server.h"
#include "../../include/poorIRC_proto.h"

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{

	struct poorIRC_config *server_config   = NULL;
	struct poorIRC_server *server_instance = NULL;


	if(poorIRC_setup(argc, argv, &server_config) != 0) {

		fprintf(stderr, "Server configuration failed, exiting...\n");
		return EXIT_FAILURE;

	}

	if(poorIRC_init(server_config, &server_instance) != 0) {

		fprintf(stderr, "Server initialization failed, exiting...\n");
		return EXIT_FAILURE;

	}

	if(poorIRC_wait_for_client(server_instance) != 0) {

		fprintf(stderr, "Server stopped working, exiting...\n");
		return EXIT_FAILURE;

	}

	return EXIT_SUCCESS;

}

