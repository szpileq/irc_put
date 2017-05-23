#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../../include/poorIRC_proto.h"

typedef struct User{
	pid_t userPID;
	int socketfd;
	char nickname[16];
	struct User *left;
	struct User *right;
	struct User *parent;
} User;

/*					API				*/

User *newUser(pid_t userPID, char nick[]);
void addUser(User **root,pid_t userPID, char nickname[]);
int deleteUser(User **rootd,pid_t userPID);
User *findUserByPID(User **root, pid_t userPID);

void showInOrder(User *root);
void showPreOrder(User *root);
void showPostOrder(User *root);
