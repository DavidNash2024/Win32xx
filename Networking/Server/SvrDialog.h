///////////////////////////////////////
// SvrDialog.h

#ifndef SVRDIALOG_H
#define SVRDIALOG_H

#include "../../Win32++/Dialog.h"
#include "Server.h"
#include "resource.h"


// Declaration of the CMyDialog class
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
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CServerSocket m_ListenSocket;
	std::vector<CServerSocket*> m_ConnectedSockets;
	BOOL m_bServerStarted;
	int  m_SocketType;
	sockaddr_in m_ClientAddr;
};

#endif //SVRDIALOG_H

