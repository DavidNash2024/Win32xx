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
    virtual BOOL    OnClickTreeItem();
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual LRESULT OnDpiChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual LRESULT OnGetDpiScaledSize(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    LRESULT OnWindowPosChanged(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

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
    static DWORD CALLBACK MyStreamOutCallback(DWORD_PTR cookie, LPBYTE pBuffer, LONG cb, LONG* pcb);

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
    bool m_isPosChanging;
};

#endif //MAINFRM_H

