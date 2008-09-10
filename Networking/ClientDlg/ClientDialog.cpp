///////////////////////////////////////
// ClientDialog.cpp

#include "ClientDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CClientDialog::CClientDialog(UINT nResID, HWND hWndParent) :
		CDialog(nResID, hWndParent), m_bClientConnected(FALSE), m_SocketType(SOCK_STREAM)
{
	// Add support for the IP Address control
	// It requires Win95 with IE4 intergrated or a later version of Windows OS.
	LoadCommonControlsEx();
}

CClientDialog::~CClientDialog()
{
}

void CClientDialog::Append(int nID, LPCTSTR buf)
{
	// This function appends text to an edit control

	HWND hEdit = GetDlgItem(nID);

	// Append Line Feed
	int ndx = GetWindowTextLength (hEdit);
	if (ndx)
	{
		SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
	}

	// Append text
	ndx = GetWindowTextLength(hEdit);
	SendMessage (hEdit, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendMessage (hEdit, EM_REPLACESEL, 0, (LPARAM) buf);
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
	case WM_ACTIVATE:
		// Give focus to the Send Edit box
		SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE);
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

void CClientDialog::OnClientDisconnect()
{
	// Called when the socket is disconnected from the server
	m_bClientConnected = FALSE;
	::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT), TRUE);

	// Update the dialog
	::SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, _T("Disconnected from server"));
	::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), FALSE);
	::EnableWindow(GetDlgItem(IDC_EDIT_SEND), FALSE);
	::EnableWindow(GetDlgItem(IDC_EDIT_PORT), TRUE);
	::EnableWindow(GetDlgItem(IDC_IPADDRESS1), TRUE);
	::EnableWindow(GetDlgItem(IDC_RADIO_TCP), TRUE);
	::EnableWindow(GetDlgItem(IDC_RADIO_UDP), TRUE);
	::SetWindowText(GetDlgItem(IDC_BUTTON_CONNECT), _T("Connect"));
}

void CClientDialog::LoadCommonControlsEx()
{
	// This function adds support for the IP address control in the dialog.
	HMODULE hComCtl;

	try
	{
		// Load the Common Controls DLL
		hComCtl = ::LoadLibrary(_T("COMCTL32.DLL"));
		if (!hComCtl)
			throw CWinException(_T("CFrame::LoadCommonControls ... Failed to load COMCTL32.DLL"));

		if (GetComCtlVersion() > 470)
		{
			// Declare a pointer to the InItCommonControlsEx function
			typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
			INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(hComCtl, "InitCommonControlsEx");

			// Call InitCommonControlsEx
			INITCOMMONCONTROLSEX InitStruct;
			InitStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
			InitStruct.dwICC = ICC_INTERNET_CLASSES;
			if((!(*pfnInit)(&InitStruct)))
				throw CWinException(_T("CFrame::LoadCommonControls ... InitCommonControlsEx failed"));
		}
		else
		{
			MessageBox(NULL, _T("Common Control Version 4.71 or later required (IE 4)"), _T("NOT SUPPORTED"), MB_ICONSTOP);
		}

		::FreeLibrary(hComCtl);
	}
	
	catch (const CWinException &e)
	{
		e.MessageBox();
		if (hComCtl)
			::FreeLibrary(hComCtl);
	}
}


void CClientDialog::OnClientConnect()
{
	// Called when the connection to the server is established
	m_bClientConnected = TRUE;
	::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT), TRUE);

	// Move focus to the Send Edit box
	::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE);
	::SetForegroundWindow(m_hWnd);

	// Update the dialog
	::SetDlgItemText(m_hWnd, IDC_EDIT_STATUS, _T("Connected to server"));
	::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), TRUE);
	::EnableWindow(GetDlgItem(IDC_EDIT_SEND), TRUE);
	::EnableWindow(GetDlgItem(IDC_EDIT_PORT), FALSE);
	::EnableWindow(GetDlgItem(IDC_IPADDRESS1), FALSE);
	::EnableWindow(GetDlgItem(IDC_RADIO_TCP), FALSE);
	::EnableWindow(GetDlgItem(IDC_RADIO_UDP), FALSE);
	::SetWindowText(GetDlgItem(IDC_BUTTON_CONNECT), _T("Disconnect"));
}

int CClientDialog::OnClientReceive()
{
	// Called when the socket has data to receive
	char buf[1025] = {0};	// assign 1025 array elements to NULL
	int size = m_Client.Receive(buf, 1024, 0); // receive at most 1024 chars
	tString t = CharToTString(buf);
	Append(IDC_EDIT_RECEIVE, t.c_str());
	return size;
}

BOOL CClientDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	// Respond to the dialog buttons
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_CONNECT:
		OnStartClient();
		return TRUE;
	case IDC_BUTTON_SEND:
		OnSend();
		// Give keyboard focus to the Send edit box
		SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE);
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
	::SendMessage( GetDlgItem(IDC_RADIO_TCP), BM_SETCHECK, BST_CHECKED, 0);
	::SendMessage( GetDlgItem(IDC_IPADDRESS1), IPM_SETADDRESS, 0, MAKEIPADDRESS(127, 0, 0, 1));

	return true;
}

