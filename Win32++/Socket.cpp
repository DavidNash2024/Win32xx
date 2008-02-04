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
// #include "WinCore.h"
#include "Socket.h"

namespace Win32xx
{

	CSocket::CSocket() : m_WSAStarted(FALSE), m_Socket(0), m_SocketType(0), m_DataSocket(0)
	{
		WSADATA wsaData;

		int WSA_Status = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (WSA_Status != 0)
			MessageBox(NULL, "WSAStartup failed", "Error", MB_OK);
		else
			m_WSAStarted = TRUE;
	}

	CSocket::~CSocket()
	{
		if (m_WSAStarted)
			WSACleanup();
	}

	void CSocket::Accept()
	{	

	    // Bind the socket.
		sockaddr_in service;

		service.sin_family = AF_INET;
		service.sin_addr.s_addr = htonl(INADDR_ANY);
		service.sin_port = htons( 27015 );

		if ( bind( m_Socket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
		{
		//	printf( "bind() failed.\n" );
			closesocket(m_Socket);
			return;
		}

	    // Listen on the socket.
		if ( listen( m_Socket, 1 ) == SOCKET_ERROR )
		//	printf( "Error listening on socket.\n");
			::MessageBox(NULL, "Error listening on socket", "Error", MB_OK);


		SOCKET AcceptSocket;
		while (1) 
		{
			do
			{
				AcceptSocket = accept( m_Socket, NULL, NULL );
			} 
			while ( AcceptSocket == SOCKET_ERROR );

			m_DataSocket = AcceptSocket; 
			break;
		}
	}

	void CSocket::Connect()
	{
		// Connect to a server.
		sockaddr_in clientService;

		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		clientService.sin_port = htons( 27015 );

		if ( connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) ) == SOCKET_ERROR) 
		{
		//	printf( "Failed to connect.\n" );
		//	WSACleanup();
			return;
		}
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

		m_DataSocket = m_Socket;

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

