/////////////////////////////
// Server.cpp
//

#include "wxx_socket.h"
#include <iostream>


// This UDP server can connect to multiple UDP clients.
// The server sends to the client from which it most recently received data.


using namespace std;

// The socket address of the most recently connected UDP client.
SOCKADDR g_saUDPClient;


///////////////////////////////////////////////////
// CServerSocket manages the network socket used by
// the server.
class CServerSocket : public CSocket
{
public:
    CServerSocket() {}
    virtual ~CServerSocket() {}
    virtual void OnReceive()
    {
        // This function is called automatically when there is data to receive.
        // Has an extra character for null termination.
        char str[1025] = {0};
        int addrlen = sizeof(g_saUDPClient);

        // Receives data and updates the UPD client socket address.
        int i = ReceiveFrom(str, 1024, 0, &g_saUDPClient, &addrlen);
        if (i >= 0)
            cout << i <<" chars received: " << str << endl;
        else
            cout << "Failed to receive" << endl;
    }
};


int main()
{
    // Create the main server socket.
    // It is used to listen for clients
    CServerSocket server;
    ZeroMemory(&g_saUDPClient, sizeof(g_saUDPClient));

    if (!server.Create(AF_INET, SOCK_DGRAM))
    {
        cout << "Failed to create socket\n" ;
        return 0;
    }

    // Bind the IP address and port# to the main socket
    if (SOCKET_ERROR == server.Bind("127.0.0.1", 3000))
    {
        cout << "Failed to bind IP address to socket\n" ;
        return 0;
    }

    cout << "UDP Server waiting to receive something from a client\n";

    // Monitor the client socket for network events, such as data ready to receive
    server.StartEvents();

    // Send data to the client
    cout << "Type data to send, type quit to exit\n";
    string s;
    for (;;)   // infinite loop
    {
        getline(cin, s);
        if (s == "quit") break;

        // Note: the server will send to the last client we received from.
        int i = server.SendTo(s.c_str(), static_cast<int>(s.length()), 0, &g_saUDPClient, sizeof(g_saUDPClient));
        if (i >= 0)
            cout << "Sending  " << i << " characters\n";
        else
            cout << "Failed to send" << endl;
    }

    return 0;
}

