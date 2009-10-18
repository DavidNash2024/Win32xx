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


BOOL CTCPClientDlg::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Respond to the various dialog buttons
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_SEND2:
		Send();
		// Give keyboard focus to the Send Edit box
		SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND2), TRUE);
		return TRUE;
	}

	return FALSE;
}

BOOL CTCPClientDlg::OnInitDialog()
{
	SetForegroundWindow();
	return TRUE;
}

void CTCPClientDlg::Receive()
{
	char str[1025] = {0};			// Assign all 1025 elements to NULL
	m_pSocket->Receive(str, 1024, 0);
	tString t = CharToTString(str);
	Append(IDC_EDIT_RECEIVE2, t.c_str());
}

void CTCPClientDlg::Send()
{
	tString t = GetDlgItemString(IDC_EDIT_SEND2);
	std::string s = TCharToString(t.c_str());
	m_pSocket->Send(s.c_str(), s.length(), 0);
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

BOOL CSvrDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
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
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// reposition dialog
	RECT rc;
	::GetWindowRect(m_hWnd, &rc);
	::MoveWindow(m_hWnd, rc.left-14, rc.top-14, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	// Set the initial state of the dialog
	SetDlgItemText(IDC_EDIT_STATUS, _T("Server Stopped"));
	SetDlgItemText(IDC_EDIT_PORT, _T("3000"));
	::SendMessage(GetDlgItem(IDC_RADIO_TCP), BM_SETCHECK, BST_CHECKED, 0);

	return true;
}

void CSvrDialog::OnStartServer()
{
	// Respond to the Start/Stop Button press
	TRACE(_T("Start/Stop Button Pressed\n"));

	if (!m_bServerStarted)
	{
		// Attempt to start the server
		if (!StartServer())	
			return;

		// Update the dialog
		::SetDlgItemText(m_hWnd, IDC_BUTTON_START, _T("Stop Server"));
		::EnableWindow(GetDlgItem(IDC_EDIT_PORT), FALSE);
		::EnableWindow(GetDlgItem(IDC_RADIO_TCP), FALSE);
		::EnableWindow(GetDlgItem(IDC_RADIO_UDP), FALSE);
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
		::SetDlgItemText(m_hWnd, IDC_BUTTON_START, _T("Start Server"));
		::EnableWindow(GetDlgItem(IDC_EDIT_PORT), TRUE);
		::EnableWindow(GetDlgItem(IDC_RADIO_TCP), TRUE);
		::EnableWindow(GetDlgItem(IDC_RADIO_UDP), TRUE);
		::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), FALSE);
		::EnableWindow(GetDlgItem(IDC_EDIT_SEND), FALSE);
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
			tString t = GetDlgItemString(IDC_EDIT_SEND);
			std::string s = TCharToString(t.c_str());
			m_MainSocket.SendTo(s.c_str(), s.length(), 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));
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
		TRACE(_T("Failed to accept connection from client"));
		return;
	} 
	
	pClient->StartEvents();

	// Create the new chat dialog
	CTCPClientDlg* pDialog = new CTCPClientDlg(IDD_DIALOG2, m_hWnd);
	pDialog->m_pSocket = pClient;
	pDialog->DoModeless();

	// Reposition the chat dialog
	RECT r = {0};
	::GetWindowRect(pDialog->GetHwnd(), &r);
	int offset  = 4 * (m_ConnectedClients.size() - 1);
	::MoveWindow(pDialog->GetHwnd(), r.left + offset, r.top + offset + 80, r.right - r.left, r.bottom - r.top, TRUE);
	::ShowWindow(pDialog->GetHwnd(), SW_SHOW);

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
			tString t = CharToTString(str);
			TRACE(_T("[Received:] ")); TRACE(t.c_str()); TRACE(_T("\n"));
			::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), TRUE);
			::EnableWindow(GetDlgItem(IDC_EDIT_SEND), TRUE);
			::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE);
		}
		break;
	}
	tString t = CharToTString(str);
	Append(IDC_EDIT_RECEIVE, t.c_str());
}

BOOL CSvrDialog::StartServer()
{
	LRESULT lr = ::SendMessage(GetDlgItem(IDC_RADIO_TCP), BM_GETCHECK, 0, 0);
	m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

	// Create the main socket
	if (!m_MainSocket.Create(m_SocketType))
	{
		Append(IDC_EDIT_STATUS, _T("Create Socket Failed"));
		return FALSE;
	}
	
	// Retrieve the local port number
	tString tPort = GetDlgItemString(IDC_EDIT_PORT);
	std::string sPort = TCharToString(tPort.c_str());
	int LocalPort = atoi(sPort.c_str());

	// Bind the socket.
	sockaddr_in service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons( (u_short)LocalPort );

	// Bind the IP address to the listening socket
	if ( m_MainSocket.Bind( (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
	{
		Append(IDC_EDIT_STATUS, _T("Bind failed"));
		return FALSE;
	}

	if (m_SocketType == SOCK_STREAM)
	{
		// Listen for connections from clients (TCP server only)
		if ( SOCKET_ERROR == m_MainSocket.Listen() )
		{
			Append(IDC_EDIT_STATUS, _T("Error listening on socket"));
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
