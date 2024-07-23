#include "scenari.h"
/*
In questo file ci sono le funzioni per inizializzare gli scenari
e interagirci
*/
void init_atzechi(){
    rooms[ATZECHI].size_oggetti = NUM_OBJ_ROOM_1;
    strcpy(rooms[ATZECHI].descrizione, "La stanza sembra divisa in 2 vasti spazi:\nA sinistra l'ambiente sembra racchiuso dentro una foresta, pieno\ndi ornamenti dorati e figure di animale.\nTra l'arredamento spicca alla vista uno **scrigno** in pietra\ncon la base ricoperta di piante rampicanti, e un **calendario**\natzeco in pietra con al centro raffigurata una testa di un animale.\nSi vede anche un ++altare++ a forma di ziqqurat con sopra qualcosa\n\nA destra invece sembra di essere dentro la stiva di un galeone, ci\nsono remi cannoni e fucili e qualche moneta d'oro sparsa qua e la.\nTra l'arredamento noti, poggiato su uno ++scaffale++, un **dipinto**\nraffigurante il ritratto di un uomo.\n\nIn fondo alla stanza quando i due arredamenti così diversi si uniscono\nsi trova una ++porta++, mezza in legno e mezza in pietra ricoperta di muschio\n");
    
    strcpy(rooms[ATZECHI].oggetti[0].enigma, "");
    rooms[ATZECHI].oggetti[0].soluzione = -1;
    rooms[ATZECHI].oggetti[0].codice = 0;
    strcpy(rooms[ATZECHI].oggetti[0].nome, "dipinto");
    strcpy(rooms[ATZECHI].oggetti[0].descrizione, "Il dipinto raffigura un nobile spagnolo i suoi occhi luccicano come\nse fossero fatti d'oro. Alla base del dipinto ci sta una sacchetta\ndelle lettere e una targhetta con scritto <<Hernan>> e poi uno spazio\nvuoto in cui poter incastrare queste lettere. Dallo spazio vuoto si intravede una **chiave**.\n"); 

    strcpy(rooms[ATZECHI].oggetti[1].enigma, "");
    rooms[ATZECHI].oggetti[1].soluzione = -1;
    rooms[ATZECHI].oggetti[1].codice = 1;
    strcpy(rooms[ATZECHI].oggetti[1].nome, "scrigno");
    strcpy(rooms[ATZECHI].oggetti[1].descrizione, "Non è uno scrigno normale, tenti di aprirlo ma non riesci, noti una serratura\nprobabilmente ti serve una chiave per aprirlo.\n");
    
    strcpy(rooms[ATZECHI].oggetti[2].enigma, "");
    rooms[ATZECHI].oggetti[2].soluzione = -1;
    rooms[ATZECHI].oggetti[2].codice = 2;
    strcpy(rooms[ATZECHI].oggetti[2].nome, "bigliettino");
    strcpy(rooms[ATZECHI].oggetti[2].descrizione, "sembrano degli appunti di qualcuno, una scritta veloce e sbavata\nrecita così: <<Se la porta vuoi aprire il prezzo di 4 monete devi pagare>>\n");

    strcpy(rooms[ATZECHI].oggetti[3].enigma, "La chiave è bloccata per prenderla devi risolvere l'enigma!\nChe cosa scrivi?:\n1: Cortes\n2: Pizarro\n3: Dias\n");
    rooms[ATZECHI].oggetti[3].soluzione = 1;
    rooms[ATZECHI].oggetti[3].codice = 3;
    strcpy(rooms[ATZECHI].oggetti[3].nome, "chiave");
    strcpy(rooms[ATZECHI].oggetti[3].descrizione,"Una chiave in ferro arruginita con la testa decorata \nda uno stemma raffigurante uno scudo con dei disegni all'interno\n");

    strcpy(rooms[ATZECHI].oggetti[4].enigma, "");
    rooms[ATZECHI].oggetti[4].soluzione = -1;
    rooms[ATZECHI].oggetti[4].codice = 4;
    strcpy(rooms[ATZECHI].oggetti[4].nome, "calendario");
    strcpy(rooms[ATZECHI].oggetti[4].descrizione, "Sul muro è raffigurato un calendario Atzeco di forma circolare, al centro\nè incastonata una **moneta** d'oro raffigurante la testa di un giaguaro.\n");

    strcpy(rooms[ATZECHI].oggetti[5].enigma, "Tenti di prendere la moneta ma è bloccata, per sbloccarla devi risolvere\nun enigma, quale tra questi e' un re Atzeco:\n1: Giorgio III\n2: Suryavarman\n3: Montezuma\n");
    rooms[ATZECHI].oggetti[5].soluzione = 3;
    rooms[ATZECHI].oggetti[5].codice = 5;
    strcpy(rooms[ATZECHI].oggetti[5].nome, "moneta");
    strcpy(rooms[ATZECHI].oggetti[5].descrizione,"Una moneta d'oro perfettamente circolare, al centro e' raffigurato\nil volto di un giaguaro\n");

    strcpy(rooms[ATZECHI].oggetti[6].enigma, "");
    rooms[ATZECHI].oggetti[6].soluzione = -1;
    rooms[ATZECHI].oggetti[6].codice = 6;
    strcpy(rooms[ATZECHI].oggetti[6].nome, "scaffale");
    strcpy(rooms[ATZECHI].oggetti[6].descrizione,"Uno scaffale in legno decorato e ma anche molto rovinato, sopra è poggiato\nun **dipinto** raffigurante un nobiluomo e al fianco del dipinto ci sta\nun **bigliettino** stropicciato");

    strcpy(rooms[ATZECHI].oggetti[7].enigma, "");
    rooms[ATZECHI].oggetti[7].soluzione = -1;
    rooms[ATZECHI].oggetti[7].codice = 7;
    strcpy(rooms[ATZECHI].oggetti[7].nome, "porta");
    strcpy(rooms[ATZECHI].oggetti[7].descrizione,"Un enorme porta senza maniglia ne modo apparente per aprirlo\nprobabilmente si aprirà a seguito di qualche evento\n");
    
    strcpy(rooms[ATZECHI].oggetti[8].enigma, "");
    rooms[ATZECHI].oggetti[8].soluzione = -1;
    rooms[ATZECHI].oggetti[8].codice = 8;
    strcpy(rooms[ATZECHI].oggetti[8].nome, "altare");
    strcpy(rooms[ATZECHI].oggetti[8].descrizione,"Al centro dell'altare ci sono 3 pulsanti, ogni pulsante è decorato\ncon la raffigurazione di un animale:\n1: serpente\n2: giaguaro\n3: armadillo\nAl centro dell'altare è incastrata una **statuetta** d'oro\nraffigurante un guerriero armanto di lancia\n");
    
    strcpy(rooms[ATZECHI].oggetti[9].enigma, "La statuetta è bloccata, non riesci a prenderla, per sbloccarla bisogna\npremere i pulsanti in un certo ordine,sotto la statuetta noti un enigma che\nrecita:\n<<prima chi mangia e poi chi viene magiato>>\nIn che ordine premi i pulsanti?(1:serpente 2:giaguaro 3:armadillo)\n1: 2-1-3\n2: 2-3-1\n3: 3-2-1\n");
    rooms[ATZECHI].oggetti[9].soluzione = 2;
    rooms[ATZECHI].oggetti[9].codice = 9;
    strcpy(rooms[ATZECHI].oggetti[9].nome, "statuetta");
    strcpy(rooms[ATZECHI].oggetti[9].descrizione,"Una statuetta d'oro raffigurante un guerriero armato di lancia con un maestoso copricapo\n");
    
}

