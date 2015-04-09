///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual void OnFileExit();
	virtual void OnFileMRU(WPARAM wParam);
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual void OnFileSave();
	virtual void OnFileSaveAs();
	virtual void OnFilePrint();
	virtual void OnInitialUpdate();
	virtual void OnPenColor();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CString m_PathName;

};

#endif //MAINFRM_H

