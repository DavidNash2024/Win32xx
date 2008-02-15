///////////////////////////////////////
// ClientDialog.cpp

#include "ClientDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CClientDialog::CClientDialog(UINT nResID, HWND hWndParent) : CDialog(nResID, hWndParent), m_bClientConnected(FALSE)
{

}

CClientDialog::~CClientDialog()
{

}

void CClientDialog::Append(int nID, LPCTSTR buf)
{
	// This function appends text to an edit control

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

BOOL CClientDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case USER_CONNECT:
		OnClientConnect();
		break;
	case USER_DISCONNECT:
		OnClientDisconnect();
		break;
	case USER_RECEIVE:
		OnClientReceive();
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CClientDialog::OnClientDisconnect()
{

	SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, "Disconnected from server");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_IPADDRESS1), TRUE);
	SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_CONNECT), "Connect");
	m_bClientConnected = FALSE;
}

void CClientDialog::OnClientConnect()
{
	// Called when the connection to the server is established
	SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, "Connected to server");
	EnableWindow(GetDlgItem(m_hWnd, IDC_BUTTON_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_SEND), TRUE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_EDIT_PORT), FALSE);
	EnableWindow(GetDlgItem(m_hWnd, IDC_IPADDRESS1), FALSE);
	SetWindowText(GetDlgItem(m_hWnd, IDC_BUTTON_CONNECT), "Disconnect");
	m_bClientConnected = TRUE;
}

int CClientDialog::OnClientReceive()
{
	char buf[1025] = {0};
	int iChars = m_Client.Receive(buf, 1024, 0);
	Append(IDC_EDIT_RECEIVE, buf);
	return iChars;
}

BOOL CClientDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
    {
	case IDC_RADIO_TCP:
		OnTCP();
		return TRUE;
	case IDC_RADIO_UDP:
		OnUDP();
		return TRUE;
	case IDC_BUTTON_CONNECT:
		OnStartClient();
		return TRUE;
	case IDC_BUTTON_SEND:
		OnSend();
		return TRUE;
    } //switch (LOWORD(wParam))

	return FALSE;
}

BOOL CClientDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// reposition dialog
	RECT rc;
	::GetWindowRect(m_hWnd, &rc);
	::MoveWindow(m_hWnd, rc.left+14, rc.top+14, rc.right - rc.left, rc.bottom - rc.top, TRUE);

	// Set the initial state of the dialog
	SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, _T("Not Connected"));
	SetDlgItemText(m_hWnd, IDC_EDIT_PORT, _T("3000"));
	::SendMessage( GetDlgItem(m_hWnd, IDC_RADIO_TCP), BM_SETCHECK, BST_CHECKED, 0);
	::SendMessage( GetDlgItem(m_hWnd, IDC_IPADDRESS1), IPM_SETADDRESS, 0, MAKEIPADDRESS(127, 0, 0, 1));

	return true;
}

void CClientDialog::OnStartClient()
{
	TRACE(_T("Connect/Disconnect Button Pressed\n"));

	if (!m_bClientConnected)
	{
		// Create the socket
		if (m_Client.Create())
		{
			// Get the port number
			std::string sPort = GetDlgItemString(IDC_EDIT_PORT);
			int RemotePort = atoi(sPort.c_str());

			// Get the IP Address from the IP Address control
			DWORD dwAddr = 0;
			SendMessage(GetDlgItem(m_hWnd, IDC_IPADDRESS1), IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );  
			in_addr addr = {0};
			addr.S_un.S_addr = htonl(dwAddr);
			std::string sAddr =  inet_ntoa(addr);

			// Connect to the server
			if (0 != m_Client.Connect(sAddr.c_str(), RemotePort))
				throw CWinException("Failed to connect to server");

			OnClientConnect();
		}
		else
			throw CWinException("Failed to create Client socket");
	}
	else
	{
		m_Client.Disconnect();
		OnClientDisconnect();
	}
}

void CClientDialog::OnSend()
{
	std::string s = GetDlgItemString(IDC_EDIT_SEND);
	m_Client.Send((char*)s.c_str(), s.length(), 0);
}

void CClientDialog::OnTCP()
{
	TRACE(_T("Radio TCP\n"));
}

void CClientDialog::OnUDP()
{
	TRACE(_T("Radio UDP\n"));
}



