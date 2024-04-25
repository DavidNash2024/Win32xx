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
    CBitmap bmImage(IDB_FILEVIEW);
    bmImage = DpiScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    CImageList smallImages;
    smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    smallImages.Add(bmImage, RGB(255, 0, 255));
    SetImageList(smallImages, LVSIL_SMALL);

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

// Called when the window is destroyed.
void CViewList::OnDestroy()
{
    SetImageList(0, LVSIL_SMALL);
}

// Configures the columns for the list view.
void CViewList::SetColumns()
{
    // empty the list
    DeleteAllItems();

    // Add the column items.
    InsertColumn(0, _T("Name"), 0, DpiScaleInt(120));
    InsertColumn(1, _T("Size"), 0, DpiScaleInt(50));
    InsertColumn(2, _T("Type"), 0, DpiScaleInt(120));
}

// Insert 4 list view items.
void CViewList::InsertItems()
{
    // Add 4th item
    int item = InsertItem(0, _T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C Header file"));

    // add 3rd item
    item = InsertItem(item, _T("ListViewApp.cpp"), 1);
    SetItemText(item, 1, _T("3 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 2nd item
    item = InsertItem(item, _T("main.cpp"), 1);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 1st item
    item = InsertItem(item, _T("ListView"), 0);
    SetItemText(item, 2, _T("Folder"));
}


/////////////////////////////////////////
// CMDIChildListView function definitions
//

// Constructor.
CMDIChildList::CMDIChildList()
{
    m_menu.LoadMenu(_T("MdiMenuList"));
    SetHandles(m_menu, 0);
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

