// Win32++  Version 6.0
// Released: 4th March, 2008 by:
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


////////////////////////////////////////////////////////
// Socket.h
//  Declaration of the CSocket class
//
// The CSocket class represents a network socket. It encapsualtes many of
// the Windows Socket SPI fuctions, providing an object-oriented approach
// to network programming. After StartEvents is called, CSocket monitors
// the socket and responds automatically to network events. This event
// monitoring, for example, automatically calls OnReceive when there is
// data on the socket to be read, and OnAccept when a server should accept
// a connection from a client.

// Users of this class should be aware that functions like OnReceive,
// OnAccept, etc. are called on a different thread from the one CSocket is
// instanciated on. The thread for these functions needs to respond quickly
// to other network events, so it shouldn't be delayed. It also doesn't run
// a message loop, so it can't be used to create windows. For these reasons
// it might be best to use PostMessage in response to these functions in a
// windows environment.

// Refer to the network samples for an example of how to use this class to
// create a TCP client & server, and a UDP client and server.

// To compile programs with CSocket, link with ws3_32.lib for Win32,
// and ws2.lib for Windows CE. Windows 95 systems will need to install the
// "Windows Sockets 2.0 for Windows 95". It's available from:
// http://support.microsoft.com/kb/182108/EN-US/


#ifndef SOCKET_H
#define SOCKET_H


#define THREAD_TIMEOUT 100


namespace Win32xx
{

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
		virtual int  Bind(const char* addr, int remotePort);
		virtual int  Bind(const struct sockaddr* name, int namelen);
		virtual int  Connect(const char* addr, int remotePort);
		virtual int  Connect(const struct sockaddr* name, int namelen);
		virtual BOOL Create(int nSocketType = SOCK_STREAM);
		virtual void Disconnect();
		virtual int  GetPeerName(struct sockaddr* name, int* namelen);
		virtual int  GetSockName(struct sockaddr* name, int* namelen);
		virtual int  GetSockOpt(int level, int optname, char* optval, int* optlen);
		virtual int  ioCtlSocket(long cmd, u_long* argp);
		virtual int  Listen(int backlog = SOMAXCONN);
		virtual int  Receive(char* buf, int len, int flags);
		virtual int  ReceiveFrom(char* buf, int len, int flags, struct sockaddr* from, int* fromlen);
		virtual int  Send(const char* buf, int len, int flags);
		virtual int  SendTo(const char* buf, int len, int flags, const struct sockaddr* to, int tolen);
		virtual int  SetSockOpt(int level, int optname, const char* optval, int optlen);
		virtual void StartEvents();
		virtual void StopEvents();
		SOCKET& GetSocket() {return m_Socket;}

		// Allow CSocket to be used as a SOCKET
		operator SOCKET() const {return m_Socket;}

	private:
		CSocket(const CSocket&);				// Disable copy construction
		CSocket& operator = (const CSocket&);	// Disable assignment operator
		static DWORD WINAPI EventThread(LPVOID thread_data);

		SOCKET m_Socket;
		HANDLE m_hEventThread;	// Handle to the thread
		HANDLE m_StopRequest;	// An event to signal the event thread should stop
		HANDLE m_Stopped;		// An event to signal the event thread is stopped

	};

}


#endif // #ifndef SOCKET_H

