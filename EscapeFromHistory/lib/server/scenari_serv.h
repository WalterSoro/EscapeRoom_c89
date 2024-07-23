#include <stdio.h>
#include <string.h>
#define TOTAL_OBJ_NUM 10 
#define NUM_ROOM 1 
#define OBJ_NAME_LENGHT 15
#define TOKEN_ROOM_1 4
// Da cambiare per aumentare il numero di ROOM
// int room_sizes[] = {10}; 

/*
Strutture dati per gli oggetti lato server
*/

typedef struct{
    char nome[OBJ_NAME_LENGHT]; //nome testuale oggetto
    int in_use; // 1 se sta venendo usato da un giocatore
    int location; // 1 se l'enigma è stato già risolto
    int code_obj; // codice dell'oggetto relativo alla room
    int code_room; // codice della room in cui si trova l'oggetto
    int collectable; // se non è collezionabile allora deve essere sbloccato usando un altro oggetto
    int item; // campo valido solo se non è collezionabile, indica il codice_oggetto con il quale deve essere usato
                // se item == -2 significa che è stato già usato
    int answer; // 1 solo se è associato un enigma se è -1 significa che è stato risolto 0 altrimenti
    int usable; // 1 solo se l'oggetto può essere usato da solo 
}Object;

void init_obj();
int init_game_obj(int room, Object* oggetti_partita);
int get_tokens_room(int room);



