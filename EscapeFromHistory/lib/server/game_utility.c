#include "game_utility.h"

/*************************** ACCOUNT ***************************/

/*
    Controlla se lo username è presente oppure no
    restituisce -1 in caso negativo, 1 in caso positivo
*/
int gestisci_user_key(char* username){
    // controlla nel file degli utenti se lo username è presente o no
    FILE *file = fopen("user.txt", "r");
    if(file == NULL){
        perror("Errore nell'apertura del file user.txt");
        return -1;
    }
    char line [256];

    char usr [USERNAME_DIM];
    char psw [PASSWORD_DIM];
    if(search_active_player(username) == 0)
        return -1;
    while (fgets(line, 256, file) != NULL) {
        // Dividi la riga in username e password
        if (sscanf(line, "%s %s", usr, psw) == 2) {
            if (strcmp(username, usr) == 0) {
                fclose(file);
                return 1;
            }
        }
    }
    
    fclose(file);
    return -1;
}

int gestisci_password(char* username, char* password){
    FILE *file = fopen("user.txt", "r");
    if(file == NULL){
        perror("Errore nell'apertura del file user.txt");
        return -1;
    }
    char line [256];

    char usr [USERNAME_DIM];
    char psw [PASSWORD_DIM];
    while (fgets(line, 256, file) != NULL) {
        // Dividi la riga in username e password
        if (sscanf(line, "%s %s", usr, psw) == 2) {
            if (strcmp(username, usr) == 0 ){
                if(strcmp(password, psw) == 0){
                    fclose(file);
                    return 1;
                }else{
                    fclose(file);
                    return -1;
                }
            }
        }
    }    
    fclose(file);
    return -1;

}

int add_user(char* username, char* password){
    // Apri il file in modalità append
    FILE *file = fopen("user.txt", "a");
    if (file == NULL) {
        perror("Error nell'apertura del file user.txt");
        return -1;
    }

    // Scrivi lo username e la password nel file
    fprintf(file, "%s %s\n", username, password);

    // Chiudi il file
    fclose(file);
    return 1;
}
/*************************** CONFIG ***************************/

void configurazione_gioco(){
    int i;
    for(i = 0; i<MAX_NUM_PARTITE; i++)
        partite_in_corso[i].free_slot = 1;
    for(i = 0; i<MAX_NUM_PLAYERS; i++)
        giocatori_attivi[i].free_slot = 1;

    num_partite_in_corso = 0;
    ngiocatori = 0;
}

/*************************** GIOCATORE ***************************/
/*
Aggiunge un nuovo giocatore attivo nell'array.
*/
int new_active_player(char* username, int sd){
    int slot, i;
    if(ngiocatori == MAX_NUM_PLAYERS - 1){
        return -1;
    }
    for(i=0; i<MAX_NUM_PLAYERS; i++){
        if(giocatori_attivi[i].free_slot==1){
            slot = i;
            break;
        }
    }
    giocatori_attivi[slot].free_slot = 0;
    giocatori_attivi[slot].sd = sd;
    strcpy(giocatori_attivi[slot].username, username);
    giocatori_attivi[slot].token_ottenuti = 0;
    giocatori_attivi[slot].partita = -1;
    strcpy(giocatori_attivi[slot].log, "\n");
    giocatori_attivi[slot].avaiable_log = 0;
    ngiocatori++;
    return 1;
}
/*
mostra giocatori attivi, usata dal comando "show" comando nascosto
del server, utilizzato in fase di debug 
*/
void show_active_player(){
    int i;
    printf("Ecco la lista dei giocatori attivi:\n");
    for(i = 0; i<MAX_NUM_PLAYERS; i++){
        if(giocatori_attivi[i].free_slot == 0)
            printf("%s\n", giocatori_attivi[i].username);
    }
}

