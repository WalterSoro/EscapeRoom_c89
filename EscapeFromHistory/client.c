#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h> //mi serve per la funzione isspace
#include <sys/select.h>
#include <time.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//#include "lib/client/utility.h"
#include "lib/client/tcp.h"

#define MSG_LEN 9
#define USERNAME_DIM 15
#define PASSWORD_DIM 50

int sd;
struct sockaddr_in server_addr;
char buffer[BUFFER_DIM];
char buffer_input[BUFFER_DIM];

void accesso();
void gestisci_comandi_client();

int main(int argc, char* argv[]){
    //int success;
    int ret;
    int s_port = 4242;
    
    sd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(s_port);
    inet_pton(AF_INET,"127.0,0.1", &server_addr.sin_addr.s_addr);

    ret = connect(sd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret<0){
        perror("Errore nella connessione: \n");
        exit(-1);
    }

    accesso();
    sleep(2);
    in_game = 1;
    time_out = 0;
    start_timer = 0;

    init_room();
    mostra_schermata_iniziale();
    while(in_game==1){
        print_comandi_client();
        gestisci_comandi_client();
    }

    close(sd);
    printf("Fine client\n");

    return 0;
}
/*
funzione che legge i comandi del client e fa la chiamata a comandi_client nel modulo tcp
gestisce in locale il comando time, e ogni volta mostra il tempo rimanente
manda sempre anche il comando CHKLOG e CHKWIN
*/
void gestisci_comandi_client(){
    /*Recupero il comando*/
    char* temp;
    char* ret;
    memset(buffer_input, 0, BUFFER_DIM);
    do{
        ret = fgets(buffer_input, 1024, stdin);
    }
    while((int)strlen(ret) == 1);
    

    strcpy(buffer, buffer_input);
    //printf("%s", buffer_input);
    char* cmd = strtok(buffer_input, " ");
    
    if(strcmp(cmd, "room\n")==0){
        mostra_room(partita_attuale.room);
        return;
    }
    if(strcmp(cmd, "use")==0){
        strcpy(buffer, strlen(cmd)+1+buffer );// sottostringa
    }else{
        temp = strtok(NULL, " ");
        if(temp) 
            strcpy(buffer, temp); //temp è null se si manda la end o la objs

        if((strcmp(cmd, "time\n")==0)){
            check_timer(timer);
            return;
        }

        if(temp) strcpy(buffer, temp);

    }
    //temp è null se si manda la end
    if(strcmp(cmd, "end\n") == 0){
        printf("Sei uscito dalla partita\n");
    }

    //mando il comando scelto
    comandi_client(cmd, buffer, sd);
    
    // ogni volta mando la CHKWIN e la CHKLOG e il timer
    if(strcmp(cmd, "start")!=0 && start_timer == 1){
        printf("\n");
        if(check_timer(timer) == -1){
            time_out = 1;
            printf("TEMPO SCADUTO\n");
            in_game=0;
            strcpy(cmd, "end\n");
            comandi_client(cmd, buffer, sd);
            return;
        } 

        strcpy(buffer, "\0");        
        strcpy(cmd,"CHKLOG");
        comandi_client(cmd, buffer, sd);

        strcpy(cmd,"CHKWIN");
        comandi_client(cmd, buffer, sd);

    }

}

/*
Funzione che gestisce il login, mandando i comandi CHKUSR per controllare lo username
e il comando CHKPSW per sapere se la password associata è corretta. In caso di errore 
stampa un messaggio di errore 
*/

int login(int sd){
    char username[USERNAME_DIM];
    char password[PASSWORD_DIM];
    int ret;
    printf("Inserisci Username(max 14 caratteri): ");
    scanf("%s", username);

    strcpy(buffer, username);
    ret = comandi_client("CHKUSR", buffer, sd);
    if(ret == -1){
        printf("Username errato\n");
        return -1;
    }
    printf("inserisci Password: ");
    
    scanf("%s",password);
    strcpy(buffer, username);
    strcat(buffer, " ");
    strcat(buffer, password);
    
    //mando sia username che password
    ret = comandi_client("CHKPSW", buffer, sd);
    if(ret == -1){
        printf("Password errata\n");
        return -1;
    } 
    if(ret == 1)
        printf("Benvenuto %s\n", username);
    return 1;
    
}

/*
Funzione che gestisce la registrazione di un nuov utente, prima controllando che lo username 
sia accettabile, e poi mandando la CHKUSR
Controlla anche che la password sia accettabile e poi manda il comando SIGNUP
*/

int signup(int sd){
    int check = 0;
    int ret;
    char  username [USERNAME_DIM];
    char password [PASSWORD_DIM];
    while(check != 1){
        printf("Inserisci Username(max 14 caratteri): ");
        scanf("%s", username);
        //fgets(username, USERNAME_DIM, stdin);
        if(strlen(username) == 0){
             printf("Errore: lo username non può essere vuoto\n");
             continue;
        }
        // Controlla che lo username non contenga spazi
        if(strchr(username, ' ') != NULL){
            printf("Errore: lo username non deve contenere spazi\n");
            continue;
        }     
        //printf("Mando il check username\n"); //LOG
        memset(buffer, 0, strlen(buffer));
        strcpy(buffer, username);
        ret = comandi_client("CHKUSR", buffer, sd);
        if(ret == 1){
            printf("Usernname non disponibile\n");
            continue;
        }
        check = 1;        
    }
    check = 0;
    while(check != 1){
        printf("Inserisci Password: ");
        scanf("%s", password);
        if(strlen(password) == 0){
             printf("Errore: la password non può essere vuota\n");
             continue;
        }
        // Controlla che lo username non contenga spazi
        if(strchr(password, ' ') != NULL){
            printf("Errore: lo password non deve contenere spazi\n");
            continue;
        }     
        check = 1;        
    }

    memset(buffer, 0, sizeof(buffer));
    strcat(buffer, username);
    strcat(buffer, " ");
    strcat(buffer, password);
    //printf("Mando il singup con buffer: %s\n", buffer); //LOG
    ret = comandi_client("SIGNUP", buffer, sd);
    if(ret == -1){
        printf("Errore durante la registrazione\n");
    }
    if(ret == 1){
        printf("Registrazione avvenuta con successo!\n");
    }
    return ret;
}
/*
Funzione intermedia che gestisce il login e la signup
*/
void accesso(){
    char c;
    int ret = 0;
    while(ret != 1){
        printf(BACKGREEN"****************** LOG IN ******************"RESET"\n");
        printf("Hai già un account? (s/n): ");
        scanf("%c", &c);

        if(c=='s'){
            ret = login(sd);
        }else if(c=='n'){
            ret = signup(sd);
        }else 
            printf("\nRISPOSTA NON VALIDA\n");
        c = getchar(); // serve per leggere \n rimasto nello stream
        printf(BACKGREEN"********************************************"RESET"\n\n\n");
    } 
}