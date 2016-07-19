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
	void LoadFile(LPCTSTR str);

	LRESULT OnDropFile(WPARAM wParam);
	LRESULT OnGetAllPoints();
	LRESULT OnSendPoint(WPARAM wParam);

	void OnFileExit();
	void OnFileMRU(WPARAM wParam);
	void OnFileNew();
	void OnFileOpen();
	void OnFileSave();
	void OnFileSaveAs();
	void OnFilePrint();
	void OnPenColor();

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CDoc m_Doc;
	CString m_PathName;

};

#endif //MAINFRM_H