/*
Funzione che si occupa dell'eliminazione di un giocatore dalle varie 
struttre dati. Se il giocatore stava all'interno di una partita viene
tolto e il flag liberato. Gli altri giocatori potranno continuare il gioco
*/
void logout_player(int sd){
    int i,n;
    int p = search_player_by_socket(sd);
    int partita = giocatori_attivi[p].partita;
    n = NUM_GAME_PLAYERS; 
    if(partita != -1){ //il giocatore sta in una partita
        
        //tolgo il giocatore dai giocatori in partita e aggiusto l'array
        for(i=0; i<n; i++){
            //printf("Confronto %s con %s\n",partite_in_corso[partita].giocatori[i]->username, giocatori_attivi[p].username);
            if(partite_in_corso[partita].giocatori[i]->free_slot == 0 && strcmp(partite_in_corso[partita].giocatori[i]->username, giocatori_attivi[p].username) == 0){                
                partite_in_corso[partita].num_giocatori--;
            }
        }   
        //gestisco chiusura partita se non ci sono più giocatori
        //printf("Giocatori rimasti nella partita %d: %d\n", partita, partite_in_corso[partita].num_giocatori);
        if(partite_in_corso[partita].num_giocatori == 0){
            printf("Cancello la partita %d\n", partita);
            partite_in_corso[partita].free_slot = 1;
            num_partite_in_corso--;
        }
    }

    if(p==-1) return;
    giocatori_attivi[p].free_slot = 1;
    ngiocatori--;
    //cerco la partita del giocatore se presente

}

int search_player_by_socket(int sd){
    int i=0;
    for(i=0; i<MAX_NUM_PLAYERS; i++){
        if(giocatori_attivi[i].free_slot == 0 && giocatori_attivi[i].sd == sd)
            return i;
    }
    return -1;
}

//Controlla se il giocatore è presente nella lista attivi
int search_active_player(char*usr){
    int i;
    for(i=0; i<MAX_NUM_PLAYERS;i++){
        if(giocatori_attivi[i].free_slot == 0 && strcmp(giocatori_attivi[i].username, usr) == 0){
            return 0;
            
        }
    }
    return -1;
}
/*
La funzione aggiunge l'evento conenuto in new_log
in tutti gli utenti connessi alla partita escluso player
*/
void add_log_players(int player, char* new_log){
    int i;
    int partita;
    int size;
    partita= giocatori_attivi[player].partita;
    size = partite_in_corso[partita].num_giocatori;
    for(i=0;i<size;i++){
        if(giocatori_attivi[player].sd == giocatori_attivi[i].sd)
            continue;
        if(giocatori_attivi[i].free_slot != 1){
            //printf("aggiorno il log di %s\n", giocatori_attivi[i].username);
            giocatori_attivi[i].avaiable_log = 1;
            strcat(giocatori_attivi[i].log, new_log);
        }
    }
}

