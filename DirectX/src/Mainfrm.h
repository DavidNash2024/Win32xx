/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "MyThread.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual void CreateView()
	{
		// Starting our thread creates the view window
		m_MyThread.StartThread(m_hWnd);
	}
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnFileOpen();
	virtual void OnFilePrint();
	virtual void OnFileSave();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMyThread m_MyThread;
};

#endif //MAINFRM_H

