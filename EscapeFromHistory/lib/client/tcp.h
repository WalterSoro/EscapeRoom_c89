#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> 
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utility.h"
#include "errori.h"

#define BUFFER_DIM 1024
#define CMD_DIM 6
#define DELIMITER ","


void recv_from(char* buffer, int socket_decscriptor);

/* gestione client */
int comandi_client(char* cmd, char* buffer, int server_socket_descriptor);

void send_to(char* buffer, int client_socket_descriptor);

