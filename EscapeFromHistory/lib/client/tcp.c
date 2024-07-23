#include "tcp.h"

/*
Prima di riceve la dimensione del messaggio in mopdalità binary e poi
il messaggio in modalità text
*/
void recv_from(char* buffer, int socket_decscriptor){
    int ret;
    uint16_t len;
    uint16_t message_len;

    memset(buffer, 0, strlen(buffer));

    ret = recv(socket_decscriptor, (void*)&message_len, sizeof(uint16_t), 0);
    if(ret<0){
        perror("Errore in fase di ricezione:\n");
        exit(-1);
    }

    len = ntohs(message_len);
    
    //printf("Ricevuta la DIMENSIONE %u\n", len);

    ret = recv(socket_decscriptor, buffer, len, 0);
    if(ret<=0){
        perror("Errore in fase di ricezione\n");
        exit(-1);
    }
}
/*
Prima di manda la dimensione del messaggio in modalità binary e poi
il messaggio in modalità text
*/
void send_to(char* buffer, int socket_descriptor){
    uint16_t message_len;
    uint16_t len;
    int ret;
    len = strlen(buffer) + 1;
    message_len = htons(len);
    //printf("Mando la dimensione %u\n", len);
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
Funzione principale del client, gestisce tutti i comandi ed effettua 
ricerche. La citicità di questa funzione è la troppa responsabilità. 
Sarebbe meglio fare funzioni apposite per ogni comando
*/

int comandi_client(char* cmd, char* buffer, int sd){
    //int ans;
    //printf("Funzione comandi client\n");
    char pack[BUFFER_DIM];
    int check;
    int ind;
    if(strcmp(cmd, "CHKUSR") == 0){
        strcpy(pack, cmd);
        strcat(pack, DELIMITER);
        strcat(pack, buffer);
        //printf("Mando il pacchetto %s\n", pack);

        send_to(pack, sd);
        recv_from(buffer, sd);
        return atoi(buffer);
    }else if(strcmp(cmd, "CHKPSW") == 0){
        strcpy(pack, cmd);
        strcat(pack, DELIMITER);
        strcat(pack, buffer);
        //printf("Mando il pacchetto %s\n", pack);

        send_to(pack, sd);
        recv_from(buffer, sd);
        return atoi(buffer);
    }else if(strcmp(cmd, "SIGNUP") == 0){
        strcpy(pack, cmd);
        strcat(pack, DELIMITER);
        strcat(pack, buffer);
        //printf("Mando il pacchetto %s\n", pack);
        send_to(pack, sd);
        recv_from(buffer, sd);
        return atoi(buffer);
    }else if(strcmp(cmd, "CHKWIN")==0){
        send_to(cmd, sd);
        recv_from(pack, sd);
        check = atoi(pack);
        if(check == 1){
            gestisci_vittoria();
            in_game = 0;
            start_timer = 0;
        }
    }else if(strcmp(cmd, "CHKLOG")==0){
        send_to(cmd, sd);
        recv_from(pack, sd);
        if(atoi(pack) == -1)
            return 0;
        printf(GREEN"%s"RESET"\n", pack);
        
    }else if(strcmp(cmd, "start")==0){
        int room;
        strcpy(pack, cmd);
        strcat(pack, DELIMITER);
        
        strcat(pack, buffer);
        room = atoi(strtok(buffer, " ")); //prendo il numero della room
        if(room <= 0){
            printf(RED"codice room bagliato o mancante"RESET"\n");
            return -1;
        }
        send_to(pack, sd);
        // qui andrebbe implementato qualcosa in più 
        // per capire che risposta è arrivata
        printf("In attesa di giocatori...\n");
        
        fflush(stdout);
        recv_from(buffer, sd); 
        if(atoi(buffer) == -1){
            printf("Codice romm non esistente\n");
        }else{
            init_timer(buffer);
            start_timer = 1;
            printf("Orario di inizio partita: %s\n", asctime(&timer));
            partita_attuale.room = room;
            partita_attuale.token_guadagnati = 0;
            mostra_room(room);
            time_out = 1;
        }
    }else if(strcmp(cmd, "look")==0){
        buffer[strlen(buffer) - 1] = '\0';
        ind = get_obj_index(buffer);
        //printf("Cerco l'ggetto %s\n",buffer);

        if((ind) == -1) {
            printf(RED"Parametro mancante o errato"RESET"\n");
            return -1;
        }
        strcpy(buffer, cmd);
        strcat(buffer, DELIMITER);
        sprintf(pack, "%d", ind);
        strcat(buffer, pack);
        send_to(buffer, sd);

        recv_from(buffer, sd);

        check = atoi(buffer);
        print_errori(check);
        if(check == 0){
            system("clear");
            printf("Descrizione oggetto:\n%s\n", rooms[partita_attuale.room-1].oggetti[ind].descrizione);
            printf("\nPremi INVIO per continuare...");
            getchar();
        }
    }else if(strcmp(cmd, "take")==0){
        buffer[strlen(buffer) - 1] = '\0';
        ind = get_obj_index(buffer);
        //printf("Cerco l'oggetto %s con indice %d\n",buffer, ind);
        if((ind) == -1) {
            printf(RED"Parametro mancante o errato"RESET"\n");
            return -1;
        }
        strcpy(buffer, cmd);
        strcat(buffer, DELIMITER);
        sprintf(pack, "%d", ind);
        strcat(buffer, pack);
        
        send_to(buffer, sd);
        
        recv_from(buffer, sd);
        check = atoi(buffer);

        print_errori(check);
        if(check == 1) {
            risolvi_enigma(ind);
            print_event_obj(ind);
            partita_attuale.token_guadagnati++;
            strcpy(buffer, "NEWTKN");
            send_to(buffer, sd);
                 
            //sprintf(buffer, "%s,%d", "done", ind);
            //send_to(buffer, sd); //mandoconferma successo al buffer
        }
        if(check == 1  || check == 0){ // compreso caso senza enigma
            sprintf(buffer, "%s,%d", "done", ind);
            send_to(buffer, sd); //mandoconferma successo al buffer
        }
    }else if(strcmp(cmd, "use")==0){
        char* temp;
        temp = strtok(buffer, " ");
        strcpy(pack, temp); //mi serve per la use ad un parametro
        if(temp && temp[strlen(temp) - 1] == '\n'){
            temp[strlen(temp) - 1] = '\0';
        }
        ind = get_obj_index(temp);
        strcpy(pack, temp); //mi serve per la use ad un parametro
        if((ind) == -1) {
            printf(RED"Parametro mancante o errato"RESET"\n");
            return -1;
        }
        temp = strtok(NULL, " ");
        
        if(!temp){//formato ad 1
            //Con la use ad un parametro mi rimane il carattere \n
            sprintf(buffer, "%s,%d", cmd, ind);
        }else{//formato a 2
            temp[strlen(temp)-1] = '\0';
            //printf("Mando la use su %d %d\n", ind, get_obj_index(temp));
            sprintf(buffer, "%s,%d %d", cmd, ind, get_obj_index(temp));
        }
        //printf("mando il buffer: %s\n", buffer);
        send_to(buffer, sd);

        recv_from(buffer, sd);
        check = atoi(buffer);
        print_errori(check);
        if(check == 1)
            check = print_event_obj(ind);
        if(check == 1){
            partita_attuale.token_guadagnati++;
            strcpy(buffer, "NEWTKN");
            send_to(buffer, sd);
        }

    }else if(strcmp(cmd, "objs\n")==0){
        send_to(cmd, sd);
        recv_from(buffer, sd);
        printf("ECCO IL TUO INVENTARIO:\n%s\n", buffer);
    }else if(strcmp(cmd, "end\n")==0){
        send_to(cmd, sd);
        in_game = 0;
        start_timer = 0;
    }else{
        printf("Comando %s non valido\n", cmd);
        return -1;
    }
    return 0;
}
