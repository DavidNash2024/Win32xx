/////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include <richedit.h>
#include "resource.h"


//For Visual C++ 6 and without a modern SDK
#ifndef DWORD_PTR
#define DWORD_PTR DWORD
#endif


// definitions for the CMainFrame class
CMainFrame::CMainFrame()
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
			ENDROPFILES* ENDrop = (ENDROPFILES*)lParam;
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
	BOOL bChanged = (BOOL)m_RichView.SendMessage(EM_GETMODIFY, 0, 0);
	if (bChanged)
		if (::MessageBox(NULL, _T("Save changes to this document"), _T("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
			OnFileSave();

	// End the application
	//Destroy();
	DestroyWindow(*this);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_FILE_PRINT:		OnFilePrint();		return TRUE;
	case IDM_EDIT_COPY:			OnEditCopy();		return TRUE;
	case IDM_EDIT_PASTE:		OnEditPaste();		return TRUE;
	case IDM_EDIT_CUT:			OnEditCut();		return TRUE;
	case IDM_EDIT_DELETE:		OnEditDelete();		return TRUE;
	case IDM_EDIT_REDO:			OnEditRedo();		return TRUE;
	case IDM_EDIT_UNDO:			OnEditUndo();		return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;

	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	OnFileMRU(wParam);	return TRUE;
	}

	return FALSE;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile((HDROP)hDropInfo, 0, (LPTSTR)szFileName, _MAX_PATH);

	m_strPathName = szFileName;
	ReadFile(szFileName);
	SetWindowTitle();
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	SendMessage(WM_SYSCOMMAND, SC_CLOSE, 0);
}

void CMainFrame::OnFileMRU(WPARAM wParam)
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);

	if (ReadFile(strMRUText))
		m_strPathName = strMRUText;
	else
		RemoveMRUEntry(strMRUText);

	SetWindowTitle();
}

void CMainFrame::OnFileNew()
{
	m_RichView.SetWindowText(_T(""));
	m_strPathName = _T("");
	SetWindowTitle();
	m_RichView.SetFontDefaults();
	m_RichView.SendMessage(EM_SETMODIFY, FALSE, 0);
}

void CMainFrame::OnFilePrint()
{
	PRINTDLG pd;

	// Initialize PRINTDLG
	ZeroMemory(&pd, sizeof(pd));
	pd.lStructSize = sizeof(pd);
	pd.hwndOwner   = m_hWnd;
	pd.hDevMode    = NULL;     // Don't forget to free or store hDevMode
	pd.hDevNames   = NULL;     // Don't forget to free or store hDevNames
	pd.Flags       = PD_USEDEVMODECOPIESANDCOLLATE | PD_RETURNDC;
	pd.nCopies     = 1;
	pd.nFromPage   = 0xFFFF;
	pd.nToPage     = 0xFFFF;
	pd.nMinPage    = 1;
	pd.nMaxPage    = 0xFFFF;
	pd.hwndOwner = m_hWnd;

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
		m_RichView.SendMessage(EM_FORMATRANGE, true, (LPARAM)&fr);

		// Set up the print job (standard printing stuff here).
		DOCINFO di;
		ZeroMemory(&di, sizeof(di));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = m_strPathName;

		// Do not print to file.
		di.lpszOutput = NULL;

		// Start the document.
		::StartDoc(hPrinterDC, &di);
		GETTEXTLENGTHEX tl;
		tl.flags = GTL_NUMCHARS;

		// Find out real size of document in characters.
		lTextLength = (LONG)m_RichView.SendMessage(EM_GETTEXTLENGTHEX, (WPARAM)&tl, 0L);

		do
		{
			// Start the page.
			::StartPage(hPrinterDC);

			// Print as much text as can fit on a page. The return value is
			// the index of the first character on the next page. Using TRUE
			// for the wParam parameter causes the text to be printed.
			lTextPrinted = (LONG)::SendMessage(m_RichView.GetHwnd(), EM_FORMATRANGE, true, (LPARAM)&fr);

			m_RichView.SendMessage(EM_DISPLAYBAND, 0, (LPARAM)&fr.rc);

			// Print last page.
			::EndPage(hPrinterDC);

			// If there is more text to print, adjust the range of characters
			// to start printing at the first character of the next page.
			if (lTextPrinted < lTextLength)
			{
				fr.chrg.cpMin = (LONG)lTextPrinted;
				fr.chrg.cpMax = -1;
			}
		}
		while (lTextPrinted < lTextLength);

		// Tell the control to release cached information.
		m_RichView.SendMessage(EM_FORMATRANGE, false, 0L);

		::EndDoc (hPrinterDC);

		// Delete DC when done.
		::DeleteDC(hPrinterDC);
	}
}

