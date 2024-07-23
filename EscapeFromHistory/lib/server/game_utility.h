#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> //mi serve per la funzione isspace
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utility.h"
#include "scenari_serv.h"
#include "errori.h"

#define DELIMITER ","
#define USERNAME_DIM 15
#define PASSWORD_DIM 50
#define MAX_NUM_PLAYERS 15
#define MAX_NUM_PARTITE 3
#define DESCRIPTION_LENGHT 1024
#define OBJ_NAME_LENGHT 15
#define NUM_GAME_PLAYERS 2
#define INVENTORY_SIZE 3
#define MAX_NUM_GAME_OBJ 10


/****************************************************************/
//                      STRUTTURE DATI GIOCO
/****************************************************************/
/*
Le strutture dati sono array, per velocizzare la ricerca
e per evitare i segmentation fault, già abbastanza 
protagonisti nelle operazioni con le stringhe.

ogni volta viene cercato uno slot libero sia di giocatore che 
di partite, controllando l'apposita variabile.
Lo slot indica pure se il giocatore è ancora online oppure no
in caso di disconnessione improvvisa.
*/

/*
La struttura giocatore serve per poter mostrare i giocatori attivi, contien:
username
free_slot-> serve per inserire nell'array
sd-> per permettere a un utente di chiedere al server di mandare un messaggio ad un giocatore specifico
token_ottenuti-> i token ottenuti dal giocatore, per stilare la classifica finale della lobby
partita-> indica la partita attuale -1 se non è in partita
log-> tiene salvate le azioni degli altri giocatori
avaible_log->indica se ci sono informazioni nel log
*/
typedef struct{
    int free_slot;
    char username[USERNAME_DIM];
    int sd;
    int token_ottenuti; 
    int partita;
    char log[1024];
    int avaiable_log; //1 se è stato scritto qualcosa
}Giocatore;

//array di giocatori attivi, e contatore di giocatori attivi
Giocatore giocatori_attivi[MAX_NUM_PLAYERS];
int ngiocatori;

/*
Strutture dati per gestire la Lobby, essendo il gioco full-coop l'inventario
è comune così come i token e il timer
array di giocatori di al massimo 
la variabile in_game, indica se la partita dei giocatori è iniziata
la variabile room indica in quale scenario è la partita
la variabile free_slot indica se la partita in questione esiste o va creata
L'inventario è composto da indirizzi di Oggetti, gli indirizzi 
punteranno all'array oggetti della partita. 
L'array oggetti contiene tutti gli oggetti della partita, verrà
popolato in fase di inizializazione con la room corrispondente
La variabile num_oggetti ci dice il numero reale di oggetti 
presente, in modo che le room non debbano per forza avere lo 
stesso numero di oggetti.
*/

typedef struct{
    int free_slot; // 1 libero
    int in_game; // 1 in game
    int room;
    Giocatore* giocatori[NUM_GAME_PLAYERS]; //array di puntatori 
    int num_giocatori;
    Object *inventario[INVENTORY_SIZE];
    int n_oggetti_inventario; //indica il numero di oggetti collezionati
    int token;
    //timer
    time_t timestamp_inizio;
    int obbiettivo;//contiene il numero minimo di token per vincere 

    Object oggetti[MAX_NUM_GAME_OBJ];
    int num_oggetti; 
} Partita;

Partita partite_in_corso[MAX_NUM_PARTITE];
int num_partite_in_corso;


/*******************************************************************************************/
//                      PROCEDURE GESTIONE FUNZIONALITA' GIOCO                  
/*******************************************************************************************/

//account
int gestisci_user_key(char* username);
int gestisci_password(char* username, char* password);
int add_user(char* username, char* password);
//aggiungere il controllo giocatore non online

//conf
void configurazione_gioco(); // da un valore di partenza al gioco

//giocatore
int new_active_player(char* username, int sd);
void show_active_player();
void logout_player(int sd);
int search_player_by_socket(int sd);
int search_active_player(char* usr);
void add_log_players(int player, char* buffer);

//partita
void mostra_partite();
int cerca_partita(int room);
int crea_partita(int room, int player); 
int gestisci_ingresso_partita(int room, int sd);
void avvia_timer(int partita, char* tempo);


//oggetti
int find_obj(int room, int obj);
int find_owned_obj(int obj, int partita);

//gestori comandi
int gestisci_look(char* buffer, int player);
int gestisci_take(char* buffer, int player);
int gestisci_use(char* buffer, int player);
int gestisci_objs(char* buffer, int player);
void gestisci_done(char* buffer, int player);
int find_owned_obj(int obj, int partita);
void gestisci_NEWTKN(int player);
int gestisci_CHKWIN(int player);
int gestisci_log_player(char* buffer, int player);



