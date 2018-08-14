///////////////////////////////////////
// SvrDialog.cpp

#include "stdafx.h"
#include "SvrDialog.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CTCPClientDlg class
CTCPClientDlg::CTCPClientDlg(UINT resID) : CDialog(resID), m_pSocket(0)
{
}

// This function appends some text to an edit control
void CTCPClientDlg::AppendText(int id, LPCTSTR buf)
{
    // Append Line Feed
    LRESULT ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {

        SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(_T( "\r\n")));
    }

    // Append text
    ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(buf));
}

void CTCPClientDlg::OnClose()
{
    // Disconnect the socket when the user closes this chat dialog
    m_pSocket->Disconnect();
}

BOOL CTCPClientDlg::OnCommand(WPARAM wparam, LPARAM lparam)
{
    // Respond to the various dialog buttons

    UNREFERENCED_PARAMETER(lparam);

    switch (LOWORD(wparam))
    {
    case IDC_BUTTON_SEND2:
        Send();
        return TRUE;
    }

    return FALSE;
}

BOOL CTCPClientDlg::OnInitDialog()
{
    SetForegroundWindow();

    m_editSend.AttachDlgItem(IDC_EDIT_SEND2, *this);
    m_editReceive.AttachDlgItem(IDC_EDIT_RECEIVE2, *this);
    m_buttonSend.AttachDlgItem(IDC_BUTTON_SEND2, *this);

    return TRUE;
}

void CTCPClientDlg::Receive()
{
    std::vector<char> bufVector( 1025, '\0' );
    char* buf = &bufVector.front(); // char array with 1025 elements initialised to '\0'
    m_pSocket->Receive(buf, 1024, 0);
    AppendText(IDC_EDIT_RECEIVE2, AtoT(buf));
}

void CTCPClientDlg::Send()
{
    CString sSend = m_editSend.GetWindowText();
    m_pSocket->Send(TtoA(sSend), lstrlen(sSend), 0);
}



/////////////////////////////////////////////
// Definitions for the CSvrDialog class
CSvrDialog::CSvrDialog(UINT resID) : CDialog(resID), m_isServerStarted(FALSE), 
                                      m_socketType(SOCK_STREAM)
{
    ZeroMemory(&m_saUDPClient, sizeof(m_saUDPClient));

    // Add support for the IP Address control
    // It requires Win95 with IE4 intergrated or a later version of Windows OS.
    LoadCommonControlsEx();
}

CSvrDialog::~CSvrDialog()
{
    StopServer();
}

// This function appends some text to an edit control
void CSvrDialog::Append(int id, LPCTSTR buf)
{
    // Append Line Feed
    LRESULT ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    if (ndx)
    {

        SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
        SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(_T("\r\n")));
    }

    // Append text
    ndx = SendDlgItemMessage(id, WM_GETTEXTLENGTH, 0, 0);
    SendDlgItemMessage(id, EM_SETSEL, ndx, ndx);
    SendDlgItemMessage(id, EM_REPLACESEL, 0, reinterpret_cast<LPARAM>(buf));
}

// respond to the user defined message posted to the dialog
INT_PTR CSvrDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case USER_ACCEPT:       return OnSocketAccept();
    case USER_DISCONNECT:   return OnSocketDisconnect(wparam);
    case USER_RECEIVE:      return OnSocketReceive(wparam);
    }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

// This function adds support for the IP address control in the dialog.
void CSvrDialog::LoadCommonControlsEx()
{
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

        ::FreeLibrary(hComCtl);
    }

    catch (const CWinException &e)
    {
        e.what();
        if (hComCtl)
            ::FreeLibrary(hComCtl);
    }
}

// Called when the dialog window is about to be closed
void CSvrDialog::OnClose()
{
    // Disconnect before shutting down the dialog.
    m_mainSocket.Disconnect();
    PostQuitMessage(0);
}

BOOL CSvrDialog::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    // Respond to the various dialog buttons
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON_START:      return OnStartServer();
    case IDC_BUTTON_SEND:       return OnSend();
    } //switch (LOWORD(wparam))

    return FALSE;
}

BOOL CSvrDialog::OnInitDialog()
{
    // Set the Icon
    SetIconLarge( IDW_MAIN );
    SetIconSmall( IDW_MAIN );

    // reposition dialog
    CRect rc = GetWindowRect();
    MoveWindow( rc.left-14, rc.top-14, rc.Width(), rc.Height(), TRUE );

    // Attach CWnd objects to the dialog's children
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

    // Set the initial state of the dialog
    m_editIP6Address.SetWindowText( _T("0000:0000:0000:0000:0000:0000:0000:0001") );
    m_radioIP4.SetCheck(BST_CHECKED);
    m_ip4Address.SetAddress(MAKEIPADDRESS(127, 0, 0, 1));
    m_editStatus.SetWindowText( _T("Server Stopped") );
    m_editPort.SetWindowText( _T("3000") );
    m_radioTCP.SetCheck(BST_CHECKED);
    if (!m_mainSocket.IsIPV6Supported())
    {
        m_radioIP6.EnableWindow(FALSE);
        m_editIP6Address.EnableWindow(FALSE);
    }

    return TRUE;
}

