//////////////////////////////////////////////
// MDIChildTreeView.cpp
//  Definitions for the CViewTree and CMDIChildTreeView classes

#include "MDIChildTreeView.h"
#include "resource.h"

///////////////////////////////////////////////
// CViewTree functions
CViewTree::CViewTree() : m_himlNormal(0)
{
}

CViewTree::~CViewTree()
{
	if (IsWindow()) DeleteAllItems();
	ImageList_Destroy(m_himlNormal);
}

void CViewTree::OnInitialUpdate()
{
	//set the image lists
	m_himlNormal = ImageList_Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	HBITMAP hbm = LoadBitmap(MAKEINTRESOURCE(IDB_CLASSVIEW));
	ImageList_AddMasked(m_himlNormal, hbm, RGB(255, 0, 0));
	SetImageList(m_himlNormal, LVSIL_NORMAL);
	::DeleteObject(hbm);

	// Adjust style to show lines and [+] button
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLongPtr(GWL_STYLE, dwStyle);

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
	TVITEM tvi = {0};
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvi.iImage = iImage;
	tvi.iSelectedImage = iImage;
	tvi.pszText = (LPTSTR)szText;

	TVINSERTSTRUCT tvis = {0};
	tvis.hParent = hParent;
	tvis.item = tvi;

	return InsertItem(tvis);
}

LRESULT CViewTree::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		{
			SetImageList(NULL, LVSIL_SMALL);
			break;
		}
	}

	return WndProcDefault(uMsg, wParam, lParam);
}


///////////////////////////////////////////////
// CMDIChildTreeView functions
CMDIChildTreeView::CMDIChildTreeView()
{
	SetChildMenu(_T("MdiMenuTree"));
	SetView(m_TreeView);
}

CMDIChildTreeView::~CMDIChildTreeView()
{
}

void CMDIChildTreeView::OnCreate()
{
	m_TreeView.Create(m_hWnd);
}

void CMDIChildTreeView::OnInitialUpdate()
{
	::SetWindowText(m_hWnd, _T("Tree-View Window"));
	SetIconLarge(IDI_CLASSES);
	SetIconSmall(IDI_CLASSES);
}

