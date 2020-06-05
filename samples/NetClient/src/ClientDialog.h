///////////////////////////////////////
// ClientDialog.h

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H


#include "Client.h"


// Declaration of the CClientDialog class
class CClientDialog : public CDialog
{
public:
    CClientDialog(UINT resID);
    virtual ~CClientDialog();

    LRESULT OnActivate(UINT msg, WPARAM wparam, LPARAM lparam);
    BOOL OnSocketDisconnect();
    BOOL OnSocketConnect();
    BOOL OnSocketReceive();

protected:
    virtual void OnClose();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);

private:
    void AppendText(const CEdit& edit, LPCTSTR text);
    void LoadCommonControlsEx();
    void OnStartClient();
    void OnSend();

    CClientSocket m_client;
    BOOL m_isClientConnected;
    int  m_socketType;

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

};

#endif //CLIENTDIALOG_H
