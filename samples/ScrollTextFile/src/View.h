/* (14-Nov-2016) [Tab/Indent: 4/4][Line/Box: 80/74]                   (View.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the CScrollWnd demo application view 
    class for a typical Windows application using the Win32++ Windows interface 
    classes, Copyright (c) 2005-2017 David Nash, under permissions granted 
    therein.

    Caveats: These materials are available under the same provisions as found 
    in the Win32++ copyright.txt notice.

    Programming Notes:
        The programming standards roughly follow those established by the 
    1997-1999 Jet Propulsion Laboratory Network Planning and Preparation 
    Subsystem project for C++ programming.

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
        CColorExDialog(COLORREF clrInit = 0, DWORD dwFlags = 0)
            : CColorDialog(clrInit, dwFlags | CC_ENABLEHOOK |
            CC_RGBINIT | CC_ANYCOLOR)
        {
            SetBoxTitle(_T("Color..."));
        }

        ~CColorExDialog(){}

        void    SetBoxTitle(const CString& title) { m_sBoxTitle = title;}

    protected:
        virtual BOOL    OnInitDialog()
                        {
                            CColorDialog::OnInitDialog();
                            SetWindowText(m_sBoxTitle);
                            return TRUE;
                        }

    private:
        CString     m_sBoxTitle;
};

/*============================================================================*/
    class
CView : public CScrollView                                                  /*

    The application scrolling window serializable class.
*-----------------------------------------------------------------------------*/
{
public:
    CView();
    virtual ~CView();

    COLORREF GetWndBkColor() const
    {
        return GetScrollBkgnd().GetLogBrush().lbColor;
    }
    void    SetWndBkColor(COLORREF rgb)
    {
        SetScrollBkgnd(CBrush(rgb));
    }
    CSize   GetFontSize() const
    {
        return m_VuFont.GetSize();
    }
    CDoc&   TheDoc() { return m_Doc; }
    void    ClientFontChoice();
    void    OnColorChoice();
    void    SetDefaults();
    void    SetAppSize(BOOL keepPos = FALSE);
    void    SetScrollParameters();
    BOOL    ShowLineNumbers() { return m_ShowLineNumbers; }
    void    ToggleLineNumbers()
    {
        m_ShowLineNumbers = !m_ShowLineNumbers;
        SetAppSize(TRUE);
    }

protected:
    void    OnInitialUpdate();
    void    OnDraw(CDC& dcMem);
    LRESULT OnPaint(UINT msg, WPARAM wparam, LPARAM lparam);
    void    PreCreate(CREATESTRUCT &cs);
    void    PreRegisterClass(WNDCLASS &wc);
    virtual void Serialize(CArchive &ar);
    LRESULT WndProc(UINT uMsg, WPARAM, LPARAM);

private:
    void    InitViewColors();
    void    TextLineOut(CDC&, UINT, UINT, LPCWSTR) const;

    // data members
    CDoc     m_Doc;                 // the document
    CFontEx  m_VuFont;              // the view display font
    COLORREF m_CustomColors[16];    // custom colors defined
    BOOL     m_ShowLineNumbers;     // display line nos if TRUE
};

/*----------------------------------------------------------------------------*/
#endif  //CAPP_VIEW_H
