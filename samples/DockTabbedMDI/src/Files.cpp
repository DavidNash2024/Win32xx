//////////////////////////////////////////////
// Files.cpp - Definitions for CViewFiles, CContainFiles
//             and DockFiles classes

#include "stdafx.h"
#include "Files.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewFiles functions
CViewFiles::CViewFiles()
{
}

CViewFiles::~CViewFiles()
{
    if (IsWindow()) DeleteAllItems();
}

void CViewFiles::OnAttach()
{
    // Set the image lists
    m_smallImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap bm(IDB_FILEVIEW);
    m_smallImages.Add( bm, RGB(255, 0, 255) );
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

int CViewFiles::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(LVITEM));
    lvi.mask = LVIF_TEXT|LVIF_IMAGE;
    lvi.iImage = image;
    lvi.pszText = const_cast<LPTSTR>(text);

    return InsertItem(lvi);
}

void CViewFiles::SetColumns()
{
    //empty the list
    DeleteAllItems();

    //initialise the columns
    LV_COLUMN lvColumn;
    ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    lvColumn.cx = 120;
    TCHAR szString[3][20] = {_T("Name"), _T("Size"), _T("Type")};
    for(int i = 0; i < 3; ++i)
    {
        lvColumn.pszText = szString[i];
        InsertColumn(i, lvColumn);
    }
}

void CViewFiles::InsertItems()
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

void CViewFiles::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:
        SetFocus();
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}


///////////////////////////////////////////////
// CContainFiles functions
CContainFiles::CContainFiles()
{
    SetTabText(_T("FileView"));
    SetTabIcon(IDI_FILEVIEW);
    SetDockCaption (_T("File View - Docking container"));
    SetView(m_viewFiles);
}

/////////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles()
{
    SetView(m_files);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

