/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CMainFrame class for the
    FilelPrintPreview program using the Win32++ Windows interface classes.

    Programming Notes: The programming style roughly follows that established
    for the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "PrintPreview.h"

/*============================================================================*/
    class
CMainFrame : public CFrame                                                  /*

    Declaration of the CMainFrame class.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame();
        virtual ~CMainFrame() {}
        virtual HWND Create(HWND parent = 0);
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
        BOOL    ReadFile(LPCTSTR path);
        void    SetPath(LPCTSTR path);
        BOOL    WriteFile(LPCTSTR path);

    protected:
        virtual void    OnClose();
        virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam);
        virtual int     OnCreate(CREATESTRUCT& cs);
        virtual void    OnInitialUpdate();
        virtual void    OnMenuUpdate(UINT id);
        virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
        virtual void    PreCreate(CREATESTRUCT& cs);
                void    SaveModifiedText();
        virtual void    SetupMenuIcons();
        virtual void    SetupToolBar();
        virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

    private:
        CMainFrame(const CMainFrame&);                // Disable copy construction
        CMainFrame& operator = (const CMainFrame&);   // Disable assignment operator

        CPrintPreviewEx m_printPreview;
        CRichView       m_richView;
        CString         m_path;
        WordWrapType    m_wrapOption;
};
/*----------------------------------------------------------------------------*/
#endif // MAINFRM_H
