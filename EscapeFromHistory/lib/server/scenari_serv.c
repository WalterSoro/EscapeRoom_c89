#include "scenari_serv.h"

int init_obj_room_1(Object* oggetti){
    //dipinto
    strcpy(oggetti[0].nome, "dipinto");
    oggetti[0].in_use = 0;
    oggetti[0].location = 0;
    oggetti[0].code_obj = 0;
    oggetti[0].code_room = 1;
    oggetti[0].collectable = 0;
    oggetti[0].item = 3;
    oggetti[0].answer = 0;
    oggetti[0].usable = 0;

    //scrigno
    strcpy(oggetti[1].nome, "scrigno");
    oggetti[1].in_use = 0;
    oggetti[1].location = 0;
    oggetti[1].code_obj = 1;
    oggetti[1].code_room = 1;
    oggetti[1].collectable = 0; //non collezionabile
    oggetti[1].item = 3; //chiave
    oggetti[1].answer = 0;
    oggetti[1].usable = -1; //non utilizzabile da solo

    //bigliettino
    strcpy(oggetti[2].nome, "bigliettino");
    oggetti[2].in_use = 0;
    oggetti[2].location = 0;
    oggetti[2].code_obj = 2;
    oggetti[2].code_room = 1;
    oggetti[2].collectable = 1; //collezionabile
    oggetti[2].item = -1; 
    oggetti[2].answer = 0;
    oggetti[2].usable = 1; //utilizzabile da solo

    //chiave
    strcpy(oggetti[3].nome, "chiave");
    oggetti[3].in_use = 0;
    oggetti[3].location = 0;
    oggetti[3].code_obj = 3;
    oggetti[3].code_room = 1;
    oggetti[3].collectable = 1; //collezionabile
    oggetti[3].item = -1; 
    oggetti[3].answer = 1; //enigma associato
    oggetti[3].usable = -1; //non utilizzabile da solo

    //calendario
    strcpy(oggetti[4].nome, "calendario");
    oggetti[4].in_use = 0;
    oggetti[4].location = 0;
    oggetti[4].code_obj = 4;
    oggetti[4].code_room = 1;
    oggetti[4].collectable = 0; //collezionabile
    oggetti[4].item = -1; 
    oggetti[4].answer = 0; //enigma associato
    oggetti[4].usable = -1; //non utilizzabile da solo

    //moneta
    strcpy(oggetti[5].nome, "moneta");
    oggetti[5].in_use = 0;
    oggetti[5].location = 0;
    oggetti[5].code_obj = 5;
    oggetti[5].code_room = 1;
    oggetti[5].collectable = 1; //collezionabile
    oggetti[5].item = -1; 
    oggetti[5].answer = 1; //enigma associato
    oggetti[5].usable = -1; //non utilizzabile da solo

    //scaffale
    strcpy(oggetti[6].nome, "scaffale");
    oggetti[6].in_use = 0;
    oggetti[6].location = 1;
    oggetti[6].code_obj = 6;
    oggetti[6].code_room = 1;
    oggetti[6].collectable = -1; 
    oggetti[6].item = -1; 
    oggetti[6].answer = 0;
    oggetti[6].usable = -1; //non utilizzabile da solo

    //porta
    strcpy(oggetti[7].nome, "porta");
    oggetti[7].in_use = 0;
    oggetti[7].location = 1;
    oggetti[7].code_obj = 7;
    oggetti[7].code_room = 1;
    oggetti[7].collectable = -1; 
    oggetti[7].item = -1; 
    oggetti[7].answer = 0;
    oggetti[7].usable = -1; //non utilizzabile da solo

    //altare
    strcpy(oggetti[8].nome, "altare");
    oggetti[8].in_use = 0;
    oggetti[8].location = 1;
    oggetti[8].code_obj = 8;
    oggetti[8].code_room = 1;
    oggetti[8].collectable = -1; //non collezionabile
    oggetti[8].item = -1; 
    oggetti[8].answer = 0;
    oggetti[8].usable = 1; //non utilizzabile da solo

    //statuetta
    strcpy(oggetti[9].nome, "statuetta");
    oggetti[9].in_use = 0;
    oggetti[9].location = 0;
    oggetti[9].code_obj = 9;
    oggetti[9].code_room = 1;
    oggetti[9].collectable = 1; //collezionabile
    oggetti[9].item = -1; 
    oggetti[9].answer = 1; //enigma associato
    oggetti[9].usable = -1; //non utilizzabile da solo


    return 10; //numero di oggetti room 1
}

int init_game_obj(int room, Object* oggetti_partita){
    printf("Inizializzo partita con stanza numero: %d\n", room);
    if(room == 1)
        return init_obj_room_1(oggetti_partita);
    /*
    ampliabile per altre rooms
    */
    return 0;
}

int get_tokens_room(int room){
    if(room == 1)
        return TOKEN_ROOM_1;
    /*
    ampliabile per altre rooms
    */
    return 0;
}
