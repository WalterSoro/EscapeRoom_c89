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

#include "scenari.h"

#define RESET   "\x1b[0m"
#define BACKGREEN "\x1b[102m"
#define GREEN   "\x1b[32m"
#define RED "\x1b[31m"


#define TIMER_DURATION 600 //10 minuti
#define ERRORS_SIZE 10

int in_game; // utente loggato
int time_out;
int start_timer; // attivo dopo il comando start della partita
struct tm timer;

/*I/O*/
void print_comandi_client();
void mostra_schermata_iniziale();

/*TIMER*/
char* timestamp();
void init_timer(char* buffer);
int check_timer();

/*GIOCO*/
void risolvi_enigma(int obj);
void gestisci_vittoria();