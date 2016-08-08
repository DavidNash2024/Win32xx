/* (27-Jan-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (MyFontDlg.cpp) *
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

	Usage Information: A font is characterized by many parameters. Some of
	its attributes are described by its corresponding LOGFONT structure.
	Other attributes not found in the LOGFONT are its foreground and
	background colors and its style string that appears in the Style box of
	the ::ChooseFont() dialog, when engaged. All of these attributes are
	found in the CHOOSEFONT structure underlying the base CFontDialog class
	accessible via the CFontDialog::GetParameters()/SetParameters() methods.
	Some are also accessble directly via other CFontDialog methods. This
	class augments the base class by providing a response to the help
	button, evaluation of the average height and width of the font, and
	serialization of object information.
	
	This class may be used in place of the CFontDialog class in instances
	the help button is required and where serialization of the font
	characteristics are meant to be persistent across executions of the
	application. It may also be used in lieu of separate declarations of a
	font, its color, and its average height and width.

	Programming Notes: The programming standards roughly follow those
	established by the 1997-1999 Jet Propulsion Laboratory Deep Space
	Network Planning and Preparation Subsystem project for C++ programming.
	
	Acknowledgement:
	The author would like to thank and acknowledge the advice, critical
	review, insight, and assistance provided by David Nash in the development
	of this work.	

********************************************************************************

	Implementation of the MyFontDialog class

*******************************************************************************/

#include "stdafx.h"
#include "MyFontDlg.h"
#include "resource.h"

/*============================================================================*/
	MyFontDialog::
MyFontDialog(const LOGFONT& lfInitial, DWORD dwFlags /* = 0 */,
    HDC hdcPrinter /* = 0 */)    					/*

	Create an object having the specified log font given in lplfInitial and
	having the attributes specified by dwFlags. If lplfInitial is NULL, use
	a default font. Throw an exception if the font cannot be created. Always
	enable the hook procedure dialog loop and help button.
*-----------------------------------------------------------------------------*/
    : CFontDialog(lfInitial, dwFlags | CF_EFFECTS | CF_ENABLEHOOK, hdcPrinter)
{
	SetBoxTitle(_T("Font"));
	SetChoiceLogFont(lfInitial);
}

/*============================================================================*/
	MyFontDialog::
MyFontDialog(const CHARFORMAT& charformat, DWORD dwFlags /* = 0 */,
    HDC hdcPrinter /* = 0 */)       					/*

	Create an object having the specified charformat and having the
	attributes specified by dwFlags. Throw an exception if the font cannot
	be created. Always enable the dialog loop hook procedure and help button.
*-----------------------------------------------------------------------------*/
    : CFontDialog(charformat, dwFlags | CF_EFFECTS | CF_ENABLEHOOK, hdcPrinter)
{
	SetBoxTitle(_T("Font"));
	LOGFONT lf = GetLogFont();
	SetChoiceLogFont(lf);
}

/*============================================================================*/
	MyFontDialog::
	MyFontDialog(DWORD dwFlags /*= 0*/,	
		HDC hdcPrinter /*= 0*/)							/*

		Create an object having the specified charformat and having the
		attributes specified by dwFlags. Throw an exception if the font cannot
		be created. Always enable the dialog loop hook procedure and help button.
*-----------------------------------------------------------------------------*/
		: CFontDialog(dwFlags | CF_INITTOLOGFONTSTRUCT | CF_EFFECTS | CF_ENABLEHOOK, hdcPrinter)
	{
		SetBoxTitle(_T("Font"));
		
		// set default font, 10pt Courier New
		CFont f;							
		f.CreatePointFont(10, _T("Courier New"));
		SetChoiceFont(f);	
	}

/*============================================================================*/
	INT_PTR MyFontDialog::
DoModal(HWND hWndOwner /* = 0 */)                                       /*

	Display the font choice dialog; hWndOwner specifies dialog's owner
	window. Enable the dialog to show the help button. The method processes
	the OnOK() member before returning control to the point of invocation
	when the OK button is activated.
*-----------------------------------------------------------------------------*/

{
          // open the dialog
	CHOOSEFONT cf = GetParameters();
	cf.Flags |= CF_SHOWHELP | CF_SCREENFONTS;
	SetParameters(cf);								

	return CFontDialog::DoModal(hWndOwner);
}

/*============================================================================*/
	LOGFONT MyFontDialog::
GetCurrentLogFont() const                                               /*

	Return the log font characteristics of the currently active font in the
	CHOOSEFONT struct's hwndOwner window.  This may not be the same as the
	current object log font, depending on whether the selection of the
	object font has been selected into the window device context.
*-----------------------------------------------------------------------------*/
{
	CClientDC dc(GetParameters().hwndOwner);
	return dc.GetLogFont();	
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

	This member executes immediately after the DoModal() member terminates
	upon activation of the OK button and before control returns to the
	point of invocation. At this point the object's log font and foreground
	color have been selected. The action here completes the selected dialog
	object, which consists of m_Font and the font text metrics.
*-----------------------------------------------------------------------------*/
{
	LOGFONT lf = GetLogFont();
	SetChoiceLogFont(lf);
}

/*============================================================================*/
	void MyFontDialog::
RecordFontMetrics()                                                     /*

	Select the current font into the device context of the object's parent
	window, whose handle is m_cf.hwndOwner if non-NUL, or of the entire
	screen if it is NULL.  Record the current font TEXTMETRIC values and
	compute the average character width and height in this window.
*-----------------------------------------------------------------------------*/
{
	  // measure the font width and height	
	CClientDC dc(GetParameters().hwndOwner);					
	dc.SelectObject(m_Font);									
	dc.GetTextMetrics(m_tm);									
	m_avgWdHt.cx = m_tm.tmAveCharWidth;							
	m_avgWdHt.cy = m_tm.tmHeight + m_tm.tmExternalLeading;		
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
		SetChoiceLogFont(lf);
		  // recover the text metrics
	 	ArchiveObject g(GetTextMetricPtr(), sizeof(TEXTMETRIC));
	 	ar >> g;
   		  // recover character height and width
		SIZE z;
		ar >> z;
		SetAvgSize(z);
		  // retrieve the font color
		COLORREF rgb;
		ar >> rgb;
		SetColor(rgb);
	}
}

/*============================================================================*/
	void MyFontDialog::
SetFontIndirect(const LOGFONT& lf)                                      /*

	Set the current object to have the characteristics contained in the
	supplied LOGFONT structure lf. Throw an exception if the font cannot
	be created.
*-----------------------------------------------------------------------------*/
{
	m_Font.CreateFontIndirect(lf);				// throws CResourceException on failure  
	CHOOSEFONT cf = GetParameters();
	memcpy(cf.lpLogFont, &lf, sizeof(LOGFONT));
}

