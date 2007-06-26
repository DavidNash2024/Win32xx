/////////////////////////////////////////////////
// Mainfrm.cpp

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
	m_strPathName = TEXT("");
	SetView(m_RichView);

	// Define the resource IDs for the toolbar
	m_ToolbarData.clear();
	m_ToolbarData.push_back ( IDM_FILE_NEW   );
	m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	m_ToolbarData.push_back ( IDM_FILE_SAVE  );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_EDIT_CUT   );
	m_ToolbarData.push_back ( IDM_EDIT_COPY  );
	m_ToolbarData.push_back ( IDM_EDIT_PASTE );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_FILE_PRINT );
	m_ToolbarData.push_back ( 0 );				// Separator
	m_ToolbarData.push_back ( IDM_HELP_ABOUT );
}

CMainFrame::~CMainFrame()
{
}

void CMainFrame::OnInitialUpdate()
{
	::DragAcceptFiles(m_hWnd, TRUE);
	SetWindowTitle();
	::SetFocus(m_RichView.GetHwnd());
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
		break;
	}

	return 0L;
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		OnFileNew();
		return TRUE;
	case IDM_FILE_OPEN:
		OnFileOpen();
		return TRUE;
	case IDM_FILE_SAVE:
		OnFileSave();
		return TRUE;
	case IDM_FILE_SAVEAS:
		OnFileSaveAs();
		return TRUE;
	case IDM_FILE_PRINT:
		OnFilePrint();
		return TRUE;
	case IDM_EDIT_COPY:
		OnEditCopy();
		return TRUE;
	case IDM_EDIT_PASTE:
		OnEditPaste();
		return TRUE;
	case IDM_EDIT_CUT:
		OnEditCut();
		return TRUE;
	case IDM_EDIT_DELETE:
		OnEditDelete();
		return TRUE;
	case IDM_EDIT_REDO:
		OnEditRedo();
		return TRUE;
	case IDM_EDIT_UNDO:
		OnEditUndo();
		return TRUE;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	} // switch cmd
	
	return FALSE;
} // CMainFrame::OnCommand(...)


void CMainFrame::OnFileNew()
{
	::SetWindowText(m_RichView.GetHwnd(), TEXT(""));
	m_strPathName = TEXT("");
	SetWindowTitle();
	m_RichView.SetFontDefaults();
	::SendMessage(m_RichView.GetHwnd(), EM_SETMODIFY, FALSE, 0);
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
		::SendMessage(m_RichView.GetHwnd(), EM_FORMATRANGE, true, (LPARAM)&fr);

		// Set up the print job (standard printing stuff here).
		DOCINFO di;
		ZeroMemory(&di, sizeof(di));
		di.cbSize = sizeof(DOCINFO);
		di.lpszDocName = m_strPathName.c_str();

		// Do not print to file.
		di.lpszOutput = NULL;

		// Start the document.
		::StartDoc(hPrinterDC, &di);
		GETTEXTLENGTHEX tl;
		tl.flags = GTL_NUMCHARS;

		// Find out real size of document in characters.
		lTextLength = (LONG)::SendMessage(m_RichView.GetHwnd(), EM_GETTEXTLENGTHEX, (WPARAM)&tl, 0L);

		do
		{
			// Start the page.
			::StartPage(hPrinterDC);

			// Print as much text as can fit on a page. The return value is
			// the index of the first character on the next page. Using TRUE
			// for the wParam parameter causes the text to be printed.
			lTextPrinted = (LONG)::SendMessage(m_RichView.GetHwnd(), EM_FORMATRANGE, true, (LPARAM)&fr);

			::SendMessage(m_RichView.GetHwnd(), EM_DISPLAYBAND, 0, (LPARAM)&fr.rc);

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
		::SendMessage(m_RichView.GetHwnd(), EM_FORMATRANGE, false, 0L);

		::EndDoc (hPrinterDC);

		// Delete DC when done.
		::DeleteDC(hPrinterDC);
	}
}

void CMainFrame::OnEditCut()
{
	::SendMessage(m_RichView.GetHwnd(), WM_CUT, 0, 0);
}
void CMainFrame::OnEditCopy()
{
	::SendMessage(m_RichView.GetHwnd(), WM_COPY, 0, 0);
}
void CMainFrame::OnEditPaste()
{
	::SendMessage(m_RichView.GetHwnd(), EM_PASTESPECIAL, CF_TEXT, 0);
}
void CMainFrame::OnEditDelete()
{
	::SendMessage(m_RichView.GetHwnd(), WM_CLEAR, 0, 0);
}
void CMainFrame::OnEditRedo()
{
	::SendMessage(m_RichView.GetHwnd(), EM_REDO, 0, 0);
}
void CMainFrame::OnEditUndo()
{
	::SendMessage(m_RichView.GetHwnd(), EM_UNDO, 0, 0);
}

BOOL CMainFrame::OnClose()
{
	//Check for unsaved text
	BOOL bChanged = (BOOL)::SendMessage(m_RichView.GetHwnd(), EM_GETMODIFY, 0, 0);
	if (bChanged)
		if (::MessageBox(NULL, TEXT("Save changes to this document"), TEXT("TextEdit"), MB_YESNO | MB_ICONWARNING) == IDYES)
			OnFileSave();

	return FALSE;
}

