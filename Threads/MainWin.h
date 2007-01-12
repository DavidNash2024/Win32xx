//////////////////////////////////////////////////////
// MainWin.h
//  Declaration of the CThreadWnd class


#ifndef MAINWIN_H
#define MAINWIN_H


#include "ThreadApp.h"
#include "ThreadWin.h"


class CMainWin : public CWnd
{
public:
	CMainWin();
	virtual ~CMainWin();
	virtual void Create();

protected:
	virtual void OnCreate();
	virtual LRESULT WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	int   m_IntArray[MAX_THREADS];
	DWORD m_ThreadID[MAX_THREADS];

};


#endif  //MAINWIN_H
