//////////////////////////////////////////////
// Files.cpp
//  Definitions for CViewFiles 

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
    CBitmap image(IDB_FILEVIEW);
	m_smallImages.Add( image, RGB(255, 0, 255) );
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

int CViewFiles::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(lvi));
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

BOOL CViewFiles::SetSubItem(int item, int subItem, LPCTSTR text)
{
    LVITEM lvi1;
    ZeroMemory(&lvi1, sizeof(lvi1));
    lvi1.mask = LVIF_TEXT;
    lvi1.iItem = item;
    lvi1.iSubItem = subItem;
    lvi1.pszText = const_cast<LPTSTR>(text);
    return static_cast<BOOL>(SendMessage(LVM_SETITEM, 0, reinterpret_cast<LPARAM>(&lvi1)));
}

void CViewFiles::InsertItems()
{
    // Add 4th item
    int item = AddItem(_T("ListViewApp.h"), 2);
    SetSubItem(item, 1, _T("1 KB"));
    SetSubItem(item, 2, _T("C Header file"));

    // add 3rd item
    item = AddItem(_T("ListViewApp.cpp"), 1);
    SetSubItem(item, 1, _T("3 KB"));
    SetSubItem(item, 2, _T("C++ Source file"));

    // add 2nd item
    item = AddItem(_T("main.cpp"), 1);
    SetSubItem(item, 1, _T("1 KB"));
    SetSubItem(item, 2, _T("C++ Source file"));

    // add 1st item
    item = AddItem(_T("ListView"), 0);
    SetSubItem(item, 2, _T("Folder"));
}



