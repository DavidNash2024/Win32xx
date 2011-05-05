////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"

#include <UIRibbonPropertyHelpers.h>
#include "RibbonUI.h"
#include "resource.h"
#include "mainfrm.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_View as the view window of the frame
	SetView(m_View);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Frame"));

	// Load the settings from the registry with 4 MRU entries
	LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
	DestroyRibbon();
}

STDMETHODIMP CMainFrame::Execute(UINT32 nCmdID, UI_EXECUTIONVERB verb, const PROPERTYKEY* key, const PROPVARIANT* ppropvarValue, IUISimplePropertySet* pCommandExecutionProperties)
{
	// This function is called when a ribbon button is pressed. 
	// Refer to IUICommandHandler::Execute in the Windows 7 SDK documentation 

	if (UI_EXECUTIONVERB_EXECUTE == verb)
	{
		switch(nCmdID)
		{
		case IDC_CMD_NEW:
			OnFileNew();
			break;
		case IDC_CMD_OPEN:
			OnFileOpen();
			break;
		case IDC_CMD_SAVE:
			OnFileSave();
			break;
		case IDC_CMD_SAVE_AS:
			OnFileSaveAs();
			break;
		case IDC_CMD_PRINT:
			OnFilePrint();
			 break;
		case IDC_CMD_COPY:
			TRACE(_T("Copy\n"));
			break;
		case IDC_CMD_CUT:
			TRACE(_T("Cut\n"));
			break;
		case IDC_CMD_PASTE:
			TRACE(_T("Paste\n"));
			break;
		case IDC_CMD_ABOUT:
			OnHelp();
			break;
		case IDC_CMD_EXIT:
			PostQuitMessage(0);
			break;
		case IDC_MRULIST:
			if (ppropvarValue != NULL && key != NULL && UI_PKEY_SelectedItem == *key)
			{	
				UINT uSelectedMRUItem = ppropvarValue->ulVal;
				MRUFileOpen(uSelectedMRUItem);
			}
			break;
		case IDC_PEN_COLOR:  // DropdownColorPicker button pressed
			{			
				if (ppropvarValue != NULL)
				{
					// Retrieve color type.	
					UINT type = ppropvarValue->uintVal;	
				
					// The Ribbon framework passes color as additional property if the color type is RGB.
					if (type == UI_SWATCHCOLORTYPE_RGB && pCommandExecutionProperties != NULL)
					{
						// Retrieve color.
						PROPVARIANT var;
						if (0 <= pCommandExecutionProperties->GetValue(UI_PKEY_Color, &var))
						{	
							UINT color = var.uintVal;
							m_View.SetPen((COLORREF)color);
						}
					}
				}
			}
			break;
		case IDC_RICHFONT:
			TRACE(_T("Font dialog button\n"));
			break;
		case IDC_RIBBONHELP:
			OnHelp();
			break;
		case IDC_CUSTOMIZE_QAT:
			TRACE(_T("Customize Quick Access ToolBar\n"));
			break;
		default:
			{
				TCHAR t[256];
				wsprintf(t, _T("Unknown Button %d\n"),nCmdID);
				TRACE(t);
			}
			break;
		}
	}

	return S_OK;
}

void CMainFrame::MRUFileOpen(UINT nMRUIndex)
{
	tString tsMRUText = GetMRUEntry(nMRUIndex);

	if (m_View.FileOpen(tsMRUText.c_str()))
		m_PathName = tsMRUText;
	else
		RemoveMRUEntry(tsMRUText.c_str());
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// Process the messages from the (non-ribbon) Menu and Tool Bar

	UNREFERENCED_PARAMETER(lParam);

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
			UINT uMRUEntry = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
			MRUFileOpen(uMRUEntry);
		}
		return TRUE;
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

void CMainFrame::OnFileNew()
{
	m_View.ClearPoints();
	m_PathName = _T("");
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

	// Extract the bitmap from the View window
	CDC ViewDC = m_View.GetDC();
	CDC MemDC = CreateCompatibleDC(ViewDC);
	MemDC.CreateCompatibleBitmap(ViewDC, Width, Height);
	BitBlt(MemDC, 0, 0, Width, Height, ViewDC, 0, 0, SRCCOPY);
	HBITMAP hbmView = MemDC.DetachBitmap();

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
	GetDIBits(MemDC, hbmView, 0, Height, NULL, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Retrieve the image data
	std::vector<byte> vBits(bi.biSizeImage, 0);	// a vector to hold the byte array
	byte* pByteArray = &vBits.front();
	GetDIBits(MemDC, hbmView, 0, Height, pByteArray, (BITMAPINFO*)&bi, DIB_RGB_COLORS);

	// Determine the scaling factors required to print the bitmap and retain its original proportions.
	float fLogPelsX1 = (float) GetDeviceCaps(ViewDC, LOGPIXELSX);
	float fLogPelsY1 = (float) GetDeviceCaps(ViewDC, LOGPIXELSY);
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

	// Cleanup
	::DeleteObject(hbmView);
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE(_T("Frame created\n"));
}

STDMETHODIMP CMainFrame::UpdateProperty(UINT32 nCmdID, __in REFPROPERTYKEY key,  __in_opt  const PROPVARIANT *currentValue, __out PROPVARIANT *newValue) 
{   
	// This function is called when a ribbon button is updated. 
	// Refer to IUICommandHandler::UpdateProperty in the Windows 7 SDK documentation
	UNREFERENCED_PARAMETER(currentValue);

    HRESULT hr = E_NOTIMPL;
    if(UI_PKEY_Enabled == key)
    {
        return UIInitPropertyFromBoolean(UI_PKEY_Enabled, TRUE, newValue);
    }

	switch(nCmdID)
    {
    case IDC_MRULIST:
        // Set up the Most Recently Used (MRU) menu
		if (UI_PKEY_Label == key)
        {
            WCHAR label[MAX_PATH] = L"Recent Files";
            hr = UIInitPropertyFromString(UI_PKEY_Label, label, newValue);
        }
        else if (UI_PKEY_RecentItems == key)
        {
            hr = PopulateRibbonRecentItems(newValue);
        }
        break;

	case IDC_PEN_COLOR:
		// Set the initial pen color
		hr = UIInitPropertyFromUInt32(key, RGB(1, 1, 1), newValue);
		break;
	} 

	return hr;
}

void CMainFrame::SetupToolBar()
{
	// Define our toolbar (used when the ribbon is not displayed)
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
//		Add case statements for each messages to be handled here
//	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

