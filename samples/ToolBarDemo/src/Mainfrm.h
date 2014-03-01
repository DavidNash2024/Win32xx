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

protected:
	virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void	OnCreate();
	virtual LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
	virtual void	OnFileExit();
	virtual void	OnFileOpen();
	virtual void	OnFilePrint();
	virtual void	OnFileSave();
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
	virtual LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
	virtual LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);
	virtual void	OnTBBigIcons();
	virtual void	OnTBCustomize();
	virtual void	OnTBDefault();
	virtual LRESULT	OnReset(LPNMTOOLBAR pNMTB);
	virtual void    SaveTBDefault();
	virtual void	SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	std::vector<TBBUTTON>	m_vTBBDefault;
	std::vector<TBBUTTON>	m_vTBBReset;
	BOOL	m_bBigIcons;
};

#endif //MAINFRM_H

