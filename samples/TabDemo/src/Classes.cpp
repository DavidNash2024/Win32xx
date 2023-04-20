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

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewClasses::OnUserDPIChanged(UINT, WPARAM, LPARAM)
{
    SetDPIImages();
    return 0;
}

// Adjusts the treeview image sizes in response to window DPI changes.
void CViewClasses::SetDPIImages()
{
    // Resize the image list.
    CBitmap bmImage(IDB_CLASSVIEW);
    bmImage = DPIScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    m_normalImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    m_normalImages.Add(bmImage, RGB(255, 0, 0));
    SetImageList(m_normalImages, LVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = m_normalImages.GetIconSize().cx;
    SetIndent(imageWidth);
}

// Process the tree-view's window messages.
LRESULT CViewClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_DPICHANGED:        return OnUserDPIChanged(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}
