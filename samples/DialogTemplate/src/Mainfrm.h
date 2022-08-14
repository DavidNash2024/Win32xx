/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "DialogsTree.h"
#include "DialogHolder.h"


///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CDockFrame
{
public:
    CMainFrame();

    // Virtual functions that override public base class functions
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);

    void DialogFromTemplateText();
    CString GetFileName() const;
    CDialogsTree* GetTree();
    void UpdateToolbar();
    void ShowTemplateText(HMODULE module, LPCTSTR dialogRes);

protected:
    // Virtual functions that override base class functions
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int     OnCreate(CREATESTRUCT& cs);
    virtual void    OnInitialUpdate();
    virtual void    OnMenuUpdate(UINT id);
    virtual void    SetupMenuIcons();
    virtual void    SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CMainFrame(const CMainFrame&);                // Disable copy construction
    CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

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
    BOOL WriteFile(LPCTSTR szFileName);

    // Private member variables
    CPrintPreview<CRichView>  m_preview;   // CRichView is the source of for CPrintPreview
    CRichView m_richView;
    CString m_pathName;
    CDockDialogsTree* m_pDockDialogsTree;
    CResourceFinder m_finder;
    CDialogHolder m_holder;
    bool m_isTemplateShown;
};

#endif //MAINFRM_H

