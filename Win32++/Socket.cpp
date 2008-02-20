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



#include <Winsock2.h>
#include "WinCore.h"
#include "Socket.h"


namespace Win32xx
{
	CSocket::CSocket() : m_Socket(0), m_hEventThread(0), m_bStopThread(FALSE)
	{
		try
		{
			// Initialise the Windows Socket services
			WSADATA wsaData;

			if (0 != WSAStartup(MAKEWORD(2,2), &wsaData))
				throw CWinException("WSAStartup failed");
		}

		catch (const CWinException &e)
		{
			e.MessageBox();
			throw;
		}
	}

	CSocket::~CSocket()
	{
		Disconnect();

		// Terminate the  Windows Socket services
		WSACleanup();
	}

	void CSocket::Accept(CSocket& rClientSock, struct sockaddr* addr, int* addrlen)
	{
		rClientSock.m_Socket = accept(m_Socket, addr, addrlen);
		if (SOCKET_ERROR == rClientSock.GetSocket())
			TRACE(_T("Accept failed\n"));
	}

	int CSocket::Bind(const char* addr, int remotePort)
	{
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		int Result = bind( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) );
		if ( 0 != Result )
			TRACE(_T("Bind failed\n"));
		return Result;
	}

	int CSocket::Bind(const struct sockaddr* name, int namelen)
	{
		int Result = bind (m_Socket, name, namelen);
		if ( 0 != Result )
			TRACE(_T("Bind failed\n"));
		return Result;
	}

	int CSocket::Connect(const char* addr, int remotePort)
	{
		sockaddr_in clientService;
		clientService.sin_family = AF_INET;
		clientService.sin_addr.s_addr = inet_addr( addr );
		clientService.sin_port = htons( (u_short)remotePort );

		int Result = connect( m_Socket, (SOCKADDR*) &clientService, sizeof(clientService) );
		if ( 0 != Result )
			TRACE(_T("Connect failed\n"));
		return Result;
	}

	int CSocket::Connect(const struct sockaddr* name, int namelen)
	{
		int Result = connect( m_Socket, (SOCKADDR*) &name, sizeof(namelen) );
		if ( 0 != Result )
			TRACE(_T("Connect failed\n"));
		return Result;
	}

	BOOL CSocket::Create( int nSocketType /*= SOCK_STREAM*/)
	{
		switch(nSocketType)
		{
		case SOCK_STREAM:
			m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
			break;
		case SOCK_DGRAM:
			m_Socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
			break;
		default:
			TRACE("Unknown Socket Type");
			return FALSE;
		}

		if(m_Socket == INVALID_SOCKET)
		{
			TRACE(_T("Failed to create socket"));
			return FALSE;
		}

		return TRUE;
	}

	void CSocket::Disconnect()
	{
		StopEvents();
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
		m_Socket = 0;
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
		//	FD_QOS		Notification of socket Quality Of Service changes
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
			TRACE(_T("Error in Event Select\n"));
			return 0;
		}

		// loop until the stop thread flag is set
		while (FALSE == pSocket->m_bStopThread)
		{
			// Wait 100 ms for a network event
			DWORD dwResult = WSAWaitForMultipleEvents(1, &hNetworkEvent, FALSE, THREAD_TIMEOUT, FALSE);

			if (WSA_WAIT_FAILED == dwResult)
			{
				TRACE(_T("WSAWaitForMultipleEvents failed"));
				return 0;
			}

			if (WSA_WAIT_TIMEOUT != dwResult)
			{

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
					return 0;
				}
			}
		}

		return 0;
	}

	int  CSocket::GetPeerName(struct sockaddr* name, int* namelen)
	{
		int Result = getpeername(m_Socket, name, namelen);
		if (0 != Result)
			TRACE(_T("Listen Failed"));
		return Result;
	}

	int  CSocket::GetSockName(struct sockaddr* name, int* namelen)
	{
		int Result = getsockname(m_Socket, name, namelen);
		if (0 != Result)
			TRACE(_T("Listen Failed"));
		return Result;
	}

	int  CSocket::GetSockOpt(int level, int optname, char* optval, int* optlen)
	{
		int Result = getsockopt(m_Socket, level, optname, optval, optlen);
		if (0 != Result)
			TRACE(_T("Listen Failed"));
		return Result;
	}

	int CSocket::ioCtlSocket(long cmd, u_long* argp)
	{
		int Result = ioctlsocket(m_Socket, cmd, argp);
		if (0 != Result)
			TRACE(_T("Listen Failed"));
		return Result;
	}

	int CSocket::Listen(int backlog /*= SOMAXCONN*/)
	{
		int Result = listen(m_Socket, backlog);
		if (0 != Result)
			TRACE(_T("Listen Failed"));
		return Result;
	}

	int CSocket::Receive(char* buf, int len, int flags)
	{
		int Result = recv(m_Socket, buf, len, flags);
		if (SOCKET_ERROR == Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	int CSocket::ReceiveFrom(char* buf, int len, int flags, struct sockaddr* from, int* fromlen)
	{
		int Result = recvfrom(m_Socket, buf, len, flags, from, fromlen);
		if (SOCKET_ERROR == Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	int CSocket::Send(const char* buf, int len, int flags)
	{
		int Result = send(m_Socket, buf, len, flags);
		if (SOCKET_ERROR == Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	int CSocket::SendTo(const char* buf, int len, int flags, const struct sockaddr* to, int tolen)
	{
		int Result =  sendto(m_Socket, buf, len, flags, to, tolen);
		if (SOCKET_ERROR == Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	int CSocket::SetSockOpt(int level, int optname, const char* optval, int optlen)
	{
		int Result = setsockopt(m_Socket, level, optname, optval, optlen);
		if (0 != Result)
			TRACE(_T("SetSockOpt failed\n"));
		return Result;
	}

	void CSocket::StartEvents()
	{
		StopEvents();
		m_hEventThread = ::CreateThread(NULL, 0, CSocket::EventThread, (LPVOID) this, 0, NULL);
	}

	void CSocket::StopEvents()
	{
		// Terminates the event thread (gracefully if possible)
		if (m_hEventThread)
		{
			DWORD dwExitCode = 0;
			SetThreadPriority(m_hEventThread, THREAD_PRIORITY_HIGHEST);
			int WatchDog = 0;

			GetExitCodeThread(m_hEventThread, &dwExitCode);
			while (STILL_ACTIVE == dwExitCode)
			{
				WatchDog++;
				m_bStopThread = TRUE;	// A flag monitored by the event thread

				// Wait up to 100ms for the thread to signal it has ended
				if ( (WAIT_TIMEOUT == ::WaitForSingleObject(m_hEventThread, THREAD_TIMEOUT) )
					&& (WatchDog >= 10) )
				{
					// Note: TerminateThread is our method of last resort to get the thread to
					//  end. An excessive delay in processing any of the notification functions
					//  can cause us to get here. (Yes one second is an excessive delay!)
					TerminateThread(m_hEventThread, 0);
					TRACE(_T(" *** Forced thread closure *** \n"));
				}
				GetExitCodeThread(m_hEventThread, &dwExitCode);
			}

			CloseHandle(m_hEventThread);
			m_hEventThread = 0;
		}
		m_bStopThread = FALSE;
	}

} // namespace Win32xx

