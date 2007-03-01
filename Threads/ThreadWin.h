//////////////////////////////////////////////////////
// ThreadWin.h
//  Declaration of the CThreadWnd class


#ifndef THREADWIN_H
#define THREADWIN_H

#include "..\Win32++\WinCore.h"


class CThreadWnd : public CWnd
{
public:
	CThreadWnd();
	virtual ~CThreadWnd(){}
	virtual void CreateWin(int i);
	virtual void OnInitialUpdate();

private:
	enum Constants
	{
		WM_WINDOWCREATED     = WM_USER+1,
		WM_ALLWINDOWSCREATED = WM_USER+2,
		WM_TESTMESSAGE       = WM_USER+3
	};

	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int m_iNum;

};


#endif  //THREADWIN_H