void CClientDialog::OnStartClient()
{
	TRACE(_T("Connect/Disconnect Button Pressed\n"));

	LRESULT lr = SendMessage(GetDlgItem(IDC_RADIO_TCP), BM_GETCHECK, 0, 0);
	m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

	if (!m_bClientConnected)
	{
		switch(m_SocketType)
		{
		case SOCK_STREAM:
			{
				// Create the socket
				if (!m_Client.Create())
				{
					MessageBox(m_hWnd, _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING);
					return;
				}

				// Get the port number
				tString tPort = GetDlgItemString(IDC_EDIT_PORT);
				std::string sPort = TCharToString(tPort.c_str());
				int RemotePort = atoi(sPort.c_str());

				// Get the IP Address from the IP Address control
				DWORD dwAddr = 0;
				SendMessage(GetDlgItem(IDC_IPADDRESS1), IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );
				in_addr addr = {0};
				addr.S_un.S_addr = htonl(dwAddr);
				std::string sAddr =  inet_ntoa(addr);

				// Temporarily disable the Connect/Disconnect button
				::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT), FALSE);

				// Connect to the server
				if (0 != m_Client.Connect(sAddr.c_str(), RemotePort))
				{
					MessageBox(m_hWnd, _T("Failed to connect to server. Is it started?"), _T("Connect Failed"), MB_ICONWARNING);
					m_Client.Disconnect();
					::EnableWindow(GetDlgItem(IDC_BUTTON_CONNECT), TRUE);
					return;
				}
				m_Client.StartEvents();
				
			}
			break;

		case SOCK_DGRAM:
			{
				// Create the socket
				if (!m_Client.Create(SOCK_DGRAM))
				{
					::MessageBox(m_hWnd, _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING);
					return;
				}

				m_Client.StartEvents();

				//Update the dialog
				::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), TRUE);
				::EnableWindow(GetDlgItem(IDC_EDIT_SEND), TRUE);
				::EnableWindow(GetDlgItem(IDC_EDIT_PORT), FALSE);
				::EnableWindow(GetDlgItem(IDC_IPADDRESS1), FALSE);
				::EnableWindow(GetDlgItem(IDC_RADIO_TCP), FALSE);
				::EnableWindow(GetDlgItem(IDC_RADIO_UDP), FALSE);
				::SetWindowText(GetDlgItem(IDC_BUTTON_CONNECT), _T("Disconnect"));
				::SetWindowText(GetDlgItem(IDC_EDIT_STATUS), _T("Ready to Send"));
				::SendMessage(m_hWnd, WM_NEXTDLGCTL, (WPARAM)GetDlgItem(IDC_EDIT_SEND), TRUE);
				m_bClientConnected = TRUE;
			}
			break;
		}
	}
	else
	{
		// Shutdown and close the client socket
		m_Client.Disconnect();

		// Update the dialog
		::EnableWindow(GetDlgItem(IDC_BUTTON_SEND), FALSE);
		::EnableWindow(GetDlgItem(IDC_EDIT_SEND), FALSE);
		::EnableWindow(GetDlgItem(IDC_EDIT_PORT), TRUE);
		::EnableWindow(GetDlgItem(IDC_IPADDRESS1), TRUE);
		::EnableWindow(GetDlgItem(IDC_RADIO_TCP), TRUE);
		::EnableWindow(GetDlgItem(IDC_RADIO_UDP), TRUE);
		::SetWindowText(GetDlgItem(IDC_BUTTON_CONNECT), _T("Connect"));
		::SetWindowText(GetDlgItem(IDC_EDIT_STATUS), _T("Not Connected"));
		m_bClientConnected = FALSE;
	}
}

void CClientDialog::OnSend()
{
	switch (m_SocketType)
	{
	case SOCK_STREAM:	// for TCP client
		{
			tString t = GetDlgItemString(IDC_EDIT_SEND);
			std::string s = TCharToString(t.c_str());
			m_Client.Send(s.c_str(), s.length(), 0);
		}
		break;
	case SOCK_DGRAM:	// for UDP client
		{
			// Get the port number
			tString tPort = GetDlgItemString(IDC_EDIT_PORT);
			std::string sPort = TCharToString(tPort.c_str());
			int RemotePort = atoi(sPort.c_str());

			// Get the IP Address from the IP Address control
			DWORD dwAddr = 0;
			SendMessage(GetDlgItem(IDC_IPADDRESS1), IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );

			sockaddr_in peer = {0};
			peer.sin_family = AF_INET;
			peer.sin_port   = htons((u_short)RemotePort);
			peer.sin_addr.S_un.S_addr = htonl(dwAddr);

			tString t = GetDlgItemString(IDC_EDIT_SEND);
			std::string s = TCharToString(t.c_str());
			m_Client.SendTo(s.c_str(), s.length(), 0, (SOCKADDR*)&peer, sizeof(peer));
		}
		break;
	}
}


