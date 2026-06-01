/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "MyCombo.h"
#include "EdgeView.h"
#include "AboutDialog.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame that has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    LRESULT AddComboBoxBand();
    void AddStringToCombo(LPCWSTR str);
    const CEdit& GetCBEdit() const { return m_combo.GetCBEdit(); }
    void Navigate(LPCWSTR str);

protected:
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int  OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override
    {
        m_selectedItem = GetCBEdit().GetWindowText();
		m_combo.SaveItems();    

        // Delete the rebar band holding the combobox.
        if (GetReBar().IsWindow())
        {
            int band = GetReBar().GetBand(m_combo);
            GetReBar().DeleteBand(band);
        }

        // Call the base class function. This recreates the toolbars.
        return CFrame::OnDpiChanged(msg, wparam, lparam);
    }
    virtual BOOL OnHelp() override;
    virtual void OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void SetupMenuIcons() override;
    virtual void SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command Handlers
    BOOL OnBack();
    BOOL OnFileExit();
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnForward();
    BOOL OnHome();
    BOOL OnReload();
    BOOL OnStop();

    // Message handlers
    LRESULT OnGetMinMaxInfo(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnHistoryChanged(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNavigationCompleted(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnNavigationStarted(UINT msg, WPARAM wparam, LPARAM lparam);
    LRESULT OnSourceChanged(UINT msg, WPARAM wparam, LPARAM lparam);

    // Member variables
    CAboutDialog  m_aboutDialog;    // Help About dialog.
    CEdgeView     m_browser;        // WebView2 control implements an edge browser.
    CMyCombo      m_combo;          // ComboBoxEx control used in the toolbar.
    CString       m_selectedItem;   // Currently selected ComboBoxEx item.
};


#endif //MAINFRM_H

