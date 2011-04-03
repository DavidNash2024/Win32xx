////////////////////////////////////////
// MainWnd.h
//  Declaration of the CMainWindow class

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"

typedef Shared_Ptr<CTestWindow> TestWndPtr;

class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow() {}
	virtual HWND Create(CWnd* pParent = 0);
	virtual void OnAllWindowsCreated();

protected:
	virtual void OnCreate();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED = WM_USER+1,	// the message sent when window is created
		WM_TESTMESSAGE   = WM_USER+2	// the test message
	};

	std::vector<TestWndPtr> m_vTestWnd;	// A vector of CTestWin smart pointers
	int m_nTestWin;						// Number of additional test windows to be created
	int m_nWindowsCreated;				// Count of windows actually created
};


#endif  //MAINWND_H
