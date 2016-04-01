/* (27-Jan-2016) [Tab/Indent: 8/8][Line/Box: 80/74]              (MyFontDlg.h) *
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
	framework. It permits customized handling of the HELP button to provide
	context help, setting of the dialog title, and serialization.

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

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Declaration of the MyFontDialog class

*******************************************************************************/

#ifndef MYFONTDIALOG_H
#define MYFONTDIALOG_H

/*============================================================================*/
	class
MyFontDialog : public CFontDialog                                       /*

*-----------------------------------------------------------------------------*/
{
	public:
		MyFontDialog(LPLOGFONT lplfInitial = NULL,
				DWORD dwFlags = 0,
				HDC hdcPrinter = 0);

		MyFontDialog(const CHARFORMAT& charformat,
				DWORD dwFlags = 0,
				HDC hdcPrinter = 0);

		~MyFontDialog(){}

			SIZE	GetAvgSize(void) const
				    { return m_avgWdHt;}
			CFont	GetChoiceFont(void) const
				    { return m_Font;}
			LOGFONT GetCurrentLogFont() const;
		virtual void 	OnOK();
			void 	RecordFontMetrics();
			void    SetAvgSize(const SIZE& sz)
				    { m_avgWdHt = sz; }
			void	SetBoxTitle(LPCTSTR title)
				    { m_sBoxTitle = title;}
			void    SetChoiceLogFont(LOGFONT& lf)
				    { SetFontIndirect(lf); RecordFontMetrics();}
			void    SetChoiceFont(const CFont& f)
				    { LOGFONT lf = f.GetLogFont();
				      SetChoiceLogFont(lf);}
			void 	SetFontIndirect(const LOGFONT& lf);

	protected:
		TEXTMETRIC 	GetTexMetric(void) const
				    { return m_tm;}
		TEXTMETRIC* 	GetTextMetricPtr()
				    { return &m_tm;}
		virtual void 	OnHelpButton();
		virtual BOOL 	OnInitDialog();
		virtual	void 	Serialize(CArchive &ar);
			void	SetTextMetric(const TEXTMETRIC& tm)
				    { m_tm = tm;}
		virtual void    SetWindowTitle() const
				    {SetWindowText(m_sBoxTitle);}

	private:
		CString 	m_sBoxTitle;
		TEXTMETRIC 	m_tm;		// font text metrics
	       	SIZE		m_avgWdHt;	// font average width & height
		CFont		m_Font;		// the current font
};

#endif
