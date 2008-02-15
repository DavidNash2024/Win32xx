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

	CSocket::CSocket() : m_WSAStarted(FALSE), m_Socket(0), m_SocketType(0), m_EventThread(0), m_bStopThread(FALSE)
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
		Disconnect();
		if (m_WSAStarted)
		{
			if (m_Socket)
				Disconnect();

			WSACleanup();
		}

		// Close handles
	//	::CloseHandle(m_EventThread);
	}

	void CSocket::Disconnect()
	{
		if (m_EventThread)
		{
			// Trigger thread to stop
			DWORD dwExitCode STILL_ACTIVE;
			int WatchDog = 0;
			while (STILL_ACTIVE == dwExitCode)
			{
				m_bStopThread = TRUE;
				GetExitCodeThread(m_EventThread, &dwExitCode);
				
				if (STILL_ACTIVE == dwExitCode) Sleep(5);
				WatchDog++;
				if (WatchDog >= 100)
				{
					TerminateThread(m_EventThread, 0);
					TRACE(" *** Forced thread closure *** \n");
				}
			}

			CloseHandle(m_EventThread);
			m_EventThread = 0;
		}

		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_bStopThread = FALSE;
	}

	DWORD WINAPI CSocket::EventThread(LPVOID thread_data)
	{
		// These are the possible network event notifications:
		//	FD_READ 	Notification of readiness for reading.
		//	FD_WRITE 	Motification of readiness for writing.
		//	FD_OOB 		Notification of the arrival of Out Of Band data.
		//	FD_ACCEPT 	Notification of incoming connections.
		//	FD_CONNECT 	Notification of completed connection or multipoint join operation.
		//	FD_CLOSE 	Notification of socket closure.
		//	FD_QOS		notification of socket Quality Of Service changes
		//	FD_ROUTING_INTERFACE_CHANGE	Notification of routing interface changes for the specified destination.
		//	FD_ADDRESS_LIST_CHANGE		Notification of local address list changes for the address family of the socket.

		WSANETWORKEVENTS NetworkEvents;
		CSocket* pSocket = (CSocket*)thread_data;
		SOCKET sClient = pSocket->m_Socket;
		HANDLE hNetworkEvent = WSACreateEvent();
		long Events = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE |
			          FD_QOS | FD_ROUTING_INTERFACE_CHANGE | FD_ADDRESS_LIST_CHANGE;

		if (pSocket->m_bStopThread)
			return 0;

		if(	SOCKET_ERROR == WSAEventSelect(sClient, hNetworkEvent, Events))
		{
			TRACE("Error in Event Select\n");
			return 0;
		}

		// loop until the stop event is set
		//while( WAIT_OBJECT_0 != WaitForSingleObject(pSocket->m_StopRequestEvent, 0) )
		while (FALSE == pSocket->m_bStopThread)
	//	for (;;)
		{

			// Wait 200 ms for a network event
			if ( WSA_WAIT_FAILED == WSAWaitForMultipleEvents(1, &hNetworkEvent, FALSE, 200, FALSE))
			{
				TRACE(_T("WSAWaitForMultipleEvents failed"));
				return 0;
			}
			else
			{
			//	if (pSocket->m_bStopThread)
			//		break;

				if ( SOCKET_ERROR == WSAEnumNetworkEvents(sClient, hNetworkEvent, &NetworkEvents) )
				{
					TRACE(_T("WSAEnumNetworkEvents failed"));
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
					shutdown(sClient, SD_BOTH);
					closesocket(sClient);
					pSocket->OnDisconnect();
					break;
				}
			}
		}

		return 0;
	}

	void CSocket::Accept(CSocket& rClientSock, struct sockaddr* addr, int* addrlen)
	{
		rClientSock.m_Socket = accept(m_Socket, addr, addrlen);
		if (INVALID_SOCKET == rClientSock.m_Socket)
			return;

		DWORD dwExitCode;
		GetExitCodeThread(rClientSock.m_EventThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			rClientSock.m_EventThread = ::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) &rClientSock, 0, NULL);
	}

	int CSocket::Bind(const struct sockaddr* name, int namelen)
	{	
		return bind (m_Socket, name, namelen);	
	}

	int CSocket::Connect(LPCTSTR addr, int remotePort)
	{
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		if ( SOCKET_ERROR == connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) ) )
		{
			TRACE(_T("Connect failed\n"));
			return SOCKET_ERROR;
		}

		// Start monitoring the socket for events
		DWORD dwExitCode;
		GetExitCodeThread(m_EventThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			m_EventThread = ::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, NULL);

		return 0;
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
		if (Error)
		{
			TRACE("Listen Failed");
			return Error;
		}

		DWORD dwExitCode;
		GetExitCodeThread(m_EventThread, &dwExitCode);
		if (dwExitCode != STILL_ACTIVE)
			m_EventThread = ::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, NULL);
		
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

	void CSocket::StopThread()
	{
		if (m_EventThread)
		{
			// Trigger thread to stop
			DWORD dwExitCode STILL_ACTIVE;
			while (STILL_ACTIVE == dwExitCode)
			{
				m_bStopThread = TRUE;
				Sleep(50);
				GetExitCodeThread(m_EventThread, &dwExitCode);
			}

			CloseHandle(m_EventThread);
			m_EventThread = 0;
		}
	}
}

