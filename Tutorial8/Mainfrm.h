///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class


#ifndef MAINFRM_H
#define MAINFRM_H

#include "../Win32++/frame.h"
#include "View.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void OnFileOpen();
	void OnFileSave();
	void OnFileSaveAs();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void LoadToolbar();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	enum Pens
	{
		RED, BLUE, GREEN, BLACK
	};

	CView m_View;
	tString m_PathName;
};

#endif //MAINFRM_H

