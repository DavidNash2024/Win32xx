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
	case IDM_FILE_NEW:			return OnFileNew();
	case IDM_FILE_OPEN:			return OnFileOpen();
	case IDM_FILE_SAVE:			return OnFileSave();
	case IDM_FILE_SAVEAS:		return OnFileSaveAs();
	case IDM_FILE_PRINT:		return OnFilePrint();
	case IDM_PEN_COLOR:			return OnPenColor();
	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	case IDM_FILE_EXIT:			return OnFileExit();
	
	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	return OnFileMRU(wParam);
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
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

	if (GetDoc().FileOpen(strMRUText))
		m_PathName = strMRUText;
	else
		RemoveMRUEntry(strMRUText);

	GetView().Invalidate();

	return TRUE;
}

BOOL CMainFrame::OnFileNew()
{
	GetDoc().GetPoints().clear();
	m_PathName = _T("");
	GetView().Invalidate();
	return TRUE;
}

BOOL CMainFrame::OnFileOpen()
{
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), _T("Scribble Files (*.dat)\0*.dat\0\0"), *this);

	if (!str.IsEmpty())
	{
		// Retrieve the PlotPoint data
		if (GetDoc().FileOpen(str))
		{
			// Save the filename
			m_PathName = str;
			AddMRUEntry(str);
		}
		else
			m_PathName=_T("");
	}

	GetView().Invalidate();
	return TRUE;
}

BOOL CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		GetDoc().FileSave(m_PathName);

	return TRUE;
}

BOOL CMainFrame::OnFileSaveAs()
{
	CFile File;
	CString str = File.SaveFileDialog(0, OFN_OVERWRITEPROMPT, _T("Save File"), _T("Scribble Files (*.dat)\0*.dat\0\0"), _T("dat"), *this);

	// Store the PlotPoint data in the file
	if (!str.IsEmpty())
	{
		m_PathName = str;

		// Save the file name
		GetDoc().FileSave(str);
		AddMRUEntry(str);
	}

	return TRUE;
}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
BOOL CMainFrame::OnFilePrint()
{
	// Get the dimensions of the View window
	CRect rcView = m_View.GetClientRect();
	int Width = rcView.Width();
	int Height = rcView.Height();

	// Copy the bitmap from the View window
	CClientDC ViewDC(m_View);
	CMemDC MemDC(ViewDC);
	CBitmap bmView;
	bmView.CreateCompatibleBitmap(ViewDC, Width, Height);
	MemDC.SelectObject(bmView);
	BitBlt(MemDC, 0, 0, Width, Height, ViewDC, 0, 0, SRCCOPY);

	// Bring up a dialog to choose the printer
	PRINTDLG pd;
	ZeroMemory(&pd, sizeof(PRINTDLG));
	pd.lStructSize = sizeof( pd );
	pd.Flags = PD_RETURNDC;
	pd.hwndOwner = GetHwnd();

	// Retrieve the printer DC
	if( PrintDlg( &pd ) )
	{
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

		BITMAPINFOHEADER bi;
		ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
		bi.biSize = sizeof(BITMAPINFOHEADER);
		bi.biHeight = Height;
		bi.biWidth = Width;
		bi.biPlanes = 1;
		bi.biBitCount =  24;
		bi.biCompression = BI_RGB;

		// Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
		// Get the size of the image data
		MemDC.GetDIBits(bmView, 0, Height, NULL, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

		// Retrieve the image data
		std::vector<byte> vBits(bi.biSizeImage, 0);	// a vector to hold the byte array
		byte* pByteArray = &vBits.front();
		MemDC.GetDIBits(bmView, 0, Height, pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

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
			(int) ((float) Height * fScaleY), 0, 0, Width, Height, pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, SRCCOPY))
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

	return TRUE;
}

void CMainFrame::OnInitialUpdate()
{
	// Here we process the command line arguments, and automatically load a file if one is specified.
	// GetCommandLineArgs retrieves our command line arguments and stores them in a vector of CString.

	std::vector<CString> args = GetCommandLineArgs();
	// The second argument (if any) contains our file name.
	if (args.size() > 1)
	{
		GetDoc().FileOpen(args[1]);
	}
}

BOOL CMainFrame::OnPenColor()
{
	static COLORREF CustColors[16] = {0};	// array of custom colors
	CHOOSECOLOR cc = {0};					// Structure used by ChooseColor

	cc.lStructSize = sizeof(CHOOSECOLOR);
	cc.Flags = CC_FULLOPEN | CC_RGBINIT;
	cc.rgbResult = m_View.GetPenColor();
	cc.lpCustColors = CustColors;
	cc.hwndOwner = *this;
	
	// Initiate the Choose Color dialog
	if (ChooseColor(&cc)==TRUE) 
	{
		m_View.SetPenColor(cc.rgbResult);
	}

	return TRUE;
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
	AddToolBarButton( IDM_PEN_COLOR );
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

