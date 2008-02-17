///////////////////////////////////////
// SvrDialog.h

#ifndef SVRDIALOG_H
#define SVRDIALOG_H

#include "../../Win32++/Dialog.h"
#include "Server.h"
#include "resource.h"


class CServerSocket;

// Declaration of the CTCPClientDlg class
class CTCPClientDlg : public CDialog
{
public:
	CTCPClientDlg(UINT nResID, HWND hWndParent = NULL);
	virtual ~CTCPClientDlg() {}
	void Send();

	CServerSocket* m_pSocket;
};


// Declaration of the CSvrDialog class
class CSvrDialog : public CDialog
{
public:
	struct Client
	{
		CServerSocket* pSocket;
		CTCPClientDlg* pDialog;
	};

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
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CServerSocket m_ListenSocket;
	std::vector<Client> m_ConnectedSockets;
	BOOL m_bServerStarted;
	int  m_SocketType;
	sockaddr_in m_ClientAddr;
};

#endif //SVRDIALOG_H

