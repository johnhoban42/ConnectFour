#include "connectfour.h"
#include "client.h"


using namespace std;


// Creates client object.
CFClient::CFClient(){
    serverSocket = -1;
    clientSocket = -1;
    color = -1;
    return;
}

// Client color access method
int CFClient::getColor(){
    return color;
}

// Client color setter method
void CFClient::setColor(int clr){
    if(clr != RED && clr != BLACK){
        return;
    }
    color = clr;
}

// Initializes socket properties and connect to the server.
int CFClient::initialize(){
    
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
string CFClient::readMsg(int len){
    char buffer[len + 1];
    memset(buffer, 0, len + 1);
    read(serverSocket, buffer, len);
    string s(buffer);
    return s;
}

// Cleans up all networking-related resources.
void CFClient::cleanup(){
    if(serverSocket){
        close(serverSocket);
    }
    if(clientSocket){
        close(clientSocket);
    }
}
