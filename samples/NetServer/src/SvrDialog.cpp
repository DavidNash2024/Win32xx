/////////////////////////////
// SvrDialog.cpp
//

#include "stdafx.h"
#include "SvrDialog.h"
#include "DialogApp.h"
#include "resource.h"


/////////////////////////////////////
// CTCPClientDlg function definitions
//

// Constructor.
CTCPClientDlg::CTCPClientDlg(UINT resID) : CDialog(resID), m_pSocket(0)
{
}

// Appends text to the specified edit control.
void CTCPClientDlg::AppendText(int id, LPCTSTR text)
{
    // This function appends text to an edit control.
    CWnd* pWnd = GetCWndPtr(::GetDlgItem(*this, id));
    assert(dynamic_cast<CEdit*>(pWnd));
    const CEdit& edit = static_cast<const CEdit&>(*pWnd);

    // Append Line Feed.
    int length = edit.GetWindowTextLength();
    if (length > 0)
        edit.AppendText(_T("\r\n"));

    edit.AppendText(text);
    TRACE(text); TRACE("\n");
}

// Called when the dialog is closed.
void CTCPClientDlg::OnClose()
{
    // Disconnect the socket when the user closes this chat dialog
    m_pSocket->Disconnect();
}

// // Respond to the various dialog buttons.
BOOL CTCPClientDlg::OnCommand(WPARAM wparam, LPARAM)
{
    switch (LOWORD(wparam))
    {
    case IDC_BUTTON_SEND2:
        Send();
        return TRUE;
    }

    return FALSE;
}

// Called when the dialog window is destroyed.
void CTCPClientDlg::OnDestroy()
{
    CSvrDialog& dialog = GetDlgApp()->GetDialog();
    dialog.PostMessage(USER_DISCONNECT, reinterpret_cast<WPARAM>(m_pSocket.get()), 0);
}

// Called before the dialog is displayed.
BOOL CTCPClientDlg::OnInitDialog()
{
    SetForegroundWindow();

    m_editSend.AttachDlgItem(IDC_EDIT_SEND2, *this);
    m_editReceive.AttachDlgItem(IDC_EDIT_RECEIVE2, *this);
    m_buttonSend.AttachDlgItem(IDC_BUTTON_SEND2, *this);

    return TRUE;
}

// Receives data from the socket.
void CTCPClientDlg::Receive()
{
    std::vector<char> bufVector( 1025, '\0' );
    char* bufArray = &bufVector.front(); // char array with 1025 elements initialized to '\0'
    if (m_pSocket->Receive(bufArray, 1024, 0) == SOCKET_ERROR)
        TRACE(_T("Network error.  Failed to receive"));

    AppendText(IDC_EDIT_RECEIVE2, AtoT(bufArray));
    TRACE("[Received:] "); TRACE(bufArray); TRACE("\n");
}

// Sends data to the socket.
void CTCPClientDlg::Send()
{
    CString text = m_editSend.GetWindowText();
    if (m_pSocket->Send(TtoA(text), lstrlen(text), 0) == SOCKET_ERROR)
    {
        if (GetLastError() != WSAEWOULDBLOCK)
            TRACE(_T("Network error.  Failed to send"));
    }
}

//////////////////////////////////
// CSvrDialog function definitions
//

// Constructor.
CSvrDialog::CSvrDialog(int resID) : CDialog(resID), m_isServerStarted(false),
                                      m_socketType(SOCK_STREAM)
{
    ZeroMemory(&m_saUDPClient, sizeof(m_saUDPClient));

    // Add support for the IP Address control
    // It requires Win95 with IE4 integrated or a later version of Windows OS.
    LoadCommonControlsEx();
}

// Destructor.
CSvrDialog::~CSvrDialog()
{
    StopServer();
}