/*************************** PARTITA ***************************/
/*
mostra partite in corso, anche quasta aggiunta in fase di debug e lasciata
*/
void mostra_partite(){
    int i;
    char stato[10];
    int giocatori;

    printf("*********** LOBBY ATTIVE ***********");
    for(i=0; i<num_partite_in_corso; i++){
        giocatori = partite_in_corso[i].num_giocatori;
        if(partite_in_corso[i].in_game == 1)
            strcpy(stato, "in gioco");
        else
            strcpy(stato, "in attesa");
        printf("Lobby %d: %d/%d giocatori stato: %s ", i+1, giocatori,NUM_GAME_PLAYERS, stato);
    }
    printf("************************************");

}
//cerca partite in attesa di giocatori
int cerca_partita(int room){
    int i;
    for(i = 0; i<num_partite_in_corso; i++){
        //Se la room coincide e la partita non è cominciata
        if(partite_in_corso[i].room == room && partite_in_corso[i].in_game == 0){
            return i; 
        }
    } 
    return -1;
}
/*Crea nuova partita se possibile*/
int crea_partita(int room, int player){
    int i;
    if(num_partite_in_corso == MAX_NUM_PARTITE){
        printf("Troppe partite in corso, impossibile crearne una nuova\n");
        return -1;
    }
    for(i=0; i<MAX_NUM_PARTITE; i++){
        if(partite_in_corso[i].free_slot == 1){
            //printf("Creo partita con room %d\n", room);
            partite_in_corso[i].in_game = 0;
            partite_in_corso[i].room = room;
            partite_in_corso[i].giocatori[0] = &giocatori_attivi[player];
            
            partite_in_corso[i].token = 0;
            partite_in_corso[i].n_oggetti_inventario = 0;
            partite_in_corso[i].obbiettivo = get_tokens_room(room);
            partite_in_corso[i].free_slot = 0;
            partite_in_corso[i].num_giocatori = 1;
            

            partite_in_corso[i].num_oggetti = init_game_obj(room, partite_in_corso[i].oggetti);
            //printf("aggiunta la partita in posizione %d con num_oggetti=%d\n", i, partite_in_corso[i].num_oggetti);
            return i;
        }
    }
    printf("Errore, nella creazione della partita\n");
    return -1;
}
/*
Gestisce l'ingresso in una nuova partita, se non ci sono partite in attesa
di giocatori allora ne crea una
*/
int gestisci_ingresso_partita(int room, int sd){
    int player = search_player_by_socket(sd);
    if(player==-1){ //non dovrebbe mai succedere
        printf("Giocatore non esistente\n");
        return -1;
    }

    int partita = cerca_partita(room);
    //printf("Partita trovata! codice %d\n", partita);
    if(partita == -1){
        //printf("Creo una nuova partita\n");
        partita = crea_partita(room, player);  
        if(partita!=-1) num_partite_in_corso++; 
        giocatori_attivi[player].partita = partita;
    
    }else{
        int temp = partite_in_corso[partita].num_giocatori;
        //printf("Valore dei giocatori della partita %d\n", temp);
        partite_in_corso[partita].giocatori[temp] = &giocatori_attivi[player];
        partite_in_corso[partita].num_giocatori++;
        if((temp+1) == NUM_GAME_PLAYERS){
            //printf("Faccio iniziare la partita\n");
            //per ora blocco solo altri ingressi
            partite_in_corso[partita].in_game = 1;
        }
        giocatori_attivi[player].partita = partita;
    }
    return partita;
}

