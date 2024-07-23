#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "lib/server/tcp.h"

int server_on; // settata dalla funzione start server, resettata da stop server
// flag on/off del server

int main(int argc, char* argv[]){
    server_on = 0; 

    int listener;
    int i;
    int ret;
    struct sockaddr_in serv_addr;//, client_addr;


    listener = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    serv_addr.sin_family = AF_INET;

    serv_addr.sin_port = htons(4242);
    

    ret = bind(listener, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    ret = listen(listener, 10);

    //azzero i set
    FD_ZERO(&master);
    FD_ZERO(&read_fds);
    //aggiunta di stdin al set dei monitorati
    FD_SET(0, &master);
    FD_SET(listener, &master);
    fdmax = listener;

    print_comandi_server();
    stdin_server();

    configurazione_gioco();

    while(server_on == 1){
        read_fds = master;

        ret = select(fdmax+1, &read_fds, NULL, NULL, NULL);
        if(ret<0){
            perror("ERRORE nella select\n");
            exit(1);
        }
        for(i=0; i<=fdmax && server_on == 1; i++){
            if(FD_ISSET(i, &read_fds)){
                if(i == 0){ // stdin
                    stdin_server();
                }else if(i == listener){ //nuovo client rilevato
                    listener_server(listener);
                }else{ //gestisco la richiesta
                    client_req(i);
                }
            }
        }
    }
    printf("FINE\n");
    return 0;
}



