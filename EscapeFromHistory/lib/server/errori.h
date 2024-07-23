
#include <stdio.h>

#define BG_RED "\x1b[41m"
#define RESET "\x1b[0m"

/*
File contentente tutti i tipi di errore e le relative stampe
Gli errori sono molto specifici in modo da poter gestire meglio
eventuali problematiche 
legenda:
TA_ERR troppi argomenti 
NEA_ERR non abbastanza argomenti
NVA_ERR argomento non valido
*/

#define TA_ERR_1 100// gestisci_look, use
#define NEA_ERR_1 101// gestisci_look, use
#define NEA_ERR_2 102// gestisci_look, use
#define NVA_ERR_1 103// gestisci_look
#define NVA_ERR_2 104// gestisci_look
#define NVA_ERR_3 105// gestisci_look
#define TAKE_ERR_1 106// gestisci_take, l'oggetto è una location
#define TAKE_ERR_2 107// gestisci_take, l'oggetto non è collezionabile
#define TAKE_ERR_3 108// gestisci_take, l'oggetto è  in uso
#define TAKE_ERR_4 109//gestisci_take, enigma già risolto
#define NEA_ERR_3 110//gestisci_use
#define NEA_ERR_4 111//gestisci_use
#define NOT_OWN_OBJ 112//gestisci_use, oggetto 2 non posseduto
#define WRONG_OBJ2 113//gestisci_use, oggetti non corripondenti
#define NOT_USABLE_OBJ 114//gestisci_use, non è possibile usare la use a 2 operandi
#define ALREDY_USE_OBJ_ERR 115 // gestisci_use, oggetto già usato



void print_errori(int code);