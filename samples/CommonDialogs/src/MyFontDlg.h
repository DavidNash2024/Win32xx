/* (26-Mar-2025)                                                 (MyFontDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the MyFontDialog class for this
    sample program using the Win32++ Windows interface classes. This class
    derives from the CFontDalog class found in the framework to add needed
    functionality in displaying the view.

*******************************************************************************/

#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

class MyFontDialog : public CFontDialog
{
public:
    MyFontDialog(DWORD dwFlags = 0, HDC hdcPrinter = nullptr);
    virtual ~MyFontDialog() override = default;

    SIZE    GetAvgSize() const                  { return m_fontSize; }
    CFont   GetChoiceFont() const               { return m_font; }
    LOGFONT GetCurrentLogFont() const           { return m_logFont; }
    TEXTMETRIC* GetTextMetric()                 { return &m_tm; }
    void    OnOK() override;
    void    SetBoxTitle(const CString& title)   { m_boxTitle = title; }
    void    SetChoiceFont(const CFont& f);
    void    RecordFontMetrics();
    void    SetChoiceLogFont(LOGFONT& lf);
    void    SetFontIndirect(const LOGFONT& lf);
    void    SetTextMetric(const TEXTMETRIC& tm) {m_tm = tm;}

protected:
    virtual BOOL    OnInitDialog() override;

private:
    MyFontDialog(const MyFontDialog&) = delete;
    MyFontDialog& operator=(const MyFontDialog&) = delete;

    CString     m_boxTitle;
    TEXTMETRIC  m_tm;       // font text metrics
    CSize       m_fontSize; // font average width & height
    CFont       m_font;     // the current font
    LOGFONT     m_logFont;  // the current logfont
};

#endif
