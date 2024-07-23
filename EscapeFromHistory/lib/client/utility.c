#include "utility.h"



/*************************** I/O ************************/

/*
Stampa a video i comandi del client, se il timer è partito significa
che è già stato fatto lo start, quindi cambio i comandi disponibili
*/
void print_comandi_client(){
    
    printf("ECCO I COMANDI DISPONIBILI:\n");
    if(start_timer == 0)
        printf(GREEN"start room"RESET" -> gioca nello scenario room (esempio start 1)\n");
    else{
        printf(GREEN"room"RESET" -> mostra la descrizione della room attuale\n");
        printf(GREEN"time"RESET" -> mostra tempo rimanente\n");
        printf(GREEN"take obj"RESET" -> permette di prendere un oggetto\n");
        printf(GREEN"use obj1"RESET" obj2 -> use oggetto 1 con oggetto 2\n");
        printf(GREEN"use obj1"RESET" -> usa l'oggetto\n");
        printf(GREEN"look location | object"RESET" -> descrive l'oggetto o la location\n");
        printf(GREEN"objs"RESET" -> visualizza inventario\n");

    }
    printf(GREEN"end"RESET" -> termina il gioco\n");
}

void mostra_schermata_iniziale(){

    printf("************************************************\n");
    printf("*                                              *\n");
    printf("*                                              *\n");
    printf("*                                              *\n");
    printf("*                 BENVENUTO IN                 *\n");
    printf("*              ESCAPE FROM HISTORY             *\n");
    printf("*                                              *\n");
    printf("*                                              *\n");
    printf("*                                              *\n");
    printf("************************************************\n");

    printf("\n\n");
    sleep(2);
    print_rooms();

    printf("Finite le stanze\n");
}

/************************** TIMER ***********************/

char* timestamp(){
    time_t current_time;
    time(&current_time);
    
    struct tm *local_time = localtime(&current_time);

    /* utilizza asctime per ottenere una stringa di caratteri leggibile */
    char *time_str = asctime(local_time);

    /* rimuovi il carattere di nuova linea, se presente */
    size_t len = strlen(time_str);
    if (len > 0 && time_str[len - 1] == '\n') {
        time_str[len - 1] = '\0';
    }
    return time_str;
}

/*
Funzione che mostra il tempo rimasto all'utente e in caso sia finito
mostra un messaggio e restituisce -1. Questa funzione viene chiamata 
ad ogni ciclo di azione
*/
int check_timer(){
    time_t tempo;
    time_t rawtime;

    //printf("Orari: \n%f\n%f\n",(float)rawtime,(float) tempo);
    rawtime = time(NULL);
    //printf("%s\n", ctime(&tempo));
    tempo = mktime(&timer);
    tempo -= 3600;
    tempo += TIMER_DURATION;
    //printf("%s\n", ctime(&tempo));
    double seconds = difftime(tempo, rawtime);
    //printf("Secondi rimanenti: %f\n", seconds);

    
    if (seconds <= 0) {
        printf("Tempo Scaduto!!\n");
        return -1;
    } else {
        int h = (int)(seconds / 3600);
        seconds -= h*3600;
        int m = (int)(seconds / 60);
        seconds -= m*60;
        printf("Tempo rimanente: %02d:%02d:%02.0f\n", h, m, seconds);
        return 1;
    }
}
//converta la strinfa in struct_tm
//la stringa deve essere nel formato "%a %b %d %H:%M:%S %Y"
void string_to_time(const char *buf, struct tm *tm) {
    int i;
    const char *months[] = {
        "Jan", "Feb", "Mar", "Apr", "May", "Jun",
        "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
    };

    const char *days[] = {
        "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"
    };

    char month_str[4];
    char day_str[4];

    sscanf(buf, "%3s %3s %d %d:%d:%d %d",
           day_str, month_str,
           &tm->tm_mday,
           &tm->tm_hour,
           &tm->tm_min,
           &tm->tm_sec,
           &tm->tm_year);
    for (i = 0; i < 12; ++i) {
        if (strncmp(month_str, months[i], 3) == 0) {
            tm->tm_mon = i;
            break;
        }
    }
    for (i = 0; i < 7; ++i) {
        if (strncmp(day_str, days[i], 3) == 0) {
            tm->tm_wday = i;
            break;
        }
    }
    tm->tm_year -= 1900;
}
/*
Inizializza il timer ricevuto in modalità testuale dal server
*/
void init_timer(char* buff){
    string_to_time(buff, &timer);
}
/************************** GIOCO ***********************/
/*
Funzione per risolvere l'enigma associato ad obj. L'utente è oobligato a
risolvere l'enigma. Ogni errore dovrà aspettare 10 secondi. Per aumentare
il valore basta cambiare la variabile penalty
*/
void risolvi_enigma(int obj){

    int ans; //risposta utente
    int penalty; //secondi da aspettare in caso di errore
    do{     
        printf("%s\n", rooms[partita_attuale.room-1].oggetti[obj].enigma);
        /*fgets(risposta, 2, stdin);
        risposta[1] = '\0';
        ans = atoi(risposta);*/
        scanf("%d", &ans);

        if(ans == rooms[partita_attuale.room-1].oggetti[obj].soluzione){
            printf(GREEN"Risposta corretta!!"RESET"\n");
            sleep(1);
            break;
        }else{
            penalty = 10;
            printf(RED"Risposta sbagliata! Rimani bloccato per %d secondi"RESET"\n",penalty);
            sleep(penalty);
        }
        //escape (debug) per uscire dall'enigma
        if(ans == -1) break;
    }while(1);

}
/*
Stampa un messaggio di vittoria, tra una room e l'altra cambia il risultato
della funzione print_ein_message, associata alla room
*/
void gestisci_vittoria(){
    printf(BACKGREEN"PARTITA FINITA HAI VINTO!"RESET"\n\n");
    printf(BACKGREEN);
    print_win_message(partita_attuale.room);
    printf(RESET);
    printf("\n\n");
    printf(BACKGREEN"HAI CONTRIBUITO ALLA PARTITA OTTENENDO %d TOKEN"RESET"\n", partita_attuale.token_guadagnati);

}