void avvia_timer(int partita, char* tempo){
    time_t rawtime;
    struct tm* timeinfo;
    rawtime = time(NULL);
    partite_in_corso[partita].timestamp_inizio = rawtime;
    timeinfo = localtime(&rawtime);
    strcpy(tempo, asctime(timeinfo));
    //sprintf(tempo, "%d:%d:%d", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}

/*************************** OGGETTI ***************************/
/*
Trova oggetto nella partita
*/
int find_obj(int player, int obj){
    int i, index, n;
    index = giocatori_attivi[player].partita;
    n = partite_in_corso[index].num_oggetti;
    for(i=0; i<n; i++){
        if(partite_in_corso[index].oggetti[i].code_obj == obj)
            return i;
    }
    return -1;
}

/*
Trova oggetto nell'inventario
*/

int find_owned_obj(int obj, int partita){
    int i;
    int size;
    size = partite_in_corso[partita].n_oggetti_inventario;
    for(i=0; i<size; i++){
        //printf("obj:%d inv:%d\n",obj,partite_in_corso[partita].inventario[i]->code_obj);
        if(obj == partite_in_corso[partita].inventario[i]->code_obj)
            return 1;
    }
    return 0;
}

int gestisci_look(char* buffer, int player){
    
    char* obj; // server per estrarre il nome dell'oggetto
    char* temp; //serve èer controllare che non ci siano troppi argomenti
    int obj_code; //server per contenere l'indice dell'oggetto ricevuto

    //printf("Gestione comando look per l'oggetto %s\n", buffer);

    obj = strtok(buffer, " ");   

    if(!obj) return NEA_ERR_1;

    temp = strtok(NULL, " ");
    if(temp) return TA_ERR_1;

    obj_code = atoi(obj);   
    if(obj_code < 0) return NVA_ERR_1;

    obj_code = find_obj(player, obj_code);

    if(obj_code == -1) return NVA_ERR_3;    

    return 0;
}

int gestisci_take(char* buffer, int player){
    char* obj;
    char* temp;
    int obj_code;
    int partita; // conterrà il codice della partita del giocatore

    obj = strtok(buffer, " ");

    temp = strtok(NULL, " ");
    if(temp) return TA_ERR_1;
    
    obj_code = atoi(obj);   
    if(obj_code < 0) return NVA_ERR_1;

    obj_code = find_obj(player, obj_code);
    if(obj_code == -1) return NVA_ERR_3;   

    partita=giocatori_attivi[player].partita;

    if(partite_in_corso[partita].oggetti[obj_code].location == 1) 
        return TAKE_ERR_1;

    if(partite_in_corso[partita].oggetti[obj_code].collectable == 0) 
        return TAKE_ERR_2;

    if(partite_in_corso[partita].oggetti[obj_code].in_use == 1) 
        return TAKE_ERR_3;

    /*FUNZIONALITA' LOG PLAYER*/
    char log[1024];
    sprintf(log, "%s ha usato la take su %s\n", giocatori_attivi[player].username,partite_in_corso[partita].oggetti[obj_code].nome);
    add_log_players(player, log);

    partite_in_corso[partita].oggetti[obj_code].in_use = 1;
    partite_in_corso[partita].inventario[partite_in_corso[partita].n_oggetti_inventario] = &partite_in_corso[partita].oggetti[obj_code];
    
    if(partite_in_corso[partita].oggetti[obj_code].answer == 1){ // Va risolto l'enigma associato
        partite_in_corso[partita].oggetti[obj_code].answer = -1;
        return 1; 
    }else if(partite_in_corso[partita].oggetti[obj_code].answer == -1){
        return TAKE_ERR_4;
    }else if(partite_in_corso[partita].oggetti[obj_code].answer == 0){ //caso take senza enigma  
        // rendo l'oggetto non più collezionabile
        partite_in_corso[partita].oggetti[obj_code].collectable = 0;
        return 0;       
    }

    // pongo l'enigma come risolto 
    partite_in_corso[partita].oggetti[obj_code].answer = -1;
    // rendo l'oggetto non più collezionabile
    partite_in_corso[partita].oggetti[obj_code].collectable = 0;
    return 0;
}

// formato comando use: "room obj1 obj2"
// gestice oggetti che non possono essere usati da soli
// controlla che l'oggetto che obj2 sia nell'inventario

int gestisci_use(char* buffer, int player){
    
    char* obj1;// serve per estrarre il nome dell'oggetto 1
    char* obj2;// serve per estrarre il nome dell'oggetto 2
    int obj_code1; //serve per contenere l'indice dell'oggetto 1 ricevuto
    int obj_code2; //serve per contenere l'indice dell'oggetto 2 ricevuto
    int partita;
    int temp;  ////serve per contenere l'indice dell'oggetto 2 relativo all'inventarios

    //printf("Gestione comando use\n");

    //se si vuole sviluppare la use con un singolo oggetto va fatto qua
    obj1 = strtok(buffer, " ");    
    if(!obj1) return NEA_ERR_3;

    obj_code1 = atoi(obj1);
    
    partita = giocatori_attivi[player].partita;
    //printf("Use su oggetto: %d\n", obj_code1);
    obj2 = strtok(NULL, " ");
    //l'oggetto è usabile da solo e non c'è il secondo parametro
    if(partite_in_corso[partita].oggetti[obj_code1].usable == 1 && !obj2){
        //la use non blocca l'oggetto visto che è immediata
        //non prevedendo enigmi
        return 1;
    }else
    if(!obj2) return NEA_ERR_4;
    //printf("codice obj2:%s\n", obj2);
    obj_code2 = atoi(obj2);

    if(partite_in_corso[partita].oggetti[obj_code1].item == -1)
        return NOT_USABLE_OBJ;
    //printf("Use con la partita numero: %d\n", partita);
    if(partite_in_corso[partita].oggetti[obj_code1].item == -2)
        return ALREDY_USE_OBJ_ERR;
    temp = find_owned_obj(obj_code2,partita);

    if(temp == 0)
        return NOT_OWN_OBJ;

    if(partite_in_corso[partita].oggetti[obj_code1].item != obj_code2)
        return WRONG_OBJ2;
    /*FUNZIONALITA LOG PLAYER*/
    char log[1024];
    sprintf(log, "%s ha usato la use su %s %s\n", giocatori_attivi[player].username,partite_in_corso[partita].oggetti[obj_code1].nome, partite_in_corso[partita].oggetti[obj_code2].nome);
    add_log_players(player, log);

    //-2 indica che l'oggetto è stato usato
    partite_in_corso[partita].oggetti[obj_code1].item = -2;
    return 1;
}
/*
EVENTUALE OTTIMIZZAZIONE
La gestisci objs prepara la stringa contenente l'inventario già pronta
Una ottimizzazione sia per quando riguarda la rete che la memoria 
potrebbe essere scaricare la complessità al client togliendo il campo "nome"
dalla struttura Object e mandando una stringa di soli code_obj separati
da virgola. Il client a quel punto esegue una ricerca per ogni code
e lo stampa.

*/
int gestisci_objs(char* buffer, int player){
    int partita;
    int size;
    int i;
    partita=giocatori_attivi[player].partita;
    size = partite_in_corso[partita].n_oggetti_inventario;
    strcpy(buffer, "\n");
    if(size == 0)
        strcpy(buffer, "Inventario Vuoto!\n");
    for(i = 0; i<size; i++){
        strcat(buffer, partite_in_corso[partita].inventario[i]->nome);
        strcat(buffer, "\n");
    }
    sprintf(buffer, "%stoken=%d\n", buffer, partite_in_corso[partita].token);
    return 0;
}
/*
Faccio meno controlli perché la done
non viene inviata direttamente dall'utente.
*/
void gestisci_done(char* buffer, int player){
    int partita;
    int size;
    int obj_code;
    partita=giocatori_attivi[player].partita;
    size = partite_in_corso[partita].n_oggetti_inventario;
    obj_code = atoi(buffer);

    partite_in_corso[partita].inventario[size] = &partite_in_corso[partita].oggetti[obj_code];
    partite_in_corso[partita].oggetti[obj_code].in_use = 0;
    partite_in_corso[partita].n_oggetti_inventario++;
    //printf("Oggetto %d aggiunto all'inventario\n");

}
/*
Aggiunge un token alla partita e manda il log a tutti i giocatori
*/
void gestisci_NEWTKN(int player){
    partite_in_corso[giocatori_attivi[player].partita].token++;
    char log[1024];
    sprintf(log, "%s ha ottenuto un token\n", giocatori_attivi[player].username);
    add_log_players(player, log);
}
/*
Controlla se la partita può finire
*/
int gestisci_CHKWIN(int player){
    if(partite_in_corso[giocatori_attivi[player].partita].obbiettivo == partite_in_corso[giocatori_attivi[player].partita].token){
        return 1;
    }else{
        return 0;
    }
}

/*
Gestisce il comando log, resetta la variabile avaiable_log e 
manda tutti il buffer con il log
*/
int gestisci_log_player(char* buffer, int player){
    if(giocatori_attivi[player].avaiable_log == 0)
        return -1;
    strcpy(buffer, giocatori_attivi[player].log);
    giocatori_attivi[player].avaiable_log = 0;
    strcpy(giocatori_attivi[player].log, "\n");
    return 1;
}
