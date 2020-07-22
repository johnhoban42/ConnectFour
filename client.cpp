#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include "connectfour.h"


class CFClient{

    private:
        int serverSocket;
        int clientSocket;

    public:

        // Creates client object.
        CFClient(){
            return;
        }

        // Initializes socket properties and connect to the server.
        void initialize(){
            
            if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
                perror("Socket failed.");
                exit(EXIT_FAILURE);
            }

            struct hostent* host = gethostbyname(HOST);

            struct sockaddr_in serverAddress;
            memset(&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(PORT);
            memcpy((char*)host->h_addr_list[0], (char*)&serverAddress.sin_addr.s_addr, host->h_length);

            if(connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
                perror("Couldn't connect to server.");
                exit(EXIT_FAILURE);
            }

        }

        // Cleans up all networking-related resources.
        void cleanup(){
            if(serverSocket){
                close(serverSocket);
            }
            if(clientSocket){
                close(clientSocket);
            }
        }

};

int main(){
    CFClient b;
    b.initialize();
    //write(b.getServerSocket(), "hello", 6);
    b.cleanup();
    return 0;
}