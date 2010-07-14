///////////////////////////////////////
// SvrDialog.cpp

#include "stdafx.h"
#include "SvrDialog.h"
#include "DialogApp.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CTCPClientDlg class
CTCPClientDlg::CTCPClientDlg(UINT nResID, HWND hWndParent) : 
				CDialog(nResID, hWndParent), m_pSocket(0)
{
}

void CTCPClientDlg::Append(int nID, LPCTSTR buf)
{
	// This function appends some text to an edit control

	HWND hEdit = GetDlgItem(nID);

	// Append Line Feed
	int ndx = GetWindowTextLength (hEdit);
	if (ndx)
	{
		SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) (_T( "\r\n")));
	}

	// Append text
	ndx = GetWindowTextLength(hEdit);
	SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) buf);
}

BOOL CTCPClientDlg::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		// Disconnect the socket when the user closes this chat dialog
		m_pSocket->Disconnect();
		break;
	}
	
	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}


BOOL CTCPClientDlg::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to the various dialog buttons

	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_SEND2:
		Send();
		return TRUE;
	}

	return FALSE;
}

BOOL CTCPClientDlg::OnInitDialog()
{
	SetForegroundWindow();

	m_EditSend.AttachDlgItem(IDC_EDIT_SEND2, this);
	m_EditReceive.AttachDlgItem(IDC_EDIT_RECEIVE2, this);
	m_ButtonSend.AttachDlgItem(IDC_BUTTON_SEND2, this);

	return TRUE;
}

void CTCPClientDlg::Receive()
{
	char str[1025] = {0};			// Assign all 1025 elements to NULL
	m_pSocket->Receive(str, 1024, 0);
	Append(IDC_EDIT_RECEIVE2, CharToTChar(str));
}

void CTCPClientDlg::Send()
{
	LPCTSTR szSend = m_EditSend.GetWindowText();
	m_pSocket->Send(TCharToChar(szSend), lstrlen(szSend), 0);
}



/////////////////////////////////////////////
// Definitions for the CSvrDialog class
CSvrDialog::CSvrDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent), 
              m_bServerStarted(FALSE), m_SocketType(SOCK_STREAM)
{
}

CSvrDialog::~CSvrDialog()
{
	StopServer();
}

void CSvrDialog::Append(int nID, LPCTSTR buf)
{
	// This function appends some text to an edit control

	HWND hEdit = GetDlgItem(nID);

	// Append Line Feed
	int ndx = GetWindowTextLength (hEdit);
	if (ndx)
	{
		SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPCTSTR)_T("\r\n")));
	}

	// Append text
	ndx = GetWindowTextLength(hEdit);
	SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) buf);
}

BOOL CSvrDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// respond to the user defined message posted to the dialog
	switch (uMsg)
	{
	case USER_ACCEPT:
		OnSocketAccept();
		break;
	case USER_DISCONNECT:
		OnSocketDisconnect((CServerSocket*)wParam);
		break;
	case USER_RECEIVE:
		OnSocketReceive((CServerSocket*)wParam);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
 	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CSvrDialog::OnSocketDisconnect(CServerSocket* pClient)
{
	// Respond to a socket disconnect notification
	Append(IDC_EDIT_STATUS, _T("Client disconnected"));


	// Allocate an iterator for our CServerSocket map
	std::map< CServerSocket*, CTCPClientDlg* >::iterator Iter;
	Iter = m_ConnectedClients.find(pClient);

	// delete the CServerSocket, and remove its pointer
	if (Iter != m_ConnectedClients.end())
	{
		delete pClient;
		delete Iter->second;
		m_ConnectedClients.erase(Iter);
	}  
}

BOOL CSvrDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// Respond to the various dialog buttons
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_START:
		OnStartServer();
		return TRUE;
	case IDC_BUTTON_SEND:
		OnSend();
		return TRUE;
    } //switch (LOWORD(wParam))

	return FALSE;
}

