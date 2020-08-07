#include "connectfour.h"


using namespace std;


class CFClient{

    private:
        int serverSocket;
        int clientSocket;
        int color;

        int gameState[8][8];

    public:

        // Creates client object.
        CFClient();

        // Client color access method
        int getColor();

        // Client color setter method
        void setColor(int clr);

        // Resets the game state.
        void resetGameState();

        // Initializes socket properties and connect to the server.
        int initialize();

        // Wrapper method for reading a message from the server
        string readMsg(int len);

        /* Read in the new game state from a client and update it on the server.
         * Networking protocol:
         * <row><column> (2 chars representing the newly filled cell)
         */
        int readGameState();

        /* Send a new game state to the server.
         * Networking protocol:
         * <row><column> (2 chars representing the newly filled cell)
        */
        int sendGameState(int row, int col);

        // Wrapper method for sending a message to the black client.
        int sendMsg(string msg);

        // Method that involves all aspects of taking a turn
        void takeTurn();

        // Cleans up all networking-related resources.
        void cleanup();

};
