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


int CViewFiles::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.mask = LVIF_TEXT|LVIF_IMAGE;
    lvi.iImage = image;
    lvi.pszText = const_cast<LPTSTR>(text);

    return InsertItem(lvi);
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

void CViewFiles::OnAttach()
{
    // Set the image lists
    m_smallImages.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_FILEVIEW);
    m_smallImages.Add(image, RGB(255, 0, 255));
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

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
    TCHAR string[3][20] = {_T("Name"), TEXT("Size"), _T("Type")};
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

LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
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
}

int CDockFiles::OnCreate(CREATESTRUCT& cs)
{
    // Set the width of the splitter bar
    SetBarWidth(8);

    return CDocker::OnCreate(cs);
}


