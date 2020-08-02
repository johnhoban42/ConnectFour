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
        void resetGameState();

        // Creates server object.
        CFServer();

        // Property getters
        int getRedSocket();
        int getBlackSocket();

        // Initialize socket properties and connect to the internet.
        int initialize();

        // Accepts the black player client.
        int acceptBlackClient();

        // Accepts the red player client.
        int acceptRedClient();

        /* Read in the new game state from a client and update it on the server.
         * Networking protocol:
         * <row><column> (2 chars representing the newly filled cell)
        */
        int readGameState(int client);

        /* Check for a winning game state about the most recent filled cell.
         * Returns RED or BLACK on winner, -1 otherwise.
         */
        int checkForWinner(int row, int col);

        // Wrapper method for sending a message to the black client.
        int sendBlack(string msg);

        // Wrapper method for sending a message to the red client.
        int sendRed(string msg);

        // Cleans up all networking-related resources.
        int cleanup();

};