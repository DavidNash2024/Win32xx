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

HANDLE g_NetworkEvent = 0;

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
			for (unsigned int i = 0; i < ConnectedSockets.size(); i++)
			{
				delete ConnectedSockets[i];
			}

			if (m_Socket)
			{
				shutdown(m_Socket, SD_BOTH);
				closesocket(m_Socket);
			}
		}

		WSACleanup();
	}

	DWORD WINAPI CSocket::ClientThread(LPVOID thread_data)
	{
		DWORD Event;

		WSANETWORKEVENTS NetworkEvents;
		char str[200];
		DWORD recvLength;
		CSocket* pSocket = (CSocket*)thread_data;
		SOCKET sClient = pSocket->m_Socket;

		while(true)
		{
			if ((Event = WSAWaitForMultipleEvents(1, &g_NetworkEvent, FALSE,WSA_INFINITE, FALSE)) == WSA_WAIT_FAILED)
			{
				TRACE("WSAWaitForMultipleEvents failed with error \n");
      			return 0;
			}

			if (WSAEnumNetworkEvents(sClient, g_NetworkEvent, &NetworkEvents) == SOCKET_ERROR)
			{
				TRACE("WSAEnumNetworkEvents failed with error \n");
  				return 0;
			}

			if (NetworkEvents.lNetworkEvents & FD_READ)
			{
				if (NetworkEvents.lNetworkEvents & FD_READ && NetworkEvents.iErrorCode[FD_READ_BIT] != 0)
				{
					TRACE("FD_READ failed with error \n");
				}
				else
				{
					recvLength = recv(sClient,str,200,0);
					str[recvLength] = '\0';
					TRACE(str);
					send(sClient,str,200,0);
				}
			}
			
			if (NetworkEvents.lNetworkEvents & FD_CLOSE)
			{
				shutdown(sClient,FD_READ|FD_WRITE);
				closesocket(sClient);
				ExitThread(0);
			}
		}

		return 0;
	}

	void CSocket::StartServer(u_short LocalPort)
	{
	    // Bind the socket.
		sockaddr_in service;

		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons( LocalPort );

		if ( bind( m_Socket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
			throw CWinException(_T("Bind failed"));

	    // Listen on the socket.
		if ( SOCKET_ERROR == listen( m_Socket, 1 ) )
			throw CWinException(_T("Error listening on socket"));

		::CreateThread(NULL, 0, CSocket::ServerThread, (LPVOID) this, 0, NULL);
	}

	void CSocket::StopServer()
	{
		for (unsigned int i = 0; i < ConnectedSockets.size(); i++)
		{
			delete ConnectedSockets[i];
		}

		// also send stop event
		// also delete server thread
	}

	DWORD WINAPI CSocket::ServerThread(LPVOID pCSocket)
	{
		CSocket* pMainSocket = (CSocket*)pCSocket;
		g_NetworkEvent = WSACreateEvent();

		while(1)
		{
			SOCKET ConnectedSocket;
			ConnectedSocket = accept( pMainSocket->m_Socket, NULL, NULL );
			if (INVALID_SOCKET == ConnectedSocket)
			{
				TRACE("Accept failed");
			}
			TRACE("Client accepted\n");

			if(WSAEventSelect(ConnectedSocket,g_NetworkEvent,FD_READ|FD_WRITE|FD_CLOSE)==	SOCKET_ERROR)
			{
				TRACE("Error in Event Select\n");
				break;
			}

			CSocket* pConnected = new CSocket;
			pConnected->m_Socket = ConnectedSocket;
			pMainSocket->ConnectedSockets.push_back(pConnected);
			::CreateThread(NULL, 0, CSocket::ClientThread, (LPVOID) pConnected, 0, NULL);
		}
		
		return 0;
	}

	void CSocket::Connect(LPCTSTR addr, u_short remotePort)
	{
		// Connect to a server.
		sockaddr_in clientService;

		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( remotePort );

		if ( SOCKET_ERROR == connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) ) )
		{
			throw CWinException (_T("Connect failed"));
		}

		OnConnect();
	}

	BOOL CSocket::Create( int nSocketType /*= SOCK_STREAM*/)
	{
		// Exit if socket WSAStartup failed or sochet already created
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

	void CSocket::Receive(int flags)
	{
	//	::CreateThread(NULL, 0, CSocket::ReceiveThread, (LPVOID) this, 0, NULL);		
	}

	DWORD WINAPI CSocket::ReceiveThread(LPVOID pCSocket)
	{
		CSocket* pSocket = (CSocket*)pCSocket;

		char buf[1024] = {0};
		int nReceived = -1;

		do
		{
			nReceived = recv(pSocket->m_Socket, buf, 1024, 0);
		}
		while (nReceived <= 0);
		
		pSocket->m_nReceived = nReceived;
		pSocket->m_strReceived = buf;
		pSocket->Send(buf, 1024, 0);
		pSocket->OnReceive();

		return 0;
	}

	int CSocket::Send( char* buf, int len, int flags)
	{
		return send(m_Socket, buf, len, flags);
	}
}

