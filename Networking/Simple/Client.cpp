#include <iostream>
#include <string>
#include "winsock2.h"
#include "../../Win32++/Socket.h"

using namespace std;
using namespace Win32xx;


class CClientSocket : public CSocket
{
public:
  CClientSocket() {}
  virtual void OnReceive()
  {
	  char str[1024] = {0};
	  int i = Receive(str, 1024, 0);

	  cout << i << " chars received: " << str << endl;
  }
};

int main()
{
    CClientSocket Client;

    if (!Client.Create(SOCK_STREAM))
    {
        cout << "Failed to create socket\n" ;
        return 0;
    }

    if (SOCKET_ERROR == Client.Connect("127.0.0.1", 3000))
    {
        cout << "Failed to connect to server. Was it running?\n";
        return 0;
    }
    cout << "Connected to server.\n";
    cout << "Type data to send, type quit to exit\n";

    Client.StartEvents();
    string s;
    for (;;)
    {
        getline(cin, s);
        if (s == "quit") break;
        int i = Client.Send(s.c_str(), (int)s.length(), 0);
        cout << "Sending  " << i << " characters\n";
    }

    return 0;
}
