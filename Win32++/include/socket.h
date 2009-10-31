// Win32++  Version 6.7
// Released: 6th November, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2009  David Nash
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
// socket.h
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


#ifndef _SOCKET_H_
#define _SOCKET_H_



#include "wincore.h"
#include <Winsock2.h>
#include <Process.h>

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
		static UINT WINAPI EventThread(LPVOID thread_data);

		SOCKET m_Socket;
		HANDLE m_hEventThread;	// Handle to the thread
		HANDLE m_StopRequest;	// An event to signal the event thread should stop
		HANDLE m_Stopped;		// An event to signal the event thread is stopped

	};
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	inline CSocket::CSocket() : m_Socket(0), m_hEventThread(0)
	{
		try
		{
			// Initialise the Windows Socket services
			WSADATA wsaData;

			if (0 != ::WSAStartup(MAKEWORD(2,2), &wsaData))
				throw CWinException(_T("WSAStartup failed"));

			m_StopRequest = ::CreateEvent(0, TRUE, FALSE, 0);
			m_Stopped = ::CreateEvent(0, TRUE, FALSE, 0);
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}
	}

	inline CSocket::~CSocket()
	{
		Disconnect();

		// Close handles
		::CloseHandle(m_StopRequest);
		::CloseHandle(m_Stopped);

		// Terminate the  Windows Socket services
		::WSACleanup();
	}

	inline void CSocket::Accept(CSocket& rClientSock, struct sockaddr* addr, int* addrlen)
	{
		rClientSock.m_Socket = ::accept(m_Socket, addr, addrlen);
		if (INVALID_SOCKET == rClientSock.GetSocket())
			TRACE(_T("Accept failed\n"));
	}

	inline int CSocket::Bind(const char* addr, int remotePort)
	{
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		int Result = ::bind( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) );
		if ( 0 != Result )
			TRACE(_T("Bind failed\n"));
		return Result;
	}

	inline int CSocket::Bind(const struct sockaddr* name, int namelen)
	{
		int Result = ::bind (m_Socket, name, namelen);
		if ( 0 != Result )
			TRACE(_T("Bind failed\n"));
		return Result;
	}

	inline int CSocket::Connect(const char* addr, int remotePort)
	{
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		int Result = ::connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) );
		if ( 0 != Result )
			TRACE(_T("Connect failed\n"));

		return Result;
	}

	inline int CSocket::Connect(const struct sockaddr* name, int namelen)
	{
		int Result = ::connect( m_Socket, name, namelen );
		if ( 0 != Result )
			TRACE(_T("Connect failed\n"));
		return Result;
	}

	inline BOOL CSocket::Create( int nSocketType /*= SOCK_STREAM*/)
	{
		switch(nSocketType)
		{
		case SOCK_STREAM:
			m_Socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
		case SOCK_DGRAM:
			m_Socket = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			break;
		default:
			TRACE(_T("Unknown Socket Type"));
			return FALSE;
		}

		if(m_Socket == INVALID_SOCKET)
		{
			TRACE(_T("Failed to create socket"));
			return FALSE;
		}

		return TRUE;
	}

	inline void CSocket::Disconnect()
	{
		::shutdown(m_Socket, SD_BOTH);
		StopEvents();
		::closesocket(m_Socket);
		m_Socket = 0;
	}

	inline UINT WINAPI CSocket::EventThread(LPVOID thread_data)
	{
		// These are the possible network event notifications:
		//	FD_READ 	Notification of readiness for reading.
		//	FD_WRITE 	Motification of readiness for writing.
		//	FD_OOB 		Notification of the arrival of Out Of Band data.
		//	FD_ACCEPT 	Notification of incoming connections.
		//	FD_CONNECT 	Notification of completed connection or multipoint join operation.
		//	FD_CLOSE 	Notification of socket closure.
		//	FD_QOS		Notification of socket Quality Of Service changes
		//	FD_ROUTING_INTERFACE_CHANGE	Notification of routing interface changes for the specified destination.
		//	FD_ADDRESS_LIST_CHANGE		Notification of local address list changes for the address family of the socket.

		WSANETWORKEVENTS NetworkEvents;
		CSocket* pSocket = (CSocket*)thread_data;
		SOCKET sClient = pSocket->m_Socket;

		WSAEVENT hNetworkEvent = ::WSACreateEvent();
		long Events = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE |
			          FD_QOS | FD_ROUTING_INTERFACE_CHANGE | FD_ADDRESS_LIST_CHANGE;

		// Associate the network event object (hNetworkEvents) with the
		// specified network events (Events) on socket sClient.
		if(	SOCKET_ERROR == WSAEventSelect(sClient, hNetworkEvent, Events))
		{
			TRACE(_T("Error in Event Select\n"));
			::SetEvent(pSocket->m_Stopped);
			::WSACloseEvent(hNetworkEvent);
			return 0;
		}

		// loop until the stop event is set
		for (;;) // infinite loop
		{
			// Wait 100 ms for a network event
			DWORD dwResult = ::WSAWaitForMultipleEvents(1, &hNetworkEvent, FALSE, THREAD_TIMEOUT, FALSE);

			// Check event for stop thread
			if(::WaitForSingleObject(pSocket->m_StopRequest, 0) == WAIT_OBJECT_0)
			{
				::WSACloseEvent(hNetworkEvent);
				::SetEvent(pSocket->m_Stopped);
				return 0;
			}

			if (WSA_WAIT_FAILED == dwResult)
			{
				TRACE(_T("WSAWaitForMultipleEvents failed\n"));
				::WSACloseEvent(hNetworkEvent);
				::SetEvent(pSocket->m_Stopped);
				return 0;
			}

			// Proceed if a network event occurred
			if (WSA_WAIT_TIMEOUT != dwResult)
			{

				if ( SOCKET_ERROR == ::WSAEnumNetworkEvents(sClient, hNetworkEvent, &NetworkEvents) )
				{
					TRACE(_T("WSAEnumNetworkEvents failed\n"));
					::WSACloseEvent(hNetworkEvent);
					::SetEvent(pSocket->m_Stopped);
					return 0;
				}

				if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
					pSocket->OnAccept();

				if (NetworkEvents.lNetworkEvents & FD_READ)
					pSocket->OnReceive();

				if (NetworkEvents.lNetworkEvents & FD_WRITE)
					pSocket->OnSend();

				if (NetworkEvents.lNetworkEvents & FD_OOB)
					pSocket->OnOutOfBand();

				if (NetworkEvents.lNetworkEvents & FD_QOS)
					pSocket->OnQualityOfService();

				if (NetworkEvents.lNetworkEvents & FD_CONNECT)
					pSocket->OnConnect();

				if (NetworkEvents.lNetworkEvents & FD_ROUTING_INTERFACE_CHANGE)
					pSocket->OnRoutingChange();

				if (NetworkEvents.lNetworkEvents & FD_ADDRESS_LIST_CHANGE)
					pSocket->OnAddresListChange();

				if (NetworkEvents.lNetworkEvents & FD_CLOSE)
				{
					::shutdown(sClient, SD_BOTH);
					::closesocket(sClient);
					pSocket->OnDisconnect();
					::WSACloseEvent(hNetworkEvent);
					::SetEvent(pSocket->m_Stopped);
					return 0;
				}
			}
		}
	}

	inline int  CSocket::GetPeerName(struct sockaddr* name, int* namelen)
	{
		int Result = ::getpeername(m_Socket, name, namelen);
		if (0 != Result)
			TRACE(_T("GetPeerName failed\n"));
		return Result;
	}

	inline int  CSocket::GetSockName(struct sockaddr* name, int* namelen)
	{
		int Result = ::getsockname(m_Socket, name, namelen);
		if (0 != Result)
			TRACE(_T("GetSockName Failed\n"));
		return Result;
	}

	inline int  CSocket::GetSockOpt(int level, int optname, char* optval, int* optlen)
	{
		int Result = ::getsockopt(m_Socket, level, optname, optval, optlen);
		if (0 != Result)
			TRACE(_T("GetSockOpt Failed\n"));
		return Result;
	}

	inline int CSocket::ioCtlSocket(long cmd, u_long* argp)
	{
		int Result = ::ioctlsocket(m_Socket, cmd, argp);
		if (0 != Result)
			TRACE(_T("ioCtlSocket Failed\n"));
		return Result;
	}

	inline int CSocket::Listen(int backlog /*= SOMAXCONN*/)
	{
		int Result = ::listen(m_Socket, backlog);
		if (0 != Result)
			TRACE(_T("Listen Failed\n"));
		return Result;
	}

	inline int CSocket::Receive(char* buf, int len, int flags)
	{
		int Result = ::recv(m_Socket, buf, len, flags);
		if (SOCKET_ERROR == Result)
			TRACE(_T("Receive failed\n"));
		return Result;
	}

	inline int CSocket::ReceiveFrom(char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
	{
		int Result = ::recvfrom(m_Socket, buf, len, flags, from, fromlen);
		if (SOCKET_ERROR == Result)
			TRACE(_T("ReceiveFrom failed\n"));
		return Result;
	}

	inline int CSocket::Send(const char* buf, int len, int flags)
	{
		int Result = ::send(m_Socket, buf, len, flags);
		if (SOCKET_ERROR == Result)
			TRACE(_T("Send failed\n"));
		return Result;
	}

	inline int CSocket::SendTo(const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
	{
		int Result =  ::sendto(m_Socket, buf, len, flags, to, tolen);
		if (SOCKET_ERROR == Result)
			TRACE(_T("SendTo failed\n"));
		return Result;
	}

	inline int CSocket::SetSockOpt(int level, int optname, const char* optval, int optlen)
	{
		int Result = ::setsockopt(m_Socket, level, optname, optval, optlen);
		if (0 != Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	inline void CSocket::StartEvents()
	{
		// This function starts the thread which monitors the socket for events.
		StopEvents();	// Ensure the thread isn't already running
		UINT ThreadID;	// a return variable required for Win95, Win98, WinME
		m_hEventThread = (HANDLE)::_beginthreadex(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, &ThreadID);
	}

	inline void CSocket::StopEvents()
	{
		// Terminates the event thread gracefully (if possible)
		if (m_hEventThread)
		{
			::SetThreadPriority(m_hEventThread, THREAD_PRIORITY_HIGHEST);
			::SetEvent(m_StopRequest);

			for (;;)	// infinite loop
			{
				// wait for the Thread stopping event to be set
				if ( WAIT_TIMEOUT == ::WaitForSingleObject(m_Stopped, THREAD_TIMEOUT * 10) )
				{
					// Note: An excessive delay in processing any of the notification functions
					// can cause us to get here. (Yes one second is an excessive delay. Its a bug!)
					TRACE(_T("*** Error: Event Thread won't die ***\n") );
				}
				else break;
			}

			::CloseHandle(m_hEventThread);
			m_hEventThread = 0;
		}

		::ResetEvent(m_StopRequest);
		::ResetEvent(m_Stopped);
	}

}


#endif // #ifndef _SOCKET_H_

