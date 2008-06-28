////////////////////////////////////////
// MainWnd.h
//  Declaration of the CMainWindow class


#ifndef MAINWND_H
#define MAINWND_H


#include "../WinDev++/WinCore.h"
#include <vector>


// Forward declaration of the CThread class
class CThread;

class CMainWindow : public CWnd
{
public:
	CMainWindow();
	virtual ~CMainWindow();
	virtual HWND Create(HWND hParent = 0);
	virtual void OnAllWindowsCreated();

protected:
	virtual void OnCreate();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Constants
	{
		WM_WINDOWCREATED = WM_USER+1,	// the message sent when window is created
		WM_TESTMESSAGE   = WM_USER+2	// the test message
	};

	std::vector<CThread*> m_pCThreads;	// A vector of CThread pointers
	int m_nThreads;						// Number of additional threads to be created
};


#endif  //MAINWND_H
