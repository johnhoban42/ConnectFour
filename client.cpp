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

// Resets the game state.
void CFClient::resetGameState(){
    for(int row = 0; row < 8; row++){
        for(int col = 0; col < 8; col++){
            gameState[row][col] = 0;
        }
    }
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

    resetGameState();

    return SUCCESS;

}

// Wrapper method for reading a message from the server
string CFClient::readMsg(int len){
    char buffer[len + 1];
    memset(buffer, 0, len + 1);
    int status = read(serverSocket, buffer, len);
    if(status == -1){
        return nullptr;
    }
    string s(buffer);
    return s;
}

/* Read in the new game state from a client and update it on the server.
    * Networking protocol:
    * <row><column> (2 chars representing the newly filled cell)
*/
int CFClient::readGameState(){
    char newState[2];
    if(read(serverSocket, newState, 2) < 0){
        return FAILURE;
    }
    
    int row = newState[1] - '0';
    int col = newState[2] - '0';

    if(color == BLACK){
        gameState[row][col] = RED;
    }else{
        gameState[row][col] = BLACK;
    }

    return SUCCESS;

}

/* Send a new game state to a client.
 * Networking protocol:
 * <row><column> (2 chars representing the newly filled cell)
 */
int CFClient::sendGameState(int row, int col){
    stringstream ss;
    ss << row << col;
    string s = ss.str();
    sendMsg(s);
    return SUCCESS;
}

// Wrapper method for sending a message to the server.
int CFClient::sendMsg(string msg){
    int msglen = msg.length();
    char* c_msg = (char*)msg.c_str();
    if(write(serverSocket, c_msg, msglen) < 0){
        return FAILURE;
    }
    return SUCCESS;
}

// Method that involves all aspects of taking a turn
void CFClient::takeTurn(){
    
    readGameState();

    // Print the game state
    for(int row = 0; row < 8; row++){
        cout << "+---+---+---+---+---+---+---+---+" << endl;
        for(int col = 0; col < 8; col++){
            switch(gameState[row][col]){
                case BLACK:
                    cout << "+ B ";
                    break;
                case RED:
                    cout << "+ R ";
                    break;
                case EMPTY:
                    cout << "+   ";
            }
            
        }
        cout << "+" << endl;
    }
    cout << "+---+---+---+---+---+---+---+---+\n" << endl;

    // Determine valid moveset and display them
    cout << "Choose the column for your next move:" << endl;
    cout << ">>> ";
    bool validMoves[8];
    for(int col = 0; col < 8; col++){
        if(gameState[0][col] == EMPTY){
            validMoves[col] = true;
            cout << "  " << col << " ";
        }else{
            validMoves[col] = false;
            cout << "    ";
        }
    }
    cout << "\n" << endl;

    // Allow the player to choose now
    bool validChoice = false;
    int colChoice;
    while(!validChoice){
        cin >> colChoice;
        if(0 <= colChoice && colChoice < 8 && validMoves[colChoice] == true){
            validChoice = true;
        }else{
            cout << "Illegal move, please choose a column with an empty cell." << endl;
        }
    }

    // Place the player's "chip" in the lowest possible cell in the column and send
    // send the new game state to the server
    int rowChoice;
    for(rowChoice = 7; rowChoice >= 0; rowChoice--){
        if(gameState[rowChoice][colChoice] == EMPTY){
            gameState[rowChoice][colChoice] = color;
            break;
        }
    }
    sendGameState(rowChoice, colChoice);

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
