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
		CColourDialog Dialog(IDD_DIALOG1, GetMyView().GetImage());
		Dialog.DoModal();
	}
	else
		MessageBox(_T("Open a Bitmap file first!"), _T("Error"), MB_OK);
}

void CMainFrame::ModifyBitmap(int cRed, int cGreen, int cBlue, BOOL bGray)
{
	GetMyView().GetImage().TintBitmap(cRed, cGreen, cBlue);
	
	if (bGray)
	{
		GetMyView().GetImage().GrayScaleBitmap();
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

	// m_UseIndicatorStatus = FALSE;	// Don't show keyboard indicators in the StatusBar
	// m_UseMenuStatus = FALSE;			// Don't show menu descriptions in the StatusBar
	// m_UseReBar = FALSE;				// Don't use a ReBar
	// m_UseThemes = FALSE;				// Don't use themes
	// m_UseToolBar = FALSE;			// Don't use a ToolBar

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
	GetMyView().LoadFileImage(NULL);
	GetMyView().ShowScrollBar(SB_BOTH, FALSE);
	GetMyView().Invalidate();

	// Set the caption
	SetWindowText(_T("FastGDI"));
}

BOOL CMainFrame::LoadFile(CString& FileName)
{
	// Load the bitmap
	BOOL IsFileLoaded = m_MyView.LoadFileImage(FileName);

	if (IsFileLoaded)
	{
		// Save the filename
		m_PathName = FileName;
		AddMRUEntry(FileName);

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

	return IsFileLoaded;
}

void CMainFrame::OnFileOpen()
{
	CFile File;
	CString str = File.OpenFileDialog(0, OFN_FILEMUSTEXIST, _T("Open File"), _T("Bitmap Files (*.bmp)\0*.bmp\0\0"), this);
	if (!str.IsEmpty())
	{
		LoadFile(str);
	}
}

BOOL CMainFrame::OnFileOpenMRU(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	UINT nMRUIndex = LOWORD(wParam) - IDW_FILE_MRU_FILE1;
	CString strMRUText = GetMRUEntry(nMRUIndex);
	CToolBar* pTB = GetToolBar();

	if (m_MyView.LoadFileImage(strMRUText))
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
	SaveFile(m_PathName);
}

void CMainFrame::OnFileSaveAs()
{
	CFile File;
	CString str = File.SaveFileDialog(0, 0, _T("Bitmap Files (*.bmp)\0*.bmp\0\0"), _T("bmp"), 0);

	if (!str.IsEmpty())
	{
		CString str1 = str.Right(4);
		str1.MakeLower();
		if (str1 != _T(".bmp")) 
			str += _T(".bmp");

		SaveFile(str);
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

void CMainFrame::SaveFile(CString& str)
{
	CFile File;
	bool DoSave = TRUE;

	if (File.Open(str, OPEN_EXISTING))
	{
		if (IDYES != MessageBox(_T("File already exists. Do you wish to overwrite it?"), _T("Saving file ") + str, MB_YESNO | MB_ICONWARNING))
			DoSave = FALSE;
	}

	if (DoSave)
	{
		m_PathName = str;
		
		// Set the caption
		CString Title = _T("FastGDI - ") + m_PathName;
		SetWindowText(Title);		
		
		// Save the file
		m_MyView.SaveFileImage(str);
		AddMRUEntry(str);
		TRACE("File Saved\n");
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

