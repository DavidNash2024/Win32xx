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
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_FILE_PRINT:		OnFilePrint();		return TRUE;
	case IDM_PEN_COLOR:			OnPenColor();		return TRUE;
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;

	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:
	case IDW_FILE_MRU_FILE5:	OnFileMRU(wParam);	return TRUE;

	case IDW_VIEW_STATUSBAR:	return OnViewStatusBar();
	case IDW_VIEW_TOOLBAR:		return OnViewToolBar();
	case IDM_HELP_ABOUT:		return OnHelp();
	}

	return FALSE;
}

int CMainFrame::OnCreate(CREATESTRUCT& cs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::OnCreate is optional.
	// Uncomment the lines below to change frame options.

	// SetUseIndicatorStatus(FALSE);	// Don't show keyboard indicators in the StatusBar
	// SetUseMenuStatus(FALSE);			// Don't show menu descriptions in the StatusBar
	// SetUseReBar(FALSE);				// Don't use a ReBar
	// SetUseThemes(FALSE);				// Don't use themes
	// SetUseToolBar(FALSE);			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(cs);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileMRU(WPARAM wParam)
// Called when a MRU file is selected from the menu
{
	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);

	if (GetDoc().FileOpen(strMRUText))
		m_PathName = strMRUText;
	else
		RemoveMRUEntry(strMRUText);

	GetView().Invalidate();
}

void CMainFrame::OnFileNew()
{
	GetDoc().GetAllPoints().clear();
	m_PathName = _T("");
	GetView().Invalidate();
}

void CMainFrame::LoadFile(LPCTSTR str)
// Called by OnFileOpen and in response to a UWM_DROPFILE message
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

	GetView().Invalidate();
}

void CMainFrame::OnFileOpen()
{
	CFileDialog FileDlg(TRUE, _T("dat"), 0, OFN_FILEMUSTEXIST, _T("Scribble Files (*.dat)\0*.dat\0\0"));
	FileDlg.SetTitle(_T("Open File"));

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		// Load the file
		return LoadFile(FileDlg.GetPathName());
	}
}

void CMainFrame::OnFileSave()
{
	if (m_PathName == _T(""))
		OnFileSaveAs();
	else
		GetDoc().FileSave(m_PathName);
}

void CMainFrame::OnFileSaveAs()
{
	CFileDialog FileDlg(FALSE, _T("dat"), 0, OFN_OVERWRITEPROMPT, _T("Scribble Files (*.dat)\0*.dat\0\0"));
	FileDlg.SetTitle(_T("Save File"));

	// Bring up the file open dialog retrieve the selected filename
	if (FileDlg.DoModal(*this) == IDOK)
	{
		CString str = FileDlg.GetPathName();

		// Save the file
		if (GetDoc().FileSave(str))
		{
			// Save the file name
			m_PathName = str;
			AddMRUEntry(m_PathName);
		}
	}

}

