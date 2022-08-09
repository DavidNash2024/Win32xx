/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyFontDlg.h) *
********************************************************************************
|                                                                              |
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the MyFontDialog class for this
    sample program using the Win32++ Windows interface classes. This class
    derives from the CFontDalog class found in the framework to add needed
    functionality in displaying the view.

    Programming Notes: The programming style roughly follows that established
    by the 1995-1999 Jet Propulsion Laboratory Deep Space Network Planning and
    Preparation Subsystem project for C++ programming.

*******************************************************************************/

#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

/*============================================================================*/
    class
MyFontDialog : public CFontDialog                                       /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyFontDialog(DWORD dwFlags = 0, HDC hdcPrinter = 0);
        virtual ~MyFontDialog(){}

        virtual SIZE    GetAvgSize() const { return m_fontSize;}
        virtual CFont   GetChoiceFont() const { return m_font;}
        virtual LOGFONT GetCurrentLogFont() const { return m_logFont;}
        virtual void    OnOK();
        virtual void    SetBoxTitle(const CString& title){ m_boxTitle = title;}
        virtual void    SetChoiceFont(const CFont& f)
                    { LOGFONT lf = f.GetLogFont();
                      SetChoiceLogFont(lf);}

    protected:
        virtual TEXTMETRIC* GetTextMetric() { return &m_tm;}
        virtual BOOL    OnInitDialog();
        virtual void    RecordFontMetrics();
        virtual void    Serialize(CArchive &ar);
        virtual void    SetChoiceLogFont(LOGFONT& lf)
                            { SetFontIndirect(lf); RecordFontMetrics();}
        virtual void    SetFontIndirect(const LOGFONT& lf);
        virtual void    SetTextMetric(const TEXTMETRIC& tm) { m_tm = tm;}
        virtual void    SetWindowTitle() const { SetWindowText(m_boxTitle); }

    private:
        MyFontDialog(const MyFontDialog&);                // Disable copy construction
        MyFontDialog& operator = (const MyFontDialog&);   // Disable assignment operator

        CString     m_boxTitle;
        TEXTMETRIC  m_tm;       // font text metrics
        CSize       m_fontSize; // font average width & height
        CFont       m_font;     // the current font
        LOGFONT     m_logFont;  // the current logfont
};
/*----------------------------------------------------------------------------*/
#endif
