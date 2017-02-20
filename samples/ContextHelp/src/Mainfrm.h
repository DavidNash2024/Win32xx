/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "View.h"
#include "Doc.h"
#include "AppHelp.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	void	ChooseHelpTopic();
	CString CreateAppDataFolder(const CString& subfolder);
	CDoc&	GetDoc()	{ return m_Doc; }
	UINT	GetIDFromCursorPos();
	void	OnF1();
	void 	OnFileExit();
	void	OnShiftF1();
	void 	OnUpdateCheckA(UINT nID);
	void 	OnUpdateCheckB(UINT nID);
	void 	OnUpdateCheckC(UINT nID);
	void	OnUpdateRangeOfIDs(UINT nIDFirst, UINT nIDLast, UINT nID);
	void 	ShowHelpTopic(UINT nID);

protected:
	virtual BOOL 	LoadRegistrySettings(LPCTSTR szKeyName);
	virtual BOOL 	OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  	OnCreate(CREATESTRUCT& cs);
	virtual void 	OnInitialUpdate();
	virtual void 	OnMenuUpdate(UINT nID);
	virtual LRESULT OnLButtonDown(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSetCursor(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual void	PreCreate(CREATESTRUCT& cs);
	virtual BOOL 	SaveRegistrySettings();
	virtual void 	SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void	NotImplemented() const;

	CView 		m_View;
	CDoc 		m_Doc;
	CAppHelp	m_AppHelp;	// Help object for context help and help about dialog
};


#endif //MAINFRM_H

