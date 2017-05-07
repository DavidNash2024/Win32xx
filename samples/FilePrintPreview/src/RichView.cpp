/* (06-Sep-2016) [Tab/Indent: 8/8][Line/Box: 80/74]            (RichView.cpp) *
********************************************************************************

	Implementation of the CRichView class

********************************************************************************

	Acknowledgment. This class was adapted from that in the PrintPreview 
	sample program appearing in the Win32++ framework sample folder, created 
	by David Nash and published under the permissions granted in that work.
	The adaptation here implements the CRichView class as self-contained
	package that implements reading, writing, print, and print preview
	functions. This work has been developed under the co-authorship of 
	Robert C. Tausworthe and David Nash, and released under the copyright 
	provisions of the Win32++ framework software, copyright (c) David Nash,
	2005-2017. The former author acknowledges and thanks the latter for his 
	patient direction and inspiration in the development of the classes of 
	these classes.

*******************************************************************************/

#include "stdafx.h"
#include "richview.h"
#include "PrintUtil.h"

/*============================================================================*/
	CRichView::
CRichView()								/*

	Construct the rich edit view window.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	CRichView::
~CRichView()								/*

	Destructor.
*-----------------------------------------------------------------------------*/
{
}

/*============================================================================*/
	void CRichView::
OnAttach()								/*

	Called by the framework to initialize the rich edit view.
*-----------------------------------------------------------------------------*/
{
	  //increase the text limit of the rich edit window
	LimitText(-1);

	  //Determine which messages will be passed to the parent
	DWORD dwMask = ENM_KEYEVENTS | ENM_DROPFILES;
	SetEventMask(dwMask);

	SetFontDefaults();
}

/*============================================================================*/
	BOOL CRichView::
GetNewFont()								/*

	Bring up the font choice dialog and choose a new font.
*-----------------------------------------------------------------------------*/
{	  // Retrieve the current character format
	CHARFORMAT2 cf2;
	ZeroMemory (&cf2, sizeof(cf2));
	cf2.cbSize = sizeof(cf2);
	cf2.dwMask = CFM_COLOR|CFM_FACE|CFM_EFFECTS;
	GetDefaultCharFormat(cf2);

	  // Fill the LOGFONT struct from CHARFORMAT2
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lstrcpy(lf.lfFaceName, cf2.szFaceName);
	lf.lfHeight = cf2.yHeight / 15;
	lf.lfWeight = (cf2.dwEffects & CFE_BOLD)? 700 : 400;
	lf.lfItalic = (BYTE)(cf2.dwEffects & CFE_ITALIC);

	  // Display the Choose Font dialog
	CFontDialog LogFont(lf, CF_SCREENFONTS | CF_EFFECTS);
	if (LogFont.DoModal(*this) == IDOK)
	{
		  // Set the Font
		CFont RichFont(LogFont.GetLogFont());
		SetFont(RichFont, TRUE);

		  // Set the font color
		cf2.crTextColor = LogFont.GetColor();
		cf2.dwEffects = 0;
		cf2.dwMask = CFM_COLOR;
		SetDefaultCharFormat(cf2);
	}

	return TRUE;
}

/*============================================================================*/
	UINT CRichView::
GetPageBreaks(CDC& dcPrinter)						/*

	Determine and record the page break locations and return the number
	of pages in the document.
*-----------------------------------------------------------------------------*/
{
	  // Get the device context of the default or currently chosen printer
	dcPrinter.SetMapMode(MM_TEXT);

	  // set up printer to receive pages: specific accommodation of the 
	  // Rich Edit document
	FORMATRANGE fr;
	fr.hdcTarget  = dcPrinter;	// format for this
	fr.hdc        = dcPrinter;	// render to this
	fr.rcPage     = GetPageRect(dcPrinter);
	fr.rc         = GetPrintRect(dcPrinter);
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	  // Find out real size of document in characters.
	LONG lTextLength;   // Length of document, in characters
	lTextLength = GetTextLengthEx(GTL_NUMCHARS);

	  // Calculate the page breaks
	LONG lTextPrinted;  // number of charcters printed so far
	m_PageBreaks.clear();
	do
	{
		lTextPrinted = FormatRange(fr, FALSE);
		if (lTextPrinted < lTextLength)
		{
			fr.chrg.cpMin = lTextPrinted; // next page begins here
			fr.chrg.cpMax = -1;
			m_PageBreaks.push_back(lTextPrinted);
		}
	} while (lTextPrinted < lTextLength);
	  // Clear the cache
	FormatRange();
	m_PageBreaks.push_back(-1);
	  // set the number of pages
	return m_PageBreaks.size();
}