// Respond to the Start/Stop Button press
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
            Append(IDC_EDIT_STATUS, _T("TCP Server Started"));
            Append(IDC_EDIT_STATUS, _T("Waiting for client ..."));
        }
        else
        {
            Append(IDC_EDIT_STATUS, _T("UDP Server Started"));
            Append(IDC_EDIT_STATUS, _T("Waiting for client data"));
        }
    }
    else
    {
        StopServer();

        // Update the dialog
        Append(IDC_EDIT_STATUS, _T("Server Stopped"));
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

// Responds to the send button
BOOL CSvrDialog::OnSend()
{
    switch(m_socketType)
    {
        case SOCK_STREAM:
            // TCP connections have a seperate chat dialog for sending/receiving data
            break;
        case SOCK_DGRAM:
            {
                CString sSend = m_editSend.GetWindowText();
                m_mainSocket.SendTo(TtoA(sSend), strlen(TtoA(sSend)), 0, (LPSOCKADDR)&m_saUDPClient, sizeof(m_saUDPClient));
            }
            break;
    }

    return TRUE;
}

// Accept the connection from the client
BOOL CSvrDialog::OnSocketAccept()
{
    ServerSocketPtr pClient = new CWorkerSocket;
    m_mainSocket.Accept(*pClient, NULL, NULL);
    if (INVALID_SOCKET == m_mainSocket.GetSocket())
    {
        TRACE("Failed to accept connection from client\n");
        TRACE(m_mainSocket.GetLastError());
        return TRUE;
    }

    pClient->StartEvents();

    // Create the new chat dialog
    TCPClientDlgPtr pDialog = new CTCPClientDlg(IDD_CHAT);
    pDialog->m_pSocket = pClient;
    pDialog->DoModeless(*this);

    // Reposition the chat dialog
    CRect rc = pDialog->GetWindowRect();
    int offset = 4 * (static_cast<int>(m_connectedClients.size()) - 1);
    pDialog->MoveWindow(rc.left + offset, rc.top + offset + 80, rc.Width(), rc.Height(), TRUE);
    pDialog->ShowWindow();

    // Add the socket and dialog to the map
    m_connectedClients.insert(std::make_pair(pClient, pDialog));

    // Update the dialog
    Append(IDC_EDIT_STATUS, _T("Client Connected"));

    return TRUE;
}

BOOL CSvrDialog::OnSocketDisconnect(WPARAM wParam)
{
    CWorkerSocket* pClient = reinterpret_cast<CWorkerSocket*>(wParam);

    // Respond to a socket disconnect notification
    Append(IDC_EDIT_STATUS, _T("Client disconnected"));


    // Allocate an iterator for our CWorkerSocket map
    std::map< ServerSocketPtr, TCPClientDlgPtr >::iterator iter;

    for (iter = m_connectedClients.begin(); iter != m_connectedClients.end(); ++iter)
    {
        if (iter->first.get() == pClient)
            break;
    }

    // delete the CWorkerSocket, and remove its pointer
    if (iter != m_connectedClients.end())
    {
        m_connectedClients.erase(iter);
    }

    return TRUE;
}

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
            m_mainSocket.ReceiveFrom(bufArray, 1024, 0, (LPSOCKADDR)&m_saUDPClient, &addrlen);
            TRACE("[Received:] "); TRACE(bufArray); TRACE("\n");
            m_buttonSend.EnableWindow(TRUE);
            m_editSend.EnableWindow(TRUE);
            GotoDlgCtrl(GetDlgItem(IDC_EDIT_SEND));
        }
        break;
    }
    Append(IDC_EDIT_RECEIVE, AtoT(bufArray));

    return TRUE;
}

BOOL CSvrDialog::StartServer()
{
    LRESULT check = m_radioTCP.GetCheck();
    m_socketType = (check == BST_CHECKED)? SOCK_STREAM : SOCK_DGRAM ;

    // Create the main socket
    check = m_radioIP4.GetCheck();
    int IPfamily = (check == BST_CHECKED)? PF_INET : PF_INET6 ;
    if (!m_mainSocket.Create(IPfamily, m_socketType))
    {
        Append(IDC_EDIT_STATUS, _T("Create Socket Failed"));
        Append(IDC_EDIT_STATUS, m_mainSocket.GetLastError());
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
        DWORD dwAddr = 0;
        m_ip4Address.GetAddress(dwAddr);
        in_addr addr;
        ZeroMemory(&addr, sizeof(in_addr));
        addr.S_un.S_addr = htonl(dwAddr);
        strAddr = AtoT( inet_ntoa(addr) );
    }

    // Retrieve the local port number
    UINT port = GetDlgItemInt(m_editPort.GetDlgCtrlID(), FALSE);

    // Bind to the socket
    Append(IDC_EDIT_STATUS, _T("Binding to socket"));
    CString str;
    str.Format( _T("Addr %s, Port %u, type %s"), strAddr.c_str(), port, (m_socketType == SOCK_STREAM)?_T("TCP"):_T("UDP") );
    Append(IDC_EDIT_STATUS, str);

    int RetVal = m_mainSocket.Bind( strAddr, port );
    if ( RetVal != 0 )
    {
        Append(IDC_EDIT_STATUS, _T("Bind failed"));
        Append(IDC_EDIT_STATUS, m_mainSocket.GetLastError());
        return FALSE;
    }

    if (m_socketType == SOCK_STREAM)
    {
        // Listen for connections from clients (TCP server only)
        RetVal = m_mainSocket.Listen();
        if  ( SOCKET_ERROR == RetVal )
        {
            Append(IDC_EDIT_STATUS, _T("Error listening on socket"));
            Append(IDC_EDIT_STATUS, m_mainSocket.GetLastError());
            return FALSE;
        }
    }

    m_mainSocket.StartEvents();

    return TRUE;
}

void CSvrDialog::StopServer()
{
    m_mainSocket.Disconnect();
    m_connectedClients.clear();
}
