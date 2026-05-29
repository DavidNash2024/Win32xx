/////////////////////////////////////////
// View.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "RichEditView.h"

// Identifiers of entries in the m_colorTable array.
enum CtlColors
{
    DfltClr = 0,             // Default color.
    DlgBg,                   // Dialog background.
    REdTxFg,                 // Rich edit text foreground.
    REdTxBg,                 // Rich edit text background.
    REdBg                    // Rich edit control background.
};

/////////////////////////////////////////////////////////////////////////
// This class provides a dialog that contains the rich edit control and a
// border.
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
    virtual void    OnCancel() override {}  // Suppress keys closing the dialog.
    virtual void    OnClose() override {}   // Suppress keys closing the dialog.
    virtual BOOL    OnInitDialog() override;
    virtual LRESULT OnNotify(WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    BOOL        AddToolTip(HWND, UINT id);
    BOOL        AddToolTip(HWND, UINT id, LPCWSTR s);
    void        AssignToolTips();
    INT_PTR     OnCtlColor(UINT, WPARAM, LPARAM);

    HWND            m_parent;       // Handle of parent frame.
    CToolTip        m_toolTip;      // Form tool tips.
    CFont           m_editFont;     // Edit box font.
    CBrush          m_bgBrush;      // Background brush object.
    CRichEditView   m_richView;     // The view of the document.
};

#endif // SDI_VIEW_H
