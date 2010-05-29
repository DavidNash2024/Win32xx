///////////////////////////////////////////////////
// Classes.cpp - Definitions for the CViewClasses, CContainClasses
//               and CDockClasses classes


#include "stdafx.h"
#include "Classes.h"
#include "resource.h"
#include "ContainerApp.h"


///////////////////////////////////////////////
// CViewClasses functions
CViewClasses::CViewClasses() : m_himlNormal(0)
{
}

CViewClasses::~CViewClasses()
{
	if (IsWindow()) DeleteAllItems();
	ImageList_Destroy(m_himlNormal);
}

void CViewClasses::OnInitialUpdate()
{
	//set the image lists
	m_himlNormal = ImageList_Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(IDB_CLASSVIEW));
	ImageList_AddMasked(m_himlNormal, hbm, RGB(255, 0, 0));
	SetImageList(m_himlNormal, LVSIL_NORMAL);
	::DeleteObject(hbm);

	// Adjust style to show lines and [+] button
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLongPtr(GWL_STYLE, dwStyle);

	DeleteAllItems();

	// Add some tree-view items
	HTREEITEM htiRoot = AddItem(NULL, _T("TreeView"), 0);
	HTREEITEM htiCTreeViewApp = AddItem(htiRoot, _T("CTreeViewApp"), 1);
	AddItem(htiCTreeViewApp, _T("CTreeViewApp()"), 3);
	AddItem(htiCTreeViewApp, _T("GetMainFrame()"), 3);
	AddItem(htiCTreeViewApp, _T("InitInstance()"), 3);
	HTREEITEM htiMainFrame = AddItem(htiRoot, _T("CMainFrame"), 1);
	AddItem(htiMainFrame, _T("CMainFrame()"), 3);
	AddItem(htiMainFrame, _T("OnCommand()"), 4);
	AddItem(htiMainFrame, _T("OnInitialUpdate()"), 4);
	AddItem(htiMainFrame, _T("WndProc()"), 4);
	HTREEITEM htiView = AddItem(htiRoot, _T("CView"), 1);
	AddItem(htiView, _T("CView()"), 3);
	AddItem(htiView, _T("OnInitialUpdate()"), 4);
	AddItem(htiView, _T("WndProc()"), 4);

	// Expand some tree-view items
	Expand(htiRoot, TVE_EXPAND);
	Expand(htiCTreeViewApp, TVE_EXPAND);
}

HTREEITEM CViewClasses::AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage)
{
	TVITEM tvi = {0};
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;

	TVINSERTSTRUCT tvis = {0};
	tvis.hParent = hParent;
	tvis.item = tvi;

	return InsertItem(tvis);
}

LRESULT CViewClasses::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			SetImageList(NULL, LVSIL_SMALL);
			break;
		}
	}

	return WndProcDefault(uMsg, wParam, lParam);
}


///////////////////////////////////////////////
// CContainClasses functions
CContainClasses::CContainClasses() 
{
	SetTabText(_T("ClassView"));
	SetTabIcon(IDI_CLASSVIEW);
	SetDockCaption (_T("Class View - Docking container"));
	SetView(m_ViewClasses);
}

void CContainClasses::AddCombo()
{
	int nComboWidth = 120; 
	CToolbar& TB = GetToolbar();
	if (TB.CommandToIndex(IDM_FILE_SAVE) < 0) return;
	 
	// Adjust button width and convert to separator   
	TB.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);
	TB.SetButtonWidth(IDM_FILE_SAVE, nComboWidth);
	 
	// Determine the size and position of the ComboBox 
	int nIndex = TB.CommandToIndex(IDM_FILE_SAVE); 
	CRect rect = TB.GetItemRect(nIndex); 
	 
	// Create the ComboboxEx window 
	m_ComboBoxEx.Create(TB.GetHwnd());
	m_ComboBoxEx.SetWindowPos(NULL, rect, SWP_NOACTIVATE);

	// Adjust the toolbar height to accomodate the ComboBoxEx control
	CRect rc = m_ComboBoxEx.GetWindowRect();
	GetToolbar().SendMessage(TB_SETBUTTONSIZE, 0, (LPARAM) MAKELONG (rc.Height(), rc.Height()));
	
	// Add the ComboBox's items
	m_ComboBoxEx.AddItems();
}

BOOL CContainClasses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_NEW:
		TRACE(_T("File New\n"));
		break;
	case IDM_FILE_OPEN:
		TRACE(_T("File Open\n"));
		break;
	case IDM_FILE_SAVE:
		TRACE(_T("FILE Save\n"));
		break;
	}

	return FALSE;
}

void CContainClasses::SetupToolbar()
{
	// Set the Bitmap resource for the toolbar
	GetToolbar().SetImages(RGB(192,192,192), IDW_MAIN, 0, 0);
	
	// Set the Resource IDs for the toolbar buttons
	AddToolbarButton( IDM_FILE_NEW         );
	AddToolbarButton( IDM_FILE_OPEN, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_FILE_SAVE, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_EDIT_CUT         );
	AddToolbarButton( IDM_EDIT_COPY        );
	AddToolbarButton( IDM_EDIT_PASTE       );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_FILE_PRINT, FALSE );
	
	AddToolbarButton( 0 );	// Separator
	AddToolbarButton( IDM_HELP_ABOUT       );

	// Add the ComboBarEx control to the toolbar
	AddCombo();

	// Match the Container's Toolbar theme to the MainFrame's toolbar theme
	ToolbarTheme tt = GetContainerApp().GetMainFrame().GetToolbar().GetToolbarTheme();
	GetToolbar().SetToolbarTheme(tt);
}


/////////////////////////////////////////////////
//  Definitions for the CDockClasses class
CDockClasses::CDockClasses() 
{ 
	SetView(m_Classes); 
}

void CDockClasses::OnInitialUpdate()
{
	// Set the width of the splitter bar
	SetBarWidth(8);
}



