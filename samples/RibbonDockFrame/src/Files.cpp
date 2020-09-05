/////////////////////////////
// Files.cpp
//

#include "stdafx.h"
#include "Files.h"
#include "resource.h"

//////////////////////////////////
// CViewFiles function definitions
//
CViewFiles::CViewFiles()
{
}

CViewFiles::~CViewFiles()
{
    if (IsWindow()) DeleteAllItems();
}

int CViewFiles::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.mask = LVIF_TEXT | LVIF_IMAGE;
    lvi.iImage = image;
    lvi.pszText = const_cast<LPTSTR>(text);

    return InsertItem(lvi);
}

void CViewFiles::InsertItems()
{
    // Add 4th item
    int item = AddItem(L"ListViewApp.h", 2);
    SetItemText(item, 1, L"1 KB");
    SetItemText(item, 2, L"C Header file");

    // add 3rd item
    item = AddItem(L"ListViewApp.cpp", 1);
    SetItemText(item, 1, L"3 KB");
    SetItemText(item, 2, L"C++ Source file");

    // add 2nd item
    item = AddItem(L"main.cpp", 1);
    SetItemText(item, 1, L"1 KB");
    SetItemText(item, 2, L"C++ Source file");

    // add 1st item
    item = AddItem(L"ListView", 0);
    SetItemText(item, 2, L"Folder");
}

void CViewFiles::OnAttach()
{
    // Set the image lists
    m_imlSmall.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bm(IDB_FILEVIEW);
    m_imlSmall.Add(bm, RGB(255, 0, 255));
    SetImageList(m_imlSmall, LVSIL_SMALL);

    // Set the report style
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

void CViewFiles::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

LRESULT CViewFiles::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
// Respond to a mouse click on the window
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

void CViewFiles::SetColumns()
{
    //empty the list
    DeleteAllItems();

    //initialise the columns
    LV_COLUMN column;
    ZeroMemory(&column, sizeof(column));
    column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.cx = 120;
    TCHAR string[3][20] = {L"Name", L"Size", L"Type"};
    for(int i = 0; i < 3; ++i)
    {
        column.pszText = string[i];
        InsertColumn(i, column);
    }
}

LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
    }

    return WndProcDefault(msg, wparam, lparam);
}

/////////////////////////////////////
// CContainFiles function definitions
//
CContainFiles::CContainFiles()
{
    SetTabText(L"FileView");
    SetTabIcon(IDI_FILEVIEW);
    SetDockCaption (L"File View - Docking container");
    SetView(m_viewFiles);
}

///////////////////////////////////
//  CDockFiles function definitions
//
CDockFiles::CDockFiles()
{
    SetView(m_files);
    SetBarWidth(8);
}

