//////////////////////////////////////////////////////
// ThreadWin.h
//  Declaration of the CThreadWnd class


#ifndef THREADWIN_H
#define THREADWIN_H

//Include WinCore.h first
#include "..\Win32++\WinCore.h"


class CThreadWnd : public CWnd
{
public:
	CThreadWnd();
	virtual ~CThreadWnd(){}
	virtual void CreateWin(int i);

private:
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int m_iNum;

};


#endif  //THREADWIN_H
