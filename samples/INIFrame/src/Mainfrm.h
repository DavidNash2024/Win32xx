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

	BOOL	OnFileExit();
	BOOL	OnFileOpen();
	BOOL	OnFilePrint();
	BOOL	OnFileSave();
	
	CString ItoT(int i);
	int		TtoI(LPCTSTR szString);
	
	void SerializeINI(BOOL IsStoring);

protected:
	virtual BOOL	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int		OnCreate(LPCREATESTRUCT pcs);
	virtual void	OnClose();
	virtual void	OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void	PreCreate(CREATESTRUCT &cs);
	virtual void	SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CRect m_rcPosition;		// Starting window position retrieved from ini file
	DWORD m_ShowCmd;		// Initial frame show state retrieved from ini file
	BOOL m_ShowStatusBar;	// Initial StatusBar show state retrieved from ini file
	BOOL m_ShowToolBar;		// Initial ToolBar show state retrieved from ini file
};

#endif //MAINFRM_H

