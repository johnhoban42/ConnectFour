#include "connectfour.h"
#include "server.h"


using namespace std;


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

    /* PART II: GAME SETUP */
    
    s.resetGameState();
    s.sendBlack("start");
    s.sendRed("start");

    /* PART III: GAMEPLAY */

    s.cleanup();
    return 0;
}
