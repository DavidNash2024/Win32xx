///////////////////////////////////////
// ClientDialog.h

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H


#include "Client.h"


// Declaration of the CClientDialog class
class CClientDialog : public CDialog
{
public:
	CClientDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CClientDialog();

	void OnClientDisconnect();
	void OnClientConnect();
	int OnClientReceive();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	void Append(int nID, LPCTSTR buf);
	void LoadCommonControlsEx();
	void OnStartClient();
	void OnSend();

	CClientSocket m_Client;
	BOOL m_bClientConnected;
	int  m_SocketType;

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	class CEditStatus : public CEdit {};
	class CEditPort : public CEdit {};
	class CEditSend : public CEdit {};
	class CEditReceive : public CEdit {};
	class CButtonConnect : public CButton {};
	class CButtonSend : public CButton {};
	class CIPAddress : public CWnd {};
	class CRadioTCP : public CButton {};
	class CRadioUDP : public CButton {};

	CEditStatus m_EditStatus;
	CEditPort	m_EditPort;
	CEditSend	m_EditSend;
	CEditReceive m_EditReceive;
	CButtonConnect m_ButtonConnect;
	CButtonSend	m_ButtonSend;
	CIPAddress	m_IPAddress;
	CRadioTCP	m_RadioTCP;
	CRadioUDP	m_RadioUDP;

};

#endif //CLIENTDIALOG_H
