///////////////////////////////////////////////////
// Classes.cpp - Definitions for the CViewClasses, CContainClasses
//               and CDockClasses classes


#include "stdafx.h"
#include "Classes.h"
#include "resource.h"
#include "TabbedMDIApp.h"


///////////////////////////////////////////////
// CViewClasses functions
CViewClasses::CViewClasses()
{
}

CViewClasses::~CViewClasses()
{
	if (IsWindow()) DeleteAllItems();
}

HTREEITEM CViewClasses::AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage)
{
	TVITEM tvi;
	ZeroMemory(&tvi, sizeof(TVITEM));
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;

	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
	tvis.hParent = hParent;
	tvis.item = tvi;

	return InsertItem(tvis);
}

void CViewClasses::OnDestroy()
{
	SetImageList(NULL, LVSIL_SMALL);
}

void CViewClasses::OnInitialUpdate()
{
	//set the image lists
	m_imlNormal.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	CBitmap bm(IDB_CLASSVIEW);
	m_imlNormal.Add( &bm, RGB(255, 0, 0) );
	SetImageList(&m_imlNormal,  LVSIL_NORMAL);

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

void CViewClasses::PreCreate(CREATESTRUCT &cs)
{
	cs.style = TVS_NOTOOLTIPS|WS_CHILD;
	cs.lpszClass = WC_TREEVIEW;
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
	CToolBar* pTB = GetToolBar();
	if (pTB->CommandToIndex(IDM_FILE_SAVE) < 0) return;

	// Adjust button width and convert to separator
	pTB->SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);
	pTB->SetButtonWidth(IDM_FILE_SAVE, nComboWidth);

	// Determine the size and position of the ComboBox
	int nIndex = pTB->CommandToIndex(IDM_FILE_SAVE);
	CRect rect = pTB->GetItemRect(nIndex);

	// Create the ComboboxEx window
	m_ComboBoxEx.Create(pTB);
	m_ComboBoxEx.SetWindowPos(NULL, rect, SWP_NOACTIVATE);

	// Adjust the toolbar height to accomodate the ComboBoxEx control
	CRect rc = m_ComboBoxEx.GetWindowRect();
	pTB->SetButtonSize( rc.Height(), rc.Height() );

	// Add the ComboBox's items
	m_ComboBoxEx.AddItems();
}

BOOL CContainClasses::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	// OnCommand responds to menu and and toolbar input

	switch(LOWORD(wParam))
	{
	case IDM_FILE_NEW:		OnFileNew();	return TRUE;
	case IDM_HELP_ABOUT:	OnHelpAbout();	return TRUE;
	}

	return FALSE;
}

void CContainClasses::OnFileNew()
{
	TRACE("File New\n");
	MessageBox(_T("File New"), _T("Button Pressed"), MB_OK);
}

void CContainClasses::OnHelpAbout()
{
	// Send a message to the frame requesting the help dialog
	GetTabbedMDIApp()->GetMainFrame()->SendMessage(WM_HELP);
}

void CContainClasses::SetupToolBar()
{
	// Set the Bitmap resource for the toolbar
	SetToolBarImages(RGB(192,192,192), IDW_MAIN, 0, 0);

	// Set the Resource IDs for the toolbar buttons
	AddToolBarButton( IDM_FILE_NEW         );
	AddToolBarButton( IDM_FILE_OPEN, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_SAVE, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_EDIT_CUT         );
	AddToolBarButton( IDM_EDIT_COPY        );
	AddToolBarButton( IDM_EDIT_PASTE       );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_FILE_PRINT, FALSE );

	AddToolBarButton( 0 );	// Separator
	AddToolBarButton( IDM_HELP_ABOUT       );

	// Add the ComboBarEx control to the toolbar
	AddCombo();
}


/////////////////////////////////////////////////
//  Definitions for the CDockClasses class
CDockClasses::CDockClasses()
{
	SetView(m_Classes);
}

int CDockClasses::OnCreate(LPCREATESTRUCT pcs)
{
	// Set the width of the splitter bar
	SetBarWidth(8);

	return CDocker::OnCreate(pcs);
}



