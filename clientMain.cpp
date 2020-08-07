#include "connectfour.h"
#include "client.h"


using namespace std;


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

    /* PART II: GAME SETUP */

    c.resetGameState();

    /* PART III: GAMEPLAY */

    // Let's get started
    string start = c.readMsg(5);
    if(start != "start"){
        c.cleanup();
        return 0;
    }

    // Main game loop
    // Alternate between taking a turn and waiting for the other client
    bool myTurn = (c.getColor() == BLACK) ? true : false;
    bool done = false;
    while(!done){

        if(myTurn){
            c.takeTurn();
            myTurn = false;
        }else{
            myTurn = true;
        }

    }

    
    c.cleanup();
    
    return 0;
}
