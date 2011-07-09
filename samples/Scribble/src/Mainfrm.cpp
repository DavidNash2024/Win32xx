////////////////////////////////////////////////////
// Mainfrm.cpp  - definitions for the CMainFrame class

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


CMainFrame::CMainFrame()
{
	// Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Scribble Sample"));

	// Load the settings from the registry with 4 MRU entries
	LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process the messages from the Menu and Tool Bar

	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		m_View.ClearPoints();
		m_PathName = _T("");
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
	case IDM_PEN_RED:
		TRACE(_T("Red pen selected\n"));
		m_View.SetPen(RGB(255,0,0));
		return TRUE;
	case IDM_PEN_BLUE:
		TRACE(_T("Blue pen selected\n"));
		m_View.SetPen(RGB(0,0,255));
		return TRUE;
	case IDM_PEN_GREEN:
		TRACE(_T("Green pen selected\n"));
		m_View.SetPen(RGB(0,196,0));
		return TRUE;
	case IDM_PEN_BLACK:
		TRACE(_T("Black pen selected\n"));
		m_View.SetPen(RGB(0,0,0));
		return TRUE;
	case IDW_VIEW_STATUSBAR:
		OnViewStatusBar();
		return TRUE;
	case IDW_VIEW_TOOLBAR:
		OnViewToolBar();
		return TRUE;
	case IDM_HELP_ABOUT:
		OnHelp();
		return TRUE;
	case IDM_FILE_EXIT:
		::PostMessage(m_hWnd, WM_CLOSE, 0, 0);
		return TRUE;
	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:
		{
			UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
			tString tsMRUText = GetMRUEntry(nMRUIndex);

			if (m_View.FileOpen(tsMRUText.c_str()))
				m_PathName = tsMRUText;
			else
				RemoveMRUEntry(tsMRUText.c_str());

			return TRUE;
		}
	}

	return FALSE;
}

void CMainFrame::OnFileOpen()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.dat)\0*.dat\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("Open File");
	ofn.Flags = OFN_FILEMUSTEXIST;

	// Bring up the dialog, and open the file
	if (!::GetOpenFileName(&ofn))
		return;

	// Retrieve the PlotPoint data
	if (m_View.FileOpen(szFilePathName))
	{

		// Save the filename
		m_PathName = szFilePathName;
		AddMRUEntry(szFilePathName);
	}
	else
		m_PathName=_T("");
}

void CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		m_View.FileSave(m_PathName.c_str());
}

