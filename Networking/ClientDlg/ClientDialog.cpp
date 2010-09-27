///////////////////////////////////////
// ClientDialog.cpp

#include "stdafx.h"
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

	// Append Line Feed
	int ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
	if (ndx)
	{
		SendDlgItemMessage(nID, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
	}

	// Append text
	ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
	SendDlgItemMessage(nID, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM) buf);
}

BOOL CClientDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		SendMessage(WM_NEXTDLGCTL, (WPARAM)(HWND)m_EditSend, TRUE);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CClientDialog::OnClientDisconnect()
{
	// Called when the socket is disconnected from the server
	m_bClientConnected = FALSE;
	m_ButtonConnect.EnableWindow( TRUE );

	// Update the dialog
	m_EditStatus.SetWindowText( _T("Disconnected from server") );
	m_ButtonSend.EnableWindow( FALSE );
	m_EditSend.EnableWindow( FALSE );
	m_EditPort.EnableWindow( TRUE );
	m_IP4Address.EnableWindow( TRUE );
	m_RadioTCP.EnableWindow( TRUE );
	m_RadioUDP.EnableWindow( TRUE );
	m_RadioIP4.EnableWindow( TRUE );
	m_ButtonConnect.SetWindowText( _T("Connect") );
	if ( m_Client.IsIPV6Supported() )
	{
		m_EditIP6Address.EnableWindow( TRUE );
		m_RadioIP6.EnableWindow( TRUE );
	}
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
			throw CWinException(_T("Failed to load COMCTL32.DLL"));

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
				throw CWinException(_T("InitCommonControlsEx failed"));
		}
		else
		{
			MessageBox( _T("Common Control Version 4.71 or later required (IE 4)"), _T("NOT SUPPORTED"), MB_ICONSTOP );
		}

		::FreeLibrary(hComCtl);
	}
	
	catch (const CWinException &e)
	{
		e.what();
		if (hComCtl)
			::FreeLibrary(hComCtl);
	}
}


void CClientDialog::OnClientConnect()
{
	// Called when the connection to the server is established
	m_bClientConnected = TRUE;
	m_ButtonConnect.EnableWindow( TRUE );

	// Move focus to the Send Edit box
	SendMessage(WM_NEXTDLGCTL, (WPARAM)(HWND)m_EditSend, TRUE);
	SetForegroundWindow();

	// Update the dialog
	m_IP4Address.EnableWindow( FALSE );
	m_EditIP6Address.EnableWindow( FALSE );
	m_ButtonSend.EnableWindow( TRUE );
	m_EditSend.EnableWindow( TRUE );
	m_EditPort.EnableWindow( FALSE );
	m_RadioIP4.EnableWindow( FALSE );
	m_RadioIP6.EnableWindow( FALSE );
	m_RadioTCP.EnableWindow( FALSE );
	m_RadioUDP.EnableWindow( FALSE );
	m_EditStatus.SetWindowText( _T("Connected to server") );
	m_ButtonConnect.SetWindowText( _T("Disconnect") );
}

int CClientDialog::OnClientReceive()
{
	// Called when the socket has data to receive
	std::vector<TCHAR> vTChar( 1025, _T('\0') );
	TCHAR* buf = &vTChar.front();	// TChar array with 1025 elements initialised to _T('\0') 
	int size = m_Client.Receive( buf, 1024, 0 ); // receive at most 1024 chars
	if (SOCKET_ERROR == size)
	{
		Append( IDC_EDIT_STATUS, _T("Receive failed.") );
		return size;
	}

	Append( IDC_EDIT_RECEIVE, buf );
	return size;
}

BOOL CClientDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Respond to the dialog buttons

	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTON_CONNECT:
		OnStartClient();
		return TRUE;
	case IDC_BUTTON_SEND:
		OnSend();
		// Give keyboard focus to the Send edit box
		SendMessage( WM_NEXTDLGCTL, (WPARAM)(HWND)m_EditSend, TRUE);
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
	CRect rc = GetWindowRect();
	MoveWindow( rc.left+14, rc.top+14, rc.Width(), rc.Height(), TRUE);

	// Attach CWnd objects to the dialog's children
	m_IP4Address.AttachDlgItem( IDC_IPADDRESS, this );
	m_EditIP6Address.AttachDlgItem( IDC_EDIT_IPV6ADDRESS, this );
	m_EditStatus.AttachDlgItem( IDC_EDIT_STATUS, this );
	m_EditPort.AttachDlgItem( IDC_EDIT_PORT, this );
	m_EditSend.AttachDlgItem( IDC_EDIT_SEND, this );
	m_EditReceive.AttachDlgItem( IDC_EDIT_RECEIVE, this );
	m_ButtonConnect.AttachDlgItem( IDC_BUTTON_CONNECT, this );
	m_ButtonSend.AttachDlgItem( IDC_BUTTON_SEND, this );
	m_RadioIP4.AttachDlgItem( IDC_RADIO_IPV4, this );
	m_RadioIP6.AttachDlgItem( IDC_RADIO_IPV6, this );
	m_RadioTCP.AttachDlgItem( IDC_RADIO_TCP, this );
	m_RadioUDP.AttachDlgItem( IDC_RADIO_UDP, this );

	// Set the initial state of the dialog
	m_EditIP6Address.SetWindowText( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
	m_RadioIP4.SendMessage( BM_SETCHECK, BST_CHECKED, 0 );
	m_EditStatus.SetWindowText( _T("Not Connected") );
	m_EditPort.SetWindowText( _T("3000") );
	m_RadioTCP.SendMessage( BM_SETCHECK, BST_CHECKED, 0 );
	m_IP4Address.SendMessage( IPM_SETADDRESS, 0, MAKEIPADDRESS(127, 0, 0, 1) );

	if (!m_Client.IsIPV6Supported())
	{
		m_RadioIP6.EnableWindow(FALSE);
		m_EditIP6Address.EnableWindow(FALSE);
	}

	return true;
}

