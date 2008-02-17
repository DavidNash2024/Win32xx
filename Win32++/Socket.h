// Win32++  Version 5.7
// Released: 15th February, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////


///////////////////////////////////////////////////////
// Socket.h
//  Declaration of the CSocket class


#ifndef SOCKET_H
#define SOCKET_H


#define THREAD_TIMEOUT 100


namespace Win32xx
{

	// The CSocket class represents a network socket. It encapsualtes many of the
	// Windows Socket fuctions, providing an object-oriented approach to network programming.
	// CSocket responds automatically to network events. For example, it calls OnReceive 
	// when there is data on the socket to be read, and OnAccept when a server should
	// accept a connection from a client.

	// Refer to the network sample for an example of how to use this class to create a
	// TCP client & server, and a UDP client and server.
	


	class CSocket
	{
	public:
		CSocket();
		virtual ~CSocket();

		// Override these functions to monitor events
		virtual void OnAccept()		{}
		virtual void OnAddresListChange() {}
		virtual void OnDisconnect()	{}
		virtual void OnConnect()	{}
		virtual void OnOutOfBand()	{}
		virtual void OnQualityOfService() {}
		virtual void OnReceive()	{}
		virtual void OnRoutingChange() {}
		virtual void OnSend()		{}
		
		// Its unlikely you would need to override these functions
		virtual void Accept(CSocket& rClientSock, struct sockaddr* addr, int* addrlen);
		virtual int  Bind(const struct sockaddr* name, int namelen);
		virtual int  Connect(const char* addr, int remotePort);
		virtual BOOL Create(int nSocketType = SOCK_STREAM);
		virtual void Disconnect();
		virtual int  Listen(int backlog = SOMAXCONN);
		virtual int  Receive(char* buf, int len, int flags);
		virtual int  ReceiveFrom(char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
		virtual int  Send(const char* buf, int len, int flags);
		virtual int  SendTo(const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
		virtual void StartNotifyEvents();
		virtual void StopNotifyEvents();
		SOCKET& GetSocket() {return m_Socket;}

		// Allow CSocket to be used as a SOCKET
		operator SOCKET() const {return m_Socket;}

	private:
		CSocket(const CSocket&);				// Disable copy construction
		CSocket& operator = (const CSocket&);	// Disable assignment operator
		static DWORD WINAPI EventThread(LPVOID thread_data);

		SOCKET m_Socket;
		HANDLE m_hEventThread;
		BOOL m_bStopThread;
	};

}


#endif // #ifndef SOCKET_H

