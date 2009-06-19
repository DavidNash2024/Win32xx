/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "frame.h"
#include "SdiApp.h"
#include "SdiView.h"
#include "SdiDoc.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	CSdiDoc& GetDoc() {return m_Doc;} 

protected:
	virtual void LoadRegistrySettings(LPCTSTR szKeyName);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual void OnMenuUpdate(UINT nID);
	virtual void SaveRegistrySettings();
	virtual void SetupToolbar();
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnUpdateCheckA(UINT nID);
	void OnUpdateCheckB(UINT nID);
	void OnUpdateCheckC(UINT nID);
	void OnUpdateRangeOfIds_Radio(UINT nID);

	CSdiView m_SdiView;
	CSdiDoc m_Doc;
};

// A global function to provide access to the CSdiDoc class
CSdiDoc& GetDoc(); 

#endif //MAINFRM_H

