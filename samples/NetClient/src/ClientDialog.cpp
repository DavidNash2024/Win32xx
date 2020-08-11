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

void CClientDialog::AppendText(const CEdit& edit, LPCTSTR text)
{
    // This function appends text to an edit control

    // Append Line Feed
    int length = edit.GetWindowTextLength();
    if (length > 0)
        edit.AppendText(_T("\r\n"));

    edit.AppendText(text);
}

INT_PTR CClientDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_ACTIVATE:       return OnActivate(msg, wparam, lparam);
    case USER_CONNECT:      return OnSocketConnect();
    case USER_DISCONNECT:   return OnSocketDisconnect();
    case USER_RECEIVE:      return OnSocketReceive();
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
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
    AppendText(m_editStatus, _T("Connected to server"));
    m_buttonConnect.SetWindowText( _T("Disconnect") );

    return TRUE;
}

// Called when the socket is disconnected from the server
BOOL CClientDialog::OnSocketDisconnect()
{
    m_isClientConnected = FALSE;
    m_buttonConnect.EnableWindow( TRUE );

    // Update the dialog
    AppendText(m_editStatus, _T("Disconnected from server"));
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
        if (WSAGetLastError() != WSAEWOULDBLOCK)
            AppendText(m_editStatus, _T("Receive failed."));

        return FALSE;
    }

    AppendText( m_editReceive, AtoT(bufArray) );
    TRACE("[Received:] "); TRACE(bufArray); TRACE("\n");

    return TRUE;
}

// This function adds support for the IP address control in the dialog.
void CClientDialog::LoadCommonControlsEx()
{
    HMODULE hModule = 0;

    try
    {
        // Load the Common Controls DLL
        hModule = ::LoadLibrary(_T("COMCTL32.DLL"));
        if (!hModule)
            throw CWinException(_T("Failed to load COMCTL32.DLL"));

        if (GetComCtlVersion() > 470)
        {
            // Declare a pointer to the InItCommonControlsEx function
            typedef BOOL WINAPI INIT_EX(INITCOMMONCONTROLSEX*);
            INIT_EX* pfnInit = (INIT_EX*)::GetProcAddress(hModule, "InitCommonControlsEx");

            // Call InitCommonControlsEx
            INITCOMMONCONTROLSEX initStruct;
            initStruct.dwSize = sizeof(INITCOMMONCONTROLSEX);
            initStruct.dwICC = ICC_INTERNET_CLASSES;
            if((!(*pfnInit)(&initStruct)))
                throw CWinException(_T("InitCommonControlsEx failed"));
        }
        else
        {
            ::MessageBox(NULL, _T("IP Address Control not supported!"), _T("Error"), MB_OK);
        }

        ::FreeLibrary(hModule);
    }

    catch (const CWinException &e)
    {
        e.what();
        if (hModule)
            ::FreeLibrary(hModule);
    }
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
    MoveWindow( rc.left+14, rc.top+14, rc.Width(), rc.Height(), TRUE);

    // Attach CWnd objects to the dialog's children
    m_ip4Address.AttachDlgItem( IDC_IPADDRESS, *this );
    m_editIP6Address.AttachDlgItem( IDC_EDIT_IPV6ADDRESS, *this );
    m_editStatus.AttachDlgItem( IDC_EDIT_STATUS, *this );
    m_editPort.AttachDlgItem( IDC_EDIT_PORT, *this );
    m_editSend.AttachDlgItem( IDC_EDIT_SEND, *this );
    m_editReceive.AttachDlgItem( IDC_EDIT_RECEIVE, *this );
    m_buttonConnect.AttachDlgItem( IDC_BUTTON_CONNECT, *this );
    m_buttonSend.AttachDlgItem( IDC_BUTTON_SEND, *this );
    m_radioIP4.AttachDlgItem( IDC_RADIO_IPV4, *this );
    m_radioIP6.AttachDlgItem( IDC_RADIO_IPV6, *this );
    m_radioTCP.AttachDlgItem( IDC_RADIO_TCP, *this );
    m_radioUDP.AttachDlgItem( IDC_RADIO_UDP, *this );

    // Set the initial state of the dialog
    m_editIP6Address.SetWindowText( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
    m_radioIP4.SetCheck(BST_CHECKED);
    AppendText(m_editStatus, _T("Not Connected"));
    m_editPort.SetWindowText( _T("3000") );
    m_radioTCP.SetCheck(BST_CHECKED);
    m_ip4Address.SetAddress( MAKEIPADDRESS(127, 0, 0, 1));

    if (!m_client.IsIPV6Supported())
    {
        m_radioIP6.EnableWindow(FALSE);
        m_editIP6Address.EnableWindow(FALSE);
    }

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
                    AppendText(m_editStatus, m_client.GetErrorString());
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
                    strAddr = m_ip4Address.GetAddress();
                }

                // Retrieve the local port number
                UINT port = GetDlgItemInt(m_editPort.GetDlgCtrlID(), FALSE);

                // Temporarily disable the Connect/Disconnect button
                m_buttonConnect.EnableWindow(FALSE);

                // Connect to the server
                if (0 != m_client.Connect(strAddr, port) )
                {
                    AppendText(m_editStatus, m_client.GetErrorString());
                    MessageBox( _T("Failed to connect to server. Is it started?"), _T("Connect Failed"), MB_ICONWARNING );
                    m_client.Disconnect();
                    m_buttonConnect.EnableWindow( TRUE );
                    return;
                }
                m_client.StartEvents();

            }
            break;

        case SOCK_DGRAM:
            {
                // Create the socket
                if (!m_client.Create(IPfamily, SOCK_DGRAM))
                {
                    AppendText(m_editStatus, m_client.GetErrorString());
                    MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
                    return;
                }

                m_client.StartEvents();

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
                AppendText(m_editStatus, _T("Connected, ready to send"));
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
        AppendText(m_editStatus, _T("Not Connected"));

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
                    AppendText(m_editStatus, _T("Send Failed"));
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
                strAddr = m_ip4Address.GetAddress();
            }

            if (SOCKET_ERROR == m_client.SendTo( TtoA(strSend), strSend.GetLength(), 0, strAddr, port ))
                if (WSAGetLastError() != WSAEWOULDBLOCK)
                    AppendText(m_editStatus, _T("SendTo Failed"));
        }
        break;
    }
}

