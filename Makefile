all: cfserver cfclient

cfserver: server.cpp
	g++ -Wall -Werror server.cpp -o cfserver

cfclient: client.cpp
	g++ -Wall -Werror client.cpp -o cfclient

clean:
	rm -f cfserver cfclient