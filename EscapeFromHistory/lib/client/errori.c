#include "errori.h"

void print_errori(int code){
    if(code == TA_ERR_1)
        printf(RED"Troppi argomenti passati con la look"RESET"\n");
    else if(code == NEA_ERR_1) 
        printf(RED"Codice obj mancante"RESET"\n");
    else if(code == NEA_ERR_2) 
        printf(RED"Codice room mancante"RESET"\n");
    else if(code == NVA_ERR_1) 
        printf(RED"Codice obj non valido"RESET"\n");
    else if(code == NVA_ERR_2) 
        printf(RED"Codice room non valido"RESET"\n");
    else if(code == NVA_ERR_3) 
        printf(RED"Codice oggetto non esistente"RESET"\n");  
    else if(code == TAKE_ERR_1)
        printf(RED"Non puoi usare take su una location"RESET"\n");
    else if(code == TAKE_ERR_2)
        printf(RED"Non è possibile prendere questo oggetto!"RESET"\n");
    else if(code == TAKE_ERR_3)
        printf(RED"L'oggetto sta venendo usato da un altro utente!"RESET"\n");
    else if(code == TAKE_ERR_4)
        printf(RED"Enigma gia risolto'"RESET"\n");
    else if(code == NEA_ERR_3)
        printf(RED"Codice obj1 mancante nella use"RESET"\n");
    else if(code == NEA_ERR_4)
        printf(RED"Codice obj2 mancante nella use"RESET"\n");
    else if(code == NOT_OWN_OBJ)
        printf(RED"Non possiedi l'oggetto obj2!"RESET"\n");
    else if(code == WRONG_OBJ2)
        printf(RED"I due oggetti non possono interagire!"RESET"\n");
    else if(code == NOT_USABLE_OBJ)
        printf(RED"L'oggetto non puo' essere usato con un altro oggetto!"RESET"\n");
    else if(code == ALREDY_USE_OBJ_ERR)
        printf(RED"L'oggetto è già stato usato da un altro giocatore"RESET"\n");
}