// Appends text to the specified edit control.
void CSvrDialog::AppendText(const CEdit& edit, LPCTSTR text)
{
    // This function appends text to an edit control

    // Append Line Feed
    int length = edit.GetWindowTextLength();
    if (length > 0)
        edit.AppendText(_T("\r\n"));

    edit.AppendText(text);
    TRACE(text); TRACE("\n");
}

// Respond to the user defined message posted to the dialog.
INT_PTR CSvrDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case USER_ACCEPT:       return OnSocketAccept();
        case USER_DISCONNECT:   return OnSocketDisconnect(wparam);
        case USER_RECEIVE:      return OnSocketReceive(wparam);
        }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Adds support for the IP address control in the dialog.
void CSvrDialog::LoadCommonControlsEx()
{
    HMODULE module = 0;

    try
    {
        // Load the Common Controls DLL
        module = ::LoadLibrary(_T("COMCTL32.DLL"));
        if (module == 0)
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
            if((!(*pfnInit)(&initStruct)))
                throw CWinException(_T("InitCommonControlsEx failed"));
        }
        else
        {
            ::MessageBox(0, _T("IP Address Control not supported!"), _T("Error"), MB_OK);
        }

        ::FreeLibrary(module);
    }

    catch (const CWinException &e)
    {
        e.what();
        if (module != 0)
            ::FreeLibrary(module);
    }
}

// Called when the dialog window is about to be closed.
void CSvrDialog::OnClose()
{
    // Disconnect before shutting down the dialog.
    m_mainSocket.Disconnect();
    PostQuitMessage(0);
}

// Respond to the various dialog buttons.
BOOL CSvrDialog::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDC_BUTTON_START:      return OnStartServer();
    case IDC_BUTTON_SEND:       return OnSend();
    } //switch (LOWORD(wparam))

    return FALSE;
}

