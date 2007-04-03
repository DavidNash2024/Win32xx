//////////////////////////////////////////////////////
// MainWnd.h
//  Declaration of the CThreadWnd class


#ifndef MAINWND_H
#define MAINWND_H


#include "ThreadApp.h"
#include "TestWnd.h"
#include "..\Win32++\Dialog.h"


class CThread;

class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow();
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
		WM_WINDOWCREATED     = WM_USER+1,	// the message sent when window is created
		WM_TESTMESSAGE       = WM_USER+2    // the test message 
	};

	int   m_iNums[MAX_THREADS];			// An array of int:  0 to MAX_THREADS-1
	HWND  m_hEdit;						// Handle to the edit window
	CThread* m_pCThreads[MAX_THREADS];	// An array of CThread pointers
	CDialog m_Dialog1;
	CDialog m_Dialog2;
};


#endif  //MAINWND_H
