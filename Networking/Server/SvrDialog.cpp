///////////////////////////////////////
// SvrDialog.cpp

#include "SvrDialog.h"
#include "resource.h"



// Definitions for the CSvrDialog class
CSvrDialog::CSvrDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent), 
              m_bServerStarted(FALSE)//, m_DisconnectingSocket(0)
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

void CSvrDialog::CleanDisconnected()
{
	for (unsigned int i = 0; i < m_DisconnectedSockets.size(); i++)
	{
		delete m_DisconnectedSockets[i];
	}
	m_DisconnectedSockets.clear();
}

BOOL CSvrDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case USER_ACCEPT:
		OnListenAccept();
		break;
	case USER_DISCONNECT:
		OnClientDisconnect((CClientSocket*)wParam);
		break;
	case USER_RECEIVE:
		OnClientReceive((CClientSocket*)wParam);
		break;
	case WM_TIMER:
		if (IDT_TIMER1 == wParam) 
			CleanDisconnected();
		break; 
 	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CSvrDialog::OnClientDisconnect(CClientSocket* pClient)
{
	Append(IDC_EDIT_STATUS, "Client disconnected");

	std::vector<CClientSocket*>::iterator Iter;

	for (Iter = m_ConnectedSockets.begin(); Iter != m_ConnectedSockets.end(); Iter++)
	{
		if (*Iter == pClient) break;
	}

	// Move the socket to the disconnected pool
	if (Iter != m_ConnectedSockets.end())
	{
		m_DisconnectedSockets.push_back(pClient);
	//	m_DisconnectedSockets.push_back(m_DisconnectingSocket);
	//	m_DisconnectingSocket = pClient;
		m_ConnectedSockets.erase(Iter);
	}    
}

BOOL CSvrDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_START:
		OnStartServer();
		return TRUE;
	case IDC_BUTTON_SEND:
		OnSend();
		return TRUE;
	case IDC_RADIO_TCP:
		OnTCP();
		return TRUE;
	case IDC_RADIO_UDP:
		OnUDP();
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

	// Start the 1 second timer
	SetTimer(m_hWnd, IDT_TIMER1, 1000, NULL);

	return true;
}

void CSvrDialog::OnStartServer()
{
	TRACE(_T("Start/Stop Button Pressed\n"));
	
	if (!m_bServerStarted)
	{
		// Create the main socket
		if (m_ListenSocket.Create())
		{
			// Retrieve the local port number
			std::string s = GetDlgItemString(IDC_EDIT_PORT);
			int LocalPort = atoi(s.c_str());

			if (!StartServer(LocalPort))
				return;

			Append(IDC_EDIT_STATUS, "Server Started");
			Append(IDC_EDIT_STATUS, "Waiting for client ...");
			::SetDlgItemText(m_hWnd, IDC_BUTTON_START, "Stop Server");
			EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), FALSE);
		} 

		else
		{
			Append(IDC_EDIT_STATUS, "Start Failed");
		}
	}
	else
	{
		StopServer();
		Append(IDC_EDIT_STATUS, "Server Stopped");
		::SetDlgItemText(m_hWnd, IDC_BUTTON_START, "Start Server");
		EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), TRUE);
	}
	m_bServerStarted = !m_bServerStarted;
}

void CSvrDialog::OnSend()
{
	// Responds to the send button

	std::string s = GetDlgItemString(IDC_EDIT_SEND);
//	m_DataSocket.Send((char*)s.c_str(), s.length(), 0);
}

void CSvrDialog::OnTCP()
{
	TRACE(_T("Radio TCP\n"));
}

void CSvrDialog::OnUDP()
{
	TRACE(_T("Radio UDP\n"));
}

void CSvrDialog::OnListenAccept()
{
	CClientSocket* pClient = new CClientSocket;
	m_ListenSocket.Accept(*pClient, NULL, NULL);
	if (INVALID_SOCKET == m_ListenSocket.GetSocket())
	{
		delete pClient;
		TRACE("Failed to accept connection from client");
	} 
	
	m_ConnectedSockets.push_back(pClient);

	Append(IDC_EDIT_STATUS, "Client Connected");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), TRUE);
}

void CSvrDialog::OnClientReceive(CClientSocket* pClient)
{
	char str[1025] = {0};
	pClient->Receive(str, 1024, 0);
	TRACE(str);
	pClient->Send(str, strlen(str), 0);
	Append(IDC_EDIT_RECEIVE, str);
}

BOOL CSvrDialog::StartServer(int LocalPort)
{
    // Bind the socket.
	sockaddr_in service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons( (u_short)LocalPort );

	if ( m_ListenSocket.Bind( (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
	{
		Append(IDC_EDIT_STATUS, "Bind failed");
		return FALSE;
	}

	// Listen on the socket.
	if ( SOCKET_ERROR == m_ListenSocket.Listen() )
	{
		Append(IDC_EDIT_STATUS, "Error listening on socket");
		return FALSE;
	}
	
	return TRUE;
}

void CSvrDialog::StopServer()
{
	m_ListenSocket.Disconnect();
	
	for (unsigned int i = 0; i < m_ConnectedSockets.size(); i++)
	{
		delete m_ConnectedSockets[i];
	}
	m_ConnectedSockets.clear();

	for (unsigned int i = 0; i < m_DisconnectedSockets.size(); i++)
	{
		delete m_DisconnectedSockets[i];
	}
	m_DisconnectedSockets.clear();

//	if (m_DisconnectingSocket)
//		delete m_DisconnectingSocket;
}
