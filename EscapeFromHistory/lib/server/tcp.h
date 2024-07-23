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

#include "game_utility.h"

#define BUFFER_DIM 1024
#define CMD_DIM 6

int server_on;
int fdmax; //valutare se tenerla qua o nel server e passarla come parametro

fd_set master;
fd_set read_fds;

/* send */
void send_to(char* buffer, int client_socket_descriptor);
void broadcast_partita(int partita, char* buffer);

/* gestione server */
void start_server();
void stop_server(); 
void stdin_server();
void comandi_server(char* buffer, int new_sd);

void listener_server(int listener);
void client_req(int new_sd);

