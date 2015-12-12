/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include <richedit.h>
#include "resource.h"



// definitions for the CMainFrame class
CMainFrame::CMainFrame() : m_IsWrapped(FALSE)
{
	m_strPathName = _T("");
	SetView(m_RichView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Notepad Sample"));

	// Load the settings from the registry with 5 MRU entries
	LoadRegistryMRUSettings(5);
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnInitialUpdate()
{
	DragAcceptFiles(TRUE);
	SetWindowTitle();
	m_RichView.SetFocus();
}

LRESULT CMainFrame::OnNotify(WPARAM wParam, LPARAM lParam)
{
	NMHDR* pNMH;
	pNMH = (LPNMHDR) lParam;
	switch (pNMH->code)
	{
	case EN_DROPFILES:
		{
			ENDROPFILES* ENDrop = reinterpret_cast<ENDROPFILES*>(lParam);
			HDROP hDropInfo = (HDROP) ENDrop->hDrop;
			OnDropFiles(hDropInfo);
		}
		return TRUE;
	}

	return CFrame::OnNotify(wParam, lParam);
}

void CMainFrame::OnClose()
{
	//Check for unsaved text
	if ( m_RichView.GetModify() )
		if (::MessageBox(NULL, _T("Save changes to this document"), _T("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
			OnFileSave();

	// Call the base function
	CFrame::OnClose();
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSaveAs();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_EDIT_COPY:			return OnEditCopy();
	case IDM_EDIT_PASTE:		return OnEditPaste();
	case IDM_EDIT_CUT:			return OnEditCut();
	case IDM_EDIT_DELETE:		return OnEditDelete();
	case IDM_EDIT_REDO:			return OnEditRedo();
	case IDM_EDIT_UNDO:			return OnEditUndo();
	case IDM_FILE_EXIT:			return OnFileExit();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_OPTIONS_WRAP:      return OnOptionsWrap();
	case IDM_OPTIONS_FONT:		return OnOptionsFont();
	case IDM_HELP_ABOUT:		return OnHelp();

	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	return OnFileMRU(wParam);
	}

	return FALSE;
}

BOOL CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile((HDROP)hDropInfo, 0, (LPTSTR)szFileName, _MAX_PATH);

	if (ReadFile(szFileName))
	{
		m_strPathName = szFileName;
		ReadFile(szFileName);
		SetWindowTitle();
		AddMRUEntry(szFileName);
	}

	return TRUE;
}

BOOL CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
	return TRUE;
}

BOOL CMainFrame::OnFileMRU(WPARAM wParam)
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);

	if (ReadFile(strMRUText))
		m_strPathName = strMRUText;
	else
		RemoveMRUEntry(strMRUText);

	SetWindowTitle();
	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	m_RichView.SetWindowText(_T(""));
	m_strPathName = _T("");
	SetWindowTitle();
	m_RichView.SetFontDefaults();
	m_RichView.SetModify(FALSE);
	return TRUE;
}

