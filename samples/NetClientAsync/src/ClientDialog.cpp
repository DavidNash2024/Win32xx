///////////////////////////////////////
// ClientDialog.cpp

#include "stdafx.h"
#include "ClientDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CClientDialog::CClientDialog(UINT resID) : CDialog(resID), 
                   m_isClientConnected(FALSE), m_socketType(SOCK_STREAM)
{
    // Add support for the IP Address control
    // It requires Win95 with IE4 intergrated or a later version of Windows OS.
    LoadCommonControlsEx();
}

CClientDialog::~CClientDialog()
{
}

void CClientDialog::AppendText(int id, LPCTSTR buf)
{
    // This function appends text to an edit control

    // Append Line Feed
    LRESULT ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {
        SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(id, EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
    }

    // Append text
    ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(id, EM_REPLACESEL, 0, (LPARAM) buf);
}

INT_PTR CClientDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_ACTIVATE:       return OnActivate(msg, wparam, lparam);
        case UWM_SOCKETMSG:
        {
            if (WSAGETSELECTERROR(lparam))
            {
                // Display the error and close the socket
                AppendText(IDC_EDIT_STATUS, _T("Socket error, closing socket."));
                closesocket(static_cast<SOCKET>(wparam));
            }
            else
            {
                switch (WSAGETSELECTEVENT(lparam))
                {
                case FD_CONNECT:    return OnSocketConnect();
                case FD_CLOSE:      return OnSocketDisconnect();
                case FD_READ:       return OnSocketReceive();
                default:            return 0;
                }
            }
        }
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

// This function adds support for the IP address control in the dialog.
void CClientDialog::LoadCommonControlsEx()
{
    HMODULE module = 0;

    try
    {
        // Load the Common Controls DLL
        module = ::LoadLibrary(_T("COMCTL32.DLL"));
        if (!module)
            throw CWinException(_T("Failed to load COMCTL32.DLL"));

        if (GetComCtlVersion() > 470)
        {
            // Declare a pointer to the InItCommonControlsEx function
            typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
            INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(module, "InitCommonControlsEx");

            // Call InitCommonControlsEx
            INITCOMMONCONTROLSEX initStruct;
            initStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
            initStruct.dwICC = ICC_INTERNET_CLASSES;
            if ((!(*pfnInit)(&initStruct)))
                throw CWinException(_T("InitCommonControlsEx failed"));
        }
        else
        {
            ::MessageBox(NULL, _T("IP Address Control not supported!"), _T("Error"), MB_OK);
        }

        ::FreeLibrary(module);
    }

    catch (const CWinException &e)
    {
        e.what();
        if (module)
            ::FreeLibrary(module);
    }
}

LRESULT CClientDialog::OnActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    // Give focus to the Send Edit box
    if (m_editSend.IsWindow())
        GotoDlgCtrl(m_editSend);

    return TRUE;
}

// Called when the dialog window is about to be closed
void CClientDialog::OnClose()
{
    // Disconnect before shutting down the dialog
    m_client.Disconnect();
    PostQuitMessage(0);
}

// Respond to the dialog buttons
BOOL CClientDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON_CONNECT:
        OnStartClient();
        return TRUE;
    case IDC_BUTTON_SEND:
        OnSend();
        // Give keyboard focus to the Send edit box
        GotoDlgCtrl(m_editSend);
        return TRUE;
    }

    return FALSE;
}

BOOL CClientDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // reposition dialog
    CRect rc = GetWindowRect();
    MoveWindow(rc.left + 14, rc.top + 14, rc.Width(), rc.Height(), TRUE);

    // Attach CWnd objects to the dialog's children
    m_ip4Address.AttachDlgItem(IDC_IPADDRESS, *this);
    m_editIP6Address.AttachDlgItem(IDC_EDIT_IPV6ADDRESS, *this);
    m_editStatus.AttachDlgItem(IDC_EDIT_STATUS, *this);
    m_editPort.AttachDlgItem(IDC_EDIT_PORT, *this);
    m_editSend.AttachDlgItem(IDC_EDIT_SEND, *this);
    m_editReceive.AttachDlgItem(IDC_EDIT_RECEIVE, *this);
    m_buttonConnect.AttachDlgItem(IDC_BUTTON_CONNECT, *this);
    m_buttonSend.AttachDlgItem(IDC_BUTTON_SEND, *this);
    m_radioIP4.AttachDlgItem(IDC_RADIO_IPV4, *this);
    m_radioIP6.AttachDlgItem(IDC_RADIO_IPV6, *this);
    m_radioTCP.AttachDlgItem(IDC_RADIO_TCP, *this);
    m_radioUDP.AttachDlgItem(IDC_RADIO_UDP, *this);

    // Set the initial state of the dialog
    m_editIP6Address.SetWindowText(_T("0000:0000:0000:0000:0000:0000:0000:0001"));
    m_radioIP4.SetCheck(BST_CHECKED);
    AppendText(IDC_EDIT_STATUS, _T("Not Connected"));
    m_editPort.SetWindowText(_T("3000"));
    m_radioTCP.SetCheck(BST_CHECKED);
    m_ip4Address.SetAddress(MAKEIPADDRESS(127, 0, 0, 1));

    if (!m_client.IsIPV6Supported())
    {
        m_radioIP6.EnableWindow(FALSE);
        m_editIP6Address.EnableWindow(FALSE);
    }

    return TRUE;
}

