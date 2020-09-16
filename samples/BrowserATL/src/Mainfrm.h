/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MyCombo.h"
#include "View.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame(void);
    virtual ~CMainFrame();

    IWebBrowser2* GetBrowser() { return m_view.GetIWebBrowser2(); }
    const CEdit* GetCBEdit() const { return m_combo.GetCBEdit(); }

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
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnHelpAbout();
    BOOL OnFileExit();
    BOOL OnForward();
    BOOL OnHome();
    BOOL OnPrint();
    BOOL OnPrintPreview();
    BOOL OnRefresh();
    BOOL OnStop();

protected:
    void AddComboBoxBand(int height);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CView     m_view;
    CMyCombo  m_combo;
};

#endif //MAINFRM_H