BOOL CSvrDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge( IDW_MAIN );
	SetIconSmall( IDW_MAIN );

	// reposition dialog
	CRect rc = GetWindowRect();
	MoveWindow( rc.left-14, rc.top-14, rc.Width(), rc.Height(), TRUE );

	// Attach CWnd objects to the dialog's children
	m_IP4Address.AttachDlgItem( IDC_IPADDRESS, this );
	m_EditIP6Address.AttachDlgItem( IDC_EDIT_IPV6ADDRESS, this );
	m_EditStatus.AttachDlgItem( IDC_EDIT_STATUS, this );
	m_EditPort.AttachDlgItem( IDC_EDIT_PORT, this );
	m_EditSend.AttachDlgItem( IDC_EDIT_SEND, this );
	m_EditReceive.AttachDlgItem( IDC_EDIT_RECEIVE, this );
	m_ButtonStart.AttachDlgItem( IDC_BUTTON_START, this );
	m_ButtonSend.AttachDlgItem( IDC_BUTTON_SEND, this );
	m_RadioIP4.AttachDlgItem( IDC_RADIO_IPV4, this );
	m_RadioIP6.AttachDlgItem( IDC_RADIO_IPV6, this );
	m_RadioTCP.AttachDlgItem( IDC_RADIO_TCP, this );
	m_RadioUDP.AttachDlgItem( IDC_RADIO_UDP, this );

	// Set the initial state of the dialog
	m_EditIP6Address.SetWindowTextW( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
	m_RadioIP4.SendMessage( BM_SETCHECK, BST_CHECKED, 0 );
	m_IP4Address.SendMessage( IPM_SETADDRESS, 0, MAKEIPADDRESS(127, 0, 0, 1) );
	m_EditStatus.SetWindowText( _T("Server Stopped") );
	m_EditPort.SetWindowText( _T("3000") );
	m_RadioTCP.SendMessage( BM_SETCHECK, BST_CHECKED, 0 );

	return true;
}

void CSvrDialog::OnStartServer()
{
	// Respond to the Start/Stop Button press
	TRACE( _T("Start/Stop Button Pressed\n") );

	if (!m_bServerStarted)
	{
		// Attempt to start the server
		if (!StartServer())	
			return;

		// Update the dialog
		m_ButtonStart.SetWindowText( _T("Stop Server") ); 
		m_IP4Address.EnableWindow(FALSE);
		m_EditIP6Address.EnableWindow(FALSE);
		m_EditPort.EnableWindow(FALSE);
		m_RadioIP4.EnableWindow(FALSE);
		m_RadioIP6.EnableWindow(FALSE);
		m_RadioTCP.EnableWindow(FALSE);
		m_RadioUDP.EnableWindow(FALSE);

		
		if (m_SocketType == SOCK_STREAM)
		{
			Append(IDC_EDIT_STATUS, _T("TCP Server Started"));
			Append(IDC_EDIT_STATUS, _T("Waiting for client ..."));
		}
		else
		{
			Append(IDC_EDIT_STATUS, _T("UDP Server Started"));
			Append(IDC_EDIT_STATUS, _T("Waiting for client data"));
		}
	}
	else
	{
		StopServer();

		// Update the dialog
		Append(IDC_EDIT_STATUS, _T("Server Stopped"));
		m_ButtonStart.SetWindowText( _T("Start Server") );
		m_IP4Address.EnableWindow(TRUE);
		m_EditIP6Address.EnableWindow(TRUE);
		m_EditPort.EnableWindow(TRUE);
		m_RadioIP4.EnableWindow(TRUE);
		m_RadioIP6.EnableWindow(TRUE);
		m_RadioTCP.EnableWindow(TRUE);
		m_RadioUDP.EnableWindow(TRUE);
		m_ButtonSend.EnableWindow(FALSE);
		m_EditSend.EnableWindow(FALSE);
	}
	m_bServerStarted = !m_bServerStarted;
}

void CSvrDialog::OnSend()
{
	// Responds to the send button

	switch(m_SocketType)
	{
		case SOCK_STREAM:
			// TCP connections have a seperate chat dialog for sending/receiving data
			break;
		case SOCK_DGRAM:
			LPCTSTR szSend = m_EditSend.GetWindowText();
			m_MainSocket.SendTo(TCharToChar(szSend), lstrlen(szSend), 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));
			break;
	}
}

void CSvrDialog::OnSocketAccept()
{
	// Accept the connection from the client
	CServerSocket* pClient = new CServerSocket;
	m_MainSocket.Accept(*pClient, NULL, NULL);
	if (INVALID_SOCKET == m_MainSocket.GetSocket())
	{
		delete pClient;
		TRACE(_T("Failed to accept connection from client\n"));
		TRACE(m_MainSocket.GetLastError());
		return;
	} 
	
	pClient->StartEvents();

	// Create the new chat dialog
	CTCPClientDlg* pDialog = new CTCPClientDlg(IDD_CHAT, m_hWnd);
	pDialog->m_pSocket = pClient;
	pDialog->DoModeless();

	// Reposition the chat dialog
	CRect rc = pDialog->GetWindowRect();
	int offset = 4 * ((int)m_ConnectedClients.size() - 1);
	pDialog->MoveWindow(rc.left + offset, rc.top + offset + 80, rc.Width(), rc.Height(), TRUE);
	pDialog->ShowWindow();

	// Add the socket and dialog to the map
	m_ConnectedClients.insert(std::make_pair(pClient, pDialog));

	// Update the dialog
	Append(IDC_EDIT_STATUS, _T("Client Connected"));
}