// Called when the connection to the server is established
BOOL CClientDialog::OnSocketConnect()
{
    m_isClientConnected = TRUE;
    m_buttonConnect.EnableWindow( TRUE );

    // Move focus to the Send Edit box
    GotoDlgCtrl(m_editSend);
    SetForegroundWindow();

    // Update the dialog
    m_ip4Address.EnableWindow( FALSE );
    m_editIP6Address.EnableWindow( FALSE );
    m_buttonSend.EnableWindow( TRUE );
    m_editSend.EnableWindow( TRUE );
    m_editPort.EnableWindow( FALSE );
    m_radioIP4.EnableWindow( FALSE );
    m_radioIP6.EnableWindow( FALSE );
    m_radioTCP.EnableWindow( FALSE );
    m_radioUDP.EnableWindow( FALSE );
    AppendText(IDC_EDIT_STATUS, _T("Connected to server"));
    m_buttonConnect.SetWindowText( _T("Disconnect") );

    return TRUE;
}

// Called when the socket is disconnected from the server
BOOL CClientDialog::OnSocketDisconnect()
{
    m_isClientConnected = FALSE;
    m_buttonConnect.EnableWindow( TRUE );

    // Update the dialog
    AppendText(IDC_EDIT_STATUS, _T("Disconnected from server"));
    m_buttonSend.EnableWindow( FALSE );
    m_editSend.EnableWindow( FALSE );
    m_editPort.EnableWindow( TRUE );
    m_ip4Address.EnableWindow( TRUE );
    m_radioTCP.EnableWindow( TRUE );
    m_radioUDP.EnableWindow( TRUE );
    m_radioIP4.EnableWindow( TRUE );
    m_buttonConnect.SetWindowText( _T("Connect") );
    if ( m_client.IsIPV6Supported() )
    {
        m_editIP6Address.EnableWindow( TRUE );
        m_radioIP6.EnableWindow( TRUE );
    }

    return TRUE;
}

// Called when the socket has data to receive
BOOL CClientDialog::OnSocketReceive()
{
    std::vector<CHAR> bufVector( 1025, '\0' );
    CHAR* bufArray = &bufVector.front(); // CHAR array with 1025 elements initialised to '\0'
    if (m_client.Receive(bufArray, 1024, 0 ) == SOCKET_ERROR)
    {
        AppendText( IDC_EDIT_STATUS, _T("Receive failed.") );
        return FALSE;
    }

    AppendText( IDC_EDIT_RECEIVE, AtoT(bufArray) );
    TRACE("[Received:] "); TRACE(bufArray); TRACE("\n");
    
    return TRUE;
}

