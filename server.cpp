#include "connectfour.h"
#include "server.h"


using namespace std;


// Resets the game state.
void CFServer::resetGameState(){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            gameState[row][col] = 0;
        }
    }
}

// Creates server object.
CFServer::CFServer(){
    redSocket = -1;
    blackSocket = -1;
    resetGameState();
}

// Property getters
int CFServer::getRedSocket(){
    return redSocket;
}

int CFServer::getBlackSocket(){
    return blackSocket;
}

// Initialize socket properties and connect to the internet.
int CFServer::initialize(){

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
int CFServer::acceptBlackClient(){
    if((blackSocket = accept(serverSocket, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0){
        perror("Couldn't accept black player client.");
        return FAILURE;
    }
    return SUCCESS;
}

// Accepts the red player client.
int CFServer::acceptRedClient(){
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
int CFServer::readGameState(int client){
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

/* Send a new game state to a client.
 * Networking protocol:
 * <row><column> (2 chars representing the newly filled cell)
 */
int CFServer::sendGameState(int client, int row, int col){
    stringstream ss;
    ss << row << col;
    string s = ss.str();
    if(client == blackSocket){
        sendBlack(s);
    }else{
        sendRed(s);
    }
    return SUCCESS;
}

/* Check for a winning game state about the most recent filled cell.
    * Returns RED or BLACK on winner, -1 otherwise.
    */
int CFServer::checkForWinner(int row, int col){
    
    // Horizontal 4-in-a-rows
    int x0 = max(0, col-3);
    for(int x = x0; x <= col; x++){
        if(x + 3 >= 8){
            break;
        }
        switch(gameState[row][x] + gameState[row][x+1] + gameState[row][x+2] + gameState[row][x+3]){
            case 4 * BLACK:
                return BLACK;
            case 4 * RED:
                return RED;
        }
    }

    // Vertical 4-in-a-rows
    int y0 = max(0, row-3);
    for(int y = y0; y <= row; y++){
        if(y + 3 >= 8){
            break;
        }
        switch(gameState[y][col] + gameState[y+1][col] + gameState[y+2][col] + gameState[y+3][col]){
            case 4 * BLACK:
                return BLACK;
            case 4 * RED:
                return RED;
        }
    }

    // Top-left to bottom-right diagonals
    int d0 = max(-3, max(-row, -col));
    for(int d = d0; d <= 0; d++){
        if(row + d + 3 >= 8 || col + d + 3 >= 8){
            break;
        }
        switch(gameState[row+d][col+d] + gameState[row+d+1][col+d+1]
            + gameState[row+d+2][row+d+2] + gameState[row+d+3][col+d+3]){
            case 4 * BLACK:
                return BLACK;
            case 4 * RED:
                return RED;
        }
    }

    // Bottom-left to top-right diagonals
    d0 = max(-3, max(-(7-row), -col));
    for(int d = d0; d <= 0; d++){
        if(row - d - 3 < 0 || col + d + 3 >= 8){
            break;
        }
        switch(gameState[row-d][col+d] + gameState[row-d-1][col+d+1]
            + gameState[row-d-2][row+d+2] + gameState[row-d-3][col+d+3]){
            case 4 * BLACK:
                return BLACK;
            case 4 * RED:
                return RED;
        }
    }

    return -1;
}

// Wrapper method for sending a message to the black client.
int CFServer::sendBlack(string msg){
    int msglen = msg.length();
    char* c_msg = (char*)msg.c_str();
    if(write(blackSocket, c_msg, msglen) < 0){
        return FAILURE;
    }
    return SUCCESS;
}

// Wrapper method for sending a message to the red client.
int CFServer::sendRed(string msg){
    int msglen = msg.length();
    char* c_msg = (char*)msg.c_str();
    if(write(redSocket, c_msg, msglen) < 0){
        return FAILURE;
    }
    return SUCCESS;
}

// Cleans up all networking-related resources.
int CFServer::cleanup(){
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
