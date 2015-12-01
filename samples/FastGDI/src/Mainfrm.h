/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "ColourDialog.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	CView& GetMyView() {return m_MyView;}
	void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL bGray);
	BOOL OnAdjustImage();
	BOOL OnFileExit();
	BOOL OnFileSave();
	BOOL OnFileSaveAs();
	BOOL OnFileNew();
	BOOL OnFileOpen();
	BOOL OnFileOpenMRU(WPARAM wParam, LPARAM lParam);
	BOOL LoadFile(CString& FileName);
	void SaveFile(CString& str);

protected:
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_MyView;
	CString m_PathName;

};

#endif //MAINFRM_H

