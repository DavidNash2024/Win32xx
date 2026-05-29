/////////////////////////////////////////
// View.h
// Authors: Robert Tausworthe, David Nash
//

#ifndef CAPP_VIEW_H
#define CAPP_VIEW_H

#include "Doc.h"

///////////////////////////////////////////////////////////////////////////////
// The view class. It is responsible for displaying the document content, and
// for handling the user interactions with the view. It is also responsible for
// maintaining the view's display attributes, such as the background color,
// the text color, and the font. It is derived from CScrollView, which provides
// the scrolling functionality for the view.
class CView : public CScrollView
{
public:
    CView();
    virtual ~CView() override = default;

    // Attributes.
    COLORREF  GetBkgndColor() const      { return m_bkgndColor; }
    COLORREF* GetColorsArray()           { return m_colors; }
    COLORREF  GetTextColor()  const      { return m_textColor; }
    CFont     GetTextFont()   const      { return m_font; }
    void      SetBkgndColor(COLORREF bk) { m_bkgndColor = bk; }
    void      SetTextFont(CFont font)    { m_font = font; }
    void      SetTextColor(COLORREF col) { m_textColor = col; }
    CDoc&     TheDoc()                   { return m_doc; }

    // Operations.
    CSize   GetFontSize() const;
    void    SetAppSize(BOOL keepPos = FALSE);
    void    SetDefaults();
    void    TextLineOut(CDC&, UINT, UINT, LPCWSTR) const;

    // Member variables.
    static const CString m_compiledOn;  // Compilation date.

protected:
    virtual void    OnDraw(CDC& dc) override;
    virtual LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void    PreCreate(CREATESTRUCT &cs) override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CView(const CView&) = delete;
    CView& operator=(const CView&) = delete;

    // Member variables.
    COLORREF    m_bkgndColor;   // Background color.
    CDoc        m_doc;          // The document.
    CFont       m_font;         // The font used to display view.
    COLORREF    m_colors[16];   // Custom colors defined.
    COLORREF    m_textColor;    // Text foreground color.
};

#endif  //CAPP_VIEW_H
