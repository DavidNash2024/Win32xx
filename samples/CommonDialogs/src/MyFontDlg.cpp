/* (27-Jan-2016) [Tab/Indent: 8/8][Line/Box: 80/74]         (MyFontDialog.cpp) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the MyFontDialog class for
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

	Usage Information: This class may be used in place of the CFontDialog
	class in instances where serialization of the font characteristics
	are meant to be persisitent across executions of the application.
	
 	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the
	development of this work.

	Programming Notes:
                The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the MyFontDialog class

*******************************************************************************/

#include "stdafx.h"
#include "MyFontDlg.h"
#include "AppHelpRC.h"

/*============================================================================*/
	MyFontDialog::
MyFontDialog(DWORD dwFlags /* = CF_SHOWHELP */, HDC hdcPrinter /* = 0 */) /*

	Create a simple default font dialog object with the title "Font", a
	10pt Courier New typeface, and font, style, strikeout, underline, text
	color, and help options enabled, as well as other options specified by
	dwFlags. The hdcPrinter parameter, if non-zero, is a handle to a printer
	device context for the printer on which the fonts are to be selected.
*-----------------------------------------------------------------------------*/
    : CFontDialog(dwFlags | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS |
	CF_ENABLEHOOK | CF_SHOWHELP , hdcPrinter)
{
	SetBoxTitle(_T("Font"));
          // default font, 10pt Courier New
	m_Font.CreatePointFont(10, _T("Courier New"));
	SetChoiceFont(m_Font);
}

/*============================================================================*/
	void MyFontDialog::
OnHelpButton()                                                          /*

	Override base class member to send the font context help identifier to
	the window whose handle is the CHOOSEFONT struct's hwndOwner.
*-----------------------------------------------------------------------------*/
{
	HWND hwndOwner = GetParameters().hwndOwner;
	if (::IsWindow(hwndOwner))
	{
		SendMessage(hwndOwner, WM_COMMAND,
		    IDC_HELP_COMDLG, IDM_HELP_FONTDLG);
	}
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

	This member is caused to execute after the DoModal() member has
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
	HDC hdc = ::GetDC(GetParameters().hwndOwner);

	  // select the current font into the device context:
	  // save the old
	CFont oldfont = (CFont)(HFONT)::SelectObject(hdc, m_Font);

	  // measure the font width and height
	::GetTextMetrics(hdc, &m_tm);
	m_avgWdHt.cx = m_tm.tmAveCharWidth;
	m_avgWdHt.cy = m_tm.tmHeight + m_tm.tmExternalLeading;

	  // restore entry environment
	::SelectObject(hdc, oldfont);
	::ReleaseDC(GetParameters().hwndOwner, hdc);
}

/*============================================================================*/
	void MyFontDialog::
Serialize(CArchive &ar)                                               	/*

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
	 	ArchiveObject g(GetTextMetricPtr(), sizeof(TEXTMETRIC));
	 	ar << g;
   		  // save character height and width
		ar << GetAvgSize();
		  // save the font color
		ar << GetParameters().rgbColors;
	}
        else    // recovering
        {
                  // recover the font as LOGFONT
		LOGFONT lf;
		ArchiveObject f(&lf, sizeof(LOGFONT));				
		ar >> f;											
		CHOOSEFONT cf = GetParameters();					
		cf.lpLogFont = &lf;									
		SetParameters(cf);									
		SetChoiceLogFont(lf);								
		  // recover the text metrics
	 	ArchiveObject g(GetTextMetricPtr(), sizeof(TEXTMETRIC));
	 	ar >> g;
   		  // recover character height and width
		SIZE z;
		ar >> z;
		m_avgWdHt = z;
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
	CFont hf;
	hf.CreateFontIndirect(lf);

	  // if it worked, put it in this object
	if(hf)
	{
		DeleteObject(m_Font);
		m_Font        = hf;
		m_LogFont     = lf;
		CHOOSEFONT cf = GetParameters();
		cf.lpLogFont  = &m_LogFont;
		SetParameters(cf);
	}
	else
	{
		::MessageBox(NULL, _T("Font creation error."),
		    _T("Error"), MB_OK | MB_ICONEXCLAMATION |
		    MB_TASKMODAL);
	}
}

