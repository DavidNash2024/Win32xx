/////////////////////////////
// SvrDialog.h
//

#ifndef SVRDIALOG_H
#define SVRDIALOG_H

#include "Server.h"

class CWorkerSocket;
class CTCPClientDlg;


// Note: Modern C++ compilers can use these typedefs instead.
// typedef std::shared_ptr<CWorkerSocket> ServerSocketPtr;
// typedef std::shared_ptr<CTCPClientDlg> TCPClientDlgPtr;
typedef Shared_Ptr<CWorkerSocket> ServerSocketPtr;
typedef Shared_Ptr<CTCPClientDlg> TCPClientDlgPtr;


//////////////////////////////////////////////////////////////
// CTCPClientDlg manages the dialog that is displayed for each
// a TCP connection which is made with the server.
class CTCPClientDlg : public CDialog
{
public:
    CTCPClientDlg(UINT resID);
    virtual ~CTCPClientDlg() {}
    ServerSocketPtr& ServerSocket() { return m_pSocket; }
    void Receive();
    void Send();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnClose();
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();

private:
    CTCPClientDlg(const CTCPClientDlg&);                // Disable copy construction
    CTCPClientDlg& operator = (const CTCPClientDlg&);   // Disable assignment operator

    // Nested classes for this dialog's child windows
    // Nesting is optional. Its done to keep the IDE's class view tidy.
    class CEditSend : public CEdit {};
    class CEditReceive : public CEdit {};
    class CButtonSend : public CButton {};

    void AppendText(int id, LPCTSTR text);

    ServerSocketPtr m_pSocket;
    CEditSend   m_editSend;
    CEditReceive m_editReceive;
    CButtonSend m_buttonSend;
};


//////////////////////////////////////////////////////////////
// CSvrDialog manages the application's main dialog.
// The dialog allows users to enter accept connects with network
// clients and communicate with them.
class CSvrDialog : public CDialog
{
public:
    CSvrDialog(int resID);
    virtual ~CSvrDialog();

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();

private:
    CSvrDialog(const CSvrDialog&);                // Disable copy construction
    CSvrDialog& operator = (const CSvrDialog&);   // Disable assignment operator

    // Nested classes for this dialog's child windows
    // Nesting is optional. Its done to keep the IDE's class view tidy.
    class CIP4Address : public CIPAddress {};
    class CEditIP6Address : public CEdit {};
    class CEditStatus : public CEdit {};
    class CEditPort : public CEdit {};
    class CEditSend : public CEdit {};
    class CEditReceive : public CEdit {};
    class CButtonStart : public CButton {};
    class CButtonSend : public CButton {};
    class CRadioIP4 : public CButton {};
    class CRadioIP6 : public CButton {};
    class CRadioTCP : public CButton {};
    class CRadioUDP : public CButton {};

    // Command Handlers
    BOOL OnSend();
    BOOL OnStartServer();

    // Message handlers
    BOOL OnSocketAccept();
    BOOL OnSocketDisconnect(WPARAM wparam);
    BOOL OnSocketReceive(WPARAM wparam);

    void AppendText(const CEdit& edit, LPCTSTR text);
    void LoadCommonControlsEx();
    BOOL StartServer();
    void StopServer();

    // Member variables
    CServerSocket m_mainSocket;
    std::map<ServerSocketPtr, TCPClientDlgPtr> m_connectedClients;// Stores TCP client sockets and TCP client dialogs
    bool m_isServerStarted;
    int  m_socketType;              // either SOCK_STREAM or SOCK_DGRAM
    sockaddr_in6  m_saUDPClient;    // connected UPD client's sockaddr

    // Member variables for dialog controls
    CIP4Address m_ip4Address;
    CEditIP6Address m_editIP6Address;
    CEditStatus m_editStatus;
    CEditPort   m_editPort;
    CEditSend   m_editSend;
    CEditReceive m_editReceive;
    CButtonStart m_buttonStart;
    CButtonSend m_buttonSend;
    CRadioIP4   m_radioIP4;
    CRadioIP6   m_radioIP6;
    CRadioTCP   m_radioTCP;
    CRadioUDP   m_radioUDP;
};

#endif //SVRDIALOG_H

