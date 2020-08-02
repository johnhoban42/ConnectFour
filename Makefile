CFLAGS = -g -Wall -Werror
CC = g++

all: cfserver cfclient

cfserver: serverMain.cpp server.o
	$(CC) $(CFLAGS) serverMain.cpp server.o -o cfserver

server.o: server.cpp
	$(CC) $(CFLAGS) -c server.cpp

cfclient: client.cpp client.o
	$(CC) $(CFLAGS) clientMain.cpp client.o -o cfclient 

client.o: client.cpp
	$(CC) $(CFLAGS) -c client.cpp

clean:
	rm -f cf* *.o