void CMainFrame::OnDropFiles(HDROP hDropInfo)
{
	TCHAR szFileName[_MAX_PATH];
	::DragQueryFile((HDROP)hDropInfo, 0, (LPTSTR)szFileName, _MAX_PATH);

	ReadFile(szFileName);
}

void CMainFrame::ReadFile(LPCTSTR szFileName)
{
	HANDLE hFile;

	// Open the file for reading
	hFile = ::CreateFile(szFileName, GENERIC_READ, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		tStringStream buf;
		buf << TEXT("Failed to load:  ") << szFileName;
		::MessageBox(NULL, buf.str().c_str(), TEXT("Warning"), MB_ICONWARNING);
		return;
	}

	//Set default font and color
	m_RichView.SetFontDefaults();

	EDITSTREAM es;

	es.dwCookie =  (DWORD_PTR) hFile;
	es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamInCallback;

	::SendMessage(m_RichView.GetHwnd(), EM_STREAMIN, SF_TEXT, (LPARAM)&es);
	::CloseHandle(hFile);


	//Clear the modified text flag
	::SendMessage(m_RichView.GetHwnd(), EM_SETMODIFY, FALSE, 0);
}

void CMainFrame::WriteFile(LPCTSTR szFileName)
{
	HANDLE hFile;

	// Open the file for writing
	hFile = ::CreateFile(szFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL,
				CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		tStringStream buf;
		buf << TEXT("Failed to load:   ") << szFileName;
		::MessageBox(NULL, buf.str().c_str(), TEXT("Warning"), MB_ICONWARNING);
		return;
	}

	EDITSTREAM es;

	es.dwCookie =  (DWORD_PTR) hFile;
	es.dwError = 0;
	es.pfnCallback = (EDITSTREAMCALLBACK) MyStreamOutCallback;

	::SendMessage(m_RichView.GetHwnd(), EM_STREAMOUT, SF_TEXT, (LPARAM)&es);
	::CloseHandle(hFile);


	//Clear the modified text flag
	::SendMessage(m_RichView.GetHwnd(), EM_SETMODIFY, FALSE, 0);
}

void CMainFrame::OnFileOpen()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	TCHAR szFilters[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
	TCHAR szFilePathName[_MAX_PATH] = TEXT("");
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = TEXT("Open File");
	ofn.Flags = OFN_FILEMUSTEXIST;

	if (!::GetOpenFileName(&ofn))
		return;

	ReadFile(szFilePathName);
	SetFileName(szFilePathName);
	SetWindowTitle();
}

void CMainFrame::OnFileSave()
{
	if (m_strPathName == TEXT(""))
		OnFileSaveAs();
	else
		WriteFile(m_strPathName.c_str());
}

void CMainFrame::OnFileSaveAs()
{
	// szFilters is a text string that includes two file name filters:
	// "*.my" for "MyType Files" and "*.*' for "All Files."
	TCHAR szFilters[] = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0\0");
	TCHAR szFilePathName[_MAX_PATH] = TEXT("");
	OPENFILENAME ofn;
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = TEXT("txt");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = TEXT("Save File");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	if (!::GetSaveFileName(&ofn))
		return;

	WriteFile(szFilePathName);
	SetFileName(szFilePathName);
	SetWindowTitle();
}

void CMainFrame::SetFileName(TCHAR* szFilePathName)
{
	//Truncate and save file name
	int i = lstrlen(szFilePathName)+1;
	while ((--i > 0) && (szFilePathName[i-1] != TEXT('\\')));

	m_strPathName = szFilePathName+i;
}

void CMainFrame::SetWindowTitle()
{
    tString Title;

	if (m_strPathName == TEXT("")) Title = TEXT("TextEdit - Untitled");

	else Title = TEXT("TextEdit - ") + m_strPathName;
	::SetWindowText(m_hWnd, Title.c_str());
}

LRESULT CMainFrame::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SETFOCUS:
		::SetFocus(m_RichView.GetHwnd());
		break;
	case WM_SIZE:
		::InvalidateRect(m_RichView.GetHwnd(), NULL, TRUE);
		break;
	}
	return 0L;
}

DWORD CALLBACK CMainFrame::MyStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Required for StreamIn
	if (!cb)
		return (1);

	*pcb = 0;
	if (!::ReadFile((HANDLE)(DWORD_PTR) dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
		::MessageBox(NULL, TEXT("ReadFile Failed"), TEXT(""), MB_OK);

	return 0;
}

DWORD CALLBACK CMainFrame::MyStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	// Required for StreamOut
	if(!cb)
		return (1);

	*pcb = 0;
	if (!::WriteFile((HANDLE)(DWORD_PTR)dwCookie, pbBuff, cb, (LPDWORD)pcb, NULL))
		::MessageBox(NULL, TEXT("WriteFile Failed"), TEXT(""), MB_OK);
	return 0;
}