void CSvrDialog::OnSocketReceive(CServerSocket* pClient)
{
	// This trick ensures our char array is NULL terminated
	char str[1025] = {0};			// Assign all 1025 elements to NULL
	
	switch (m_SocketType)
	{
	case SOCK_STREAM:
		{
			// Pass this on to the TCP chat dialog
			std::map< CServerSocket*, CTCPClientDlg* >::iterator Iter;
			Iter = m_ConnectedClients.find(pClient);
			Iter->second->Receive();
		}
		break;
	case SOCK_DGRAM:
		{
			int addrlen = sizeof(m_ClientAddr);
			m_MainSocket.ReceiveFrom(str, 1024, 0, (SOCKADDR*)&m_ClientAddr, &addrlen); 
			TRACE(_T("[Received:] ")); TRACE(CharToTChar(str)); TRACE(_T("\n"));
			m_ButtonSend.EnableWindow(TRUE);
			m_EditSend.EnableWindow(TRUE);
			SendMessage( WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE );
		}
		break;
	}
	Append(IDC_EDIT_RECEIVE, CharToTChar(str));
}

BOOL CSvrDialog::StartServer()
{
	LRESULT lr = m_RadioTCP.SendMessage( BM_GETCHECK, 0, 0 );
	m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

	// Create the main socket
	lr = m_RadioIP4.SendMessage( BM_GETCHECK, 0, 0 );
	int IPfamily = (lr == BST_CHECKED)? PF_INET : PF_INET6 ;
	if (!m_MainSocket.Create(IPfamily, m_SocketType))
	{
		Append(IDC_EDIT_STATUS, _T("Create Socket Failed"));
		Append(IDC_EDIT_STATUS, m_MainSocket.GetLastError());
		return FALSE;
	}

	// Retrieve the IP Address
	std::string sAddr;
	if (PF_INET6 == IPfamily)
	{
		sAddr = TCharToChar( m_EditIP6Address.GetWindowText() );
	}
	else
	{
		DWORD dwAddr = 0;
		m_IP4Address.SendMessage( IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );
		in_addr addr = {0};
		addr.S_un.S_addr = htonl(dwAddr);
		sAddr = inet_ntoa(addr);
	}

	// Retrieve the local port number
	std::string sPort = TCharToChar( m_EditPort.GetWindowText() );

	// Bind to the socket
	Append(IDC_EDIT_STATUS, _T("Binding to socket"));
	char szText[80];
	wsprintfA(szText, "Addr %s, Port %s, type %s", sAddr.c_str(), sPort.c_str(), (m_SocketType == SOCK_STREAM)?"TCP":"UDP" );
	Append(IDC_EDIT_STATUS, CharToTChar(szText));
	
	int RetVal = m_MainSocket.Bind(sAddr.c_str(), sPort.c_str() );
	if ( RetVal != 0 )
	{
		Append(IDC_EDIT_STATUS, _T("Bind failed"));
		Append(IDC_EDIT_STATUS, m_MainSocket.GetLastError());
		return FALSE;
	}

	if (m_SocketType == SOCK_STREAM)
	{
		// Listen for connections from clients (TCP server only)
		RetVal = m_MainSocket.Listen();
		if	( SOCKET_ERROR == RetVal )
		{
			Append(IDC_EDIT_STATUS, _T("Error listening on socket"));
			Append(IDC_EDIT_STATUS, m_MainSocket.GetLastError());
			return FALSE;
		}
	}

	m_MainSocket.StartEvents();

	return TRUE;
}

void CSvrDialog::StopServer()
{
	m_MainSocket.Disconnect();
	
	// Delete the client connections
	std::map<CServerSocket*, CTCPClientDlg*>::iterator iter;
	for (iter = m_ConnectedClients.begin(); iter != m_ConnectedClients.end(); iter++)
	{
		delete (*iter).first;
		delete (*iter).second;
	}
	
	m_ConnectedClients.clear();
}
