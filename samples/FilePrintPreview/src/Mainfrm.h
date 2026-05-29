/////////////////////////////////////////
// Mainfrm.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MAINFRM_H
#define MAINFRM_H

#include "AboutDialog.h"
#include "RichView.h"
#include "PrintPreview.h"

/////////////////////////////////////////////////////////////////////////////
// This class manages the main frame of the application. It contains a
// CRichView object which is the view of the application. It also manages the
// menu and toolbar commands, and the MRU file list.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;
    const CString& GetPath() const  { return m_path;}
    CRichView&  GetRichView()       { return m_richView; }

    BOOL    OnDropFiles(HDROP dropInfo);
    BOOL    OnEditCopy();
    BOOL    OnEditCut();
    BOOL    OnEditDelete();
    BOOL    OnEditPaste();
    BOOL    OnEditRedo();
    BOOL    OnEditUndo();
    BOOL    OnFileExit();
    BOOL    OnFileMRU(WPARAM wparam);
    BOOL    OnFileNew();
    BOOL    OnFileOpen();
    BOOL    OnFilePreview();
    BOOL    OnFilePrint(HWND parent);
    BOOL    OnFilePrintSetup(HWND parent);
    BOOL    OnFileQuickPrint();
    BOOL    OnFileSave();
    BOOL    OnFileSaveAs();
    BOOL    OnOptionsFont();
    BOOL    OnOptionsWrap(WordWrapType);
    void    QuickPrint(CPrintDialog& printDlg);
    BOOL    ReadFile(LPCWSTR path);
    void    SaveModifiedText();
    void    SetPath(LPCWSTR path);
    BOOL    WriteFile(LPCWSTR path);

protected:
    virtual void    OnClose() override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& cs) override;
    virtual BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual void    OnMenuUpdate(UINT id) override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT& cs) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    CAboutDialog    m_aboutDialog;
    CPrintPreviewEx m_printPreview;
    CRichView       m_richView;
    CString         m_path;
    WordWrapType    m_wrapOption;
};

#endif // MAINFRM_H
