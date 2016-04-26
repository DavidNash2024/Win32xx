///////////////////////////////////////
// ClientDialog.h

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H


#include "Client.h"


// Declaration of the CClientDialog class
class CClientDialog : public CDialog
{
public:
	CClientDialog(UINT nResID);
	virtual ~CClientDialog();

	LRESULT OnActivate(UINT uMsg, WPARAM wParam, LPARAM lParam);
	BOOL OnSocketDisconnect();
	BOOL OnSocketConnect();
	BOOL OnSocketReceive();

protected:
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

//private:
public:
	void AppendText(int nID, LPCTSTR buf);
	void LoadCommonControlsEx();
	void OnStartClient();
	void OnSend();

	CClientSocket m_Client;
	BOOL m_IsClientConnected;
	int  m_SocketType;

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	class CIPAddress : public CWnd {};
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

	CIPAddress	m_IP4Address;
	CEditIP6Address	m_EditIP6Address;
	CEditStatus m_EditStatus;
	CEditPort	m_EditPort;
	CEditSend	m_EditSend;
	CEditReceive m_EditReceive;
	CButtonConnect m_ButtonConnect;
	CButtonSend	m_ButtonSend;
	CRadioIP4	m_RadioIP4;
	CRadioIP6	m_RadioIP6;
	CRadioTCP	m_RadioTCP;
	CRadioUDP	m_RadioUDP;

};

#endif //CLIENTDIALOG_H
