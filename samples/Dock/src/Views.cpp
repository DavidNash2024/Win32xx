/////////////////////////////
// Views.cpp
//


#include "stdafx.h"
#include "Views.h"
#include "resource.h"


///////////////////////////////////
// CViewSimple function definitions
//

// Called when part of the window needs to be redrawn.
void CViewSimple::OnDraw(CDC& dc)
{
    //Centre some text in the window
    CRect rc = GetClientRect();
    dc.DrawText(_T("Simple View"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

// Respond to a mouse click on the window
LRESULT CViewSimple::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called when the window is resised.
LRESULT CViewSimple::OnSize(UINT msg, WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(msg);
    UNREFERENCED_PARAMETER(wparam);
    UNREFERENCED_PARAMETER(lparam);

    Invalidate();
    return 0;
}

// Process the window's messages.
LRESULT CViewSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    case WM_SIZE:               return OnSize(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

/////////////////////////////////
// CViewList function definitions
//

// Constructor.
CViewList::CViewList()
{
}

// Destructor.
CViewList::~CViewList()
{
    if (IsWindow()) DeleteAllItems();
}

// Adds an item to the list-view with text and an image.
int CViewList::AddItem(LPCTSTR szText, int nImage)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iImage = nImage;
    lvi.pszText = const_cast<LPTSTR>(szText);

    return InsertItem(lvi);
}

// Inserts 4 items into the List-view.
void CViewList::InsertItems()
{
    // Add 4th item
    int item = AddItem(_T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C Header file"));

    // add 3rd item
    item = AddItem(_T("ListViewApp.cpp"), 1);
    SetItemText(item, 1, _T("3 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 2nd item
    item = AddItem(_T("main.cpp"), 1);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 1st item
    item = AddItem(_T("ListView"), 0);
    SetItemText(item, 2, _T("Folder"));
}

// Called when a window handle (HWND) is attached to the List-View.
void CViewList::OnAttach()
{
    // Set the image lists
    m_smallImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bmImage(IDB_FILEVIEW);
    m_smallImages.Add( bmImage, RGB(255, 0, 255) );
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

// Called when the window is destroyed.
void CViewList::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Called when the mouse is clicked on the window.
LRESULT CViewList::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Configure the List-view's columns.
void CViewList::SetColumns()
{
    //empty the list
    DeleteAllItems();

    //initialise the columns
    LV_COLUMN column;
    ZeroMemory(&column, sizeof(column));
    column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.cx = 120;
    TCHAR string[3][20] = {TEXT("Name"), TEXT("Size"), TEXT("Type")};
    for(int i = 0; i < 3; ++i)
    {
        column.pszText = string[i];
        InsertColumn(i, column);
    }
}

// Process the list-view's window messages.
LRESULT CViewList::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

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
    if (IsWindow())
        DeleteAllItems();
}

// Adds an item the the tree-view control.
HTREEITEM CViewTree::AddItem(HTREEITEM hParent, LPCTSTR text, int image)
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

// Called when a window handle (HWND) is attached to this object.
void CViewTree::OnAttach()
{
    //set the image lists
    m_normalImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bmImage(IDB_CLASSVIEW);
    m_normalImages.Add( bmImage, RGB(255, 0, 0) );
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

// Called when the window is destroyed.
void CViewTree::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Called when the mouse is clicked on the window.
LRESULT CViewTree::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Process window messages for the tree-view control.
LRESULT CViewTree::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

/////////////////////////////////
// CViewText function definitions
//

// Called when a window handle (HWND) is attached to this object.
void CViewText::OnAttach()
{
    SetWindowText(_T("Text Edit Window\r\n\r\n You can type some text here ..."));
}

// Specifiy the CREATESTRUCT parameters before the window is created.
void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.lpszClass = _T("EDIT");
    cs.style = ES_MULTILINE|WS_CHILD;
}
