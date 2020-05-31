//////////////////////////////////////////////////////////////
// Views.cpp -  Definitions for the CViewSimple, CViewList,
//              CViewTree and CViewText classes

#include "stdafx.h"
#include "Views.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewSimple functions
void CViewSimple::OnDraw(CDC& dc)
{
    //Centre some text in the window
    CRect rc = GetClientRect();
    dc.DrawText(_T("Simple View"), -1, rc, DT_CENTER|DT_VCENTER|DT_SINGLELINE);
}

LRESULT CViewSimple::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch(msg)
    {
    case WM_SIZE:
        Invalidate();
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}


///////////////////////////////////////////////
// CViewList functions
CViewList::CViewList()
{
}

CViewList::~CViewList()
{
    if (IsWindow()) DeleteAllItems();
}

void CViewList::OnAttach()
{
    // Set the image lists
    m_smallImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_FILEVIEW);
    m_smallImages.Add(image, RGB(255, 0, 255) );
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

int CViewList::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT|LVIF_IMAGE;
    lvi.iImage = image;
    lvi.pszText = const_cast<LPTSTR>(text);

    return InsertItem(lvi);
}

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


///////////////////////////////////////////////
// CViewTree functions
CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
    if (IsWindow()) DeleteAllItems();
}

void CViewTree::OnAttach()
{
    //set the image lists
    m_smallImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_CLASSVIEW);
    m_smallImages.Add(image, RGB(255, 0, 0) );
    SetImageList(m_smallImages, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

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

HTREEITEM CViewTree::AddItem(HTREEITEM hParent, LPCTSTR text, int image)
{
    TVITEM tvi;
    ZeroMemory(&tvi, sizeof(tvi));
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.iImage = image;
    tvi.iSelectedImage = image;
    tvi.pszText = (LPTSTR)text;

    TVINSERTSTRUCT tvis;
    ZeroMemory(&tvis, sizeof(tvis));
    tvis.hParent = hParent;
    tvis.item = tvi;

    return InsertItem(tvis);
}


///////////////////////////////////////////////
// CViewText functions
void CViewText::OnAttach()
{
    SetWindowText(_T("Text Edit Window\r\n\r\n\r\n\r\n You can type some text here ..."));
}

void CViewText::PreCreate(CREATESTRUCT& cs)
{
    cs.lpszClass = _T("EDIT");
    cs.style = ES_MULTILINE|WS_CHILD;
}

