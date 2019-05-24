/* (31-Dec-2017) [Tab/Indent: 4/4][Line/Box: 80/74]                 (FontEx.h) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of an extension of the CFont class 
    using the Win32++ Windows interface classes, Copyright c) 2005-2019 
    David Nash, under permissions granted therein. Extensions include font 
    size, color, font dialog option flags, and coordinate mapping mode.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Network Planning 
    and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#pragma once
#ifndef CFONTEX_H
#define CFONTEX_H

#include "StdAfx.h"


static  const   COLORREF rgbDefaultTxColor  = RGB(0, 0, 0);
static  const   int      nDefaultFontSize   = 10;
static  const   TCHAR    lpszFaceDefault[]  = _T("Courier New"); 

/*============================================================================*/
    class
CFontExDialog : public CFontDialog                                          /*

    Extend the CFontDialog class to change the window title.
*-----------------------------------------------------------------------------*/
{
    public:
        CFontExDialog(DWORD dwFlags = 0, HDC hdcPrinter = 0)
            : CFontDialog(dwFlags | CF_ENABLEHOOK, hdcPrinter)
                { SetBoxTitle(_T("Font...")); }
        virtual ~CFontExDialog(){}

                void    SetBoxTitle(const CString& title)
                            { m_DlgBoxTitle = title; }

    protected:
        virtual BOOL    OnInitDialog()
                            {   CFontDialog::OnInitDialog();
                                SetWindowText(m_DlgBoxTitle);
                                return TRUE; }

    private:
        CString m_DlgBoxTitle;
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
        virtual void    Serialize(CArchive &ar);
            
    private:
        void    SaveFontSize();

          // protected data
        CFont    m_font;
        COLORREF m_txcolor; 
        DWORD    m_flags;
        CSize    m_fontSize;
};

/*----------------------------------------------------------------------------*/

#endif // CFONTEX_H
