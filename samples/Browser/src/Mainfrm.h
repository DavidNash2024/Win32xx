/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MyCombo.h"
#include "EventSink.h"


//////////////////////////////////////
// Declaration of the CMainFrame class
//
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    void AddComboBoxBand(int height);
    void ConnectEvents();
    void DisconnectEvents();
    IWebBrowser2* GetIWebBrowser2() const { return m_browser.GetIWebBrowser2(); }
    const CEdit* GetCBEdit() const  { return m_combo.GetCBEdit(); }
    IConnectionPoint* GetConnectionPoint(REFIID riid);

    void OnBeforeNavigate2(DISPPARAMS* pDispParams);
    void OnCommandStateChange(DISPPARAMS* pDispParams);
    void OnDocumentComplete(DISPPARAMS* pDispParams);
    void OnDownloadBegin(DISPPARAMS* pDispParams);
    void OnDownloadComplete(DISPPARAMS* pDispParams);
    void OnNavigateComplete2(DISPPARAMS* pDispParams);
    void OnNewWindow2(DISPPARAMS* pDispParams);
    void OnProgressChange(DISPPARAMS* pDispParams);
    void OnPropertyChange(DISPPARAMS* pDispParams);
    void OnStatusTextChange(DISPPARAMS* pDispParams);
    void OnTitleChange(DISPPARAMS* pDispParams);

    BOOL OnBack();
    BOOL OnFileExit();
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnForward();
    BOOL OnHelpAbout();
    BOOL OnHome();
    BOOL OnPrint();
    BOOL OnPrintPreview();
    BOOL OnRefresh();
    BOOL OnStop();

protected:
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int  OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CWebBrowser m_browser;          // Win32++'s web browser defined in wxx_webbrowser.h
    CMyCombo    m_combo;            // ComboBoxEx control used in the toolbar.
    CEventSink  m_eventSink;        // Routes event notifications from IWebBrowser.
    DWORD       m_eventCookie;      // Token that uniquely identifies this connection.
};


#endif //MAINFRM_H

