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

    // Expand some tree-view items.
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}

// Add a tree view item.
HTREEITEM CViewTree::AddItem(HTREEITEM hParent, LPCTSTR text, int image)
{
    TVITEM tvi;
    ZeroMemory(&tvi, sizeof(tvi));
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.iImage = image;
    tvi.iSelectedImage = image;
    tvi.pszText = const_cast<LPTSTR>(text);

    TVINSERTSTRUCT tvis;
    ZeroMemory(&tvis, sizeof(tvis));
    tvis.hParent = hParent;
    tvis.item = tvi;

    return InsertItem(tvis);
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

