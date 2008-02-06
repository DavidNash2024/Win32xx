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

	CSocket::CSocket() : m_WSAStarted(FALSE), m_Socket(0), m_SocketType(0), m_DataSocket(0)
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
		if (m_DataSocket)
		{
			shutdown(m_DataSocket, SD_BOTH);
			closesocket(m_DataSocket);
		}

		if (m_Socket)
		{
			shutdown(m_Socket, SD_BOTH);
			closesocket(m_Socket);
		}
		if (m_WSAStarted) WSACleanup();
	}

	void CSocket::Accept()
	{
	    // Bind the socket.
		sockaddr_in service;

		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons( 27015 );

		if ( bind( m_Socket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
			throw CWinException(_T("Bind failed"));

	    // Listen on the socket.
		if ( SOCKET_ERROR == listen( m_Socket, 1 ) )
			throw CWinException(_T("Error listening on socket"));

		::CreateThread(NULL, 0, CSocket::AcceptThread, (LPVOID) this, 0, NULL);
	}

	DWORD WINAPI CSocket::AcceptThread(LPVOID pCSocket)
	{
		CSocket* pSocket = (CSocket*)pCSocket;
		SOCKET AcceptSocket;

		do
		{
			AcceptSocket = accept( pSocket->m_Socket, NULL, NULL );
		}
		while ( SOCKET_ERROR == AcceptSocket );

		pSocket->m_DataSocket = AcceptSocket;
		pSocket->OnAccept();
		return 0;
	}

	void CSocket::Connect()
	{
		// Connect to a server.
		sockaddr_in clientService;

		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		clientService.sin_port = htons( 27015 );

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

	int CSocket::Receive( char* buf, int len, int flags)
	{
		return recv(m_DataSocket, buf, len, flags);
	}

	int CSocket::Send( char* buf, int len, int flags)
	{
		return send(m_DataSocket, buf, len, flags);
	}
}

