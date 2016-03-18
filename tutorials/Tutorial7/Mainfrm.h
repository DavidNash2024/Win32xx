///////////////////////////////////////////////////////
// Mainfrm.h
//  Declaration of the CMainFrame class

#ifndef MAINFRM_H
#define MAINFRM_H

#include "targetver.h"
#include "wxx_frame.h"
#include "wxx_commondlg.h"
#include "View.h"


class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	void LoadFile(LPCTSTR str);

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
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;

};

#endif //MAINFRM_H

