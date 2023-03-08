/////////////////////////////
// Classes.cpp
//

#include "stdafx.h"
#include "Classes.h"
#include "resource.h"

/////////////////////////////////////
// CViewClasses function definitions.
//
CViewClasses::CViewClasses()
{
}

CViewClasses::~CViewClasses()
{
    if (IsWindow()) DeleteAllItems();
}

// Called when the window is created and attached to the CWnd.
void CViewClasses::OnAttach()
{
    //set the image lists
    int scale = DPIScaleInt(1);
    m_normalImages.Create(16 * scale, 15 * scale, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_CLASSVIEW);
    image = DPIScaleUpBitmap(image);
    m_normalImages.Add(image, RGB(255, 0, 0) );
    SetImageList(m_normalImages, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button.
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

    DeleteAllItems();

    // Add some tree-view items.
    HTREEITEM htiRoot = InsertItem(_T("TreeView"), 0, 0);
    HTREEITEM htiCTreeViewApp = InsertItem(_T("CTreeViewApp"), 1, 1, htiRoot);
    InsertItem(_T("CTreeViewApp()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("GetMainFrame()"), 3, 3, htiCTreeViewApp);
    InsertItem(_T("InitInstance()"), 3, 3, htiCTreeViewApp);
    HTREEITEM htiMainFrame = InsertItem(_T("CMainFrame"), 1, 1, htiRoot);
    InsertItem(_T("CMainFrame()"), 3, 3, htiMainFrame);
    InsertItem(_T("OnCommand()"), 4, 4, htiMainFrame);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiMainFrame);
    HTREEITEM htiView = InsertItem(_T("CView"), 1, 1, htiRoot);
    InsertItem(_T("CView()"), 3, 3, htiView);
    InsertItem(_T("OnInitialUpdate()"), 4, 4, htiView);
    InsertItem(_T("WndProc()"), 4, 4, htiView);

    // Expand some tree-view items.
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}
