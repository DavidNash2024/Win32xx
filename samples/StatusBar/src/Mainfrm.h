/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "MyStatusBar.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

	virtual CStatusBar* GetStatusBar() const	{ return const_cast<CMyStatusBar*>(&m_MyStatusBar); }

protected:
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int		OnCreate(LPCREATESTRUCT pcs);
	virtual LRESULT OnDrawItem(WPARAM wParam, LPARAM lParam);
	virtual void	OnFileExit();
	virtual void	OnFileOpen();
	virtual void	OnFilePrint();
	virtual void	OnFileSave();
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	PreCreate(CREATESTRUCT &cs);
	virtual void	SetupToolBar();
	virtual void	RecalcLayout();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CMyStatusBar m_MyStatusBar;
};

#endif //MAINFRM_H