/*============================================================================*/
	void CRichView::
PreCreate(CREATESTRUCT& cs)						/*

*-----------------------------------------------------------------------------*/
{
	cs.style = ES_AUTOHSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | WS_CHILD |
		WS_CLIPCHILDREN | WS_HSCROLL | WS_VISIBLE | WS_VSCROLL;

	cs.dwExStyle = WS_EX_CLIENTEDGE | WS_EX_ACCEPTFILES;
	cs.lpszClass = RICHEDIT_CLASS; // RichEdit ver 2.0
}

/*============================================================================*/
	void CRichView::
PrintDC(UINT nPage, CDC& dcPrinter, CDC& dcDevice)			/*

	Render the RichEdit document nPage formatted for the dcPrinter to the
	given dcDevice.
*-----------------------------------------------------------------------------*/
{
	FORMATRANGE fr;
	ZeroMemory(&fr, sizeof(fr));
	fr.hdcTarget  = dcPrinter;	// format for this
	fr.hdc        = dcDevice;	// render to this
	fr.rcPage     = GetPageRect(dcPrinter);
	fr.rc         = GetPrintRect(dcPrinter);
	fr.chrg.cpMin = (nPage > 0) ? m_PageBreaks[nPage - 1] : 0;
	fr.chrg.cpMax = m_PageBreaks[nPage];
	  // display text from the RichEdit control on the memory dc
	FormatRange(fr, TRUE);
	  // tell the RichView control to release its cached information.
	FormatRange();
}

/*============================================================================*/
	void CRichView::
PrintPages(CPrintDialog& PrintDlg)					/*

	Print the document without bringing up the print dialog box using the
	given PrintDlg object parameters.
*-----------------------------------------------------------------------------*/
{
	  // get the printer's device context
	CDC dcPrinter = PrintDlg.GetPrinterDC();
	  // assign values to the rich edit control FORMATRANGE struct
	FORMATRANGE fr;
	ZeroMemory(&fr, sizeof(fr));
	fr.hdc       = dcPrinter;	// render to this device
	fr.hdcTarget = dcPrinter;	// device to format for
	fr.rcPage    = GetPageRect(dcPrinter);	// entire area of device, twips
	fr.rc        = GetPrintRect(dcPrinter);	// range of text to format

	  // set the range of text to print as the entire document: defaults
	fr.chrg.cpMin = 0;
	fr.chrg.cpMax = -1;

	  // start the print job
	DOCINFO di;
	ZeroMemory(&di, sizeof(di));
	di.cbSize      = sizeof(DOCINFO);
	di.lpszDocName = m_sDocPath;	// name of document to print
	di.lpszOutput  = NULL;		// NULL here means output to fr.hdc
	dcPrinter.StartDoc(&di);	// start the printing
	  // get the length of document to print
	LONG lTextLength = GetTextLengthEx(GTL_NUMCHARS); 
	LONG lTextPrinted;  // amount of document printed so far
	do
	{
		  // start at the first page
		dcPrinter.StartPage();
		  // Print as much text as will fit on a page. The return 
		  // value is the index of the first character on the next
		  // page.  Using TRUE for the wParam parameter causes the 
		  // text to be  printed.
		lTextPrinted = FormatRange(fr, TRUE);
		DisplayBand(fr.rc);
		  // indicate printing this page is complete
		dcPrinter.EndPage();
		  // If there is more text to print, adjust the range of 
		  // characters for StartPage() to begin printing at the first 
		  // character of the next page, for the length default of
		  // characters.
		if (lTextPrinted < lTextLength)
		{
			fr.chrg.cpMin = lTextPrinted;
			fr.chrg.cpMax = -1;
		}
	} while (lTextPrinted < lTextLength); // while there is more to print.
	  // tell the control to release its cached information
	FormatRange();
	  // end the print job
	dcPrinter.EndDoc();
}

