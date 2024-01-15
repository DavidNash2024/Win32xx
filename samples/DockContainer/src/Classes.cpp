///////////////////////////////////////////////////
// Classes.cpp - Definitions for the CViewClasses, CContainClasses
//               and CDockClasses classes
//


#include "stdafx.h"
#include "Classes.h"
#include "ContainerApp.h"
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
    // Set the image lists
    SetDPIImages();

    // Adjust style to show lines and [+] button
    DWORD dwStyle = GetStyle();
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(dwStyle);

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

// Respond to a mouse click on the window.
LRESULT CViewClasses::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called in response to a WM_DPICHANGED_BEFOREPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewClasses::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetDPIImages();
    return FinalWindowProc(msg, wparam, lparam);
}

// Set the CREATESTURCT parameters befoe the window is created.
void CViewClasses::PreCreate(CREATESTRUCT& cs)
{
    cs.style = TVS_NOTOOLTIPS | WS_CHILD;
}

// Adjusts the treeview image sizes in response to window DPI changes.
void CViewClasses::SetDPIImages()
{
    // Resize the image list.
    CBitmap bmImage(IDB_CLASSVIEW);
    bmImage = DpiScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    CImageList normalImages;
    normalImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    normalImages.Add(bmImage, RGB(255, 0, 0));
    SetImageList(normalImages, LVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = normalImages.GetIconSize().cx;
    SetIndent(imageWidth);
}

// Process the tree-view's window messages.
LRESULT CViewClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        case WM_DPICHANGED_BEFOREPARENT: return OnDpiChangedBeforeParent(msg, wparam, lparam);
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
    SetDockCaption (_T("Class View - Docking container"));
    SetView(m_viewClasses);
}

// Process the command messages (WM_COMMAND).
BOOL CContainClasses::OnCommand(WPARAM wparam, LPARAM)
{
    // OnCommand responds to menu and and toolbar input
    UINT id = LOWORD(wparam);
    switch(id)
    {
    case IDM_FILE_NEW:      return OnFileNew();
    case IDM_HELP_ABOUT:    return OnHelpAbout();
    }

    return FALSE;
}

// Demonstrates responding to the container's toolbar.
BOOL CContainClasses::OnFileNew()
{
    TRACE("File New\n");
    MessageBox(_T("File New"), _T("Button Pressed"), MB_OK);
    return TRUE;
}

// Demonstrates responding to the container's toolbar.
// Invokes the program's Help About dialog.
BOOL CContainClasses::OnHelpAbout()
{
    // Send a message to the frame requesting the help dialog
    GetAncestor().SendMessage(WM_HELP);
    return TRUE;
}

// Set the Bitmap resource for the toolbar
void CContainClasses::SetupToolBar()
{
    SetToolBarImages(RGB(192, 192, 192), IDW_MAIN, 0, 0);

    // Set the Resource IDs for the toolbar buttons
    AddToolBarButton(IDM_FILE_NEW);
    AddToolBarButton(IDM_FILE_OPEN, FALSE);

    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FILE_SAVE, FALSE);

    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_EDIT_CUT);
    AddToolBarButton(IDM_EDIT_COPY);
    AddToolBarButton(IDM_EDIT_PASTE);

    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_FILE_PRINT, FALSE);

    AddToolBarButton(0);  // Separator
    AddToolBarButton(IDM_HELP_ABOUT);
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
