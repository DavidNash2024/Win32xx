/* (14-Nov-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CColorExDialog and CView classes
    for this sample program using the Win32++ Windows interface classes.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CAPP_VIEW_H
#define CAPP_VIEW_H

#include "FontEx.h"
#include "Doc.h"

/*============================================================================*/
    class
CColorExDialog : public CColorDialog                                      /*

    Extend the CColorDialog class to change the window title.
*-----------------------------------------------------------------------------*/
{
    public:
        CColorExDialog(COLORREF init = 0, DWORD flags = 0)
            : CColorDialog(init, flags | CC_ENABLEHOOK | CC_RGBINIT | CC_ANYCOLOR)
                { SetBoxTitle(_T("Color..."));}
        virtual ~CColorExDialog(){}

        void    SetBoxTitle(const CString& title) { m_boxTitle = title;}

    protected:
        BOOL    OnInitDialog()
                    { SetWindowText(m_boxTitle);
                      return TRUE; }

    private:
        CColorExDialog(const CColorExDialog&);                // Disable copy construction
        CColorExDialog& operator = (const CColorExDialog&);   // Disable assignment operator

        CString m_boxTitle;
};

/*============================================================================*/
    class
CView : public CScrollView                                                  /*

    This class enables displaying the text lines of a document in a scrolling
    window whose parameters may be serialized.
*-----------------------------------------------------------------------------*/
{
    public:
        CView();
        virtual ~CView(){}

        COLORREF GetWndBkColor() const
                    { return GetScrollBkgnd().GetLogBrush().lbColor; }
        void    SetWndBkColor(COLORREF rgb)
                    { m_bkColor = rgb; SetScrollBkgnd(CBrush(rgb)); }
        CSize   GetFontSize() const
                    { return m_fontEx.GetSize(); }
        CDoc&   TheDoc() { return m_doc; }
        void    ClientFontChoice();
        void    OnColorChoice();
        void    SetDefaults();
        void    SetAppSize(BOOL keepPos = FALSE);
        void    SetScrollParameters();
        BOOL    ShowLineNumbers() { return m_showNumbers; }
        BOOL    ToggleLineNumbers();

    protected:
        void    OnInitialUpdate();
        void    OnDraw(CDC& memDC);
        LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
        void    PreCreate(CREATESTRUCT& cs);
        void    Serialize(CArchive& ar);

    private:
        CView(const CView&);                // Disable copy construction
        CView& operator = (const CView&);   // Disable assignment operator

        void    InitViewColors();
        void    TextLineOut(CDC&, UINT, UINT, LPCWSTR) const;

        CDoc     m_doc;           // the document
        CFontEx  m_fontEx;        // the view display font
        COLORREF m_bkColor,       // the view display background color
                 m_colors[16];    // custom colors defined
        BOOL     m_showNumbers;   // display line numbers if TRUE
        LPCWSTR  m_endOfView;     // end of document string
};
/*----------------------------------------------------------------------------*/
#endif  //CAPP_VIEW_H
