/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	BOOL OnFileExit();
	BOOL OnFileOpen();
	BOOL OnFilePrint();
	BOOL OnFileSave();
	void OnBottom();
	void OnLeft();
	void OnRight();
	void OnTop();
	void SetReBarPos();
	void SetWrapState(CToolBar* pToolBar, BOOL bWrap);

protected:
	virtual CRect	GetViewRect() const;
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int 	OnCreate(CREATESTRUCT& cs);
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	SetupToolBar();
	virtual void	RecalcLayout();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CReBar m_ReBar;
	CToolBar m_ToolBar;
	CImageList m_ToolBarImages;
	CView m_View;
};

#endif //MAINFRM_H

