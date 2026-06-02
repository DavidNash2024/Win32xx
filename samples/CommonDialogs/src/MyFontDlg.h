/////////////////////////////////////////
// MyFileDlg.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

/////////////////////////////////////////////////////////
// This class derives from the Win32++ CFontDialog class.
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
    TEXTMETRIC  m_tm;       // Font text metrics.
    CSize       m_fontSize; // Font average width & height.
    CFont       m_font;     // Current font.
    LOGFONT     m_logFont;  // Current logfont.
};

#endif
