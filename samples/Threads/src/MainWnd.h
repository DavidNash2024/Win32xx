////////////////////////////////////////
// MainWnd.h
//  Declaration of the CMainWindow class

#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyThread.h"
#include "MyEdit.h"

typedef Shared_Ptr<CMyThread> MyThreadPtr;


class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow() {}
	virtual void AppendText(LPCTSTR szStr);
	virtual HWND Create(HWND hParent = 0);
	virtual void OnAllWindowsCreated();
	virtual void OnWindowCreated();

protected:
	virtual void OnClose();
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnDestroy();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED = WM_USER+1,	// the message sent when window is created
		WM_TESTMESSAGE   = WM_USER+2	// the test message
	};

	std::vector<MyThreadPtr> m_vMyThread;// A vectore of CMyThread smart pointers
	int m_nTestWnd;						// Number of additional test windows to be created
	int m_nWindowsCreated;				// Count of windows actually created
	CMyEdit m_EditWnd;
};


#endif  //MAINWND_H
