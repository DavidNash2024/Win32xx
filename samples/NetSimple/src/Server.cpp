//////////////////////////////////////////////
// Server.cpp

#include <iostream>
#include "wxx_socket.h"

using namespace std;


class CWorkerSocket : public CSocket
{
public:
    CWorkerSocket() {}
    virtual ~CWorkerSocket() {}
    virtual void OnReceive()
    {
        // This function is called automatically when there is data to receive
        // Has an extra character for null termination.
        char str[1025] = {0};
        int i = Receive(str, 1024, 0);
        cout << i <<" chars received: " << str << endl;
    }
};

int main()
{
    // Create the main server socket.
    // It is used to listen for clients
    CSocket server;
    if (!server.Create(AF_INET, SOCK_STREAM))
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

    // Listen on the socket for clients to connect
    if (SOCKET_ERROR == server.Listen())
    {
        cout << "Listen on socket failed\n";
        return 0;
    }

    // Create the socket to communicate with the Client
    CWorkerSocket client;
    cout << "Waiting for the client to connect\n";
    do
    {
        server.Accept(client, NULL, NULL);
    }
    while (SOCKET_ERROR == static_cast<int>(client.GetSocket()));

    cout << "Client connected\n";

    // Monitor the client socket for network events, such as data ready to receive
    client.StartEvents();

    // Send data to the client
    cout << "Type data to send, type quit to exit\n";
    string s;
    for (;;)   // infinite loop
    {
        getline(cin, s);
        if (s == "quit") break;
        int i = client.Send(s.c_str(), static_cast<int>(s.length()), 0);
        cout << "Sending  " << i << " characters\n";
    }

    return 0;
}

