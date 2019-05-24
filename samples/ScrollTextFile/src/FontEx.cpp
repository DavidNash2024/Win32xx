/* (31-Dec-2017) [Tab/Indent: 4/4][Line/Box: 80/74]               (FontEx.cpp) *
********************************************************************************
|                                                                              |
|                         robert.c.tausworthe@ieee.org                         |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of an extension of the CFont class
    using the Win32++ Windows interface classes, Copyright (c) 2005-2019 
    David Nash, under permissions granted therein. Extensions include font 
    size, color, font dialog option flags, and coordinate mapping mode.

    Caveats: These materials are available under the same provisions as 
    found in the Win32++ copyright.txt notice.

    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Network Planning 
    and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "StdAfx.h"
#include "FontEx.h"

/*============================================================================*/
    CFontEx::
CFontEx()                                                                   /*

    Create an extended font object.
*-----------------------------------------------------------------------------*/
{
    m_txcolor = RGB(0, 0, 0);
    m_flags   = 0;
}

/*============================================================================*/
    void CFontEx::
Choose(LPCTSTR wintitle /* =  NULL */)                                      /*

    Select the object font typeface, characteristics, color, and size in the 
    device context dc. The font background color is presumed to be supplied by 
    the application. Label the choice box using wintitle.
*-----------------------------------------------------------------------------*/
{
      // load the current font
    LOGFONT lf;
    m_font.GetObject(sizeof(LOGFONT), &lf);
      // open the dialog
    CFontExDialog fd;
    if (wintitle != NULL)
        fd.SetBoxTitle(wintitle);
    CHOOSEFONT cf = fd.GetParameters();
      // display effects and color boxes, and use logfont provided, as
      // well as any other initializations in flags
    cf.Flags |= CF_SCREENFONTS |CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | 
        m_flags;
    cf.lpLogFont = &lf;
    cf.rgbColors = m_txcolor;
    fd.SetParameters(cf);
    if(fd.DoModal())
    {
          // bring choice elements into this view
        m_font.CreateFontIndirect(fd.GetLogFont());
        m_txcolor = fd.GetColor();
        SaveFontSize();
    }
}

/*============================================================================*/
    void CFontEx::
SaveFontSize()                                                              /*

    Record the width and height of the object font in client device units.
*-----------------------------------------------------------------------------*/
{
      // select the current font into a temporary device context
    CClientDC dc(NULL);
    dc.SelectObject(m_font);
      // measure the font width and height
    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    m_fontSize.cx = tm.tmAveCharWidth;
    m_fontSize.cy = tm.tmHeight + tm.tmExternalLeading;
}

/*============================================================================*/
    void CFontEx::
Serialize(CArchive &ar)                                                     /*

    Save and restore the current CFontEx object in the archive ar,
    maintaining compatibility between ANSI and UNICODE versions of archived
    objects. The difference between the two character representations lies
    in the lengths of the face name strings in the two modes.
*-----------------------------------------------------------------------------*/
{
    LOGFONT lf;
      // get the size of the LOGFONT, sans face name
    UINT lfTopLength = sizeof(LOGFONTA) - LF_FACESIZE*sizeof(CHAR);
      // f will archive this top part
    ArchiveObject f(&lf, lfTopLength);
    if (ar.IsStoring()) // storing
    {
        m_font.GetObject(sizeof(LOGFONT), &lf);
          // store the face name separately: ar recognizes the char mode
        CString face = lf.lfFaceName;
        ar << f;    // store the top part
        ar<< face;  // store the face name part
        ar << m_txcolor;  // store the rest
        ar << m_flags;
    }
    else    // recovering
    {
        CString face;
        ar >> f;    // recover the top part
        ar >> face; // recover the face name and put it in the LOGFONT
        memcpy(lf.lfFaceName, face.c_str(),
            (face.GetLength() + 1) * sizeof(TCHAR));
        m_font.CreateFontIndirect((const LOGFONT&)lf);
        SaveFontSize();
         // recover the rest
        ar >> m_txcolor;
        ar >> m_flags;
    }
}


/*============================================================================*/
        void CFontEx::
SetDefault()                                                                /*

    Create the default view font. Throw an exception if it cannot be created.
*-----------------------------------------------------------------------------*/
{
    m_font.CreatePointFont(10 * nDefaultFontSize, lpszFaceDefault);
    SaveFontSize();
    m_txcolor = rgbDefaultTxColor;
}
/*----------------------------------------------------------------------------*/