BOOL CMainFrame::OnFilePrint()
{
	PRINTDLG pd;

	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = *this;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	pd.nCopies     = 1;
	pd.nFromPage   = 0xFFFF;
	pd.nToPage     = 0xFFFF;
	pd.nMinPage    = 1;
	pd.nMaxPage    = 0xFFFF;
	pd.hwndOwner = *this;

	if (PrintDlg(&pd)==TRUE)
	{
		HDC hPrinterDC = pd.hDC;

		// This code basically taken from MS KB article Q129860

		FORMATRANGE fr;
		int	nHorizRes   = ::GetDeviceCaps(hPrinterDC, HORZRES);
		int	nVertRes    = ::GetDeviceCaps(hPrinterDC, VERTRES);
		int nLogPixelsX = ::GetDeviceCaps(hPrinterDC, LOGPIXELSX);
		int nLogPixelsY = ::GetDeviceCaps(hPrinterDC, LOGPIXELSY);
		LONG lTextLength;   // Length of document.
		LONG lTextPrinted;  // Amount of document printed.

		// Ensure the printer DC is in MM_TEXT mode.
		::SetMapMode ( hPrinterDC, MM_TEXT );

		// Rendering to the same DC we are measuring.
		ZeroMemory(&fr, sizeof(fr));
		fr.hdc = hPrinterDC;
		fr.hdcTarget = hPrinterDC;

		// Set up the page.
		int margin = 200; // 1440 TWIPS = 1 inch.
		fr.rcPage.left     = fr.rcPage.top = margin;
		fr.rcPage.right    = (nHorizRes/nLogPixelsX) * 1440 - margin;
		fr.rcPage.bottom   = (nVertRes/nLogPixelsY) * 1440 - margin;

		// Set up margins all around.
		fr.rc.left   = fr.rcPage.left ;//+ 1440;
		fr.rc.top    = fr.rcPage.top ;//+ 1440;
		fr.rc.right  = fr.rcPage.right ;//- 1440;
		fr.rc.bottom = fr.rcPage.bottom ;//- 1440;

		// Default the range of text to print as the entire document.
		fr.chrg.cpMin = 0;
		fr.chrg.cpMax = -1;
		m_RichView.FormatRange(&fr, TRUE);

		// Set up the print job (standard printing stuff here).
		DOCINFO di;
		ZeroMemory(&di, sizeof(di));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = m_strPathName;

		// Do not print to file.
		di.lpszOutput = NULL;

		// Start the document.
		::StartDoc(hPrinterDC, &di);

		// Find out real size of document in characters.
		lTextLength = m_RichView.GetTextLengthEx(GTL_NUMCHARS);

		do
		{
			// Start the page.
			::StartPage(hPrinterDC);

			// Print as much text as can fit on a page. The return value is
			// the index of the first character on the next page. Using TRUE
			// for the wParam parameter causes the text to be printed.
			lTextPrinted = m_RichView.FormatRange(&fr, TRUE);
			m_RichView.DisplayBand(&fr.rc);

			// Print last page.
			::EndPage(hPrinterDC);

			// If there is more text to print, adjust the range of characters
			// to start printing at the first character of the next page.
			if (lTextPrinted < lTextLength)
			{
				fr.chrg.cpMin = lTextPrinted;
				fr.chrg.cpMax = -1;
			}
		}
		while (lTextPrinted < lTextLength);

		// Tell the control to release cached information.
		m_RichView.FormatRange(&fr, FALSE);

		::EndDoc (hPrinterDC);

		// Delete DC when done.
		::DeleteDC(hPrinterDC);
	}

	return TRUE;
}

BOOL CMainFrame::OnEditCut()
{
	m_RichView.Cut();
	return TRUE;
}

BOOL CMainFrame::OnEditCopy()
{
	m_RichView.Copy();
	return TRUE;
}

BOOL CMainFrame::OnEditPaste()
{
	m_RichView.PasteSpecial(CF_TEXT);
	return TRUE;
}

BOOL CMainFrame::OnEditDelete()
{
	m_RichView.Clear();
	return TRUE;
}

BOOL CMainFrame::OnEditRedo()
{
	m_RichView.Redo();
	return TRUE;
}

BOOL CMainFrame::OnEditUndo()
{
	m_RichView.Undo();
	return TRUE;
}

void CMainFrame::OnMenuUpdate(UINT nID)
{
	if (nID == IDM_OPTIONS_WRAP)
	{
		GetFrameMenu().CheckMenuItem(nID, m_IsWrapped ? MF_CHECKED : MF_UNCHECKED);
	}

	CFrame::OnMenuUpdate(nID);
}

BOOL CMainFrame::OnOptionsFont()
{
	// Retrieve the current character format
	CHARFORMAT2 cf2;
	ZeroMemory (&cf2, sizeof(cf2));
	cf2.cbSize = sizeof(cf2);
	cf2.dwMask = CFM_COLOR|CFM_FACE|CFM_EFFECTS;
	m_RichView.GetDefaultCharFormat(cf2);

	// Fill the LOGFONT struct from CHARFORMAT2
	LOGFONT lf;
	ZeroMemory(&lf, sizeof(lf));
	lstrcpy(lf.lfFaceName, cf2.szFaceName);
	lf.lfHeight = cf2.yHeight/15;
	lf.lfWeight = (cf2.dwEffects & CFE_BOLD)? 700 : 400;
	lf.lfItalic = (BYTE)(cf2.dwEffects & CFE_ITALIC);

	// Fill the CHOOSEFONT stucture
	CHOOSEFONT cf;
	ZeroMemory(&cf, sizeof(cf));
	cf.lStructSize = sizeof(cf);
	cf.hwndOwner = *this;
	cf.lpLogFont = &lf;						// current logfont
	cf.rgbColors = cf2.crTextColor;			// current text color
	cf.lpLogFont = &lf;
	cf.Flags = CF_SCREENFONTS | CF_EFFECTS | CF_INITTOLOGFONTSTRUCT;

	if (ChooseFont(&cf))
	{
		// Set the Font
		CFont RichFont(cf.lpLogFont);
		m_RichView.SetFont(RichFont, TRUE);

		// Set the font color
		cf2.crTextColor = cf.rgbColors;
		cf2.dwEffects = 0;
		cf2.dwMask = CFM_COLOR;
		m_RichView.SetDefaultCharFormat(cf2);
	}

	return TRUE;
}