void CClientDialog::OnStartClient()
{
	TRACE(_T("Connect/Disconnect Button Pressed\n"));

	LRESULT lr = m_RadioTCP.SendMessage( BM_GETCHECK, 0, 0 );
	m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

	lr = m_RadioIP4.SendMessage( BM_GETCHECK, 0, 0 );
	int IPfamily = (lr == BST_CHECKED)? PF_INET : PF_INET6 ;

	if (!m_bClientConnected)
	{
		switch(m_SocketType)
		{
		case SOCK_STREAM:
			{
				// Create the socket

				if (!m_Client.Create(IPfamily, SOCK_STREAM))
				{
					Append(IDC_EDIT_STATUS, m_Client.GetLastError());
					MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
					return;
				}

				// Retrieve the IP Address
				tString tAddr;
				if (PF_INET6 == IPfamily)
				{
					tAddr = m_EditIP6Address.GetWindowText();
				}
				else
				{
					DWORD dwAddr = 0;
					m_IP4Address.SendMessage( IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );
					in_addr addr = {0};
					addr.S_un.S_addr = htonl(dwAddr);
					tAddr = CharToTChar( inet_ntoa(addr) );
				}

				// Retrieve the local port number
				tString tPort = m_EditPort.GetWindowText();

				// Temporarily disable the Connect/Disconnect button
				m_ButtonConnect.EnableWindow( FALSE);

				// Connect to the server
				if (0 != m_Client.Connect(tAddr.c_str(), tPort.c_str()) )
				{
					Append(IDC_EDIT_STATUS, m_Client.GetLastError());
					MessageBox( _T("Failed to connect to server. Is it started?"), _T("Connect Failed"), MB_ICONWARNING );
					m_Client.Disconnect();
					m_ButtonConnect.EnableWindow( TRUE );
					return;
				}
				m_Client.StartEvents();
				
			}
			break;

		case SOCK_DGRAM:
			{
				// Create the socket
				if (!m_Client.Create(IPfamily, SOCK_DGRAM))
				{
					Append(IDC_EDIT_STATUS, m_Client.GetLastError());
					MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
					return;
				}

				m_Client.StartEvents();

				//Update the dialog
				m_IP4Address.EnableWindow( FALSE );
				m_EditIP6Address.EnableWindow( FALSE );
				m_ButtonSend.EnableWindow( TRUE );
				m_EditSend.EnableWindow( TRUE );
				m_EditPort.EnableWindow( FALSE );
				m_RadioIP4.EnableWindow( FALSE );
				m_RadioIP6.EnableWindow( FALSE );
				m_RadioTCP.EnableWindow( FALSE );
				m_RadioUDP.EnableWindow( FALSE );
				m_ButtonConnect.SetWindowText( _T("Disconnect") );
				m_EditStatus.SetWindowText( _T("Ready to Send") );
				SendMessage( WM_NEXTDLGCTL, (WPARAM)(HWND)m_EditSend, TRUE );
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
		m_IP4Address.EnableWindow( TRUE );
		m_ButtonSend.EnableWindow( FALSE );
		m_EditSend.EnableWindow( FALSE );
		m_EditPort.EnableWindow( TRUE );
		m_RadioIP4.EnableWindow( TRUE );
		m_RadioTCP.EnableWindow( TRUE );
		m_RadioUDP.EnableWindow( TRUE );
		m_ButtonConnect.SetWindowText( _T("Connect") );
		m_EditStatus.SetWindowText( _T("Not Connected") );

		if (m_Client.IsIPV6Supported())
		{
			m_RadioIP6.EnableWindow(TRUE);
			m_EditIP6Address.EnableWindow(TRUE);
		}
		m_bClientConnected = FALSE;
	}
}

void CClientDialog::OnSend()
{
	switch (m_SocketType)
	{
	case SOCK_STREAM:	// for TCP client
		{
			LPCTSTR szSend = GetDlgItemText(IDC_EDIT_SEND);
			m_Client.Send(szSend, lstrlen(szSend), 0);
		}
		break;
	case SOCK_DGRAM:	// for UDP client
		{
			LRESULT lr = m_RadioIP4.SendMessage( BM_GETCHECK, 0, 0 );
			int IPfamily = (lr == BST_CHECKED)? PF_INET : PF_INET6 ;

			tString tPort = m_EditPort.GetWindowText();
			tString tSend = m_EditSend.GetWindowText();
			
			// Retrieve the IP Address
			tString tAddr;
			if (PF_INET6 == IPfamily)
			{
				tAddr = m_EditIP6Address.GetWindowText();
			}
			else
			{
				DWORD dwAddr = 0;
				m_IP4Address.SendMessage( IPM_GETADDRESS, 0, (LPARAM) (LPDWORD) &dwAddr );
				in_addr addr = {0};
				addr.S_un.S_addr = htonl(dwAddr);
				tAddr = CharToTChar( inet_ntoa(addr) );
			}

			m_Client.SendTo( tSend.c_str(), lstrlen(tSend.c_str()), 0, tAddr.c_str(), tPort.c_str() );
		}
		break;
	}
}


