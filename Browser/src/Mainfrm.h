/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "MyBrowser.h"
#include "MyCombo.h"
#include "EventSink.h"

typedef std::basic_stringstream<TCHAR> tStringStream;

// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
	CMainFrame(void);
	virtual ~CMainFrame();
	virtual void ConnectEvents();
	virtual CMyBrowser& GetBrowser() const { return (CMyBrowser&)m_View; }
	virtual IConnectionPoint* GetConnectionPoint(REFIID riid);
	virtual void OnBeforeNavigate2(DISPPARAMS* pDispParams);
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
	virtual void OnTitleChange(DISPPARAMS* pDispParams);

protected:
	void AddComboBoxBand(int Listbox_Height);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual void OnCreate();
	virtual void OnInitialUpdate();
	virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
	virtual void SetupToolbar();
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	CMyBrowser m_View;
	CEventSink		m_EventSink;		// Routes event notifications from IWebBrowser
	CMyCombo m_ComboboxEx;
	DWORD			m_eventCookie;		// Token that uniquely identifies this connection
};


#endif //MAINFRM_H