BOOL CMainFrame::OnOptionsWrap()
{
	m_RichView.SetTargetDevice(NULL, m_IsWrapped);
	m_IsWrapped = !m_IsWrapped;
	return TRUE;
}

BOOL CMainFrame::ReadFile(LPCTSTR szFileName)
{
	BOOL Succeeded = FALSE;

	try
	{
		// Open the file for reading
		CFile File;
		File.Open(szFileName, OPEN_EXISTING);

		EDITSTREAM es;
		es.dwCookie =  (DWORD_PTR) File.GetHandle();
		es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamInCallback;
		m_RichView.StreamIn(SF_TEXT, es);

		//Clear the modified text flag
		m_RichView.SetModify(FALSE);

		Succeeded = TRUE;
	}

	catch (const CFileException& e)
	{
		CString str = _T("Failed to load:  ");
		str += e.GetFilePath();
		::MessageBox(NULL, str, A2T(e.what()), MB_ICONWARNING);
		return FALSE;
	}

	return Succeeded;
}

BOOL CMainFrame::WriteFile(LPCTSTR szFileName)
{
	BOOL Succeeded = FALSE;
	try
	{
		// Open the file for writing
		CFile File;
		File.Open(szFileName, CREATE_ALWAYS);

		EDITSTREAM es;
		es.dwCookie =  (DWORD_PTR) File.GetHandle();
		es.dwError = 0;
		es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamOutCallback;
		m_RichView.StreamOut(SF_TEXT, es);

		//Clear the modified text flag
		m_RichView.SetModify(FALSE);

		Succeeded = TRUE;
	}

	catch (const CFileException&)
	{
		CString str = _T("Failed to write:  ");
		str += szFileName;
		::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	return Succeeded;
}

BOOL CMainFrame::OnFileOpen()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	CString Filters( _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"), 46);
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), Filters, *this);

	if (!str.IsEmpty())
	{
		ReadFile(str);
		SetFileName(str);
		AddMRUEntry(str);
		SetWindowTitle();
	}

	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	if (m_strPathName == _T(""))
		OnFileSaveAs();
	else
		WriteFile(m_strPathName);

	return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	CString Filters(_T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"), 46);
	CFile File;
	CString str = File.SaveFileDialog(0, OFN_OVERWRITEPROMPT, _T("Save File"), Filters, _T("txt"), *this);

	if (!str.IsEmpty())
	{
		WriteFile(str);
		SetFileName(str);
		AddMRUEntry(str);
		SetWindowTitle();
	}

	return TRUE;
}

void CMainFrame::SetFileName(LPCTSTR szFilePathName)
{
	//Truncate and save file name
	int i = lstrlen(szFilePathName)+1;
	while ((--i > 0) && (szFilePathName[i-1] != _T('\\')));

	m_strPathName = szFilePathName+i;
}

void CMainFrame::SetWindowTitle()
{
    CString Title;

	if (m_strPathName == _T(""))
		Title = _T("TextEdit");
	else
		Title = m_strPathName + _T(" - TextEdit");

	SetWindowText(Title);
}

void CMainFrame::SetupToolBar()
{
	// Define the resource IDs for the toolbar
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT   );
	AddToolBarButton( IDM_EDIT_COPY  );
	AddToolBarButton( IDM_EDIT_PASTE );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );

}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//
//	}

	return WndProcDefault(uMsg, wParam, lParam);
}

DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Required for StreamIn
	if (!cb)
		return (1);

	*pcb = 0;
	if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
		::MessageBox(NULL, _T("ReadFile Failed"), _T(""), MB_OK);

	return 0;
}

DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Required for StreamOut
	if(!cb)
		return (1);

	*pcb = 0;
	if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
		::MessageBox(NULL, _T("WriteFile Failed"), _T(""), MB_OK);
	return 0;
}

