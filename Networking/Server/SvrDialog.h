///////////////////////////////////////
// SvrDialog.h

#ifndef SVRDIALOG_H
#define SVRDIALOG_H

#include "../../Win32++/Dialog.h"
#include "Server.h"
#include "resource.h"

#define IDT_TIMER1 400


// Declaration of the CMyDialog class
class CSvrDialog : public CDialog
{
public:
	CSvrDialog(UINT nResID, HWND hWndParent = NULL);
	virtual ~CSvrDialog();
	void Append(int nID, LPCTSTR buf);
	void CleanDisconnected();
	void OnClientDisconnect(CClientSocket* pClient);
	void OnClientReceive(CClientSocket* pClient);
	void OnListenAccept();
	void OnSend();
	void OnStartServer();
	void OnTCP();
	void OnUDP();
	BOOL StartServer(int LocalPort);
	void StopServer();
	
protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	CListenSocket m_ListenSocket;
	std::vector<CClientSocket*> m_ConnectedSockets;
//	CClientSocket* m_DisconnectingSocket;
	std::vector<CClientSocket*> m_DisconnectedSockets;
	BOOL m_bServerStarted;
};

#endif //SVRDIALOG_H

