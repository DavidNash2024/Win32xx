//////////////////////////////////////////////////////
// MainWin.h
//  Declaration of the CThreadWnd class


#ifndef MAINWIN_H
#define MAINWIN_H


#include "ThreadApp.h"
#include "ThreadWin.h"


class CMainWnd : public CWnd
{
public:
	CMainWnd();
	virtual ~CMainWnd();
	virtual void Create();
	virtual void OnAllWindowsCreated();
	virtual void PerformanceTest();
	virtual void SendText(LPCTSTR str);

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED = WM_USER+1,
		WM_TESTMESSAGE   = WM_USER+2
	};

	int   m_IntArray[MAX_THREADS];
	DWORD m_ThreadID[MAX_THREADS];
	HWND m_hEdit;
};


#endif  //MAINWIN_H
