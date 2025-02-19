/* (22-Oct-2024) [Tab/Indent: 4/4][Line/Box: 80/74]               (FontEx.cpp) *
********************************************************************************
|                                                                              |
|                Authors: Robert Tausworthe, David Nash, 2020                  |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of an extension of the CFont class
    using the Win32++ Windows interface classes. Extensions include font
    size, color, font dialog option flags, and coordinate mapping mode.

     Programming Notes: The programming style roughly follows that established
     got the 1995-1999 Jet Propulsion Laboratory Network Planning and
     Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
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
Choose(LPCWSTR wintitle /* =  nullptr */)                                      /*

    Select the object font typeface, characteristics, color, and size in the
    device context dc. The font background color is presumed to be supplied by
    the application. Label the choice box using wintitle.
*-----------------------------------------------------------------------------*/
{
      // load the current font
    LOGFONT lf = m_font.GetLogFont();
      // open the dialog
    CFontExDialog fd;
    if (wintitle != nullptr)
        fd.SetBoxTitle(wintitle);
    CHOOSEFONT cf = fd.GetParameters();
      // display effects and color boxes, and use logfont provided, as
      // well as any other initializations in flags
    cf.Flags |= CF_SCREENFONTS |CF_EFFECTS | CF_INITTOLOGFONTSTRUCT | m_flags;
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
    CClientDC dc(0);
    dc.SelectObject(m_font);
      // measure the font width and height
    TEXTMETRIC tm;
    dc.GetTextMetrics(tm);
    m_fontSize.cx = tm.tmAveCharWidth;
    m_fontSize.cy = tm.tmHeight + tm.tmExternalLeading;
}

/*============================================================================*/
    void CFontEx::
SetDefault()                                                                /*

    Create the default view font. Throw an exception if it cannot be created.
*-----------------------------------------------------------------------------*/
{
    m_txcolor = RGB(0, 0, 0);
    constexpr int fontSize = 10;
    constexpr int tenthsOfPoint = 10;
    LPCWSTR fontName   = L"Courier New";
    m_font.CreatePointFont(tenthsOfPoint * fontSize, fontName);
    SaveFontSize();
}
/*----------------------------------------------------------------------------*/
