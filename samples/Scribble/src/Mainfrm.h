///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "Doc.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	CDoc& GetDoc() { return m_Doc; }

	BOOL OnFileExit();
	BOOL OnFileMRU(WPARAM wParam);
	BOOL OnFileNew();
	BOOL OnFileOpen();
	BOOL OnFileSave();
	BOOL OnFileSaveAs();
	BOOL OnFilePrint();
	BOOL OnPenColor();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(LPCREATESTRUCT pcs);
	virtual void OnInitialUpdate();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CDoc m_Doc;
	CString m_PathName;

};

#endif //MAINFRM_H

