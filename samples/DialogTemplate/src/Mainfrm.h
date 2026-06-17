/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "DialogsTree.h"
#include "DialogHolder.h"
#include "AboutDialog.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    void DialogFromTemplateText();
    CString GetFileName() const;
    CDialogsTree* GetTree();
    void UpdateToolbar();
    void ShowTemplateText(HMODULE module, LPCWSTR dialogRes);

protected:
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

    // Called when the mainframe window's DPI has changed.
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override
    {
        // Let the mainframe window process its own DPI change first.
        LRESULT result = CDockFrame::OnDpiChanged(msg, wparam, lparam);

        // Destroy and recreate the dialog with the new DPI.
        if (m_holder.IsWindow() && m_holder.m_dialog.IsWindow())
            m_holder.ShowDialog(this, m_dialogArray.data());

        return result;
    }

    // Called when the mainframe's position changes. We lock the dialog in
    // place relative to the mainframe's position.
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam) override
    {
        // Allow the frame to finish processing its own position change first.
        CDockFrame::OnWindowPosChanged(msg, wparam, lparam);

        // Safely verify that your Dialog Holder and Dialog exist and are active
        // Replace 'm_dialogHolder' with your actual member variable name
        if (m_holder.IsWindow() && m_holder.m_dialog.IsWindow())
        {
            // Don't rerun the whole ShowDialog (which destroys/creates the window).
            // Just extract the dimensions and shift coordinates dynamically.
            DWORD style = m_holder.m_dialog.GetStyle();
            LPWINDOWPOS lpwndpos = (LPWINDOWPOS)lparam;
            if (style & WS_CHILD)
            {
                // Calculate the new positon of the dialog holder.
                CRect holderRect = m_holder.GetWindowRect();
                int left = lpwndpos->x + lpwndpos->cx - holderRect.Width();
                int top = lpwndpos->y + lpwndpos->cy - holderRect.Height();

                // Set the dialog holder's position.
                m_holder.SetWindowPos(nullptr, left, top, 0, 0,
                    SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
            else
            {
                // Calculate the now position of the top level dialog.
                CRect exactScaledRect = m_holder.m_dialog.GetWindowRect();
                int left = lpwndpos->x + lpwndpos->cx - exactScaledRect.Width();
                int top = lpwndpos->y + lpwndpos->cy - exactScaledRect.Height();

                // Set the position of the top level dialog.
                m_holder.m_dialog.SetWindowPos(nullptr, left, top, 0, 0,
                    SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
            }
        }

        return 0;
    }


private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    // Command handlers
    BOOL OnEditCopy();
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFilePrint();
    BOOL OnFilePreview();
    BOOL OnFileSave();
    BOOL OnPreviewClose();
    BOOL OnPreviewPrint();
    BOOL OnPreviewSetup();

    // Message handlers
    LRESULT OnDropFile(WPARAM wparam);
    LRESULT OnSelectTreeItem();

    // Callback procedures
    static DWORD CALLBACK MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG* pcb);

    // Private member functions
    void Reset();
    BOOL WriteFile(LPCWSTR szFileName);

    // Private member variables
    CAboutDialog m_aboutDialog;
    CRichView m_richView;
    CPrintPreview<CRichView>  m_preview;   // CRichView is the source of for CPrintPreview
    CString m_pathName;
    CDockDialogsTree* m_pDockDialogsTree;
    CResourceFinder m_finder;
    CDialogHolder m_holder;
    std::vector<unsigned char> m_dialogArray;
    bool m_isTemplateShown;
};

#endif //MAINFRM_H

