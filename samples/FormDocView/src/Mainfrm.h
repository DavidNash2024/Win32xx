/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "FormApp.h"
#include "FormView.h"
#include "FormDoc.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame();
	virtual ~CMainFrame();
	CFormDoc& GetDoc()		{ return m_Doc; }

protected:
	virtual BOOL LoadRegistrySettings(LPCTSTR szKeyName);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual int  OnCreate(CREATESTRUCT& cs);
	virtual void OnFileExit();
	virtual void OnInitialUpdate();
	virtual void PreCreate(CREATESTRUCT& cs);
	virtual void OnMenuUpdate(UINT nID);
	virtual BOOL SaveRegistrySettings();
	virtual void SetupToolBar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	void OnUpdateCheckA(UINT nID);
	void OnUpdateCheckB(UINT nID);
	void OnUpdateCheckC(UINT nID);
	void OnUpdateRangeOfIds_Radio(UINT nID);

	CFormView m_SdiView;
	CFormDoc m_Doc;
};


#endif //MAINFRM_H

