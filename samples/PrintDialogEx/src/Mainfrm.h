/////////////////////////////
// Mainfrm.h
//

#ifndef MAINFRM_H
#define MAINFRM_H

#include "RichView.h"
#include "resource.h"


// Encoding IDs
const int ANSI         = 0;            // Default for plain text
const int UTF8         = 1;            // Default for rich text
const int UTF16LE      = 2;
const int UTF16LE_BOM  = 3;



///////////////////////////////////////////////////////////
// CMainFrame manages the application's main window.
// The main window is a frame which has a menubar, toolbar,
// statusbar and view window.
class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame();
    virtual HWND Create(HWND parent = 0);
    void UpdateToolbar();

protected:
    // Virtual functions that override base class functions
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual int OnCreate(CREATESTRUCT& cs);
    virtual void OnInitialUpdate();
    virtual void OnMenuUpdate(UINT id);
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam);
    virtual void SetStatusIndicators();
    virtual void SetStatusParts();
    virtual void SetupMenuIcons();
    virtual void SetupToolBar();
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam);

private:
    // Static callback functions
    static  DWORD CALLBACK MyStreamInCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG* pcb);
    static  DWORD CALLBACK MyStreamOutCallback(DWORD cookie, LPBYTE pBuffer, LONG cb, LONG* pcb);

    // Command handlers
    void OnDropFiles(HDROP hDropInfo);
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
    BOOL OnFilePrintNow();
    BOOL OnFilePrintSetup();
    BOOL OnFileSaveAs();
    BOOL OnFileSave();
    BOOL OnOptionsFont();
    BOOL OnOptionsWrap();
    BOOL OnUpdateRangeOfIDs(UINT idFirst, UINT idLast, UINT id);
    BOOL OnPreviewClose();
    BOOL OnPreviewPrint();
    BOOL OnPreviewSetup();

    int  AdjustForDPI(int value) const;
    void ClearContents();
    void DetermineEncoding(CFile& file);
    int  GetTextPartWidth(LPCTSTR text) const;
    BOOL ReadFile(LPCTSTR fileName);
    void RestoreFocus() { ::SetFocus(m_oldFocus); }
    void SaveFocus() { m_oldFocus = ::GetFocus(); }
    void SaveModifiedText();
    void SetEncoding(int encoding);
    void SetPathName(LPCTSTR fullFileName);
    void SetWindowTitle();
    BOOL WriteFile(LPCTSTR fileName);

    // Member variables
    CPrintPreview<CRichView>  m_preview;   // CRichView is the source of for CPrintPreview
    CRichView m_richView;
    CString m_pathName;
    int m_encoding;
    bool m_isToolbarShown;
    bool m_isWrapped;
    bool m_isRTF;
    HWND m_oldFocus;
};

#endif //MAINFRM_H

