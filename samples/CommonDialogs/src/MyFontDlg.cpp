/* (26-Mar-2025)                                            (MyFontDialog.cpp) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Implementation of the MyFontDialog class for
    applications using the Win32++ Windows interface classes. This class
    derives from the CFontDalog class found in the framework.

*******************************************************************************/

#include "stdafx.h"
#include "MyFontDlg.h"


// Create a simple default font dialog object. The hdcPrinter parameter is a
// handle to a printer device context for the printer on which the fonts are to
// be selected.
MyFontDialog:: MyFontDialog(DWORD dwFlags, HDC hdcPrinter /* = nullptr */)
    : CFontDialog(dwFlags | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS |
        CF_ENABLEHOOK, hdcPrinter)
{
    m_tm = {};
    SetBoxTitle(L"Font");

    // Set the default font.
    m_font.CreatePointFont(10, L"Courier New");
    SetChoiceFont(m_font);
}

// Perform any actions required for initialization of this object when the font
// choice is being initialized.
BOOL MyFontDialog::OnInitDialog()
{
    SetWindowText(m_boxTitle);
    return TRUE;
}

// This member is called when the OK button is pressed.
void MyFontDialog::OnOK()
{
    LOGFONT lf = GetLogFont();
    SetChoiceLogFont(lf);
}

// Record the current font TEXTMETRIC values and the average character width
// and height for the dialog's owner window, or the desktop if hwndOwner is 0.
void MyFontDialog::RecordFontMetrics()
{
    CWindowDC dc(GetParameters().hwndOwner);
    dc.SelectObject(m_font);

    // Measure the font width and height.
    dc.GetTextMetrics(m_tm);
    m_fontSize.cx = m_tm.tmAveCharWidth;
    m_fontSize.cy = m_tm.tmHeight + m_tm.tmExternalLeading;
}

// Sets the font choice from the specified font.
void MyFontDialog::SetChoiceFont(const CFont& f)
{
    LOGFONT lf = f.GetLogFont();
    SetChoiceLogFont(lf);
}

// Sets the font choice from the specified logfont.
void MyFontDialog::SetChoiceLogFont(LOGFONT& lf)
{
    SetFontIndirect(lf);
    RecordFontMetrics();
}

// Set the current font to have the characteristics contained in the supplied
// LOGFONT structure.
void MyFontDialog::SetFontIndirect(const LOGFONT& lf)
{
    try
    {
        m_font.CreateFontIndirect(lf);
        m_logFont = lf;
        CHOOSEFONT cf = GetParameters();
        cf.lpLogFont = &m_logFont;
        SetParameters(cf);
    }

    catch (const CResourceException&)
    {
        ::MessageBox(nullptr, L"Font creation error.",
            L"Error", MB_OK | MB_ICONEXCLAMATION | MB_TASKMODAL);
    }
}