/*============================================================================*/
	BOOL CRichView::
ReadFile(LPCTSTR szFileName)						/*

	Open the szFileName file as a rich edit view stream for display in
	the main window.
*-----------------------------------------------------------------------------*/
{
	try
	{
		// Open the file for reading
		CFile File;
		File.Open(szFileName, OPEN_EXISTING);
		SetDocName(szFileName);
		EDITSTREAM es;
		es.dwCookie = (DWORD_PTR) File.GetHandle();
		es.pfnCallback = (EDITSTREAMCALLBACK) RVStreamInCallback;
		StreamIn(SF_TEXT, es);

		//Clear the modified text flag
		SetModify(FALSE);
	}
	catch (const CFileException& e)
	{
		CString str = _T("Failed to load:  ");
		str += e.GetFilePath();
		::MessageBox(NULL, str, AtoT(e.what()), MB_ICONWARNING);
		return FALSE;
	}
	return TRUE;
}
	
/*============================================================================*/
	void CRichView::
SetFontDefaults()							/*

*-----------------------------------------------------------------------------*/
{
	  //Set font
	if (m_Font.GetHandle() == 0)
		m_Font.CreateFont(16, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET,
		    OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, 
		    FF_MODERN, _T("Courier New"));
	SetFont(m_Font, FALSE);

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
	void CRichView::
WordWrap(WordWrapType setting)						/*

	Set the rich view word wrapping to the new setting.
*-----------------------------------------------------------------------------*/
{
	  // Note:  Although the CRichEdit::SetTargetDevice() function itself  
	  // is documented, the first two forms of it used below are not 
	  // described in any Microsoft documentation. In fact, the only
	  // occurrences found in the Microsoft Windows Software Development 
	  // Kit are found in the CRichEditView::WrapChanged() method source 
	  // code. The CRichView::WordWrap() method derives from that source.
	switch (setting)
	{
	    case WRAP_NONE:
		SetTargetDevice(NULL, 1);
		break;

	    case WRAP_WINDOW:
		SetTargetDevice(NULL, 0);
		break;

	    case WRAP_PRINTER:
	    {
		CPrintDialog PrintDlg(PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC);
		CDC dcPrinter = PrintDlg.GetPrinterDC();
		CRect rc = GetPrintRect(dcPrinter);
		SetTargetDevice(dcPrinter, rc.Width());
		break;
	    }
	    
	    default:
		break;
	}
}

/*============================================================================*/
	BOOL CRichView::
WriteFile(LPCTSTR szFileName)						/*

*-----------------------------------------------------------------------------*/
{
	try
	{
		  // Open the file for writing
		CFile File;
		File.Open(szFileName, CREATE_ALWAYS);

		EDITSTREAM es;
		es.dwCookie =  (DWORD_PTR) File.GetHandle();
		es.dwError = 0;
		es.pfnCallback = (EDITSTREAMCALLBACK) RVStreamOutCallback;
		StreamOut(SF_TEXT, es);

		  //Clear the modified text flag
		SetModify(FALSE);
	}

	catch (const CFileException&)
	{
		CString str = _T("Failed to write:  ");
		str += szFileName;
		::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	return TRUE;
}

/*******************************************************************************

	Static streaming callback functions

*=============================================================================*/
	DWORD CALLBACK CRichView::
RVStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)	/*

*-----------------------------------------------------------------------------*/
{
	  // Required for StreamIn
	if (!cb)
		return (1);

	*pcb = 0;
	if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb,
	    NULL))
		::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);

	return 0;
}

/*============================================================================*/
	DWORD CALLBACK CRichView::
RVStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)	/*

*-----------------------------------------------------------------------------*/
{
	  // Required for StreamOut
	if(!cb)
		return (1);

	*pcb = 0;
	if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb,
	    NULL))
		::MessageBox(NULL, _T("WriteFile Failed"), _T(""), MB_OK);
	return 0;
}
