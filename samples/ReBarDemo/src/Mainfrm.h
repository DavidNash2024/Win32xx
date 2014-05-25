/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	void OnBottom();
	void OnLeft();
	void OnRight();
	void OnTop();

protected:
	virtual CRect	GetViewRect() const;
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int 	OnCreate(LPCREATESTRUCT pcs);
	virtual void	OnFileExit();
	virtual void	OnFileOpen();
	virtual void	OnFilePrint();
	virtual void	OnFileSave();
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	SetupToolBar();
	virtual void	SetWrapState(CToolBar* pToolBar, BOOL bWrap);
	virtual void	RecalcLayout();
	virtual void	SetReBarPos();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CReBar m_ReBar;
	CToolBar m_ToolBar;
	CImageList m_ToolBarImages;
	CView m_View;
};

#endif //MAINFRM_H

