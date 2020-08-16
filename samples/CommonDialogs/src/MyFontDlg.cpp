/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]         (MyFontDialog.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the MyFontDialog class for
    applications using the Win32++ Windows interface classes. This class
    derives from the CFontDalog class found in the framework. 
    
    Programming Notes: The programming standards roughly follow those 
    established by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
    Planning and Preparation Subsystem project for C++ programming.

*******************************************************************************/

#include "stdafx.h"
#include "MyFontDlg.h"

/*============================================================================*/
    MyFontDialog::
MyFontDialog(DWORD dwFlags, HDC hdcPrinter /* = 0 */)           /*

    Create a simple default font dialog object with the title "Font", a
    10pt Courier New typeface, and font, style, strikeout, underline, text
    color, as well as other options specified by dwFlags. The hdcPrinter
    parameter, if non-zero, is a handle to a printer device context for the
    printer on which the fonts are to be selected.
*-----------------------------------------------------------------------------*/
    : CFontDialog(dwFlags | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS |
    CF_ENABLEHOOK, hdcPrinter)
{
    ZeroMemory(&m_tm, sizeof(m_tm));
    SetBoxTitle(_T("Font"));
      // default font, 10pt Courier New
    m_font.CreatePointFont(10, _T("Courier New"));
    SetChoiceFont(m_font);
}

/*============================================================================*/
    BOOL MyFontDialog::
OnInitDialog()                                                          /*

    Perform any actions required for initialization of this object when
    the font choice is being initialized.
*-----------------------------------------------------------------------------*/
{
      // do the parent class initialization first (it currently does nothing)
    CDialog::OnInitDialog();
    SetWindowTitle();
    return TRUE;
}

/*============================================================================*/
    void MyFontDialog::
OnOK()                                                                  /*

    This member is caused to execute after the () member has
    terminated with the OK button activated. The action here is just to
    create the logfont form of the selected font.
*-----------------------------------------------------------------------------*/
{
    LOGFONT lf = GetLogFont();
    SetChoiceLogFont(lf);
}

/*============================================================================*/
    void MyFontDialog::
RecordFontMetrics()                                                     /*

    Record the current font TEXTMETRIC values and the average character
    width and height for the given parent window whose handle is
    m_cf.hwndOwner, if non-NUL, or for the entire screen if NULL.
*-----------------------------------------------------------------------------*/{
      // get the handle to the hWnd's device context
    CWindowDC dc(GetParameters().hwndOwner);

      // select the current font into the device context:
      // save the old
    CFont oldfont = dc.SelectObject(m_font);

      // measure the font width and height
    dc.GetTextMetrics(m_tm);
    m_fontSize.cx = m_tm.tmAveCharWidth;
    m_fontSize.cy = m_tm.tmHeight + m_tm.tmExternalLeading;

      // restore entry environment
    dc.SelectObject(oldfont);
}

/*============================================================================*/
    void MyFontDialog::
Serialize(CArchive &ar)                                                 /*

        Called to serialize the current font dialog object to or deserialize it
    from the archive ar, depending on the sense of IsStoring().  Leaves the
    archive open for for further operations.
*-----------------------------------------------------------------------------*/
{
    // perform loading or storing
    if (ar.IsStoring())
    {
          // save the font as a LOGFONT
        LOGFONT lf = GetLogFont();
        ArchiveObject f(&lf, sizeof(LOGFONT));              
            ar << f;
          // save m_tm
        ArchiveObject g(GetTextMetric(), sizeof(TEXTMETRIC));
        ar << g;
          // save character height and width
        ar << GetAvgSize();
          // save the font color
        ar << GetParameters().rgbColors;
    }
    else 
    {     // recover the font as LOGFONT
        LOGFONT lf;
        ArchiveObject f(&lf, sizeof(LOGFONT));              
        ar >> f;                                            
        CHOOSEFONT cf = GetParameters();                    
        cf.lpLogFont = &lf;                                 
        SetParameters(cf);                                  
        SetChoiceLogFont(lf);                               
          // recover the text metrics
        ArchiveObject g(GetTextMetric(), sizeof(TEXTMETRIC));
        ar >> g;
          // recover character height and width
        SIZE z;
        ar >> z;
        m_fontSize = z;
          // retrieve the font color
        COLORREF rgb;
        ar >> rgb;
        SetColor(rgb);          
    }
}

/*============================================================================*/
    void MyFontDialog::
SetFontIndirect(const LOGFONT& lf)                                      /*

    Set the current font to have the characteristics contained in the
    supplied LOGFONT structure lf. Copy the face name into the font choice
    style name.
*-----------------------------------------------------------------------------*/
{
  // convert lf to a CFont
    try
    {
        m_font.CreateFontIndirect(lf);

        // if it worked, put it in this object
        m_logFont = lf;
        CHOOSEFONT cf = GetParameters();
        cf.lpLogFont = &m_logFont;
        SetParameters(cf);
    }
    
    catch (const CResourceException&) 
    {
        ::MessageBox(NULL, _T("Font creation error."),
            _T("Error"), MB_OK | MB_ICONEXCLAMATION |
            MB_TASKMODAL);
    }
}
/*----------------------------------------------------------------------------*/
