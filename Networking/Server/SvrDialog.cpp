///////////////////////////////////////
// SvrDialog.cpp

#include "SvrDialog.h"
#include "resource.h"


CTCPClientDlg::CTCPClientDlg(UINT nResID, HWND hWndParent) : 
				CDialog(nResID, hWndParent), m_pSocket(0)
{
}

void CTCPClientDlg::Send()
{
}

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

	HWND hEdit = GetDlgItem(m_hWnd, nID);

	// Append Line Feed
	int ndx = GetWindowTextLength (hEdit);
	if (ndx)
	{
		SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) "\r\n"));
	}

	// Append text
	ndx = GetWindowTextLength(hEdit);
	SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) ((LPSTR) buf));
}

BOOL CSvrDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
 	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CSvrDialog::OnSocketDisconnect(CServerSocket* pClient)
{
	// Respond to a socket disconnect notification
	Append(IDC_EDIT_STATUS, "Client disconnected");

	// Iterate through the vector, looking for the matching CServerSocket pointer 
	std::vector<Client>::iterator Iter;
	for (Iter = m_ConnectedSockets.begin(); Iter != m_ConnectedSockets.end(); Iter++)
	{
		Client c = *Iter;
		if (c.pSocket == pClient)
			break;
	}

	// delete the CServerSocket, and remove its pointer
	if (Iter != m_ConnectedSockets.end())
	{
		Client c = *Iter;
		delete c.pSocket;
		delete c.pDialog;
		m_ConnectedSockets.erase(Iter);
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
	SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, _T("Server Stopped"));
	SetDlgItemText(m_hWnd, IDC_EDIT_PORT, _T("3000"));
	::SendMessage(GetDlgItem(m_hWnd, IDC_RADIO_TCP), BM_SETCHECK, BST_CHECKED, 0);

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
		SetDlgItemText(m_hWnd, IDC_BUTTON_START, "Stop Server");
		EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), FALSE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_RADIO_TCP), FALSE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_RADIO_UDP), FALSE);
		if (m_SocketType == SOCK_STREAM)
		{
			Append(IDC_EDIT_STATUS, "TCP Server Started");
			Append(IDC_EDIT_STATUS, "Waiting for client ...");
		}
		else
		{
			Append(IDC_EDIT_STATUS, "UDP Server Started");
			Append(IDC_EDIT_STATUS, "Waiting for client data");
		}
	}
	else
	{
		StopServer();

		// Update the dialog
		Append(IDC_EDIT_STATUS, "Server Stopped");
		SetDlgItemText(m_hWnd, IDC_BUTTON_START, "Start Server");
		EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), TRUE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_RADIO_TCP), TRUE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_RADIO_UDP), TRUE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), FALSE);
		EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), FALSE);
	}
	m_bServerStarted = !m_bServerStarted;
}

void CSvrDialog::OnSend()
{
	// Responds to the send button

	std::string s = GetDlgItemString(IDC_EDIT_SEND);

	switch(m_SocketType)
	{
		case SOCK_STREAM:
			break;
		case SOCK_DGRAM:
			m_ListenSocket.SendTo(s.c_str(), s.length(), 0, (SOCKADDR*)&m_ClientAddr, sizeof(m_ClientAddr));
			break;
	}
}

void CSvrDialog::OnSocketAccept()
{
	// Accept the connection from the client
	CServerSocket* pClient = new CServerSocket;
	m_ListenSocket.Accept(*pClient, NULL, NULL);
	if (INVALID_SOCKET == m_ListenSocket.GetSocket())
	{
		delete pClient;
		TRACE("Failed to accept connection from client");
		return;
	} 
	
	pClient->StartNotifyEvents();

	Client c;
	c.pSocket = pClient;
	c.pDialog = new CTCPClientDlg(IDD_DIALOG2, m_hWnd);
	c.pDialog->m_pSocket = pClient;
	c.pDialog->DoModeless();
	m_ConnectedSockets.push_back(c);

	// Update the dialog
	Append(IDC_EDIT_STATUS, "Client Connected");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), TRUE);
}

void CSvrDialog::OnSocketReceive(CServerSocket* pClient)
{
	// This trick ensures our char array is NULL terminated
	char str[1025] = {0};			// Assign all 1025 elements to NULL
	
	switch (m_SocketType)
	{
	case SOCK_STREAM:
		{
			pClient->Receive(str, 1024, 0); // Receive up to 1024 chars
			TRACE(str);
			pClient->Send(str, strlen(str), 0);
		}
		break;
	case SOCK_DGRAM:
		{
			int addrlen = sizeof(m_ClientAddr);
			m_ListenSocket.ReceiveFrom(str, 1024, 0, (SOCKADDR*)&m_ClientAddr, &addrlen); 
			TRACE("[Received:] "); TRACE(str); TRACE("\n");
			EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), TRUE);
			EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), TRUE);			
		}
		break;
	}
	
	Append(IDC_EDIT_RECEIVE, str);
}

BOOL CSvrDialog::StartServer()
{
	LRESULT lr = SendMessage(GetDlgItem(m_hWnd, IDC_RADIO_TCP), BM_GETCHECK, 0, 0);
	m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

	// Create the main socket
	if (!m_ListenSocket.Create(m_SocketType))
	{
		Append(IDC_EDIT_STATUS, "Create Socket Failed");
		return FALSE;
	}
	
	// Retrieve the local port number
	std::string s = GetDlgItemString(IDC_EDIT_PORT);
	int LocalPort = atoi(s.c_str());

	// Bind the socket.
	sockaddr_in service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons( (u_short)LocalPort );

	// Bind the IP address to the listening socket
	if ( m_ListenSocket.Bind( (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
	{
		Append(IDC_EDIT_STATUS, "Bind failed");
		return FALSE;
	}

	if (m_SocketType == SOCK_STREAM)
	{
		// Listen for connections from clients (TCP server only)
		if ( SOCKET_ERROR == m_ListenSocket.Listen() )
		{
			Append(IDC_EDIT_STATUS, "Error listening on socket");
			return FALSE;
		}
	}

	m_ListenSocket.StartNotifyEvents();

	return TRUE;
}

void CSvrDialog::StopServer()
{
	m_ListenSocket.Disconnect();
	
	// Delete the client connections
	for (unsigned int i = 0; i < m_ConnectedSockets.size(); i++)
	{
		delete m_ConnectedSockets[i].pDialog;
		delete m_ConnectedSockets[i].pSocket;
	}
	m_ConnectedSockets.clear();
}