// Called before the dialog is displayed.
BOOL CSvrDialog::OnInitDialog()
{
    // Set the Icon.
    SetIconLarge( IDW_MAIN );
    SetIconSmall( IDW_MAIN );

    // reposition dialog.
    CRect rc = GetWindowRect();
    MoveWindow( rc.left-14, rc.top-14, rc.Width(), rc.Height(), TRUE );

    // Attach CWnd objects to the dialog's children.
    m_ip4Address.AttachDlgItem( IDC_IPADDRESS, *this );
    m_editIP6Address.AttachDlgItem( IDC_EDIT_IPV6ADDRESS, *this );
    m_editStatus.AttachDlgItem( IDC_EDIT_STATUS, *this );
    m_editPort.AttachDlgItem( IDC_EDIT_PORT, *this );
    m_editSend.AttachDlgItem( IDC_EDIT_SEND, *this );
    m_editReceive.AttachDlgItem( IDC_EDIT_RECEIVE, *this );
    m_buttonStart.AttachDlgItem( IDC_BUTTON_START, *this );
    m_buttonSend.AttachDlgItem( IDC_BUTTON_SEND, *this );
    m_radioIP4.AttachDlgItem( IDC_RADIO_IPV4, *this );
    m_radioIP6.AttachDlgItem( IDC_RADIO_IPV6, *this );
    m_radioTCP.AttachDlgItem( IDC_RADIO_TCP, *this );
    m_radioUDP.AttachDlgItem( IDC_RADIO_UDP, *this );

    // Set the initial state of the dialog.
    m_editIP6Address.SetWindowText( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
    m_radioIP4.SetCheck(BST_CHECKED);
    m_ip4Address.SetAddress(MAKEIPADDRESS(127, 0, 0, 1));
    AppendText(m_editStatus, _T("Server Stopped"));
    m_editPort.SetWindowText( _T("3000") );
    m_radioTCP.SetCheck(BST_CHECKED);
    if (!m_mainSocket.IsIPV6Supported())
    {
        m_radioIP6.EnableWindow(FALSE);
        m_editIP6Address.EnableWindow(FALSE);
    }

    return TRUE;
}

// Respond to the Start/Stop Button press.
BOOL CSvrDialog::OnStartServer()
{
    TRACE("Start/Stop Button Pressed\n");

    if (!m_isServerStarted)
    {
        // Attempt to start the server
        if (!StartServer())
            return FALSE;

        // Update the dialog
        m_buttonStart.SetWindowText( _T("Stop Server") );
        m_ip4Address.EnableWindow(FALSE);
        m_editIP6Address.EnableWindow(FALSE);
        m_editPort.EnableWindow(FALSE);
        m_radioIP4.EnableWindow(FALSE);
        m_radioIP6.EnableWindow(FALSE);
        m_radioTCP.EnableWindow(FALSE);
        m_radioUDP.EnableWindow(FALSE);


        if (m_socketType == SOCK_STREAM)
        {
            AppendText(m_editStatus, _T("TCP Server Started"));
            AppendText(m_editStatus, _T("Waiting for client ..."));
        }
        else
        {
            AppendText(m_editStatus, _T("UDP Server Started"));
            AppendText(m_editStatus, _T("Waiting for client data"));
        }
    }
    else
    {
        StopServer();

        // Update the dialog
        AppendText(m_editStatus, _T("Server Stopped"));
        m_buttonStart.SetWindowText( _T("Start Server") );
        m_ip4Address.EnableWindow(TRUE);
        m_editPort.EnableWindow(TRUE);
        m_radioIP4.EnableWindow(TRUE);
        m_radioTCP.EnableWindow(TRUE);
        m_radioUDP.EnableWindow(TRUE);
        m_buttonSend.EnableWindow(FALSE);
        m_editSend.EnableWindow(FALSE);
        if (m_mainSocket.IsIPV6Supported())
        {
            m_radioIP6.EnableWindow(TRUE);
            m_editIP6Address.EnableWindow(TRUE);
        }
    }
    m_isServerStarted = !m_isServerStarted;

    return m_isServerStarted;
}

// Responds to the send button.
BOOL CSvrDialog::OnSend()
{
    switch(m_socketType)
    {
        case SOCK_STREAM:
            // TCP connections have a separate chat dialog for sending/receiving data
            break;
        case SOCK_DGRAM:
            {
                CString sSend = m_editSend.GetWindowText();
                if (m_mainSocket.SendTo(TtoA(sSend), (int)strlen(TtoA(sSend)), 0, (LPSOCKADDR)&m_saUDPClient, (int)sizeof(m_saUDPClient)) == SOCKET_ERROR)
                {
                    if (GetLastError() != WSAEWOULDBLOCK)
                    {
                        TRACE(_T("Network error.  Failed to send"));
                        return FALSE;
                    }
                }
            }
            break;
    }

    return TRUE;
}

// Accept the connection from the client.
BOOL CSvrDialog::OnSocketAccept()
{
    ServerSocketPtr pClient(new CWorkerSocket);
    m_mainSocket.Accept(*pClient, NULL, NULL);
    if (INVALID_SOCKET == m_mainSocket.GetSocket())
    {
        TRACE("Failed to accept connection from client\n");
        TRACE(m_mainSocket.GetErrorString());
        return FALSE;
    }

    pClient->StartEvents();

    // Create the new chat dialog.
    TCPClientDlgPtr pDialog(new CTCPClientDlg(IDD_CHAT));
    pDialog->ServerSocket() = pClient;
    pDialog->DoModeless(*this);

    // Reposition the chat dialog.
    CRect rc = pDialog->GetWindowRect();
    int offset = 4 * (static_cast<int>(m_connectedClients.size()) - 1);
    pDialog->MoveWindow(rc.left + offset, rc.top + offset + 80, rc.Width(), rc.Height(), TRUE);
    pDialog->ShowWindow();

    // Add the socket and dialog to the map.
    m_connectedClients.insert(std::make_pair(pClient, pDialog));

    // Update the dialog.
    AppendText(m_editStatus, _T("Client Connected"));

    return TRUE;
}

// Called when the socket is disconnected.
BOOL CSvrDialog::OnSocketDisconnect(WPARAM wParam)
{
    CWorkerSocket* pClient = reinterpret_cast<CWorkerSocket*>(wParam);
    AppendText(m_editStatus, _T("Client disconnected"));

    // Iterator for our CWorkerSocket map.
    std::map< ServerSocketPtr, TCPClientDlgPtr >::iterator iter;

    for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
    {
        if (iter->first.get() == pClient)
            break;
    }

    // Delete the CWorkerSocket, and remove its pointer.
    if (iter != m_connectedClients.end())
    {
        m_connectedClients.erase(iter);
    }

    return TRUE;
}

// Called when the socket receives data.
BOOL CSvrDialog::OnSocketReceive(WPARAM wparam)
{
    CWorkerSocket* pClient = reinterpret_cast<CWorkerSocket*>(wparam);
    std::vector<char> vChar(1025, '\0');
    char* bufArray = &vChar.front(); // char array with 1025 elements

    switch (m_socketType)
    {
    case SOCK_STREAM:
        {
            // Pass this on to the TCP chat dialog
            std::map< ServerSocketPtr, TCPClientDlgPtr >::iterator Iter;

            for (Iter = m_connectedClients.begin(); Iter != m_connectedClients.end(); ++Iter)
            {
                if (Iter->first.get() == pClient)
                break;
            }

            if (Iter !=  m_connectedClients.end() )
                Iter->second->Receive();
        }
        break;
    case SOCK_DGRAM:
        {
            int addrlen = sizeof(m_saUDPClient);

            // Receive data and update the UDP client socket address.
            if (m_mainSocket.ReceiveFrom(bufArray, 1024, 0, (LPSOCKADDR)&m_saUDPClient, &addrlen) == SOCKET_ERROR)
            {
                TRACE(_T("Network error.  Failed to receive"));
                return FALSE;
            }
            else
                TRACE("[Received:] "); TRACE(bufArray); TRACE("\n");

            m_buttonSend.EnableWindow(TRUE);
            m_editSend.EnableWindow(TRUE);
            GotoDlgCtrl(GetDlgItem(IDC_EDIT_SEND));
        }
        break;
    }
    AppendText(m_editReceive, AtoT(bufArray));

    return TRUE;
}

// Start the server.
BOOL CSvrDialog::StartServer()
{
    LRESULT check = m_radioTCP.GetCheck();
    m_socketType = (check == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

    // Create the main socket
    check = m_radioIP4.GetCheck();
    int IPfamily = (check == BST_CHECKED)? PF_INET : PF_INET6 ;
    if (!m_mainSocket.Create(IPfamily, m_socketType))
    {
        AppendText(m_editStatus, _T("Create Socket Failed"));
        AppendText(m_editStatus, m_mainSocket.GetErrorString());
        return FALSE;
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

    // Bind to the socket
    AppendText(m_editStatus, _T("Binding to socket"));
    CString str;
    str.Format( _T("Addr %s, Port %u, type %s"), strAddr.c_str(), port, (m_socketType == SOCK_STREAM)?_T("TCP"):_T("UDP") );
    AppendText(m_editStatus, str);

    int RetVal = m_mainSocket.Bind( strAddr, port );
    if ( RetVal != 0 )
    {
        AppendText(m_editStatus, _T("Bind failed"));
        AppendText(m_editStatus, m_mainSocket.GetErrorString());
        return FALSE;
    }

    if (m_socketType == SOCK_STREAM)
    {
        // Listen for connections from clients (TCP server only)
        if  (m_mainSocket.Listen() == SOCKET_ERROR)
        {
            AppendText(m_editStatus, _T("Error listening on socket"));
            AppendText(m_editStatus, m_mainSocket.GetErrorString());
            return FALSE;
        }
    }

    m_mainSocket.StartEvents();

    return TRUE;
}

// Stop the server.
void CSvrDialog::StopServer()
{
    m_mainSocket.Disconnect();
    m_connectedClients.clear();
}
