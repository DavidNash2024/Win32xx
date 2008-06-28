/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "../WinDev++/frame.h"
#include "View.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	virtual void OnBeforeNavigate(DISPPARAMS* pDispParams);
	virtual void OnCommandStateChange(DISPPARAMS* pDispParams);
	virtual void OnDocumentBegin(DISPPARAMS* pDispParams);
	virtual void OnDocumentComplete(DISPPARAMS* pDispParams);
	virtual void OnDownloadBegin(DISPPARAMS* pDispParams);
	virtual void OnDownloadComplete(DISPPARAMS* pDispParams);
	virtual void OnNavigateComplete2(DISPPARAMS* pDispParams);
	virtual void OnNewWindow2(DISPPARAMS* pDispParams);
	virtual void OnProgressChange(DISPPARAMS* pDispParams);
	virtual void OnPropertyChange(DISPPARAMS* pDispParams);
	virtual void OnStatusTextChange(DISPPARAMS* pDispParams);
	virtual void OnTimer(WPARAM wParam);
	virtual void OnTitleChange(DISPPARAMS* pDispParams);

protected:
	void AddListboxBand(int Listbox_Height);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual LRESULT WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CView m_View;
	CWnd m_ComboboxEx;
};

#endif //MAINFRM_H

