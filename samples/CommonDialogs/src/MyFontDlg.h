/* (28-Aug-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyFontDlg.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

    Contents Description: Declaration of the MyFontDialog class for
    applications using the Win32++ Windows interface classes, Copyright
    (c) 2005-2016 David Nash, under permissions granted therein. This class
    derives from the CFontDalog class found in the Win32++ Windows interface
    framework.

        Caveats: The copyright displayed above extends only to the author's
    original contributions to the subject class, and to the alterations,
    additions, deletions, and other treatments of materials that may have
    been extracted from the cited sources.  Unaltered portions of those
    materials retain their original copyright status. The author hereby
    grants permission to any person obtaining a copy of this treatment
    of the subject class and any associated documentation composed by
    the author, to utilize this material, free of charge and without
    restriction or limitation, subject to the following conditions:

        The above copyright notice, as well as that of David Nash
        and Win32++, together with the respective permission
        conditions shall be included in all copies or substantial
        portions of this material so copied, modified, merged,
        published, distributed, or otherwise held by others.

    These materials are provided "as is", without warranty of any kind,
    express or implied, including but not limited to: warranties of
    merchantability, fitness for a particular purpose, and non-infringement.
    In no event shall the authors or copyright holders be liable for any
    claim, damages, or other liability, whether in an action of contract,
    tort or otherwise, arising from, out of, or in connection with, these
    materials, the use thereof, or any other other dealings therewith.

    Acknowledgement:
        The author would like to thank and acknowledge the advice,
        critical review, insight, and assistance provided by David Nash
        in the development of this work.

    Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
        Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

    Declaration of the MyFontDialog class

*******************************************************************************/

#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

#include "ListBoxDlgRC.h"

/*============================================================================*/
    class
MyFontDialog : public CFontDialog                                       /*

*-----------------------------------------------------------------------------*/
{
    public:
        MyFontDialog(DWORD dwFlags = 0, HDC hdcPrinter = 0);

        ~MyFontDialog(){}

                SIZE    GetAvgSize() const { return m_avgWdHt;}
                CFont   GetChoiceFont() const { return m_Font;}
                LOGFONT GetCurrentLogFont() const { return m_LogFont;}
        virtual void    OnOK();
                void    SetBoxTitle(const CString& title){ m_sBoxTitle = title;}
                void    SetChoiceFont(const CFont& f)
                          { LOGFONT lf = f.GetLogFont();
                            SetChoiceLogFont(lf);}

    protected:
            TEXTMETRIC  GetTexMetric() const { return m_tm;}
            TEXTMETRIC* GetTextMetricPtr() { return &m_tm;}
        virtual BOOL    OnInitDialog();
                void    RecordFontMetrics();
        virtual void    Serialize(CArchive &ar);
                void    SetChoiceLogFont(LOGFONT& lf)
                          { SetFontIndirect(lf); RecordFontMetrics();}
                void    SetFontIndirect(const LOGFONT& lf);
                void    SetTextMetric(const TEXTMETRIC& tm)
                          { m_tm = tm;}
        virtual void    SetWindowTitle() const
                          {SetWindowText(m_sBoxTitle);}

    private:
        CString     m_sBoxTitle;
        TEXTMETRIC  m_tm;       // font text metrics
        CSize       m_avgWdHt;  // font average width & height
        CFont       m_Font;     // the current font
        LOGFONT     m_LogFont;  // the current logfont
};

#endif
