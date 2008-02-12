///////////////////////////////////////
// SvrDialog.cpp

#include "SvrDialog.h"
#include "resource.h"


// Definitions for the CSvrDialog class
CSvrDialog::CSvrDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent), m_bServerStarted(FALSE)
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
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CSvrDialog::OnClientClose()
{
	Append(IDC_EDIT_STATUS, "Client disconnected");
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
			Append(IDC_EDIT_STATUS, "Server Started");
			Append(IDC_EDIT_STATUS, "Waiting for client ...");
			::SetDlgItemText(m_hWnd, IDC_BUTTON_START, "Stop Server");
			EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), FALSE);

			// Retrieve the local port number
			std::string s = GetDlgItemString(IDC_EDIT_PORT);
			int LocalPort = atoi(s.c_str());

			StartServer(LocalPort);
		} 

		else
		{
			Append(IDC_EDIT_STATUS, "Start Failed");
		}
	}
	else
	{
		StopServer();
		m_ListenSocket.Disconnect();
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
		throw CWinException (_T("Failed to accept connection from client"));
	} 
	
	ConnectedSockets.push_back(pClient);

	Append(IDC_EDIT_STATUS, "Client Connected");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), TRUE);
}

void CSvrDialog::OnClientReceive(CClientSocket* pClient)
{
	char str[200];
	int recvLength = pClient->Receive(str, 200, 0);
	str[recvLength] = '\0';
	TRACE(str);
	pClient->Send(str, 200, 0);
	Append(IDC_EDIT_RECEIVE, str);
}

void CSvrDialog::StartServer(int LocalPort)
{
    // Bind the socket.
	sockaddr_in service;

	service.sin_family = AF_INET;
	service.sin_addr.s_addr = htonl(INADDR_ANY);
	service.sin_port = htons( (u_short)LocalPort );

	if ( m_ListenSocket.Bind( (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR )
		throw CWinException(_T("Bind failed"));

	// Listen on the socket.
	if ( SOCKET_ERROR == m_ListenSocket.Listen( 1 ) )
		throw CWinException(_T("Error listening on socket"));

}

void CSvrDialog::StopServer()
{
	for (unsigned int i = 0; i < ConnectedSockets.size(); i++)
	{
		delete ConnectedSockets[i];
	}
	
	ConnectedSockets.clear();
}
