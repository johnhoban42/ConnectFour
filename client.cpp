#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include "connectfour.h"


using namespace std;


class CFClient{

    private:
        int serverSocket;
        int clientSocket;
        int color;

    public:

        // Creates client object.
        CFClient(){
            serverSocket = -1;
            clientSocket = -1;
            color = -1;
            return;
        }

        // Client color access method
        int getColor(){
            return color;
        }

        // Client color setter method
        void setColor(int clr){
            if(clr != RED && clr != BLACK){
                return;
            }
            color = clr;
        }

        // Initializes socket properties and connect to the server.
        int initialize(){
            
            if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
                perror("Socket failed.");
                return FAILURE;
            }

            struct hostent* host = gethostbyname(HOST);

            struct sockaddr_in serverAddress;
            memset(&serverAddress, 0, sizeof(serverAddress));
            serverAddress.sin_family = AF_INET;
            serverAddress.sin_port = htons(PORT);
            memcpy((char*)host->h_addr_list[0], (char*)&serverAddress.sin_addr.s_addr, host->h_length);

            if(connect(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0){
                perror("Couldn't connect to server.");
                return FAILURE;
            }

            return SUCCESS;

        }

        // Wrapper method for reading a message from the server
        string readMsg(int len){
            char buffer[len + 1];
            memset(buffer, 0, len + 1);
            read(serverSocket, buffer, len);
            string s(buffer);
            return s;
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

    /* PART I: CLIENT-SERVER PREPARATIONS */

    CFClient c;

    // Connect to the server and determine the player's color
    cout << "Connecting to server..." << endl;
    if(c.initialize() == FAILURE){
        cout << "Couldn't connect to the server, aborting." << endl;
        exit(1);
    }
    string color = c.readMsg(5);
    cout << "Connected." << endl;
    if(color == "black"){
        // If we're the first client to connect, we have to wait for the red client too
        c.setColor(BLACK);
        cout << "Your color is BLACK." << endl;
        cout << "Waiting for the red player to connect..." << endl;
        string ready = c.readMsg(5);
    }else{
        c.setColor(RED);
        cout << "Your color is RED." << endl;
    }
    cout << "Both players are ready, let's start the game!" << endl;

    
    c.cleanup();
    
    return 0;
}