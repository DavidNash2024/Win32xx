///////////////////////////////////////
// ClientDialog.h

#ifndef CLIENTDIALOG_H
#define CLIENTDIALOG_H

#include "../../Win32++/dialog.h"
#include "Client.h"
#include "resource.h"


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
	virtual BOOL DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);

private:
	void Append(int nID, LPCTSTR buf);
	void LoadCommonControlsEx();
	void OnStartClient();
	void OnSend();

	CClientSocket m_Client;
	BOOL m_bClientConnected;
	int  m_SocketType;
};

#endif //CLIENTDIALOG_H
