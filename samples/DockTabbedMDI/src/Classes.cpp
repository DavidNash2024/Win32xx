///////////////////////////////////////////////////
// Classes.cpp - Definitions for the CViewClasses, CContainClasses
//               and CDockClasses classes
//


#include "stdafx.h"
#include "Classes.h"
#include "resource.h"
#include "TabbedMDIApp.h"
#include "resource.h"


////////////////////////////////////
// CViewClasses function definitions
//

// Constructor.
CViewClasses::CViewClasses()
{
}

// Destructor.
CViewClasses::~CViewClasses()
{
    if (IsWindow()) DeleteAllItems();
}

// Called when a window handle (HWND) is attached to CViewClasses.
void CViewClasses::OnAttach()
{
    //set the image lists
    m_normalImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bm(IDB_CLASSVIEW);
    m_normalImages.Add( bm, RGB(255, 0, 0) );
    SetImageList(m_normalImages,  LVSIL_NORMAL);

    // Adjust style to show lines and [+] button
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

    DeleteAllItems();

    // Add some tree-view items
    HTREEITEM htiRoot = InsertItem(_T("TreeView"), 0, 0);
    HTREEITEM htiCTreeViewApp = InsertItem(_T("CTreeViewApp"), 1, 1, htiRoot);
    InsertItem(_T("CTreeViewApp()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("GetMainFrame()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("InitInstance()"), 3, 3, htiCTreeViewApp);
    HTREEITEM htiMainFrame = InsertItem(_T("CMainFrame"), 1, 1, htiRoot);
    InsertItem(_T("CMainFrame()"), 3, 3, htiMainFrame);
    InsertItem(_T("OnCommand()"), 4, 4, htiMainFrame);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiMainFrame);
    HTREEITEM htiView = InsertItem(_T("CView"), 1, 1, htiRoot);
    InsertItem(_T("CView()"), 3, 3, htiView);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiView);
    InsertItem(_T("WndProc()"), 4, 4, htiView);

    // Expand some tree-view items
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}

// Called when the window is destroyed.
void CViewClasses::OnDestroy()
{
    SetImageList(0, LVSIL_SMALL);
}

// Sets the CREATESTRUCT parameters before the window is created.
void CViewClasses::PreCreate(CREATESTRUCT& cs)
{
    cs.style = TVS_NOTOOLTIPS|WS_CHILD;
    cs.lpszClass = WC_TREEVIEW;
}

// Processes the tree-view's window messages.
LRESULT CViewClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEACTIVATE:
            SetFocus();
            break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

///////////////////////////////////////
// CContainClasses function definitions
//

// Constructor.
CContainClasses::CContainClasses()
{
    SetTabText(_T("ClassView"));
    SetTabIcon(IDI_CLASSVIEW);
    SetDockCaption(_T("Class View - Docking container"));
    SetView(m_viewClasses);
}

// Adds a ComboBoxEx control to the container's toolbar.
void CContainClasses::AddCombo()
{
    int width = 120;
    CToolBar& tb = GetToolBar();
    if (tb.CommandToIndex(IDM_FILE_SAVE) < 0) return;

    // Adjust button width and convert to separator
    tb.SetButtonStyle(IDM_FILE_SAVE, TBSTYLE_SEP);
    tb.SetButtonWidth(IDM_FILE_SAVE, width);

    // Determine the size and position of the ComboBox
    int index = tb.CommandToIndex(IDM_FILE_SAVE);
    CRect rect = tb.GetItemRect(index);

    // Create the ComboboxEx window
    m_comboBoxEx.Create(tb);
    m_comboBoxEx.SetWindowPos(0, rect, SWP_NOACTIVATE);

    // Adjust the toolbar height to accommodate the ComboBoxEx control
    CRect rc = m_comboBoxEx.GetWindowRect();
    tb.SetButtonSize(rc.Height(), rc.Height());

    // Add the ComboBox's items
    m_comboBoxEx.AddItems();
}

// Responds to menu and and toolbar input.
BOOL CContainClasses::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_NEW:    return OnFileNew();
    case IDM_HELP_ABOUT:  return OnHelpAbout();
    }

    return FALSE;
}

// Respond to the New button in the container's toolbar.
BOOL CContainClasses::OnFileNew()
{
    TRACE("File New\n");
    MessageBox(_T("File New"), _T("Button Pressed"), MB_OK);

    return TRUE;
}

// Respond to the help button on the container's toolbar.
// Displays the application's help about dialog.
BOOL CContainClasses::OnHelpAbout()
{
    // Send a message to the frame requesting the help dialog
    GetAncestor().SendMessage(WM_HELP);

    return TRUE;
}

// Set the Bitmap resource for the toolbar
void CContainClasses::SetupToolBar()
{
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


////////////////////////////////////
// CDockClasses function definitions
//

// Constructor.
CDockClasses::CDockClasses()
{
    SetView(m_classes);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