void CClientDialog::OnStartClient()
{
    TRACE("Connect/Disconnect Button Pressed\n");

    LRESULT check = m_radioTCP.GetCheck();
    m_socketType = (check == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

    check = m_radioIP4.GetCheck();
    int IPfamily = (check == BST_CHECKED)? PF_INET : PF_INET6 ;

    if (!m_isClientConnected)
    {
        switch(m_socketType)
        {
        case SOCK_STREAM:
            {
                // Create the socket

                if (!m_client.Create(IPfamily, SOCK_STREAM))
                {
                    AppendText(IDC_EDIT_STATUS, m_client.GetLastError());
                    MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
                    return;
                }

                // Retrieve the IP Address
                CString strAddr;
                if (PF_INET6 == IPfamily)
                {
                    strAddr = m_editIP6Address.GetWindowText();
                }
                else
                {
                    DWORD dwAddr = 0;
                    m_ip4Address.GetAddress(dwAddr);
                    in_addr addr;
                    ZeroMemory(&addr, sizeof(in_addr));
                    addr.S_un.S_addr = htonl(dwAddr);
                    strAddr = AtoT( inet_ntoa(addr) );
                }

                // Retrieve the local port number
                UINT port = GetDlgItemInt(m_editPort.GetDlgCtrlID(), FALSE);

                // Temporarily disable the Connect/Disconnect button
                m_buttonConnect.EnableWindow(FALSE);

                // Connect to the server
                if (0 != m_client.Connect(strAddr, port) )
                {
                    AppendText(IDC_EDIT_STATUS, m_client.GetLastError());
                    MessageBox( _T("Failed to connect to server. Is it started?"), _T("Connect Failed"), MB_ICONWARNING );
                    m_client.Disconnect();
                    m_buttonConnect.EnableWindow( TRUE );
                    return;
                }

                long events = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE;
                m_client.StartAsync(*this, UWM_SOCKETMSG, events);
                OnSocketConnect();

            }
            break;

        case SOCK_DGRAM:
            {
                // Create the socket
                if (!m_client.Create(IPfamily, SOCK_DGRAM))
                {
                    AppendText(IDC_EDIT_STATUS, m_client.GetLastError());
                    MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
                    return;
                }

                long events = FD_READ | FD_WRITE | FD_OOB | FD_ACCEPT | FD_CONNECT | FD_CLOSE;
                m_client.StartAsync(*this, UWM_SOCKETMSG, events);

                //Update the dialog
                m_ip4Address.EnableWindow( FALSE );
                m_editIP6Address.EnableWindow( FALSE );
                m_buttonSend.EnableWindow( TRUE );
                m_editSend.EnableWindow( TRUE );
                m_editPort.EnableWindow( FALSE );
                m_radioIP4.EnableWindow( FALSE );
                m_radioIP6.EnableWindow( FALSE );
                m_radioTCP.EnableWindow( FALSE );
                m_radioUDP.EnableWindow( FALSE );
                m_buttonConnect.SetWindowText( _T("Disconnect") );
                AppendText(IDC_EDIT_STATUS, _T("Connected, ready to send"));
                GotoDlgCtrl(m_editSend);
                m_isClientConnected = TRUE;
            }
            break;
        }
    }
    else
    {
        // Shutdown and close the client socket
        m_client.Disconnect();

        // Update the dialog
        m_ip4Address.EnableWindow( TRUE );
        m_buttonSend.EnableWindow( FALSE );
        m_editSend.EnableWindow( FALSE );
        m_editPort.EnableWindow( TRUE );
        m_radioIP4.EnableWindow( TRUE );
        m_radioTCP.EnableWindow( TRUE );
        m_radioUDP.EnableWindow( TRUE );
        m_buttonConnect.SetWindowText( _T("Connect") );
        AppendText(IDC_EDIT_STATUS, _T("Not Connected"));

        if (m_client.IsIPV6Supported())
        {
            m_radioIP6.EnableWindow(TRUE);
            m_editIP6Address.EnableWindow(TRUE);
        }
        m_isClientConnected = FALSE;
    }
}

void CClientDialog::OnSend()
{
    switch (m_socketType)
    {
    case SOCK_STREAM:   // for TCP client
        {
            CString sSend = GetDlgItemText(IDC_EDIT_SEND);
            if (SOCKET_ERROR == m_client.Send(TtoA(sSend), sSend.GetLength(), 0))
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                    AppendText(IDC_EDIT_STATUS, _T("Send Failed"));
        }
        break;
    case SOCK_DGRAM:    // for UDP client
        {
            LRESULT check = m_radioIP4.GetCheck();
            int IPfamily = (check == BST_CHECKED)? PF_INET : PF_INET6 ;

            UINT port = GetDlgItemInt(m_editPort.GetDlgCtrlID(), FALSE);
            CString strSend = m_editSend.GetWindowText();

            // Retrieve the IP Address
            CString strAddr;
            if (PF_INET6 == IPfamily)
            {
                strAddr = m_editIP6Address.GetWindowText();
            }
            else
            {
                DWORD dwAddr = 0;
                m_ip4Address.GetAddress(dwAddr);
                in_addr addr;
                ZeroMemory(&addr, sizeof(in_addr));
                addr.S_un.S_addr = htonl(dwAddr);
                strAddr = AtoT( inet_ntoa(addr) );
            }

            if (SOCKET_ERROR == m_client.SendTo( TtoA(strSend), strSend.GetLength(), 0, strAddr, port ))
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                    AppendText(IDC_EDIT_STATUS, _T("SendTo Failed"));
        }
        break;
    }
}

