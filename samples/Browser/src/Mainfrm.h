/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H


#include "MyCombo.h"
#include "EventSink.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    
    void ConnectEvents();
    CWebBrowser& GetBrowser() const { return (CWebBrowser&)m_View; }
    CEdit* GetComboEdit() const { return m_ComboboxEx.GetEdit(); }
    IConnectionPoint* GetConnectionPoint(REFIID riid);
    void OnBeforeNavigate2(DISPPARAMS* pDispParams);
    void OnCommandStateChange(DISPPARAMS* pDispParams);
    void OnDocumentBegin(DISPPARAMS* pDispParams);
    void OnDocumentComplete(DISPPARAMS* pDispParams);
    void OnDownloadBegin(DISPPARAMS* pDispParams);
    void OnDownloadComplete(DISPPARAMS* pDispParams);
    void OnNavigateComplete2(DISPPARAMS* pDispParams);
    void OnNewWindow2(DISPPARAMS* pDispParams);
    void OnProgressChange(DISPPARAMS* pDispParams);
    void OnPropertyChange(DISPPARAMS* pDispParams);
    void OnStatusTextChange(DISPPARAMS* pDispParams);
    void OnTitleChange(DISPPARAMS* pDispParams);
    BOOL OnFileExit();
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnHelpAbout();
    BOOL OnForward();
    BOOL OnBack();
    BOOL OnRefresh();
    BOOL OnHome();
    BOOL OnStop();


protected:
    void AddComboBoxBand(int Listbox_Height);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CWebBrowser m_View;
    CEventSink  m_EventSink;        // Routes event notifications from IWebBrowser
    CMyCombo    m_ComboboxEx;
    DWORD       m_eventCookie;      // Token that uniquely identifies this connection
};


#endif //MAINFRM_H

