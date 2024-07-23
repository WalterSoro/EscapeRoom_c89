CLIENT_OBJ = lib/client/tcp.o lib/client/utility.o lib/client/errori.o lib/client/scenari.o
SERVER_OBJ = lib/server/tcp.o lib/server/utility.o lib/server/errori.o lib/server/game_utility.o lib/server/scenari_serv.o 

all: server client libreria_server libreria_client

client: client.o $(CLIENT_OBJ)
	gcc -Wall client.o $(CLIENT_OBJ) -o client

server: server.o $(SERVER_OBJ)
	gcc -Wall server.o $(SERVER_OBJ) -o server

libreria_client: $(CLIENT_OBJ)
	gcc -Wall client.o $(CLIENT_OBJ) -o libreria_client

libreria_server: $(SERVER_OBJ)
	gcc -Wall server.o $(SERVER_OBJ) -o libreria_server

# regola pe la compilazione 
%.o: %.c
	gcc -Wall -c -o $@ $<

# Regola server
lib/server/%.o: lib/server/%.c
	gcc -Wall -c -o $@ $<

# Regola client
lib/client/%.o: lib/client/%.c
	gcc -Wall -c -o $@ $<

clean:
	rm -f *.o client server 
	rm -f lib/client/*.o  lib/server/*.o
