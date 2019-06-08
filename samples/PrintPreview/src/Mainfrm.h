////////////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();

    CRichView& GetRichView() { return m_richView; }

    static  DWORD CALLBACK MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb);
    static  DWORD CALLBACK MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb);

    BOOL OnDropFiles(HDROP hDropInfo);
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnEditRedo();
    BOOL OnEditUndo();
    BOOL OnFileMRU(WPARAM wparam);
    BOOL OnFileExit();
    BOOL OnFileNew();
    BOOL OnFileOpen();
    BOOL OnFilePreview();
    BOOL OnFilePrint();
    BOOL OnFilePrintSetup();
    BOOL OnFileQuickPrint();
    BOOL OnFileSaveAs();
    BOOL OnFileSave();
    BOOL OnOptionsFont();
    BOOL OnOptionsWrap();
    void OnPreviewClose();
    void OnPreviewPrint();
    void OnPreviewSetup();

    BOOL ReadFile(LPCTSTR fileName);
    void RestoreFocus()         {::SetFocus(m_oldFocus);}
    void SaveFocus() { m_oldFocus = ::GetFocus(); }
    void SaveModifiedText();
    void SetPathName(LPCTSTR fullFileName);
    void SetWindowTitle();
    BOOL WriteFile(LPCTSTR fileName);

protected:
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);
    
private:
    CPrintPreview<CRichView>  m_preview;   // CRichView is the source of for CPrintPreview
    CRichView m_richView;
    CString m_pathName;
    bool m_isWrapped;
    HWND m_oldFocus;

};

#endif //MAINFRM_H

