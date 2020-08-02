#include "connectfour.h"


using namespace std;


class CFClient{

    private:
        int serverSocket;
        int clientSocket;
        int color;

    public:

        // Creates client object.
        CFClient();

        // Client color access method
        int getColor();

        // Client color setter method
        void setColor(int clr);

        // Initializes socket properties and connect to the server.
        int initialize();

        // Wrapper method for reading a message from the server
        string readMsg(int len);

        // Cleans up all networking-related resources.
        void cleanup();

};
