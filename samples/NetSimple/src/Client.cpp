//////////////////////////////////////////////
// Client.cpp

#include <iostream>
#include "wxx_socket.h"

using namespace std;


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

		cout << i << " chars received: " << str << endl;
	}
};

int main()
{
	CClientSocket Client;

	// Create the socket to communicate with the Server
	if (!Client.Create(AF_INET, SOCK_STREAM))
	{
		cout << "Failed to create socket\n" ;
		return 0;
	}

	// Connect to the server
	if (SOCKET_ERROR == Client.Connect("127.0.0.1", 3000))
	{
		cout << "Failed to connect to server. Was it running?\n";
		return 0;
	}
	cout << "Connected to server.\n";
	cout << "Type data to send, type quit to exit\n";

	// Monitor the client socket for network events, such as data ready to receive
	Client.StartEvents();

	// Send data to the server
	string s;
	for (;;)	// Infinite loop
	{
		getline(cin, s);
		if (s == "quit") break;
		int i = Client.Send(s.c_str(), static_cast<int>(s.length()), 0);
		cout << "Sending  " << i << " characters\n";
	}

	return 0;
}

