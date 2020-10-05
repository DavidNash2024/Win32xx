//////////////////////////////////////////////
// MDIChildTreeView.cpp
//  Definitions for the CViewTree and CMDIChildTreeView classes

#include "stdafx.h"
#include "MDIChildTreeView.h"
#include "resource.h"


/////////////////////////////////
// CViewTree function definitions
//

// Constructor.
CViewTree::CViewTree()
{
}

// Destructor.
CViewTree::~CViewTree()
{
    if (IsWindow()) DeleteAllItems();
}

// Called when the window is destroyed.
void CViewTree::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Called when a window handle (HWND) is attached to CViewTree.
void CViewTree::OnAttach()
{
    // Set the image lists.
    m_normalImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_CLASSVIEW);
    m_normalImages.Add( image, RGB(255, 0, 0) );
    SetImageList(m_normalImages, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button.
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

    DeleteAllItems();

    // Add some tree-view items.
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

    // Expand some tree-view items.
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}


/////////////////////////////////////////
// CMDIChildTreeView function definitions
//

// Constructor.
CMDIChildTree::CMDIChildTree()
{
    m_Menu.LoadMenu(_T("MdiMenuTree"));
    SetHandles(m_Menu, NULL);
    SetView(m_TreeView);
}

// Destructor.
CMDIChildTree::~CMDIChildTree()
{
}

// Called when the window is created.
int CMDIChildTree::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText(_T("Tree-View Window"));
    SetIconLarge(IDI_CLASSES);
    SetIconSmall(IDI_CLASSES);

    return CMDIChild::OnCreate(cs);
}

