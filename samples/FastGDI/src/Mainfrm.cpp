////////////////////////////////////////////////////
// Mainfrm.cpp

#include "stdafx.h"
#include "mainfrm.h"
#include "resource.h"


// Definitions for the CMainFrame class
CMainFrame::CMainFrame()
{
	// Constructor for CMainFrame. Its called after CFrame's constructor

	//Set m_MyView as the view window of the frame
	SetView(m_MyView);

	// Set the registry key name, and load the initial window position
	// Use a registry key name like "CompanyName\\Application"
	LoadRegistrySettings(_T("Win32++\\Fast GDI Demo"));

	// Load the settings from the registry with 4 MRU entries
	LoadRegistryMRUSettings(4);
}

CMainFrame::~CMainFrame()
{
	// Destructor for CMainFrame.
}

void CMainFrame::OnAdjustImage()
{
	if (GetMyView().GetImage())
	{
		// Initiate the Colour Adjust Dialog
		CColourDialog Dialog(IDD_DIALOG1);
		Dialog.DoModal();
	}
	else
		MessageBox(_T("Open a Bitmap file first!"), _T("Error"), MB_OK);
}

void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL bGray)
{
	CBitmap* pBitmap = CBitmap::FromHandle(GetMyView().GetImage());
	pBitmap->TintBitmap(cRed, cGreen, cBlue);
	if (bGray)
	{
		pBitmap->GrayScaleBitmap();
	}

	GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);
}

BOOL CMainFrame::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_EXIT:			OnFileExit();		return TRUE;
	case IDM_FILE_NEW:			OnFileNew();		return TRUE;
	case IDM_FILE_OPEN:			OnFileOpen();		return TRUE;
	case IDM_FILE_SAVE:			OnFileSave();		return TRUE;
	case IDM_FILE_SAVEAS:		OnFileSaveAs();		return TRUE;
	case IDM_HELP_ABOUT:		OnHelp();			return TRUE;
	case IDM_IMAGE_ADJUST:		OnAdjustImage();	return TRUE;
	case IDW_VIEW_STATUSBAR:	OnViewStatusBar();	return TRUE;
	case IDW_VIEW_TOOLBAR:		OnViewToolBar();	return TRUE;
	case IDW_FILE_MRU_FILE1:
	case IDW_FILE_MRU_FILE2:	// Intentionally blank
	case IDW_FILE_MRU_FILE3:
	case IDW_FILE_MRU_FILE4:	OnFileOpenMRU(wParam, lParam);	return TRUE;
	}

	return FALSE;
}

int CMainFrame::OnCreate(LPCREATESTRUCT pcs)
{
	// OnCreate controls the way the frame is created.
	// Overriding CFrame::Oncreate is optional.
	// The default for the following variables is TRUE

	// m_bUseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_bUseMenuStatus = FALSE;		// Don't show menu descriptions in the StatusBar
	// m_bUseReBar = FALSE;				// Don't use a ReBar
	// m_bUseThemes = FALSE;            // Don't use themes
	// m_bUseToolBar = FALSE;			// Don't use a ToolBar

	// call the base class function
	return CFrame::OnCreate(pcs);
}

void CMainFrame::OnFileExit()
{
	// Issue a close request to the frame
	PostMessage(WM_CLOSE);
}

void CMainFrame::OnFileNew()
{
	CToolBar* pTB = GetToolBar();
	pTB->DisableButton(IDM_FILE_SAVEAS);
	pTB->DisableButton(IDM_IMAGE_ADJUST);
	m_MyView.FileOpen(NULL);
	m_MyView.Invalidate();

	// Set the caption
	SetWindowText(_T("FastGDI"));
}

void CMainFrame::OnFileOpen()
{
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), _T("Bitmap Files (*.bmp)\0*.bmp\0\0"), this);
	if (!str.IsEmpty())
	{
		// Load the bitmap
		m_MyView.FileOpen(str);

		// Save the filename
		m_PathName = str;
		AddMRUEntry(str);

		// Turn on the ToolBar adjust button
		CToolBar* pTB = GetToolBar();
		pTB->EnableButton(IDM_FILE_SAVEAS);
		pTB->EnableButton(IDM_IMAGE_ADJUST);
		GetFrameMenu()->EnableMenuItem(IDM_IMAGE_ADJUST, MF_BYCOMMAND | MF_ENABLED);

		// Resize the frame to match the bitmap
		if (GetMyView().GetImage())
		{
			GetMyView().ShowScrollBar(SB_BOTH, FALSE);
			CRect rcImage = GetMyView().GetImageRect();
			AdjustFrameRect(rcImage);
		}

		GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

		// Set the caption
		CString str = _T("FastGDI - ") + m_PathName;
		SetWindowText(str);
	}
}

