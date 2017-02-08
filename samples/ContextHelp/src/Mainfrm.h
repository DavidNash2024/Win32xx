/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "wxx_frame.h"
#include "AppHelp.h"
#include "App.h"
#include "View.h"
#include "Doc.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();

	void	ChooseHelpTopic();
	CDoc&	GetDoc()	{ return m_Doc; }
	UINT	GetIDFromCursorPos();
	CString MakeAppDataPath(const CString& subpath);
	void	OnF1();
	void 	OnFileExit();
	void	OnShiftF1();
	void 	OnUpdateCheckA(UINT nID);
	void 	OnUpdateCheckB(UINT nID);
	void 	OnUpdateCheckC(UINT nID);
	void 	OnUpdateRangeOfIds_Radio(UINT nID);
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

	void NotImplemented() const;

	CView 		m_View;
	CDoc 		m_Doc;
	BOOL    	m_IsChoosingTopic;	// TRUE while choosing a topic
	CAppHelp	m_AppHelp;			// Help object for context help 
	CHelpAbout	m_HelpAbout;		// the AboutBox dialog

};

// A global function to provide access to the CDoc class
CDoc& GetDoc();

#endif //MAINFRM_H

