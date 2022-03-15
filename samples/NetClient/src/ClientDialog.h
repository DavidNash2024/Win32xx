/////////////////////////////
// ClientDialog.h
//

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H


#include "Client.h"


//////////////////////////////////////////////////////////////
// CClientDialog manages the application's main dialog.
// The dialog allows users to enter information to communicate
// to the NetServer application.
class CClientDialog : public CDialog
{
public:
    CClientDialog(UINT resID);
    virtual ~CClientDialog();

protected:
    // Virtual functions that override base class functions
    virtual void    OnClose();
    virtual BOOL    OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);

private:
    // Nested classes for this dialog's child windows
    // Nesting is optional. Its done to keep the IDE's class view tidy.
    class CIP4Address : public CIPAddress {};
    class CEditIP6Address : public CEdit {};
    class CEditStatus : public CEdit {};
    class CEditPort : public CEdit {};
    class CEditSend : public CEdit {};
    class CEditReceive : public CEdit {};
    class CButtonConnect : public CButton {};
    class CButtonSend : public CButton {};
    class CRadioIP4 : public CButton {};
    class CRadioIP6 : public CButton {};
    class CRadioTCP : public CButton {};
    class CRadioUDP : public CButton {};

    // Message handlers
    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    // Command handlers
    BOOL OnSocketDisconnect();
    BOOL OnSocketConnect();
    BOOL OnSocketReceive();

    void AppendText(const CEdit& edit, LPCTSTR text);
    void LoadCommonControlsEx();
    void OnStartClient();
    void OnSend();

    // Member variables
    CIP4Address m_ip4Address;
    CEditIP6Address m_editIP6Address;
    CEditStatus m_editStatus;
    CEditPort   m_editPort;
    CEditSend   m_editSend;
    CEditReceive m_editReceive;
    CButtonConnect m_buttonConnect;
    CButtonSend m_buttonSend;
    CRadioIP4   m_radioIP4;
    CRadioIP6   m_radioIP6;
    CRadioTCP   m_radioTCP;
    CRadioUDP   m_radioUDP;
    
    CClientSocket m_client;
    bool m_isClientConnected;
    int  m_socketType;
};

#endif //CLIENTDIALOG_H
