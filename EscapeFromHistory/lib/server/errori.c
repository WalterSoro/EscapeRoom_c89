#include "errori.h"

void print_errori(int code){
    printf(BG_RED);
    if(code == TA_ERR_1)
        printf("Troppi argomenti passati con la look\n");
    else if(code == NEA_ERR_1) 
        printf("Codice obj mancante\n");
    else if(code == NEA_ERR_2) 
        printf("Codice room mancante\n");
    else if(code == NVA_ERR_1) 
        printf("Codice obj non valido\n");
    else if(code == NVA_ERR_2) 
        printf("Codice room non valido\n");
    else if(code == NVA_ERR_3) 
        printf("Codice oggetto non esistente\n");  
    else if(code == TAKE_ERR_1)
        printf("Non puoi usare take su una location\n");
    else if(code == TAKE_ERR_2)
        printf("Non è possibile prendere questo oggetto!\n");
    else if(code == TAKE_ERR_3)
        printf("L'oggetto sta venendo usato da un altro utente!\n");
    else if(code == TAKE_ERR_4)
        printf("Enigma gia risolto'\n");
    else if(code == NEA_ERR_3)
        printf("Codice obj1 mancante nella use\n");
    else if(code == NEA_ERR_4)
        printf("Codice obj2 mancante nella use\n");
    else if(code == NOT_OWN_OBJ)
        printf("Non possiedi l'oggetto obj2!\n");
    else if(code == WRONG_OBJ2)
        printf("I due oggetti non possono interagire!\n");
    else if(code == NOT_USABLE_OBJ)
        printf("L'oggetto non puo' essere usato con un altro oggetto!\n");
    else if(code == ALREDY_USE_OBJ_ERR)
        printf("L'oggetto è già stato usato da un altro giocatore\n");
    printf(RESET);
}



