//////////////////////////////////////////////
// MDIChildListView.cpp
//  Definitions for the CViewList and CMDIChildListView classes

#include "stdafx.h"
#include "MDIChildListView.h"
#include "resource.h"


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

// Called when a window handle (HWND) is attached to CViewList.
void CViewList::OnAttach()
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

// Called when the window is destroyed.
void CViewList::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Adds an item to the list view.
int CViewList::AddItem(LPCTSTR text, int image)
{
    LVITEM lvi;
    ZeroMemory(&lvi, sizeof(lvi));
    lvi.mask = LVIF_TEXT|LVIF_IMAGE;
    lvi.iImage = image;
    lvi.pszText = const_cast<LPTSTR>(text);

    return InsertItem(lvi);
}

// Configures the columns for the list view.
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

// Adds 4 items to the list view.
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


/////////////////////////////////////////
// CMDIChildListView function definitions
//

// Constructor.
CMDIChildList::CMDIChildList()
{
    m_menu.LoadMenu(_T("MdiMenuList"));
    SetHandles(m_menu, NULL);
    SetView(m_listView);
}

// Destructor.
CMDIChildList::~CMDIChildList()
{
}

// Called when the window is created.
int CMDIChildList::OnCreate(CREATESTRUCT& cs)
{
    SetWindowText( _T("List-View Window") );
    SetIconLarge(IDI_FILES);
    SetIconSmall(IDI_FILES);

    return CMDIChild::OnCreate(cs);
}

