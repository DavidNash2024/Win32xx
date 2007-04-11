//////////////////////////////////////////////////////
// MainWnd.h
//  Declaration of the CThreadWnd class


#ifndef MAINWND_H
#define MAINWND_H

#include "TestWnd.h"
#include "MyDialog.h"


class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow();
	virtual void Create();
	virtual void CreateTestWindows(int nWindows);
	virtual void OnAllWindowsCreated();
	virtual void PerformanceTest();
	virtual void SendText(LPCTSTR str);
	virtual void SetTestMessages(int nTestMessages) {m_nTestMessages = nTestMessages;}

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

	CTestWindow** m_pCTestWindows;	// An array of CTestWindow pointers
	HWND  m_hEdit;			// Handle to the edit window
	HFONT m_hFont;			// A font for the edit window
	int m_nTestMessages;	// Number of test messages to be sent
	int m_nTestWindows;		// Number of test windows
};


#endif  //MAINWND_H