void CMainFrame::OnEditCut()
{
	m_RichView.SendMessage(WM_CUT, 0, 0);
}
void CMainFrame::OnEditCopy()
{
	m_RichView.SendMessage(WM_COPY, 0, 0);
}
void CMainFrame::OnEditPaste()
{
	m_RichView.SendMessage(EM_PASTESPECIAL, CF_TEXT, 0);
}
void CMainFrame::OnEditDelete()
{
	m_RichView.SendMessage(WM_CLEAR, 0, 0);
}
void CMainFrame::OnEditRedo()
{
	m_RichView.SendMessage(EM_REDO, 0, 0);
}
void CMainFrame::OnEditUndo()
{
	m_RichView.SendMessage(EM_UNDO, 0, 0);
}

BOOL CMainFrame::ReadFile(LPCTSTR szFileName)
{
	// Open the file for reading
	CFile File;
	if (!File.Open(szFileName, OPEN_EXISTING))
	{
		CString str = _T("Failed to load:  ");
		str += szFileName;
		::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	//Set default font and color
	m_RichView.SetFontDefaults();

	EDITSTREAM es;
	es.dwCookie =  (DWORD_PTR) File.GetHandle();
	es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamInCallback;
	m_RichView.SendMessage(EM_STREAMIN, SF_TEXT, (LPARAM)&es);

	//Clear the modified text flag
	m_RichView.SendMessage(EM_SETMODIFY, FALSE, 0);

	return TRUE;
}

BOOL CMainFrame::WriteFile(LPCTSTR szFileName)
{
	// Open the file for writing
	CFile File;
	if (!File.Open(szFileName, CREATE_ALWAYS))
	{
		CString str = _T("Failed to write:  ");
		str += szFileName;
		::MessageBox(NULL, str, _T("Warning"), MB_ICONWARNING);
		return FALSE;
	}

	EDITSTREAM es;

	es.dwCookie =  (DWORD_PTR) File.GetHandle();
	es.dwError = 0;
	es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamOutCallback;

	m_RichView.SendMessage(EM_STREAMOUT, SF_TEXT, (LPARAM)&es);

	//Clear the modified text flag
	m_RichView.SendMessage(EM_SETMODIFY, FALSE, 0);

	return TRUE;
}

void CMainFrame::OnFileOpen()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	CString Filters( _T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"), 46);
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), Filters, this);

	if (!str.IsEmpty())
	{
		ReadFile(str);
		SetFileName(str);
		AddMRUEntry(str);
		SetWindowTitle();
	}
}

void CMainFrame::OnFileSave()
{
	if (m_strPathName == _T(""))
		OnFileSaveAs();
	else
		WriteFile(m_strPathName);
}

void CMainFrame::OnFileSaveAs()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	CString Filters(_T("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0"), 46);
	CFile File;
	CString str = File.SaveFileDialog(0, OFN_OVERWRITEPROMPT, _T("Save File"), Filters, _T("txt"), this);
	
	if (!str.IsEmpty())
	{
		WriteFile(str);
		SetFileName(str);
		AddMRUEntry(str);
		SetWindowTitle();
	}
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

