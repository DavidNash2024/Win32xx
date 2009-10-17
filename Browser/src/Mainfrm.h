/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "stdafx.h"
#include "View.h"
#include "ComboBoxEx.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	virtual void Navigate();
	virtual void OnBeforeNavigate(DISPPARAMS* pDispParams);
	virtual void OnCommandStateChange(DISPPARAMS* pDispParams);
	virtual void OnDocumentBegin(DISPPARAMS* pDispParams);
	virtual void OnDocumentComplete(DISPPARAMS* pDispParams);
	virtual void OnDownloadBegin(DISPPARAMS* pDispParams);
	virtual void OnDownloadComplete(DISPPARAMS* pDispParams);
	virtual void OnEditCopy();
	virtual void OnEditCut();
	virtual void OnEditDelete();
	virtual void OnEditPaste();
	virtual void OnNavigateComplete2(DISPPARAMS* pDispParams);
	virtual void OnNewWindow2(DISPPARAMS* pDispParams);
	virtual void OnProgressChange(DISPPARAMS* pDispParams);
	virtual void OnPropertyChange(DISPPARAMS* pDispParams);
	virtual void OnStatusTextChange(DISPPARAMS* pDispParams);
	virtual void OnTimer(WPARAM wParam);
	virtual void OnTitleChange(DISPPARAMS* pDispParams);

protected:
	void AddComboBoxBand(int Listbox_Height);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CComboBoxEx m_ComboBoxEx;
	CComboEdit m_ComboEdit;
};

#endif //MAINFRM_H

