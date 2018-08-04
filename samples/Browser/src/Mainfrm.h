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
    CWebBrowser& GetBrowser() const { return (CWebBrowser&)m_view; }
    CEdit* GetComboEdit() const { return m_comboboxEx.GetEdit(); }
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
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CWebBrowser m_view;
    CEventSink  m_eventSink;        // Routes event notifications from IWebBrowser
    CMyCombo    m_comboboxEx;
    DWORD       m_eventCookie;      // Token that uniquely identifies this connection
};


#endif //MAINFRM_H

