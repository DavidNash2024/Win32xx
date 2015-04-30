#ifndef MAINFRM_H
#define MAINFRM_H


#include "wxx_wincore.h"
#include "wxx_wceframe.h"
#include "wxx_dialog.h"


class CMainFrame : public CWceFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnDraw(CDC& dc);	
	virtual void OnInitialUpdate();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif   // MAINFRM_H
