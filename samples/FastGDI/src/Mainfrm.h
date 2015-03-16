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
	CMainFrame(void);
	virtual ~CMainFrame();
	CView& GetMyView() {return m_MyView;}
	void ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL bGray);
	BOOL LoadFile(CString& FileName);
	void SaveFile(CString& str);

protected:
	virtual void OnAdjustImage();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(LPCREATESTRUCT pcs);
	virtual void OnFileExit();
	virtual void OnFileSave();
	virtual void OnFileSaveAs();
	virtual void OnFileNew();
	virtual void OnFileOpen();
	virtual BOOL OnFileOpenMRU(WPARAM wParam, LPARAM lParam);
	virtual void OnInitialUpdate();
	virtual void OnMenuUpdate(UINT nID);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_MyView;
	CString m_PathName;

};

#endif //MAINFRM_H

