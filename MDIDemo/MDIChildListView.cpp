//////////////////////////////////////////////
// MDIChildListView.cpp
//  Definitions for the CViewList and CMDIChildListView classes

#include "MDIChildListView.h"
#include "resource.h"

///////////////////////////////////////////////
// CViewList functions
CViewList::CViewList() : m_himlSmall(0)
{
}

CViewList::~CViewList()
{
	if (IsWindow()) DeleteAllItems();
}

void CViewList::OnInitialUpdate()
{
	// Set the image lists
	m_himlSmall = ImageList_Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(IDB_FILEVIEW));
	ImageList_AddMasked(m_himlSmall, hbm, RGB(255, 0, 255));
	SetImageList(m_himlSmall, LVSIL_SMALL);

	// Set the report style
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

	SetColumns();
	InsertItems();
}

int CViewList::AddItem(LPCTSTR szText, int nImage)
{
	LVITEM lvi = {0};
	lvi.mask = LVIF_TEXT|LVIF_IMAGE;
	lvi.iImage = nImage;
	lvi.pszText = (LPTSTR)szText;

	return InsertItem(lvi);
}

void CViewList::SetColumns()
{
	//empty the list
	DeleteAllItems();

	//initialise the columns
	LV_COLUMN lvColumn = {0};
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_LEFT;
	lvColumn.cx = 120;
	TCHAR szString[3][20] = {TEXT("Name"), TEXT("Size"), TEXT("Type")};
	for(int i = 0; i < 3; ++i)
	{
		lvColumn.pszText = szString[i];
		InsertColumn(i, lvColumn);
	}
}

BOOL CViewList::SetSubItem(int nItem, int nSubItem, LPCTSTR szText)
{
	LVITEM lvi1 = {0};
	lvi1.mask = LVIF_TEXT;
	lvi1.iItem = nItem;
	lvi1.iSubItem = nSubItem;
	lvi1.pszText = (LPTSTR)szText;
	return (BOOL)SendMessage(LVM_SETITEM, 0L, (LPARAM)&lvi1);
}

void CViewList::InsertItems()
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


///////////////////////////////////////////////
// CMDIChildListView functions
CMDIChildListView::CMDIChildListView()
{
	SetChildMenu(_T("MdiMenuList"));
	SetView(m_ListView);
}

CMDIChildListView::~CMDIChildListView()
{
}

void CMDIChildListView::OnCreate()
{
	m_ListView.Create(m_hWnd);
}

void CMDIChildListView::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("List-View Window"));
	SetIconLarge(IDI_FILES);
	SetIconSmall(IDI_FILES);
}

