///////////////////////////////////////
// SvrDialog.h

#ifndef SVRDIALOG_H
#define SVRDIALOG_H


#include "Server.h"


class CServerSocket;
class CTCPClientDlg;

typedef Shared_Ptr<CServerSocket> ServerSocketPtr;
typedef Shared_Ptr<CTCPClientDlg> TCPClientDlgPtr;

// Declaration of the CTCPClientDlg class
class CTCPClientDlg : public CDialog
{
public:
	CTCPClientDlg(UINT nResID);
	virtual ~CTCPClientDlg() {}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnClose();
	virtual BOOL OnInitDialog();
	void Append(int nID, LPCTSTR buf);
	void Receive();
	void Send();

	ServerSocketPtr m_pSocket;

private:
	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	class CEditSend : public CEdit {};
	class CEditReceive : public CEdit {};
	class CButtonSend : public CButton {};

	CEditSend	m_EditSend;
	CEditReceive m_EditReceive;
	CButtonSend	m_ButtonSend;
};


// Declaration of the CSvrDialog class
class CSvrDialog : public CDialog
{
public:
	CSvrDialog(UINT nResID);
	virtual ~CSvrDialog();
	void Append(int nID, LPCTSTR buf);
	void LoadCommonControlsEx();
	BOOL OnSocketDisconnect(WPARAM wParam);
	BOOL OnSocketReceive(WPARAM wParam);
	BOOL OnSocketAccept();
	void OnSend();
	void OnStartServer();
	BOOL StartServer();
	void StopServer();

protected:
	virtual void OnDestroy();
	virtual BOOL OnInitDialog();
	virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CServerSocket m_MainSocket;
	std::map<ServerSocketPtr, TCPClientDlgPtr> m_ConnectedClients;// Stores TCP client sockets and TCP client dialogs
	BOOL m_bServerStarted;
	int  m_SocketType;				// either SOCK_STREAM or SOCK_DGRAM
	sockaddr_in6  m_saUDPClient;	// connected UPD client's sockaddr	

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	class CIP4Address : public CWnd {};
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

	CIP4Address	m_IP4Address;
	CEditIP6Address	m_EditIP6Address;
	CEditStatus m_EditStatus;
	CEditPort	m_EditPort;
	CEditSend	m_EditSend;
	CEditReceive m_EditReceive;
	CButtonStart m_ButtonStart;
	CButtonSend	m_ButtonSend;
	CRadioIP4	m_RadioIP4;
	CRadioIP6	m_RadioIP6;
	CRadioTCP	m_RadioTCP;
	CRadioUDP	m_RadioUDP;

};

#endif //SVRDIALOG_H

