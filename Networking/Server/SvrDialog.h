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
	void OnClientClose(CClientSocket* pClient);
	void OnClientReceive(CClientSocket* pClient);
	void OnListenAccept();
	void StartServer(int LocalPort);
	void StopServer();

protected:
	virtual BOOL OnInitDialog();
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	void Append(int nID, LPCTSTR buf);
	void OnStartServer();
	void OnSend();
	void OnTCP();
	void OnUDP();

	CListenSocket m_ListenSocket;
	std::vector<CClientSocket*> m_ConnectedSockets;
	BOOL m_bServerStarted;
};

#endif //SVRDIALOG_H

