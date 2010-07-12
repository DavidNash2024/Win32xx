///////////////////////////////////////
// SvrDialog.h

#ifndef SVRDIALOG_H
#define SVRDIALOG_H


#include "Server.h"


class CServerSocket;

// Declaration of the CTCPClientDlg class
class CTCPClientDlg : public CDialog
{
public:
	CTCPClientDlg(UINT nResID, HWND hWndParent = NULL);
	virtual ~CTCPClientDlg() {}
	virtual BOOL DialogProc (UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	void Append(int nID, LPCTSTR buf);
	void Receive();
	void Send();

	CServerSocket* m_pSocket;

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
	CSvrDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CSvrDialog();
	void Append(int nID, LPCTSTR buf);
	void OnSocketDisconnect(CServerSocket* pClient);
	void OnSocketReceive(CServerSocket* pClient);
	void OnSocketAccept();
	void OnSend();
	void OnStartServer();
	BOOL StartServer();
	void StopServer();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CServerSocket m_MainSocket;
	std::map<CServerSocket*, CTCPClientDlg*> m_ConnectedClients;// Stores TCP client sockets and TCP client dialogs
	BOOL m_bServerStarted;
	int  m_SocketType;			// either SOCK_STREAM or SOCK_DGRAM
	sockaddr_in m_ClientAddr;	// Stores connect address from UDP client

	// Nested classes for this dialog's child windows
	// Nesting is optional. Its done to keep the IDE's class view tidy.
	class CEditStatus : public CEdit {};
	class CEditPort : public CEdit {};
	class CEditSend : public CEdit {};
	class CEditReceive : public CEdit {};
	class CButtonStart : public CButton {};
	class CButtonSend : public CButton {};
	class CRadioTCP : public CButton {};
	class CRadioUDP : public CButton {};

	CEditStatus m_EditStatus;
	CEditPort	m_EditPort;
	CEditSend	m_EditSend;
	CEditReceive m_EditReceive;
	CButtonStart m_ButtonStart;
	CButtonSend	m_ButtonSend;
	CRadioTCP	m_RadioTCP;
	CRadioUDP	m_RadioUDP;

};

#endif //SVRDIALOG_H

