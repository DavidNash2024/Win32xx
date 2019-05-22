/* (11-Nov-2016) [Tab/Indent: 8/8][Line/Box: 80/74]                (MainFrm.h) *
********************************************************************************

    Declaration of the CMainFrame class for the FilelPrintPreview program.

********************************************************************************

    Acknowledgement. This demo program was adapted from the PrintPreview 
    sample program appearing in the Win32++ framework sample folder, created 
    by  David Nash and published under the permissions granted in that work.
    The adaptation here reimplements the PrintView window as a separate 
    popup window that appears on screen along with the regular program 
    window. This work has been developed under the co-authorship of Robert
    Tausworthe and David Nash, and released under the copyright provisions 
    of the Win32++ Interface Classes software, copyright (c) David Nash,
    2005-2018. The former author acknowledges and thanks the latter for his 
    patient direction and inspiration in the development of the classes of 
    these classes.

*******************************************************************************/


#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "PrintPreview.h"

/*============================================================================*/
    class
CMainFrame : public CFrame                                              /*

    Declaration of the CMainFrame class.
*-----------------------------------------------------------------------------*/
{
    public:
        CMainFrame();
        virtual ~CMainFrame();

        const CString& GetPathName() const  { return m_PathName;}
        CRichView& GetRichView() { return m_RichView; }
        
        BOOL    OnDropFiles(HDROP hDropInfo);
        BOOL    OnEditCopy();
        BOOL    OnEditCut();
        BOOL    OnEditDelete();
        BOOL    OnEditPaste();
        BOOL    OnEditRedo();
        BOOL    OnEditUndo();
        BOOL    OnFileExit();
        BOOL    OnFileMRU(WPARAM wParam);
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

        void    QuickPrint(CPrintDialog& PrintDlg);
        BOOL    ReadFile(LPCTSTR szFileName);
        void    SetPathName(LPCTSTR szFilePathName)
                    {   m_PathName = szFilePathName;}
        void    SetWindowTitle();
        BOOL    WriteFile(LPCTSTR szFileName);

    protected:
        virtual void    OnClose();
        virtual BOOL    OnCommand(WPARAM wParam, LPARAM lParam);
        virtual int     OnCreate(CREATESTRUCT& cs);
        virtual void    OnInitialUpdate();
        virtual void    OnMenuUpdate(UINT nID);
        virtual LRESULT OnNotify(WPARAM wParam, LPARAM lParam);
        virtual void    PreCreate(CREATESTRUCT& cs);
                void    SaveModifiedText();
        virtual void    SetupMenuIcons();
        virtual void    SetupToolBar();
        virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);

    private:
        CPrintPreview   m_PrintPreview;
        CRichView       m_RichView;
        CString         m_PathName;
        WordWrapType    m_WrapOption;
};
/*----------------------------------------------------------------------------*/
#endif // MAINFRM_H