void CMainFrame::OnFileSaveAs()
{
	// Fill the OPENFILENAME structure
	TCHAR szFilters[] = _T("Scribble Files (*.dat)\0*.dat\0\0");
	TCHAR szFilePathName[_MAX_PATH] = _T("");
	OPENFILENAME ofn = {0};
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = m_hWnd;
	ofn.lpstrFilter = szFilters;
	ofn.lpstrFile = szFilePathName;
	ofn.lpstrDefExt = _T("dat");
	ofn.nMaxFile = _MAX_PATH;
	ofn.lpstrTitle = _T("SaveAs File");
	ofn.Flags = OFN_OVERWRITEPROMPT;

	// Open the file save dialog, and open the file
	if (!::GetSaveFileName(&ofn))
		return;

	// Store the PLotPoint data in the file
	m_PathName = szFilePathName;

	// Save the file name
	m_View.FileSave(szFilePathName);
	AddMRUEntry(szFilePathName);
}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CMainFrame::OnFilePrint()
{
	// Get the dimensions of the View window
	CRect rcView = m_View.GetClientRect();
	int Width = rcView.Width();
	int Height = rcView.Height();

	// Copy the bitmap from the View window
	CClientDC ViewDC(&m_View);
	CMemDC MemDC(&ViewDC);
	CBitmap bmView(::CreateCompatibleBitmap(ViewDC, Width, Height));
	MemDC.SelectObject(&bmView);
	BitBlt(MemDC, 0, 0, Width, Height, ViewDC, 0, 0, SRCCOPY);

	// Bring up a dialog to choose the printer
	PRINTDLG pd = {0};
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = m_hWnd;

	// Retrieve the printer DC
	if( !PrintDlg( &pd ) )
	{
		TRACE(_T("PrintDlg canceled"));
		return;
	}

	// Zero and then initialize the members of a DOCINFO structure.
	DOCINFO di;
	memset( &di, 0, sizeof(DOCINFO) );
	di.cbSize = sizeof(DOCINFO);
	di.lpszDocName = _T("Scribble Printout");
	di.lpszOutput = (LPTSTR) NULL;
	di.lpszDatatype = (LPTSTR) NULL;
	di.fwType = 0;

	// Begin a print job by calling the StartDoc function.
	if (SP_ERROR == StartDoc(pd.hDC, &di))
		throw CWinException(_T("Failed to start print job"));

	// Inform the driver that the application is about to begin sending data.
	if (0 > StartPage(pd.hDC))
		throw CWinException(_T("StartPage failed"));

	BITMAPINFOHEADER bi = {0};
	bi.biSize = sizeof(BITMAPINFOHEADER);
	bi.biHeight = Height;
	bi.biWidth = Width;
	bi.biPlanes = 1;
	bi.biBitCount =  24;
	bi.biCompression = BI_RGB;

	// Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
	// Get the size of the image data
	MemDC.GetDIBits(&bmView, 0, Height, NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Retrieve the image data
	std::vector<byte> vBits(bi.biSizeImage, 0);	// a vector to hold the byte array
	byte* pByteArray = &vBits.front();
	MemDC.GetDIBits(&bmView, 0, Height, pByteArray, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Determine the scaling factors required to print the bitmap and retain its original proportions.
	float fLogPelsX1 = (float) ViewDC.GetDeviceCaps(LOGPIXELSX);
	float fLogPelsY1 = (float) ViewDC.GetDeviceCaps(LOGPIXELSY);
	float fLogPelsX2 = (float) GetDeviceCaps(pd.hDC, LOGPIXELSX);
	float fLogPelsY2 = (float) GetDeviceCaps(pd.hDC, LOGPIXELSY);
	float fScaleX = MAX(fLogPelsX1, fLogPelsX2) / MIN(fLogPelsX1, fLogPelsX2);
	float fScaleY = MAX(fLogPelsY1, fLogPelsY2) / MIN(fLogPelsY1, fLogPelsY2);

    // Compute the coordinates of the upper left corner of the centered bitmap.
	int cWidthPels = GetDeviceCaps(pd.hDC, HORZRES);
	int xLeft = ((cWidthPels / 2) - ((int) (((float) Width) * fScaleX)) / 2);
	int cHeightPels = GetDeviceCaps(pd.hDC, VERTRES);
	int yTop = ((cHeightPels / 2) - ((int) (((float) Height) * fScaleY)) / 2);

    // Use StretchDIBits to scale the bitmap and maintain its original proportions
	if (GDI_ERROR == (UINT)StretchDIBits(pd.hDC, xLeft, yTop, (int) ((float) Width * fScaleX),
		(int) ((float) Height * fScaleY), 0, 0, Width, Height, pByteArray, (BITMAPINFO*)&bi, DIB_RGB_COLORS, SRCCOPY))
	{
		throw CWinException(_T("Failed to resize image for printing"));
	}

	// Inform the driver that the page is finished.
	if (0 > EndPage(pd.hDC))
		throw CWinException(_T("EndPage failed"));

	// Inform the driver that document has ended.
	if(0 > EndDoc(pd.hDC))
		throw CWinException(_T("EndDoc failed"));
}

void CMainFrame::OnInitialUpdate()
{
	// Here we process the command line arguments, and automatically load a file if one is specified.
	// GetCommandLineW retrieves our command line arguments.
	// CommandLineToArgvW parses the command line arguements in to an array of strings
	// The first string (lpArgv[0]) contains the name of our program
	// The second string (lpArg[1]) contains an additional parameter (presumably a filename to load).
	// CommandLineToArgvW is not supported in Win95, Win98 or WinME


	// CommandLineToArgvW might not be supported, so use run-time dynamic linking to call the function
	HMODULE hMod = LoadLibrary(_T("Shell32.dll"));
	if (hMod)
	{
		// Get a pointer to the CommandLineToArgvW function
		LPWSTR* (WINAPI* fpGetCommandLineW)(LPCWSTR, int*);
		fpGetCommandLineW = (LPWSTR* (WINAPI*)(LPCWSTR, int*))::GetProcAddress(hMod, "CommandLineToArgvW");

		if (fpGetCommandLineW)
		{
			int argCount = 0;
			LPWSTR* lpArgv = (*fpGetCommandLineW)(::GetCommandLineW(), &argCount);

			// The second argument (if any) contains our file name.
			if (argCount >= 2)
			{
				m_View.FileOpen((W2T(lpArgv[1])));
			}

			LocalFree(lpArgv);
		}

		FreeLibrary(hMod);
	}
}

void CMainFrame::SetupToolBar()
{
	// Define our toolbar
	AddToolBarButton( IDM_FILE_NEW   );
	AddToolBarButton( IDM_FILE_OPEN  );
	AddToolBarButton( IDM_FILE_SAVE  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_EDIT_CUT,   FALSE );
	AddToolBarButton( IDM_EDIT_COPY,  FALSE );
	AddToolBarButton( IDM_EDIT_PASTE, FALSE );
	AddToolBarButton( IDM_FILE_PRINT );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_PEN_RED    );
	AddToolBarButton( IDM_PEN_BLUE   );
	AddToolBarButton( IDM_PEN_GREEN  );
	AddToolBarButton( IDM_PEN_BLACK  );
	AddToolBarButton( 0 );				// Separator
	AddToolBarButton( IDM_HELP_ABOUT );
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{

//	} // switch (uMsg)

	return WndProcDefault(uMsg, wParam, lParam);
} // LRESULT CMainFrame::WndProc(...)

