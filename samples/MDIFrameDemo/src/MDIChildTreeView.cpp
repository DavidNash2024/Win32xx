//////////////////////////////////////////////
// MDIChildTreeView.cpp
//  Definitions for the CViewTree and CMDIChildTreeView classes

#include "stdafx.h"
#include "MDIChildTreeView.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewTree functions
CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
	if (IsWindow()) DeleteAllItems();
}

void CViewTree::OnDestroy()
{
	SetImageList(NULL, LVSIL_SMALL);
}


void CViewTree::OnInitialUpdate()
{
	//set the image lists
	m_imlNormal.Create(16, 15, ILC_COLOR32 | ILC_MASK, 1, 0);
	CBitmap bmImage(IDB_CLASSVIEW);
	m_imlNormal.Add( bmImage, RGB(255, 0, 0) );
	SetImageList(m_imlNormal, LVSIL_NORMAL);

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
// CMDIChildTreeView functions
CMDIChildTreeView::CMDIChildTreeView()
{
	HINSTANCE hResource = GetApp()->GetResourceHandle();
	HMENU hChildMenu = LoadMenu(hResource, _T("MdiMenuTree"));
	SetHandles(hChildMenu, NULL);
	SetView(m_TreeView);
}

CMDIChildTreeView::~CMDIChildTreeView()
{
}

int CMDIChildTreeView::OnCreate(LPCREATESTRUCT pcs)
{
	UNREFERENCED_PARAMETER(pcs);
	m_TreeView.Create(*this);

	SetWindowText(_T("Tree-View Window"));
	SetIconLarge(IDI_CLASSES);
	SetIconSmall(IDI_CLASSES);
	
	return 0;
}

