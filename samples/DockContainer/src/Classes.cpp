///////////////////////////////////////////////////
// Classes.cpp - Definitions for the CViewClasses, CContainClasses
//               and CDockClasses classes


#include "stdafx.h"
#include "Classes.h"
#include "ContainerApp.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewClasses functions
CViewClasses::CViewClasses()
{
}

CViewClasses::~CViewClasses()
{
    if (IsWindow()) DeleteAllItems();
}

HTREEITEM CViewClasses::AddItem(HTREEITEM hParent, LPCTSTR text, int image)
{
    TVITEM tvi;
    ZeroMemory(&tvi, sizeof(TVITEM));
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.iImage = image;
    tvi.iSelectedImage = image;
    tvi.pszText = const_cast<LPTSTR>(text);

    TVINSERTSTRUCT tvis;
    ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
    tvis.hParent = hParent;
    tvis.item = tvi;

    return InsertItem(tvis);
}

void CViewClasses::OnAttach()
{
    //set the image lists
    m_normalImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bm(IDB_CLASSVIEW);
    m_normalImages.Add( bm, RGB(255, 0, 0) );
    SetImageList(m_normalImages, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button
    DWORD dwStyle = GetStyle();
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(dwStyle);

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

void CViewClasses::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

LRESULT CViewClasses::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse click on the window
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

void CViewClasses::PreCreate(CREATESTRUCT& cs)
{
    cs.style = TVS_NOTOOLTIPS|WS_CHILD;
    cs.lpszClass = WC_TREEVIEW;
}

LRESULT CViewClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}



///////////////////////////////////////////////
// CContainClasses functions
CContainClasses::CContainClasses() 
{
    SetTabText(_T("ClassView"));
    SetTabIcon(IDI_CLASSVIEW);
    SetDockCaption (_T("Class View - Docking container"));
    SetView(m_viewClasses);
}

void CContainClasses::AddCombo()
{
    int comboWidth = 120; 
    CToolBar& tb = GetToolBar();
    if (tb.CommandToIndex(IDM_FILE_SAVE) < 0) return;
     
    // Adjust button width and convert to separator   
    tb.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);
    tb.SetButtonWidth(IDM_FILE_SAVE, comboWidth);
     
    // Determine the size and position of the ComboBox 
    int index = tb.CommandToIndex(IDM_FILE_SAVE);
    CRect rect = tb.GetItemRect(index);
     
    // Create the ComboboxEx window 
    m_comboBoxEx.Create(tb);
    m_comboBoxEx.SetWindowPos(NULL, rect, SWP_NOACTIVATE);

    // Adjust the toolbar height to accommodate the ComboBoxEx control
    CRect rc = m_comboBoxEx.GetWindowRect();
    tb.SetButtonSize( rc.Height(), rc.Height() );
    
    // Add the ComboBox's items
    m_comboBoxEx.AddItems();
}

BOOL CContainClasses::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);

    // OnCommand responds to menu and and toolbar input
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_NEW:      return OnFileNew();
    case IDM_HELP_ABOUT:    return OnHelpAbout();
    }

    return FALSE;
}

BOOL CContainClasses::OnFileNew()
{
    TRACE("File New\n");
    MessageBox(_T("File New"), _T("Button Pressed"), MB_OK);
    return TRUE;
}

BOOL CContainClasses::OnHelpAbout()
{
    // Send a message to the frame requesting the help dialog
    GetContainerApp()->GetMainFrame().SendMessage(WM_HELP);
    return TRUE;
}

void CContainClasses::SetupToolBar()
{
    // Set the Bitmap resource for the toolbar
    SetToolBarImages(RGB(192,192,192), IDW_MAIN, 0, 0);
    
    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton( IDM_FILE_NEW         );
    AddToolBarButton( IDM_FILE_OPEN, FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_FILE_SAVE, FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_EDIT_CUT         );
    AddToolBarButton( IDM_EDIT_COPY        );
    AddToolBarButton( IDM_EDIT_PASTE       );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_FILE_PRINT, FALSE );
    
    AddToolBarButton( 0 );  // Separator
    AddToolBarButton( IDM_HELP_ABOUT       );

    // Add the ComboBarEx control to the toolbar
    AddCombo();
}


/////////////////////////////////////////////////
//  Definitions for the CDockClasses class
CDockClasses::CDockClasses() 
{ 
    SetView(m_classes); 

    // Set the width of the splitter bar
    SetBarWidth(8);
}

