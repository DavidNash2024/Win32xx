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
    void OnDropFiles(HDROP hDropInfo);
    void OnEditCut();
    void OnEditCopy();
    void OnEditPaste();
    void OnEditDelete();
    void OnEditRedo();
    void OnEditUndo();
    void OnEncodeANSI();
    void OnEncodeUTF8();
    void OnEncodeUTF16();
    void OnFileMRU(WPARAM wparam);
    void OnFileExit();
    void OnFileNewPlain();
    void OnFileNewRich();
    void OnFileOpen();
    void OnFilePreview();
    void OnFilePrint();
    void OnFilePrintSetup();
    void OnFileQuickPrint();
    void OnFileSaveAs();
    void OnFileSave();
    void OnOptionsFont();
    void OnOptionsWrap();
    void OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);
    void OnPreviewClose();
    void OnPreviewPrint();
    void OnPreviewSetup();
    void OnToolbarUpdate();

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
    CPrintPreview<CRichView>  m_preview;   // CRichView is the source of for CPrintPreview
    CRichView m_richView;
    CString m_oldStatus[4];                // Array of CString holding old status;
    CString m_pathName;
    Encoding m_encoding;
    bool m_isWrapped;
    bool m_isRTF;
    HWND m_oldFocus;
};

#endif //MAINFRM_H

