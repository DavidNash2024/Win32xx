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

void CViewFiles::OnDestroy()
{
	SetImageList(NULL, LVSIL_SMALL);
}

void CViewFiles::OnAttach()
{
	// Set the image lists
	m_imlSmall.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	CBitmap bm(IDB_FILEVIEW);
	m_imlSmall.Add( bm, RGB(255, 0, 255) );
	SetImageList(m_imlSmall, LVSIL_SMALL);

	// Set the report style
	DWORD dwStyle = GetStyle();
	SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

	SetColumns();
	InsertItems();
}

int CViewFiles::AddItem(LPCTSTR szText, int nImage)
{
	LVITEM lvi;
	ZeroMemory(&lvi, sizeof(lvi));
	lvi.mask = LVIF_TEXT|LVIF_IMAGE;
	lvi.iImage = nImage;
	lvi.pszText = const_cast<LPTSTR>(szText);

	return InsertItem(lvi);
}

LRESULT CViewFiles::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
// Respond to a mouse click on the window
{
	// Set window focus. The docker will now report this as active.
	SetFocus();
	return FinalWindowProc(uMsg, wParam, lParam);
}

void CViewFiles::SetColumns()
{
	//empty the list
	DeleteAllItems();

	//initialise the columns
	LV_COLUMN lvColumn;
	ZeroMemory(&lvColumn, sizeof(lvColumn));
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

BOOL CViewFiles::SetSubItem(int nItem, int nSubItem, LPCTSTR szText)
{
	LVITEM lvi1;
	ZeroMemory(&lvi1, sizeof(lvi1));
	lvi1.mask = LVIF_TEXT;
	lvi1.iItem = nItem;
	lvi1.iSubItem = nSubItem;
	lvi1.pszText = const_cast<LPTSTR>(szText);
	return static_cast<BOOL>(SendMessage(LVM_SETITEM, 0L, (LPARAM)&lvi1));
}

LRESULT CViewFiles::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:		return OnMouseActivate(uMsg, wParam, lParam);
	}

	return WndProcDefault(uMsg, wParam, lParam);
}
		


///////////////////////////////////////////////
// CContainFiles functions
CContainFiles::CContainFiles() 
{
	SetTabText(_T("FileView"));
	SetTabIcon(IDI_FILEVIEW);
	SetDockCaption (_T("File View - Docking container"));
	SetView(m_ViewFiles);
}

/////////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles() 
{ 
	SetView(m_Files);
	SetBarWidth(8);
}

