//////////////////////////////////////////////////////
// TestWnd.h
//  Declaration of the CTestWindow class


#ifndef TESTWND_H
#define TESTWND_H


#include "../../Win32++/wincore.h"
#include <sstream>


class CTestWindow : public CWnd
{
public:
	CTestWindow();
	virtual ~CTestWindow(){}
	virtual void CreateWin(int i);
	virtual void OnInitialUpdate();

private:
	enum Constants
	{
		WM_WINDOWCREATED     = WM_USER+1,
		WM_TESTMESSAGE       = WM_USER+2
	};

	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

	int m_nWindow;
};


#endif  //TESTWIN_H
