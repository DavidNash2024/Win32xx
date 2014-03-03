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
	LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
	BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	int 	OnCreate(LPCREATESTRUCT pcs);
	LRESULT OnCustHelp(LPNMHDR pNMHDR);
	LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
	void	OnFileExit();
	void	OnFileOpen();
	void	OnFilePrint();
	void	OnFileSave();
	void	OnInitialUpdate();
	LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
	LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
	LRESULT	OnReset(LPNMTOOLBAR pNMTB);
	LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);
	void	OnTBBigIcons();
	void	OnTBCustomize();
	void	OnTBDefault();
	void    SaveTBDefault();
	void	SetupToolBar();
	LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	std::vector<TBBUTTON>	m_vTBBDefault;
	std::vector<TBBUTTON>	m_vTBBReset;
	BOOL	m_bBigIcons;
};

#endif //MAINFRM_H

