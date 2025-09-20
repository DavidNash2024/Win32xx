/* (26-Mar-2025)                                                      (View.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the  CView class for the
    CommonDialogs sample application using the Win32++ Windows interface
    classes. This particular view class utilizes a rich edit control to
    contain, view, and manage the document's data.  It also  applies methods
    for setting the view's foreground and background colors and the edit box
    font.

*******************************************************************************/

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

// Identifiers of entries in the m_colorTable array.
enum CtlColors
{
    DfltClr = 0,             // default color
    DlgBg,                   // dialog background
    REdTxFg,                 // rich edit text foreground
    REdTxBg,                 // rich edit text background
    REdBg                    // rich edit control background
};

class CView : public CDialog
{
public:
    CView(UINT id);
    virtual ~CView() override = default;

    virtual HWND   Create(HWND) override;
    CRichEditView& GetRichView() { return m_richView;}
    CFont          GetEditFont() { return m_editFont;}
    void           NoDocOpen();
    void           OnPageSetup();
    void           OnPrintDocument(LPCWSTR);
    void           SetEditFont(const CFont& f);
    void           SetBgColor(const CBrush& br) { m_bgBrush = br;}
    void           SetRichEditColors(COLORREF, COLORREF, COLORREF);

protected:
    virtual INT_PTR DialogProc(UINT, WPARAM, LPARAM) override;
    virtual void    OnCancel() override {}  // Suppress keys closing the dialog
    virtual void    OnClose() override {}   // Suppress keys closing the dialog
    virtual BOOL    OnInitDialog() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    BOOL        AddToolTip(HWND, UINT id);
    BOOL        AddToolTip(HWND, UINT id, LPCWSTR s);
    void        AssignToolTips();
    INT_PTR     OnCtlColor(UINT, WPARAM, LPARAM);

    HWND            m_parent;       // handle of parent frame
    CToolTip        m_toolTip;      // form tool tips
    CFont           m_editFont;     // edit box font
    CBrush          m_bgBrush;      // backbround brush object
    CRichEditView   m_richView;     // the view of the document
};

#endif // SDI_VIEW_H
