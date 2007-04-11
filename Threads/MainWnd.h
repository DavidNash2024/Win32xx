////////////////////////////////////////
// MainWnd.h
//  Declaration of the CMainWindow class


#ifndef MAINWND_H
#define MAINWND_H

#include "..\Win32++\WinCore.h"

// Forward declaration of the CThread class
class CThread;

class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow();
	virtual void Create();
	virtual void CreateThreads();
	virtual void OnAllWindowsCreated();

protected:
	virtual void OnCreate();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED     = WM_USER+1,	// the message sent when window is created
		WM_TESTMESSAGE       = WM_USER+2    // the test message 
	};

	CThread** m_pCThreads;	// An array of CThread pointers
	int m_nThreads;			// Number of additional threads to be created
};


#endif  //MAINWND_H
