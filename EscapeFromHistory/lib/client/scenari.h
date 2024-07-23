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

#define DESCRIPTION_LENGHT 1024
#define OBJ_NAME_LENGHT 15
#define NUM_SCENARI 1
#define NUM_OBJ_ROOM_1 10 //vedere con precisione quanti
#define ATZECHI 0
#define RESET   "\x1b[0m"
#define BACKGREEN "\x1b[102m"
#define GREEN   "\x1b[32m"

/********************************************************

        Strutture dati per gli oggetti lato client

********************************************************/


/*
Struttura per gestire le stampe degli oggetti e le losro descrizioni, 
negli oggetti con enigma è presente anche il testo dell'enigma
associato e la soluzione
*/
typedef struct{
    int codice;
    char descrizione[DESCRIPTION_LENGHT];
    int soluzione; //Valido solo negli oggetti con enigma
    char enigma[DESCRIPTION_LENGHT]; //Valido solo negli oggetti con enigma

    char nome[OBJ_NAME_LENGHT];
}Object_cl;


/*
Struttura per salvare le room, contiene la descrizione e gli 
oggetti presenti
*/
typedef struct{
    int codice;
    char descrizione[DESCRIPTION_LENGHT];
    Object_cl oggetti[NUM_OBJ_ROOM_1];
    int size_oggetti; //quanti oggetti
}Room;
Room rooms[NUM_SCENARI];

/*
struttura per la partita attuale del giocatore
contiene la room attualmente in gioco
e i token guadagnati dal giocatore, informazione usata 
solo nella stampa del messaggio di vittoria
*/
typedef struct{
    int room;
    int token_guadagnati;
}Partita;

Partita partita_attuale;


void init_room();
void print_rooms();
void mostra_room(int room);
int get_obj_index();
int print_event_obj(int obj);
void print_win_message(int room);