BOOL CMainFrame::OnFileOpenMRU(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);
	CToolBar* pTB = GetToolBar();

	if (m_MyView.FileOpen(strMRUText))
	{
		m_PathName = strMRUText;
		pTB->EnableButton(IDM_FILE_SAVEAS);
		pTB->EnableButton(IDM_IMAGE_ADJUST);

		// Adjust the window size
		CRect rcImage = GetMyView().GetImageRect();
		AdjustFrameRect(rcImage);
	}
	else
	{
		RemoveMRUEntry(strMRUText);
		pTB->DisableButton(IDM_FILE_SAVEAS);
		pTB->DisableButton(IDM_IMAGE_ADJUST);
	}

	// Resize the frame to match the bitmap
	if (GetMyView().GetImage())
	{
		GetMyView().ShowScrollBar(SB_BOTH, FALSE);
		CRect rcImage = GetMyView().GetImageRect();
		AdjustFrameRect(rcImage);
	}

	GetMyView().RedrawWindow(0, 0, RDW_NOERASE|RDW_INVALIDATE|RDW_UPDATENOW);

	// Set the caption
	CString str = _T("FastGDI - ") + m_PathName;
	SetWindowText(str);
	return TRUE;
}

void CMainFrame::OnFileSave()
{
	if (!m_PathName.IsEmpty())
	{
		CString str = m_PathName + _T("  already exists.\nDo you want to replace it?");

		if (IDYES == MessageBox(str, _T("FileSaveAs"), MB_YESNO | MB_ICONWARNING))
			m_MyView.FileSave(m_PathName);
	}
}

void CMainFrame::OnFileSaveAs()
{
	CFile File;
	CString str = File.SaveFileDialog(0, OFN_OVERWRITEPROMPT, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"), _T("bmp"), 0);
	if (!str.IsEmpty())
	{
		// Set the caption
		m_PathName = str;
		CString Title = _T("FastGDI - ") + m_PathName;
		SetWindowText(Title);

		// Save the file name
		m_MyView.FileSave(str);
	}
}

void CMainFrame::OnInitialUpdate()
{
	// The frame is now created.
	// Place any additional startup code here.

	TRACE("Frame created\n");
}

inline void CMainFrame::OnMenuUpdate(UINT nID)
// Called when menu items are about to be displayed
{
	bool IsImageLoaded = (GetMyView().GetImage() != 0);

	switch(nID)
	{
	case IDM_FILE_SAVE:
		GetFrameMenu()->EnableMenuItem(IDM_FILE_SAVE, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	case IDM_FILE_SAVEAS:
		GetFrameMenu()->EnableMenuItem(IDM_FILE_SAVEAS, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	case IDM_IMAGE_ADJUST:
		GetFrameMenu()->EnableMenuItem(IDM_IMAGE_ADJUST, IsImageLoaded? MF_ENABLED : MF_GRAYED);
		break;
	}
}

void CMainFrame::SetupToolBar()
{
	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW  );
	AddToolBarButton( IDM_FILE_OPEN );
	AddToolBarButton( IDM_FILE_SAVEAS, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_IMAGE_ADJUST, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT );

	// Use large toolbar buttons
	SetToolBarImages(RGB(192, 192, 192), IDB_TOOLBAR_BIG, 0, 0);
}

LRESULT CMainFrame::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_SIZE:
		{
			// Remove scrollbars and redraw the view if the frame is maximized
			if (wParam == SIZE_MAXIMIZED)
			{
				GetView()->ShowScrollBar(SB_BOTH, FALSE);
				GetView()->Invalidate();
			}
			
			break;
		}
	}

	// pass unhandled messages on for default processing
	return WndProcDefault(uMsg, wParam, lParam);
}

