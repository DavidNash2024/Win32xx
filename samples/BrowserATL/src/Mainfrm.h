/////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MyCombo.h"
#include "View.h"


#define UWM_BEFORENAVIGATE2       (WM_APP + 0x0001)
#define UWM_COMMANDSTATECHANGE    (WM_APP + 0x0002)
#define UWM_DOCUMENTBEGIN         (WM_APP + 0x0003)
#define UWM_DOCUMENTCOMPLETE      (WM_APP + 0x0004)
#define UWM_DOWNLOADBEGIN         (WM_APP + 0x0005)
#define UWM_DOWNLOADCOMPLETE      (WM_APP + 0x0006)
#define UWM_NAVIGATECOMPLETE2     (WM_APP + 0x0007)
#define UWM_NEWWINDOW2            (WM_APP + 0x0008)
#define UWM_PROGRESSCHANGE        (WM_APP + 0x0009)
#define UWM_PROPERTYCHANGE        (WM_APP + 0x000A)
#define UWM_STATUSTEXTCHANGE      (WM_APP + 0x000B)
#define UWM_TITLECHANGE           (WM_APP + 0x000C)


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame(void);
    virtual ~CMainFrame();

    IWebBrowser2* GetBrowser() { return m_view.GetIWebBrowser2(); }
    const CEdit* GetCBEdit() const { return m_combo.GetCBEdit(); }

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
    void AddComboBoxBand(int Listbox_Height);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
    CView     m_view;
    CMyCombo  m_combo;
};

#endif //MAINFRM_H

