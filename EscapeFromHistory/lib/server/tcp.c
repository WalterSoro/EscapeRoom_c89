#include "tcp.h"
 
void start_server(){
    if(server_on == 1){
        printf("Il Server è già in esecuzione!\n");
        return;
    }
    printf("Server avviato\n");
    server_on = 1;
}
/*
Ferma il server, impossibile se ci sono giocatori connessi
*/
void stop_server(){
    if(ngiocatori != 0){
        printf("Non è possibile chiudere il server!\n");
        printf("Ci sono ancora giocatori attivi, ecco la lista:\n");
        fflush(stdout);
        show_active_player();
        return;
    }
    server_on = 0;
}
/*
Gestisce la connessione con stdin
il comando show non è mostrato a video e serve per debug mostrando
i giocatori connessi
*/
void stdin_server(){
    char buffer[1024];
    
    do{
        //uso la scanf perché i comandi sono composti da singole parole
        char cmd[CMD_DIM];
        scanf("%s", buffer);
        strcpy(cmd, buffer);

        if(strcmp(cmd, "stop") == 0){
            stop_server();    
            return;
        }else if(strcmp(cmd, "start") == 0){
            start_server();
            break;
        }else if(strcmp(cmd, "show") == 0){
            show_active_player();
            break;
        }else{
            printf("COMANDO NON VALIDO! ECCO I COMANDI VALIDI: \n");
            printf("start -> avviare il server\n");
            printf("stop -> fermare il server\n" );
        }
        
    }while(1);
}
/*
Gestisci l'arrivo di un nuovo client
*/
void listener_server(int listener){   
    struct sockaddr_in client_addr;
    unsigned int len;
    int new_sd;
    fflush(stdout); 
    len = sizeof(client_addr);
    new_sd = accept(listener, (struct sockaddr*)&client_addr, &len);
    printf("Nuova client rilevato da porta %u\n", ntohs(client_addr.sin_port)); //LOG
    FD_SET(new_sd, &master);

    if(new_sd>fdmax){
        fdmax = new_sd;
    }
}
/*
Riceve la richiesta da un client, in caso di chiusura client
invoca la logout_player per eliminare il player dalle struttre dati
*/
void client_req(int new_sd){
    int ret;
    uint16_t message_len;
    char buffer[BUFFER_DIM];

    ret = recv(new_sd, (void*)&message_len, sizeof(uint16_t), 0);
    if(ret<0){
        perror("Errore in fase di ricezione:\n");
        close(new_sd);
        FD_CLR(new_sd, &master);
        return;
    }
    if(ret == 0){
        printf("Chiusura client rilevata:\n");
        logout_player(new_sd);
        close(new_sd);
        FD_CLR(new_sd, &master);
        return;
    }
    message_len = ntohs(message_len);
    //printf("ricevuta dimensione %u\n", message_len); //LOG
    //memset(buffer, 0, (size_t) message_len);

    ret = recv(new_sd, (void*)buffer, message_len, 0);
    if(ret<0){
        perror("Errore in fase di ricezione:\n");
        return;
    }
    //printf("ricevuta messaggio %s\n", buffer); //LOG
    comandi_server(buffer, new_sd);
}
/*
Riceve il buffer letto dalla recv e il socket descriptor del client
che ha mandato il comando. Preleva il comando usando la strtok con
il delimitatore DELIMITER. A seconda del comando viene riusata la
strtok ma stavolta con lo spazio come delimitatore per ottenere 
i dati allegati al comando
ad esempio "CHKPSW,username password"
Ecco la lista dei comandi e cosa fanno:
CHKUSR controlla che lo username arrivato sia valido, quindi univoco o corrispondente
CHKPSW controlla che la password inviata sia associata allo user specificato
SIGNUP aggiunge una nuova coppia user password
end\n fa finire la partita del giocatore
start fa iniziare la partita nell room specificata
look, take, use, objs svolgono le azioni come da specifiche
done ricevuto sempre dopo una take di un oggetto, sblocca l'oggetto da in uso a non in uso
NEWTKN aggiunge un token alla partita del giocatore 
CHKLOG gestisce la funzionalità log player
*/
void comandi_server(char* buffer, int new_sd){
    char temp[BUFFER_DIM];
    int check;
    char usr [USERNAME_DIM];
    char psw [PASSWORD_DIM];
    strcpy(temp, buffer);
    char* cmd =  strtok(temp, DELIMITER);
    printf("Comando ricevuto: %s\n", cmd);

    if(strcmp(cmd, "CHKUSR") == 0){
        buffer = strtok(NULL, DELIMITER);
        int check = gestisci_user_key(buffer); //gsame_utility
        sprintf(buffer, "%d", check);
        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "CHKPSW") == 0){
        buffer = strtok(NULL, DELIMITER);
        //printf("Ecco il buffer: %s\n", buffer);
        sscanf(buffer, "%s %s", usr, psw);
        //printf("Ricevuti user:%s \n e password:%s\n", usr, psw);
        strcpy(temp, usr);
        check = gestisci_password(usr, psw); //game_utility
        if(check == 1) check = new_active_player(temp, new_sd);
        sprintf(buffer, "%d", check);
        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "SIGNUP") == 0){
        buffer = strtok(NULL, DELIMITER);
        sscanf(buffer, "%s %s", usr, psw);
        check = add_user(usr, psw); //game_utility
        if(check == 1) check=new_active_player(usr, new_sd);
        sprintf(buffer, "%d", check);
        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "end\n")==0){
        logout_player(new_sd);
        close(new_sd);
        FD_CLR(new_sd, &master);
    }else if(strcmp(cmd, "start") == 0){
        buffer = strtok(NULL, DELIMITER);
        int stanza = atoi(buffer);
        //restituisce -1 in caso di errore, altrimenti la partita
        if(stanza-1>=NUM_ROOM){ //caso in cui la room non esista
            check = -1;
            sprintf(buffer, "%d", check);
            send_to(buffer, new_sd);
        }else{
            check = gestisci_ingresso_partita(stanza, new_sd);
            if(check != -1){
                if(partite_in_corso[check].in_game == 1){
                    printf("Mando in broadcast\n");
                    char temp[26];
                    avvia_timer(check, temp);
                
                    strcpy(buffer, temp); //avvia il timer della partita
                    broadcast_partita(check, buffer); //avvia tutti gli utenti
                }
            }else{
                sprintf(buffer, "%d", check);
                send_to(buffer, new_sd);
            }
        }
    }else if(strcmp(cmd, "look") == 0){
        buffer = strtok(NULL, DELIMITER);
        //printf("ricevuto buffer %s\n", buffer);

        check = gestisci_look(buffer, search_player_by_socket(new_sd));
        sprintf(buffer, "%d", check);
        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "take") == 0){
        buffer = strtok(NULL, DELIMITER);
        //printf("ricevuto buffer %s\n", buffer);
        check = gestisci_take(buffer,search_player_by_socket(new_sd));
        sprintf(buffer, "%d", check);

        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "use") == 0){
        buffer = strtok(NULL, DELIMITER);
        printf("ricevuto buffer %s\n", buffer);
        check = gestisci_use(buffer,search_player_by_socket(new_sd));
        sprintf(buffer, "%d", check);

        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "NEWTKN") == 0){
        gestisci_NEWTKN(search_player_by_socket(new_sd));
        
    }else if(strcmp(cmd, "objs\n") == 0){
        gestisci_objs(buffer, search_player_by_socket(new_sd));
        send_to(buffer, new_sd);
    }else if(strcmp(cmd, "done")==0){
        buffer = strtok(NULL, DELIMITER);
        //printf("ricevuto buffer %s\n", buffer);
        gestisci_done(buffer, search_player_by_socket(new_sd));
    }else if(strcmp(cmd, "CHKWIN")==0){
        check = gestisci_CHKWIN(search_player_by_socket(new_sd));
        
        sprintf(buffer, "%d", check);
        send_to(buffer, new_sd);
        if(check == 1){//partita vinta
            logout_player(new_sd);
            close(new_sd);
            FD_CLR(new_sd, &master);
        }
    }else if(strcmp(cmd, "CHKLOG")==0){
        check = search_player_by_socket(new_sd);
        check = gestisci_log_player(buffer, check);
        //printf("Mando il buffer %s\n", buffer);
        if(check == -1)
            sprintf(buffer, "%d", check);

        send_to(buffer, new_sd);
    }else{
        printf("Comando non valido\n");
    }
}
/*
Manda un pacchetto al socket, manda prima la dimensione e poi il messaggio
*/
void send_to(char* buffer, int socket_descriptor){
    uint16_t message_len;
    uint16_t len;
    int ret;
    len = strlen(buffer) + 1;
    message_len = htons(len);
    //printf("Mando la dimensione %u\n", le n);
    ret = send(socket_descriptor, &message_len, sizeof(uint16_t), 0);
    if(ret<0){
        perror("Errore durante l'invio della dimensione\n");
        exit(-1);
    }
    //printf("Mando il pacchetto %s\n", buffer);
    ret = send(socket_descriptor, buffer, len,0);
    if(ret<0){
        perror("Errore durante l'invio del messaggio\n");
        exit(-1);
    }
}
/*
Funzione chimata per finire la creazione della partita, quando l'ultimo giocatore fa l'ingresso
in partita allora viene fatto broadcast a tutti i giocatori in partita rimasti in attesa
*/
void broadcast_partita(int partita, char* buffer){
    int i, sd;
    for(i = 0; i<partite_in_corso[partita].num_giocatori; i++   ){
        sd = partite_in_corso[partita].giocatori[i]->sd;
        send_to(buffer, sd);
    }
}

