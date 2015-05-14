/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "View.h"
#include "Doc.h"

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();

	CDoc& GetDoc() {return m_Doc; }

	BOOL	OnFileExit();
	BOOL	OnFileOpen();
	BOOL	OnFilePrint();
	BOOL	OnFileSave();

protected:
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int		OnCreate(LPCREATESTRUCT pcs);
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	PreCreate(CREATESTRUCT &cs);
	virtual void	SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CDoc m_Doc;
};

#endif //MAINFRM_H