// Sends the bitmap extracted from the View window to a printer of your choice
// This function provides a useful reference for printing bitmaps in general
void CMainFrame::OnFilePrint()
{
	// Get the dimensions of the View window
	CRect rcView = GetView().GetClientRect();
	int Width = rcView.Width();
	int Height = rcView.Height();

	// Copy the bitmap from the View window
	CClientDC ViewDC(GetView());
	CMemDC dcMem(ViewDC);
	CBitmap bmView;
	bmView.CreateCompatibleBitmap(ViewDC, Width, Height);
	dcMem.SelectObject(bmView);
	BitBlt(dcMem, 0, 0, Width, Height, ViewDC, 0, 0, SRCCOPY);

	CPrintDialog PrintDlg;

	try
	{
		// Bring up a dialog to choose the printer
		if (PrintDlg.DoModal(GetView()) == IDOK)	// throws exception if there is no default printer
		{
			// Zero and then initialize the members of a DOCINFO structure.
			DOCINFO di;
			memset(&di, 0, sizeof(DOCINFO));
			di.cbSize = sizeof(DOCINFO);
			di.lpszDocName = _T("Scribble Printout");
			di.lpszOutput = (LPTSTR)NULL;
			di.lpszDatatype = (LPTSTR)NULL;
			di.fwType = 0;

			// Begin a print job by calling the StartDoc function.
			CDC dcPrint = PrintDlg.GetPrinterDC();
			if (SP_ERROR == StartDoc(dcPrint, &di))
				throw CUserException(_T("Failed to start print job"));

			// Inform the driver that the application is about to begin sending data.
			if (0 > StartPage(dcPrint))
				throw CUserException(_T("StartPage failed"));

			BITMAPINFOHEADER bi;
			ZeroMemory(&bi, sizeof(BITMAPINFOHEADER));
			bi.biSize = sizeof(BITMAPINFOHEADER);
			bi.biHeight = Height;
			bi.biWidth = Width;
			bi.biPlanes = 1;
			bi.biBitCount = 24;
			bi.biCompression = BI_RGB;

			// Note: BITMAPINFO and BITMAPINFOHEADER are the same for 24 bit bitmaps
			// Get the size of the image data
			dcMem.GetDIBits(bmView, 0, Height, NULL, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

			// Retrieve the image data
			std::vector<byte> vBits(bi.biSizeImage, 0);	// a vector to hold the byte array
			byte* pByteArray = &vBits.front();
			dcMem.GetDIBits(bmView, 0, Height, pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS);

			// Determine the scaling factors required to print the bitmap and retain its original proportions.
			float fLogPelsX1 = (float)ViewDC.GetDeviceCaps(LOGPIXELSX);
			float fLogPelsY1 = (float)ViewDC.GetDeviceCaps(LOGPIXELSY);
			float fLogPelsX2 = (float)GetDeviceCaps(dcPrint, LOGPIXELSX);
			float fLogPelsY2 = (float)GetDeviceCaps(dcPrint, LOGPIXELSY);
			float fScaleX = MAX(fLogPelsX1, fLogPelsX2) / MIN(fLogPelsX1, fLogPelsX2);
			float fScaleY = MAX(fLogPelsY1, fLogPelsY2) / MIN(fLogPelsY1, fLogPelsY2);

			// Compute the coordinates of the upper left corner of the centered bitmap.
			int cWidthPels = GetDeviceCaps(dcPrint, HORZRES);
			int xLeft = ((cWidthPels / 2) - ((int)(((float)Width) * fScaleX)) / 2);
			int cHeightPels = GetDeviceCaps(dcPrint, VERTRES);
			int yTop = ((cHeightPels / 2) - ((int)(((float)Height) * fScaleY)) / 2);

			// Use StretchDIBits to scale the bitmap and maintain its original proportions
			if (GDI_ERROR == (UINT)StretchDIBits(dcPrint, xLeft, yTop, (int)((float)Width * fScaleX),
				(int)((float)Height * fScaleY), 0, 0, Width, Height, pByteArray, reinterpret_cast<BITMAPINFO*>(&bi), DIB_RGB_COLORS, SRCCOPY))
			{
				throw CUserException(_T("Failed to resize image for printing"));
			}

			// Inform the driver that the page is finished.
			if (0 > EndPage(dcPrint))
				throw CUserException(_T("EndPage failed"));

			// Inform the driver that document has ended.
			if (0 > EndDoc(dcPrint))
				throw CUserException(_T("EndDoc failed"));
		}
	}

	catch (const CException& e)
	{
		// Display a message box indicating why printing failed.
		CString strMsg = CString(e.GetText()) + CString("\n") + e.GetErrorString();
		CString strType = CString(e.what());
		::MessageBox(NULL, strMsg, strType, MB_ICONWARNING);
	}

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

void CMainFrame::OnPenColor()
{
	// array of custom colors, initialized to white
	static COLORREF CustColors[16] = {	RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255),
										RGB(255,255,255), RGB(255,255,255), RGB(255,255,255), RGB(255,255,255) };
	
	CColorDialog ColorDlg;
	ColorDlg.SetCustomColors(CustColors);
	
	// Initialize the Choose Color dialog
	if (ColorDlg.DoModal(*this) == IDOK)
	{
		// Store the custom colors in the static array
		memcpy(CustColors, ColorDlg.GetCustomColors(), 16*sizeof(COLORREF));
		
		// Retrieve the chosen color
		m_View.SetPenColor(ColorDlg.GetColor());
	}
}

void CMainFrame::SetupToolBar()
// Configures the ToolBar
{
	// Define our toolbar buttons
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

LRESULT CMainFrame::OnDropFile(WPARAM wParam)
// Called in response to the UWM_DROPFILE user defined message
{
	// wParam is a pointer (LPCTSTR) to the filename
	LPCTSTR szFileName = reinterpret_cast<LPCTSTR>(wParam);
	assert(szFileName);

	// Load the file
	LoadFile(szFileName);
	return 0L;
}

LRESULT CMainFrame::OnGetAllPoints()
// Called in response to the UWM_GETALLPOINTS user defined message
{
	// Get a pointer to the vector of PlotPoints
	std::vector<PlotPoint>* pAllPoints = &GetDoc().GetAllPoints();

	// Cast the pointer to a LRESULT and return it
	return reinterpret_cast<LRESULT>(pAllPoints);
}

LRESULT CMainFrame::OnSendPoint(WPARAM wParam)
// Called in response to the UWM_SENDPOINT user defined message
{
	// wParam is a pointer to the vector of PlotPoints
	PlotPoint* pPP = reinterpret_cast<PlotPoint*>(wParam);

	// Dereference the pointer and store the vector of PlotPoints in CDoc
	assert(pPP);
	GetDoc().StorePoint(*pPP);
	return 0L;
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Called to handle the window's messages
{
	switch (uMsg)
	{
	case UWM_DROPFILE:			return OnDropFile(wParam);
	case UWN_GETALLPOINTS:		return OnGetAllPoints();
	case UWM_SENDPOINT:			return OnSendPoint(wParam);
	}

	//Use the default message handling for remaining messages
	return WndProcDefault(uMsg, wParam, lParam);
}

