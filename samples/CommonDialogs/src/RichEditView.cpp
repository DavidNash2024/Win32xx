/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]          (CRichEditView.h) *
********************************************************************************
|                                                                              |
|                   Copyright (c) 2016, Robert C. Tausworthe                   |
|                             All Rights Reserved.                             |
|                          robert.c.tausworthe@ieee.org                        |
|                                                                              |
===============================================================================*

	Contents Description: Implementation of the CRichEditView class for the
	CommonDialogs sample application using the Win32++ Windows interface
	classes, Copyright (c) 2005-2016 David Nash, under permissions granted
	therein.

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


	Special Conventions:

 	Acknowledgement:
		The author would like to thank and acknowledge the advice,
		critical review, insight, and assistance provided by David Nash
		in the development of this work.

	Programming Notes:
               The programming standards roughly follow those established
                by the 1997-1999 Jet Propulsion Laboratory Deep Space Network
		Planning and Preparation Subsystem project for C++ programming.

********************************************************************************

	Implementation of the CRichEditView class

*******************************************************************************/

#include "stdafx.h"
#include "StdApp.h"

/*============================================================================*/
	CRichEditView::
CRichEditView()                                                         /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CRichEditView::
~CRichEditView()                                                        /*

*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	void CRichEditView::
Clean()									/*

	Clear the control of all text.
*-----------------------------------------------------------------------------*/
{
	long len = GetTextLength();
	SetSel(0, len);
	Clear();
}

/*============================================================================*/
	BOOL CRichEditView::
IsSelected()								/*

	Return TRUE if text is currently selected.
*-----------------------------------------------------------------------------*/
{
	CHARRANGE r;
	GetSel(r);
	return (r.cpMin != r.cpMax);
}


/*============================================================================*/
	void CRichEditView::
OnInitialUpdate()                                         		/*

*-----------------------------------------------------------------------------*/
{
	  // increase the text limit of the rich edit window: here, (DWORD)-1,
	  // or the maximum DWORD value.
	LimitText(-1);

	  //Determine which messages will be passed to the parent
	DWORD dwMask = ENM_KEYEVENTS | ENM_DROPFILES | ENM_MOUSEEVENTS;
	SetEventMask(dwMask);
	SetFocus();

}

/*============================================================================*/
	void CRichEditView::
SetColors(COLORREF txfg, COLORREF txbg, COLORREF bg)                  /*

	Set the rich edit control text foreground and background colors and the
	control background color.  This is needed only when the colors change
	(not like other controls).
*-----------------------------------------------------------------------------*/
{
	CHARFORMAT2 chf;
        chf.cbSize = sizeof(chf);
        chf.dwMask = CFM_COLOR | CFM_BACKCOLOR;
        chf.dwEffects = 0;
	chf.crTextColor = txfg;
        chf.crBackColor = txbg;
	SetDefaultCharFormat(chf);
	SetBackgroundColor(FALSE, bg);
}

/*============================================================================*/
	void CRichEditView::
SetFont(HFONT hFont, BOOL bRedraw) const                                /*

*-----------------------------------------------------------------------------*/
{
	CRichEdit::SetFont(hFont, bRedraw);

  // Required for Dev-C++
#ifndef IMF_AUTOFONT
#define IMF_AUTOFONT			0x0002
#endif

	  // Prevent Unicode characters from changing the font
	LRESULT lres = SendMessage(EM_GETLANGOPTIONS, 0, 0);
	lres &= ~IMF_AUTOFONT;
	SendMessage(EM_SETLANGOPTIONS, 0, lres);
}

/*============================================================================*/
	void CRichEditView::
SetWrapping(int wrap)                                                  /*

	Set the rich edit view window to use word wrapping (wrap is TRUE) or
	not use word wrapping (wrap is FALSE), or to use word wrapping to a
	given length (wrap = length > 1).
*-----------------------------------------------------------------------------*/
{
	  // This feature is not documented by Microsoft. An internet search
	  // reveals that the wrap values is interpreted as follows:
	  // 0  turns word wrap on based on the window width
	  // 1  turns word wrap off completely
	  // >1 turns word wrap on for a line width equal to the wrap value
	SetTargetDevice(NULL, (wrap == 0 ? TRUE : FALSE));
}

/*============================================================================*/
	BOOL CRichEditView::
ReadFile(const CFile& file)      					/*

	Read the text-only contents of the opened file into this rich edit
	control. Set the modified state FALSE and return TRUE on success. Return
	FALSE otherwise. The file must have been successfully opened in the
	OPEN_EXISTING mode.
*-----------------------------------------------------------------------------*/
{
	try
	{
		EDITSTREAM es;
		es.dwCookie =  (DWORD_PTR) file.GetHandle();
		es.pfnCallback = (EDITSTREAMCALLBACK) StreamInCallback;
		StreamIn(SF_TEXT, es);

		//Clear the modified text flag
	}

	catch (const CFileException& e)
	{
		CString str = _T("Failed to load:  ");
		str += e.GetFilePath();
		::MessageBox(NULL, str, A2T(e.what()), MB_ICONWARNING);
		return FALSE;
	}

	SetModify(FALSE);
	return TRUE;
}

/*============================================================================*/
	BOOL CRichEditView::
WriteFile(const CFile& file)                                            /*

	Write the text-only contents of this rich edit control into the opened
	file. Set the modified state FALSE and return TRUE on success. Return
	FALSE otherwise. The file must have been successfully opened in the
	CREATE_ALWAYS mode.
*-----------------------------------------------------------------------------*/
{
	try
	{
		EDITSTREAM es;
		es.dwCookie =  (DWORD_PTR) file.GetHandle();
		es.dwError = 0;
		es.pfnCallback = (EDITSTREAMCALLBACK) StreamOutCallback;
		StreamOut(SF_TEXT, es);

		//Clear the modified text flag
	}

	catch (const CFileException&)
	{
		CString str = _T("Failed to write:\n    ");
		str += file.GetFilePath();
		::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	SetModify(FALSE);
	return TRUE;
}

/*******************************************************************************

	Static Callback Functions
	
*=============================================================================*/
	DWORD CALLBACK CRichEditView::
StreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)   /*

	Tansfer data into the rich edit view window. The dwCookie is an
	app-defined value that directs the deposition of cb bytes of data into
	the pbBuff. The pcb points to the location receiving the number of bytes
	actually transferred in. Returns zero on success, or nonzero if cb is
	zero, indicating completion. Throws an exception on gilr error. In this
	usage, dwCookie contains the handle of an open file.
*-----------------------------------------------------------------------------*/
{
	// Required for StreamIn termination
	if (!cb)
		return (1);

	*pcb = 0;
	if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb,
	    NULL))
		::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);

	return 0;
}

/*============================================================================*/
	DWORD CALLBACK CRichEditView::
StreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)  /*

	Tansfer data out of the rich edit view window. The dwCookie is an
	app-defined value that directs the transfer of cb bytes of data from
	the pbBuff. The pcb points to the location receiving the number of bytes
	actually transferred out. Returns zero on success, or nonzero if cb is
	zero. Throws an exception on file error. In this usage, dwCookie
	contains the handle of an open file.
*-----------------------------------------------------------------------------*/
{
	// Required for StreamOut termination
	if(!cb)
		return (1);

	*pcb = 0;
	if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb,
	    NULL))
		::MessageBox(NULL, _T("WriteFile Failed"), _T(""), MB_OK);
	return 0;
}


