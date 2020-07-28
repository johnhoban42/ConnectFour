#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include "connectfour.h"


using namespace std;


class CFServer{

    private:
        // Networking
        int serverSocket;
        struct sockaddr_in address;
        int addrlen;

        int redSocket;
        int blackSocket;

        int gameState[8][8];

    public:

        // Resets the game state.
        void resetGameState(){
            for(int row = 0; row < 8; row++){
                for(int col = 0; col < 8; col++){
                    gameState[row][col] = 0;
                }
            }
        }

        // Creates server object.
        CFServer(){
            redSocket = -1;
            blackSocket = -1;
            resetGameState();
        }

        // Property getters
        int getRedSocket(){
            return redSocket;
        }

        int getBlackSocket(){
            return blackSocket;
        }

        // Initialize socket properties and connect to the internet.
        int initialize(){

            addrlen = sizeof(address);
            int opt = 1;

            if((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == 0){
                perror("Socket failed.");
                return FAILURE;
            }

            if(setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
                perror("setsockopt() failed.");
                return FAILURE;
            }

            address.sin_family = AF_INET;
            address.sin_addr.s_addr = INADDR_ANY;
            address.sin_port = htons(PORT);

            if(bind(serverSocket, (struct sockaddr*)&address, sizeof(address)) < 0){
                perror("Bind failed.");
                return FAILURE;
            }

            if(listen(serverSocket, 2) < 0){
                perror("Listen failed.");
                return FAILURE;
            }

            return SUCCESS;

        }

        // Accepts the black player client.
        int acceptBlackClient(){
            if((blackSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
                perror("Couldn't accept black player client.");
                return FAILURE;
            }
            return SUCCESS;
        }

        // Accepts the red player client.
        int acceptRedClient(){
            if((redSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
                perror("Couldn't accept red player client.");
                return FAILURE;
            }
            return SUCCESS;
        }

        /* Read in the new game state from a client and update it on the server.
         * Networking protocol:
         * <row><column> (2 chars representing the newly filled cell)
        */
        int readGameState(int client){
            char newState[2];
            if(read(client, newState, 2) < 0){
                return FAILURE;
            }
            
            int row = newState[1] - '0';
            int col = newState[2] - '0';

            if(client == blackSocket){
                gameState[row][col] = BLACK;
            }else{
                gameState[row][col] = RED;
            }

            return SUCCESS;

        }

        // Wrapper method for sending a message to the black client.
        int sendBlack(string msg){
            int msglen = msg.length();
            char* c_msg = (char*)msg.c_str();
            if(write(blackSocket, c_msg, msglen) < 0){
                return FAILURE;
            }
            return SUCCESS;
        }

        // Wrapper method for sending a message to the red client.
        int sendRed(string msg){
            int msglen = msg.length();
            char* c_msg = (char*)msg.c_str();
            if(write(redSocket, c_msg, msglen) < 0){
                return FAILURE;
            }
            return SUCCESS;
        }

        // Cleans up all networking-related resources.
        int cleanup(){
            if(serverSocket > 0){
                close(serverSocket);
            }
            if(blackSocket > 0){
                close(blackSocket);
            }
            if(redSocket > 0){
                close(redSocket);
            }
            return SUCCESS;
        }

};

int main(){

    /* PART I: CLIENT-SERVER PREPARATIONS */

    CFServer s;

    // Initialize
    cout << "Initializing server..." << endl;
    if(s.initialize() == FAILURE){
        cout << "Couldn't initialize server, aborting." << endl;
        exit(1);
    }

    // Receive black client and notify the client of its color
    cout << "Waiting for first client to connect..." << endl;
    if(s.acceptBlackClient() == FAILURE){
        cout << "Error accepting first client, aborting." << endl;
        exit(1);
    }
    s.sendBlack("black");

    // Receive red client and notify the client of its color
    // Also notify the black client that the red client connected
    cout << "Waiting for second client to connect..." << endl;
    if(s.acceptRedClient() == FAILURE){
        cout << "Error accepting second client, aborting." << endl;
        exit(1);
    }
    s.sendRed("red");
    s.sendBlack("ready");

    cout << "Starting the game now." << endl;
    s.cleanup();
    return 0;
}
