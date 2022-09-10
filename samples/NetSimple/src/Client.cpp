/////////////////////////////
// Client.cpp
//


#include "wxx_socket.h"
#include <iostream>


using namespace std;

// UDP Client.
// Initiate a connect to the UDP server by sending it some text.


///////////////////////////////////////////////////
// CClientSocket manages the network socket used by
// the client.
class CClientSocket : public CSocket
{
public:
    CClientSocket() {}
    virtual void OnReceive()
    {
        // This function is called automatically when there is data to receive.
        // Has an extra character for null termination.
        char str[1025] = {0};
        int i = Receive(str, 1024, 0);
        if (i >= 0)
            cout << i << " chars received: " << str << endl;
        else
            cout << "Failed to receive" << endl;
    }
};

int main()
{
    CClientSocket client;

    // Create the socket to communicate with the UDP Server
    if (!client.Create(AF_INET, SOCK_DGRAM))
    {
        cout << "Failed to create socket\n" ;
        return 0;
    }

    cout << "Type data to send to connect to the UDP server" << endl;
    cout << "Type quit to exit\n";

    // Monitor the client socket for network events, such as data ready to receive
    client.StartEvents();

    // Send data to the server
    string s;

    for (;;)    // Infinite loop
    {
        getline(cin, s);
        if (s == "quit") break;
        int i = client.SendTo(s.c_str(), static_cast<int>(s.length()), 0, "127.0.0.1", 3000);
        cout << "Sending  " << i << " characters\n";
    }

    return 0;
}