void init_room(){
    
    init_atzechi();
    /*
    ampliabile per altre rooms
    */
}

void print_rooms(){
    int i;
    for(i=0; i<NUM_SCENARI; i++){
        system("clear");
        printf("ROOM %d:\n%s\n\n",i+1, rooms[ATZECHI].descrizione);
        printf("Premi INVIO per visualizzare la prossima...");
        getchar();
    }
}
/*Mostra la singola room*/
void mostra_room(int room){
    printf("%s\n", rooms[room-1].descrizione);
    /*
    ampliabile per altre rooms
    */
}

int get_obj_index(char* buffer){
    int i;
    int att = partita_attuale.room - 1;
    for(i=0; i<rooms[att].size_oggetti; i++){
        if(strcmp(buffer, rooms[att].oggetti[i].nome)==0){
            return i;
        }
    }
    return -1;
}
/*
funzione che stampa, dato un oggetto, il messaggio che appare quando 
viene aperto con un altro oggetto o si sblocca un enigma
*/
int print_event_obj(int obj){
    if(obj == 1) //scrigno
        printf("Lo scrigno e' ricolmo di una montagna di tesori, tra di essi spicca alla\nvista una moneta con lo stemma della corona spagnola\nLa prendi e "BACKGREEN"ottieni un TOKEN"RESET"\n");
    if(obj == 3) //chiave
        printf("Insieme alla chiave scende anche una moneta d'oro con inciso \nlo stemma della corona spagnola\n"BACKGREEN"Hai ottenuto un TOKEN"RESET"\n");
    if(obj == 5) //moneta
        printf("Riesci a prendere la moneta e la aggiungi all'inventario! "BACKGREEN"Hai ottenuto un token"RESET"\n");
    if(obj==2){
        printf("Il biglettino è stropicciato, scritto con inchiostro nero e in modo frettoloso\nrecita così:\n<<Se la porta vuoi aprire il prezzo di 3 monete devi pagare>>\n");
        return 0;
    }if(obj==9)
        printf("Riesci a prendere la statuetta, alla base è attaccata una moneta d'oro\n"BACKGREEN"Hai ottenuto un token"RESET"\n");
    sleep(1);
    return 1;
}
/*
stampa il messaggio di vittoria della room
*/
void print_win_message(int room){
    if(room == 1){
        printf("Hai trovato tutti i token necessari, La porta in fondo alla stanza si apre e puoi uscire");
    }
    /*
    ampliabile per altre rooms
    */
}