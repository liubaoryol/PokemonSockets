CC=gcc
CFLAGS=-pthread
OS=Fedora


makeall: clientmake servermake

clientmake: PokemonClient.c
	$(CC) PokemonClient.c -o clientPoke $$(mysql_config --cflags --libs)

servermake: TCPMainServer-Thread.c
	$(CC) $(CFLAGS) TCPMainServer-Thread.c -o TCPMainServer-Thread

clean:
	rm -f clientPoke TCPMainServer-Thread


