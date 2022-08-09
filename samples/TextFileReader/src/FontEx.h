/* (31-Dec-2017) [Tab/Indent: 4/4][Line/Box: 80/74]                 (FontEx.h) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of an extension of the CFont class
    using the Win32++ Windows interface classes. Extensions include font size,
    color, font dialog option flags, and coordinate mapping mode.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef CFONTEX_H
#define CFONTEX_H

#include "stdafx.h"

/*============================================================================*/
    class
CFontExDialog : public CFontDialog                                          /*

    Extend the CFontDialog class to change the window title.
*-----------------------------------------------------------------------------*/
{
    public:
        CFontExDialog(DWORD flags = 0, HDC printer = 0)
            : CFontDialog(flags | CF_ENABLEHOOK, printer)
                { SetBoxTitle(_T("Font...")); }
        virtual ~CFontExDialog(){}

        void    SetBoxTitle(const CString& title)
                    { m_dlgBoxTitle = title; }

    protected:
        BOOL    OnInitDialog()
                    {  SetWindowText(m_dlgBoxTitle); return TRUE; }

    private:
        CFontExDialog(const CFontExDialog&);                // Disable copy construction
        CFontExDialog& operator = (const CFontExDialog&);   // Disable assignment operator

        CString m_dlgBoxTitle;
};

/*============================================================================*/
    class
CFontEx : public CObject                                                       /*

    A class to extend font information to include its characteristics,
    color, initialization flags for the font dialog box, and size within
    a given device context, with serialization.
*-----------------------------------------------------------------------------*/
{
    public:
        CFontEx();
        CFontEx(COLORREF txtcolor, DWORD options)
                    { m_txcolor = txtcolor; m_flags = options; }
        virtual ~CFontEx(){}

        void    Choose(LPCTSTR wintitle = NULL);
        void    SetDefault();

        DWORD   GetFlags() const        { return m_flags; }
        CFont&  GetFont()               { return m_font; }
        CSize   GetSize() const         { return m_fontSize; }
        COLORREF GetTxColor() const     { return m_txcolor; }
        void    SetFlags(DWORD f)       { m_flags = f; }
        void    SetFont(CFont f)        { m_font = f; SaveFontSize(); }
        void    SetTxColor(COLORREF c)  { m_txcolor = c; }

    protected:
        void    Serialize(CArchive &ar);

    private:
        CFontEx(const CFontEx&);                // Disable copy construction
        CFontEx& operator = (const CFontEx&);   // Disable assignment operator

        void    SaveFontSize();

        CFont    m_font;
        COLORREF m_txcolor;
        DWORD    m_flags;
        CSize    m_fontSize;
};
/*----------------------------------------------------------------------------*/
#endif // CFONTEX_H
