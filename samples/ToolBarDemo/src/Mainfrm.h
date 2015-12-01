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

	BOOL	OnFileExit();
	BOOL	OnFileOpen();
	BOOL	OnFilePrint();
	BOOL	OnFileSave();
	BOOL	OnTBBigIcons();
	BOOL	OnTBCustomize();
	BOOL	OnTBDefault();
	void    SaveTBDefault();

protected:
	virtual LRESULT OnBeginAdjust(LPNMTOOLBAR pNMTB);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int OnCreate(CREATESTRUCT& cs);
	virtual LRESULT OnCustHelp(LPNMHDR pNMHDR);
	virtual LRESULT OnEndAdjust(LPNMHDR pNMHDR);
	virtual LRESULT OnGetButtonInfo(LPNMTOOLBAR pNMTB);
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnQueryDelete(LPNMTOOLBAR pNMTB);
	virtual LRESULT OnQueryInsert(LPNMTOOLBAR pNMTB);
	virtual LRESULT	OnReset(LPNMTOOLBAR pNMTB);
	virtual LRESULT OnToolBarChange(LPNMTOOLBAR pNMTB);
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	std::vector<TBBUTTON>	m_vTBBDefault;
	std::vector<TBBUTTON>	m_vTBBReset;
	BOOL	m_UseBigIcons;
};

#endif //MAINFRM_H

