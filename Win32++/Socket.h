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

#include "WinCore.h"

namespace Win32xx
{
	class CSocket
	{
	public:
		CSocket();
		~CSocket();
		SOCKET& GetSocket() const {return (const SOCKET) m_DataSocket;}

		virtual void Accept();
		virtual void Connect();
		virtual BOOL Create( int nSocketType = SOCK_STREAM );
		virtual int Receive( char* buf, int len, int flags);
		virtual int Send( char* buf, int len, int flags);

		virtual BOOL OnAccept()  {return FALSE;}
		virtual BOOL OnClose() 	 {return FALSE;}
		virtual BOOL OnConnect() {return FALSE;}
		virtual BOOL OnReceive() {return FALSE;}

	private:
		static DWORD WINAPI AcceptThread(LPVOID pCThread);

		BOOL m_WSAStarted;
		SOCKET m_Socket;
		SOCKET m_DataSocket;
		int m_SocketType;
	};

}


#endif // #ifndef SOCKET_H

