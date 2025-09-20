/* (26-Mar-2025)                                                   (MainFrm.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CMainFrame class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes.

*******************************************************************************/

#ifndef SDI_MAINFRM_H
#define SDI_MAINFRM_H


class CMainFrame : public CFrame
{
public:
    CMainFrame();
    virtual ~CMainFrame() override = default;
    virtual HWND Create(HWND parent = nullptr) override;

    AboutBox& GetAboutBox() { return m_aboutBox; }
    void    SetDocExt(LPCWSTR ext)       { m_docExt = ext; }
    void    SetDocFilter(LPCWSTR filter) { m_docFilter = filter; }
    void    SetWindowTitle(LPCWSTR);
    void    UpdateToolbar();

protected:
    virtual void    OnMenuUpdate(UINT id) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual int     OnCreate(CREATESTRUCT& rcs) override;
    virtual         BOOL    OnHelp() override;
    virtual void    OnInitialUpdate() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;
    virtual void    SetupMenuIcons() override;
    virtual void    SetupToolBar() override;
    virtual LRESULT WndProc(UINT msg, WPARAM, LPARAM) override;

private:
    CMainFrame(const CMainFrame&) = delete;
    CMainFrame& operator=(const CMainFrame&) = delete;

    BOOL    DropFiles(LPARAM lparam);
    CRichEditView& GetRichView() { return m_view.GetRichView();}
    void    InitCtlColors();
    void    OnCloseDoc();
    void    OnColorChoice();
    void    OnEditFind();
    void    OnEditReplace();
    void    OnExit();
    BOOL    OnFileMRU(WPARAM wparam);
    void    OnFontChoice();
    void    OnNewDoc();
    void    OnOpenDoc();
    void    OnPrint();
    void    OnSave();
    void    OnSaveAs();
    void    OnWrapText();
    BOOL    OpenDoc(LPCWSTR);
    void    SetReBarColors(COLORREF, COLORREF, COLORREF, COLORREF);
    BOOL    SetRichEditColor();
    void    SetViewBgColor();

    CDoc         m_doc;                    // the document
    CView        m_view;                   // the view
    AboutBox     m_aboutBox;               // the AboutBox dialog
    CString      m_docDir;                 // for the file open/save dialogs
    CString      m_docExt;                 // document file extension
    CString      m_docFilter;              // document file open/save filter
    CBitmap      m_colorbmp;               // for the color choice menu item
    BOOL         m_isTextWrap;             // wrap text in rich edit if true
    CColorChoice m_colorChoice;            // the control color choice
    MyFontDialog m_fontChoice;             // edit control font
    MyFindReplaceDialog m_findReplaceDlg;  // find-replace dialog
};

#endif // SDI_MAINFRM_H
