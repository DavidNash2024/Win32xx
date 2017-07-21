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

LRESULT CViewSimple::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_SIZE:
		Invalidate();
		break;
	}

	return WndProcDefault(uMsg, wParam, lParam);
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
	m_imlSmall.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	CBitmap bmImage(IDB_FILEVIEW);
	m_imlSmall.Add( bmImage, RGB(255, 0, 255) );
	SetImageList(m_imlSmall, LVSIL_SMALL);

	// Set the report style
	DWORD dwStyle = GetStyle();
	SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

	SetColumns();
	InsertItems();
}

int CViewList::AddItem(LPCTSTR szText, int nImage)
{
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(LVITEM));
	lvi.mask = LVIF_TEXT|LVIF_IMAGE;
	lvi.iImage = nImage;
	lvi.pszText = const_cast<LPTSTR>(szText);

	return InsertItem(lvi);
}

void CViewList::SetColumns()
{
	//empty the list
	DeleteAllItems();

	//initialise the columns
	LV_COLUMN lvColumn;
	ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
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
	LVITEM lvi1;
	ZeroMemory(&lvi1, sizeof(LVITEM));
	lvi1.mask = LVIF_TEXT;
	lvi1.iItem = nItem;
	lvi1.iSubItem = nSubItem;
	lvi1.pszText = const_cast<LPTSTR>(szText);
	return static_cast<BOOL>(SendMessage(LVM_SETITEM, 0, reinterpret_cast<LPARAM>(&lvi1)));
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
	m_imlNormal.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	CBitmap bmImage(IDB_CLASSVIEW);
	m_imlNormal.Add( bmImage, RGB(255, 0, 0) );
	SetImageList(m_imlNormal, LVSIL_NORMAL);

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

HTREEITEM CViewTree::AddItem(HTREEITEM hParent, LPCTSTR szText, int iImage)
{
	TVITEM tvi;
	ZeroMemory(&tvi, sizeof(TVITEM));
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;

	TVINSERTSTRUCT tvis;
	ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
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

