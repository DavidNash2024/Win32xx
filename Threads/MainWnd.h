//////////////////////////////////////////////////////
// MainWnd.h
//  Declaration of the CThreadWnd class


#ifndef MAINWND_H
#define MAINWND_H


#include "ThreadApp.h"
#include "ThreadWnd.h"


class CMainWnd : public CWnd
{
public:
	CMainWnd();
	virtual ~CMainWnd();
	virtual void Create();
	virtual void OnAllWindowsCreated();
	virtual void PerformanceTest();
	virtual void SendText(LPCTSTR str);
	static DWORD WINAPI ThreadCallback(LPVOID pInt);

protected:
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void OnSize();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED     = WM_USER+1,
		WM_TESTMESSAGE       = WM_USER+2
	};

	int   m_IntArray[MAX_THREADS];
	DWORD m_ThreadID[MAX_THREADS];
	HWND m_hEdit;
	HANDLE m_ThreadHandles[MAX_THREADS];
	static CThreadWnd* m_ThreadCWnds[MAX_THREADS];	// Array of CThreadWnd pointers
};


#endif  //MAINWND_H
