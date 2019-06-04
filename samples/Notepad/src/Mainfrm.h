////////////////////////////////////////////////////
// Mainfrm.h

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "resource.h"


enum Encoding
{
    ANSI          = 0,            // Default for plain text
    UTF8          = 1,            // Default for rich text
    UTF16LE       = 2,
    UTF16LE_BOM   = 3,
};


// Declaration of the CMainFrame class
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    
    static  DWORD CALLBACK MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb);
    static  DWORD CALLBACK MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG *pcb);

    void ClearContents();
    void DetermineEncoding(CFile& file);
    BOOL OnDropFiles(HDROP hDropInfo);
    BOOL OnEditCut();
    BOOL OnEditCopy();
    BOOL OnEditPaste();
    BOOL OnEditDelete();
    BOOL OnEditRedo();
    BOOL OnEditUndo();
    BOOL OnEncodeANSI();
    BOOL OnEncodeUTF8();
    BOOL OnEncodeUTF16();
    BOOL OnFileMRU(WPARAM wparam);
    BOOL OnFileExit();
    BOOL OnFileNewPlain();
    BOOL OnFileNewRich();
    BOOL OnFileOpen();
    BOOL OnFilePreview();
    BOOL OnFilePrint();
    BOOL OnFilePrintSetup();
    BOOL OnFileQuickPrint();
    BOOL OnFileSaveAs();
    BOOL OnFileSave();
    BOOL OnOptionsFont();
    BOOL OnOptionsWrap();
    void OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);
    void    OnPreviewClose();
    void    OnPreviewPrint();
    void    OnPreviewSetup();

    BOOL ReadFile(LPCTSTR fileName);
    void RestoreFocus() { ::SetFocus(m_oldFocus); }
    void SaveFocus() { m_oldFocus = ::GetFocus(); }
    void SaveModifiedText();
    void SetEncoding(Encoding encoding);
    void SetPathName(LPCTSTR fullFileName);
    void SetStatusIndicators();
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
    CPrintPreview<CRichView>  m_printPreview;   // CRichView is the source of for CPrintPreview
    CRichView m_richView;
    CString m_oldStatus[4];                     // Array of CString holding old status;
    CString m_pathName;
    Encoding m_encoding;
    bool m_isWrapped;
    bool m_isRTF;
    HWND m_oldFocus;
};

#endif //MAINFRM_H

