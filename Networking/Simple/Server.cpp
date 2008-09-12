#include <iostream>
#include <string>
#include "winsock2.h"
#include "../../Win32++/socket.h"

using namespace std;
using namespace Win32xx;


class CServerSocket : public CSocket
{
public:
	CServerSocket() {}
	virtual ~CServerSocket() {}
	virtual void OnReceive()
	{
		// This function is called automatically when there is data to receive
		char str[1024] = {0};
		int i = Receive(str, 1024, 0);
		cout << i <<" chars received: " << str << endl;
	}
};

int main()
{
	// Create the main server socket.
	// It is used to listen for clients
	CServerSocket Server;
	if (!Server.Create(SOCK_STREAM))
	{
		cout << "Failed to create socket\n" ;
		return 0;
	}

	// Bind the IP address and port# to the main socket
	if (SOCKET_ERROR == Server.Bind("127.0.0.1", 3000))
	{
		cout << "Failed to bind IP address to socket\n" ;
		return 0;
	}

	// Listen on the socket for clients to connect
	if (SOCKET_ERROR == Server.Listen())
	{
		cout << "Listen on socket failed\n";
		return 0;
	}

	// Create the socket to communicate with the Client
	CServerSocket Client;
	cout << "Waiting for the client to connect\n";
	do
	{
		Server.Accept(Client, NULL, NULL);
	}
	while (SOCKET_ERROR == Client.GetSocket());

	cout << "Client connected\n";

	// Monitor the client socket for network events, such as data ready to receive
	Client.StartEvents();

	// Send data to the client
	cout << "Type data to send, type quit to exit\n";
	string s;
	for (;;)   // infinite loop
	{
		getline(cin, s);
		if (s == "quit") break;
		int i = Client.Send(s.c_str(), (int)s.length(), 0);
		cout << "Sending  " << i << " characters\n";
	}

	return 0;
}

