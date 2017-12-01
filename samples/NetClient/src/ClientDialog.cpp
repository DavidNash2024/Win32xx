///////////////////////////////////////
// ClientDialog.cpp

#include "stdafx.h"
#include "ClientDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CClientDialog::CClientDialog(UINT nResID) : CDialog(nResID), 
                   m_IsClientConnected(FALSE), m_SocketType(SOCK_STREAM)
{
    // Add support for the IP Address control
    // It requires Win95 with IE4 intergrated or a later version of Windows OS.
    LoadCommonControlsEx();
}

CClientDialog::~CClientDialog()
{
}

void CClientDialog::AppendText(int nID, LPCTSTR buf)
{
    // This function appends text to an edit control

    // Append Line Feed
    LRESULT ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {
        SendDlgItemMessage(nID, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM) (_T("\r\n")));
    }

    // Append text
    ndx = SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(nID, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM) buf);
}

INT_PTR CClientDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:       return OnActivate(uMsg, wParam, lParam);
    case USER_CONNECT:      return OnSocketConnect();
    case USER_DISCONNECT:   return OnSocketDisconnect();
    case USER_RECEIVE:      return OnSocketReceive();
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(uMsg, wParam, lParam);
}

LRESULT CClientDialog::OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(uMsg);
    UNREFERENCED_PARAMETER(wParam);
    UNREFERENCED_PARAMETER(lParam);

    // Give focus to the Send Edit box
    if (m_EditSend.IsWindow())
        GotoDlgCtrl(m_EditSend);

    return TRUE;
}

void CClientDialog::OnDestroy()
{
    PostQuitMessage(0);
}

BOOL CClientDialog::OnSocketConnect()
{
    // Called when the connection to the server is established
    m_IsClientConnected = TRUE;
    m_ButtonConnect.EnableWindow( TRUE );

    // Move focus to the Send Edit box
    GotoDlgCtrl(m_EditSend);
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

    return TRUE;
}

BOOL CClientDialog::OnSocketDisconnect()
{
    // Called when the socket is disconnected from the server
    m_IsClientConnected = FALSE;
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

    return TRUE;
}

BOOL CClientDialog::OnSocketReceive()
{
    // Called when the socket has data to receive
    std::vector<CHAR> vChar( 1025, '\0' );
    CHAR* buf = &vChar.front(); // CHAR array with 1025 elements initialised to '\0'
    int size = m_Client.Receive( buf, 1024, 0 ); // receive at most 1024 chars
    if (SOCKET_ERROR == size)
    {
        AppendText( IDC_EDIT_STATUS, _T("Receive failed.") );
        return size;
    }

    AppendText( IDC_EDIT_RECEIVE, AtoT(buf) );
    
    return TRUE;
}

