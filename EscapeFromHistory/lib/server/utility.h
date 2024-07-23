#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TIMER_DURATION 300 //5 minuti

#define RESET   "\x1b[0m"
#define BACKRED "\x1b[102m"
#define GREEN   "\x1b[32m"

void lettura_input(char* buffer);
void print_comandi_server();
