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


////////////////////////////////////////////////////////
// Socket.cpp
//  Definitions for the CSocket class:


#include <Winsock2.h>
#include "WinCore.h"
#include "Socket.h"



namespace Win32xx
{

	CSocket::CSocket() : m_WSAStarted(FALSE), m_Socket(0), m_SocketType(0)
	{
		WSADATA wsaData;
		int WSA_Status = WSAStartup(MAKEWORD(2,2), &wsaData);

		if (WSA_Status == 0)
			m_WSAStarted = TRUE;
		else
			throw CWinException(_T("WSAStartup failed"));
	}

	CSocket::~CSocket()
	{
		if (m_WSAStarted)
		{
			if (m_Socket)
				Disconnect();
			
			WSACleanup();
		}
	}

	void CSocket::Disconnect()
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
	}

	DWORD WINAPI CSocket::EventThread(LPVOID thread_data)
	{
		DWORD Event;

		WSANETWORKEVENTS NetworkEvents;
		CSocket* pSocket = (CSocket*)thread_data;
		SOCKET sClient = pSocket->m_Socket;
		HANDLE NetworkEvent = WSACreateEvent();

		if(WSAEventSelect(sClient, NetworkEvent,FD_READ|FD_WRITE|FD_CLOSE|FD_ACCEPT)==	SOCKET_ERROR)
		{
			TRACE("Error in Event Select\n");
			return 1;
		}

		for (;;) // an infinite loop
		{
			if ((Event = WSAWaitForMultipleEvents(1, &NetworkEvent, FALSE,WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
			{
				TRACE("WSAWaitForMultipleEvents failed with error \n");
      			return 1;
			}

			if (WSAEnumNetworkEvents(sClient, NetworkEvent, &NetworkEvents) == SOCKET_ERROR)
			{
				TRACE("WSAEnumNetworkEvents failed with error \n");
  				return 1;
			}

			if (NetworkEvents.lNetworkEvents & FD_ACCEPT)
			{
				TRACE("Accepting connection from client");
				pSocket->OnAccept();
			}

			if (NetworkEvents.lNetworkEvents & FD_READ)
			{
				if (NetworkEvents.lNetworkEvents & FD_READ && NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					TRACE("FD_READ failed with error \n");
				}
				else
				{
					pSocket->OnReceive();
				}
			}

			if (NetworkEvents.lNetworkEvents & FD_CONNECT)
			{
				pSocket->OnConnect();
			}
			
			if (NetworkEvents.lNetworkEvents & FD_CLOSE)
			{
				shutdown(sClient,FD_READ|FD_WRITE);
				closesocket(sClient);
				pSocket->OnClose();
				ExitThread(0);
			}
		}

		return 0;
	}

	void CSocket::Accept(CSocket& rClientSock, struct sockaddr* addr, int* addrlen)
	{
		rClientSock.m_Socket = accept(m_Socket, addr, addrlen);
		::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) &rClientSock, 0, NULL); 
	}

	int CSocket::Bind(const struct sockaddr* name, int namelen)
	{
		return bind (m_Socket, name, namelen);
	}

	void CSocket::Connect(LPCTSTR addr, int remotePort)
	{
		// Connect to a server.
		sockaddr_in clientService;

		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		if ( SOCKET_ERROR == connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) ) )
		{
			throw CWinException (_T("Connect failed"));
		}

		// Start monitoring the socket for events
		::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, NULL);

	}

	BOOL CSocket::Create( int nSocketType /*= SOCK_STREAM*/)
	{
		// Exit if socket WSAStartup failed or socket already created
		// then ...

		switch(nSocketType)
		{
			case SOCK_STREAM:
				m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
				break;
			case SOCK_DGRAM:
				m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
				break;
			default:
				return FALSE;
		}

		if(m_Socket == INVALID_SOCKET)
		{
			return FALSE;
		}

		return TRUE;
	}

	int CSocket::Listen(int backlog /*= SOMAXCONN*/)
	{
		int Error = listen(m_Socket, backlog);
		::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, NULL);

		return Error;
	}

	int CSocket::Receive(char* buf, int len, int flags)
	{
		return recv(m_Socket, buf, len, flags);		
	}

	int CSocket::Send(const char* buf, int len, int flags)
	{
		return send(m_Socket, buf, len, flags);
	}
}