void CClientDialog::LoadCommonControlsEx()
{
    // This function adds support for the IP address control in the dialog.
    HMODULE hComCtl = 0;

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
            ::MessageBox(NULL, _T("IP Address Control not supported!"), _T("Error"), MB_OK);
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

BOOL CClientDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
    // Respond to the dialog buttons

    UNREFERENCED_PARAMETER(lParam);

    UINT nID = LOWORD(wParam);
    switch (nID)
    {
    case IDC_BUTTON_CONNECT:
        OnStartClient();
        return TRUE;
    case IDC_BUTTON_SEND:
        OnSend();
        // Give keyboard focus to the Send edit box
        GotoDlgCtrl(m_EditSend);
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
    m_IP4Address.AttachDlgItem( IDC_IPADDRESS, *this );
    m_EditIP6Address.AttachDlgItem( IDC_EDIT_IPV6ADDRESS, *this );
    m_EditStatus.AttachDlgItem( IDC_EDIT_STATUS, *this );
    m_EditPort.AttachDlgItem( IDC_EDIT_PORT, *this );
    m_EditSend.AttachDlgItem( IDC_EDIT_SEND, *this );
    m_EditReceive.AttachDlgItem( IDC_EDIT_RECEIVE, *this );
    m_ButtonConnect.AttachDlgItem( IDC_BUTTON_CONNECT, *this );
    m_ButtonSend.AttachDlgItem( IDC_BUTTON_SEND, *this );
    m_RadioIP4.AttachDlgItem( IDC_RADIO_IPV4, *this );
    m_RadioIP6.AttachDlgItem( IDC_RADIO_IPV6, *this );
    m_RadioTCP.AttachDlgItem( IDC_RADIO_TCP, *this );
    m_RadioUDP.AttachDlgItem( IDC_RADIO_UDP, *this );

    // Set the initial state of the dialog
    m_EditIP6Address.SetWindowText( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
    m_RadioIP4.SetCheck(BST_CHECKED);
    m_EditStatus.SetWindowText( _T("Not Connected") );
    m_EditPort.SetWindowText( _T("3000") );
    m_RadioTCP.SetCheck(BST_CHECKED);
    m_IP4Address.SetAddress( MAKEIPADDRESS(127, 0, 0, 1));

    if (!m_Client.IsIPV6Supported())
    {
        m_RadioIP6.EnableWindow(FALSE);
        m_EditIP6Address.EnableWindow(FALSE);
    }

    return TRUE;
}

void CClientDialog::OnStartClient()
{
    TRACE("Connect/Disconnect Button Pressed\n");

    LRESULT lr = m_RadioTCP.GetCheck();
    m_SocketType = (lr == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

    lr = m_RadioIP4.GetCheck();
    int IPfamily = (lr == BST_CHECKED)? PF_INET : PF_INET6 ;

    if (!m_IsClientConnected)
    {
        switch(m_SocketType)
        {
        case SOCK_STREAM:
            {
                // Create the socket

                if (!m_Client.Create(IPfamily, SOCK_STREAM))
                {
                    AppendText(IDC_EDIT_STATUS, m_Client.GetLastError());
                    MessageBox( _T("Failed to create Client socket"), _T("Connect Failed"), MB_ICONWARNING );
                    return;
                }

                // Retrieve the IP Address
                CString strAddr;
                if (PF_INET6 == IPfamily)
                {
                    strAddr = m_EditIP6Address.GetWindowText();
                }
                else
                {
                    DWORD dwAddr = 0;
                    m_IP4Address.GetAddress(dwAddr);
                    in_addr addr;
                    ZeroMemory(&addr, sizeof(in_addr));
                    addr.S_un.S_addr = htonl(dwAddr);
                    strAddr = AtoT( inet_ntoa(addr) );
                }

                // Retrieve the local port number
                UINT port = GetDlgItemInt(m_EditPort.GetDlgCtrlID(), FALSE);

                // Temporarily disable the Connect/Disconnect button
                m_ButtonConnect.EnableWindow(FALSE);

                // Connect to the server
                if (0 != m_Client.Connect(strAddr, port) )
                {
                    AppendText(IDC_EDIT_STATUS, m_Client.GetLastError());
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
                    AppendText(IDC_EDIT_STATUS, m_Client.GetLastError());
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
                GotoDlgCtrl(m_EditSend);
                m_IsClientConnected = TRUE;
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
        m_IsClientConnected = FALSE;
    }
}

void CClientDialog::OnSend()
{
    switch (m_SocketType)
    {
    case SOCK_STREAM:   // for TCP client
        {
            CString sSend = GetDlgItemText(IDC_EDIT_SEND);
            if (SOCKET_ERROR == m_Client.Send(TtoA(sSend), sSend.GetLength(), 0))
                m_EditStatus.SetWindowText( _T("Send Failed") );
        }
        break;
    case SOCK_DGRAM:    // for UDP client
        {
            LRESULT lr = m_RadioIP4.GetCheck();
            int IPfamily = (lr == BST_CHECKED)? PF_INET : PF_INET6 ;

            UINT port = GetDlgItemInt(m_EditPort.GetDlgCtrlID(), FALSE);
            CString strSend = m_EditSend.GetWindowText();

            // Retrieve the IP Address
            CString strAddr;
            if (PF_INET6 == IPfamily)
            {
                strAddr = m_EditIP6Address.GetWindowText();
            }
            else
            {
                DWORD dwAddr = 0;
                m_IP4Address.GetAddress(dwAddr);
                in_addr addr;
                ZeroMemory(&addr, sizeof(in_addr));
                addr.S_un.S_addr = htonl(dwAddr);
                strAddr = AtoT( inet_ntoa(addr) );
            }

            if (SOCKET_ERROR == m_Client.SendTo( TtoA(strSend), strSend.GetLength(), 0, strAddr, port ))
                m_EditStatus.SetWindowText( _T("SendTo Failed") );
        }
        break;
    }
}

