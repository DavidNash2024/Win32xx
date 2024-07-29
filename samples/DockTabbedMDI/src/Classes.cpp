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
    SetDPIImages();

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
    SetImageList(normalImages, TVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = normalImages.GetIconSize().cx;
    SetIndent(imageWidth);
}

void CViewClasses::PreCreate(CREATESTRUCT& cs)
{
    // Call base class to set defaults.
    CTreeView::PreCreate(cs);

    // Add the WS_EX_COMPOSITED to reduce flicker.
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
        cs.dwExStyle |= WS_EX_COMPOSITED;
}

// Processes the tree-view's window messages.
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

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
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
}

// Handle the window's messages.
LRESULT CContainClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
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

// This function overrides CDocker::RecalcDockLayout to elimate jitter
// when the dockers are resized. The technique used here is is most
// appropriate for a complex arrangement of dockers.  It might not suite
// other docking applications. To support this technique the
// WS_EX_COMPOSITED extended style has been added to some view windows.
void CDockClasses::RecalcDockLayout()
{
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
    {
        if (GetDockAncestor()->IsWindow())
        {
            GetTopmostDocker()->LockWindowUpdate();
            CRect rc = GetTopmostDocker()->GetViewRect();
            GetTopmostDocker()->RecalcDockChildLayout(rc);
            GetTopmostDocker()->UnlockWindowUpdate();
            GetTopmostDocker()->UpdateWindow();
        }
    }
    else
        CDocker::RecalcDockLayout();
}

// Handle the window's messages.
LRESULT CDockClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
