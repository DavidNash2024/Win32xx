/////////////////////////////
// Classes.cpp
//

#include "pch.h"
#include "Classes.h"
#include "resource.h"

/////////////////////////////////////
// CViewClasses function definitions.
//

CViewClasses::~CViewClasses()
{
    if (IsWindow()) DeleteAllItems();
}

// Called when the window is created and attached to the CWnd.
void CViewClasses::OnAttach()
{
    // Set the image lists.
    SetDPIImages();

    // Adjust style to show lines and [+] button.
    DWORD style = GetStyle();
    style |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(style);

    DeleteAllItems();

    // Add some tree-view items.
    HTREEITEM htiRoot = InsertItem(L"TreeView", 0, 0);
    HTREEITEM htiCTreeViewApp = InsertItem(L"CTreeViewApp", 1, 1, htiRoot);
    InsertItem(L"CTreeViewApp()", 3, 3, htiCTreeViewApp);
    InsertItem(L"GetMainFrame()", 3, 3, htiCTreeViewApp);
    InsertItem(L"InitInstance()", 3, 3, htiCTreeViewApp);
    HTREEITEM htiMainFrame = InsertItem(L"CMainFrame", 1, 1, htiRoot);
    InsertItem(L"CMainFrame()", 3, 3, htiMainFrame);
    InsertItem(L"OnCommand()", 4, 4, htiMainFrame);
    InsertItem(L"OnInitialUpdate()", 4, 4, htiMainFrame);
    HTREEITEM htiView = InsertItem(L"CView", 1, 1, htiRoot);
    InsertItem(L"CView()", 3, 3, htiView);
    InsertItem(L"OnInitialUpdate()", 4, 4, htiView);
    InsertItem(L"WndProc()", 4, 4, htiView);

    // Expand some tree-view items.
    Expand(htiRoot, TVE_EXPAND);
    Expand(htiCTreeViewApp, TVE_EXPAND);
}

// Respond to a change in DPI. This function is only called when using
// Per Monitor DPI Awareness, version 2. This is specified by loading
// the appropriate manifest in the resource script (resource.rc).
LRESULT CViewClasses::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetDPIImages();
    return FinalWindowProc(msg, wparam, lparam);
}

// Adjusts the treeview image sizes in response to window DPI changes.
void CViewClasses::SetDPIImages()
{
    // Resize the image list.
    CBitmap bmImage(IDB_CLASSVIEW);
    bmImage = DpiScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    CImageList normalImages;
    normalImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    normalImages.Add(bmImage, RGB(255, 0, 0));
    SetImageList(normalImages, TVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = normalImages.GetIconSize().cx;
    SetIndent(imageWidth);
}

// Process the tree-view's window messages.
LRESULT CViewClasses::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DPICHANGED_BEFOREPARENT:  return OnDpiChangedBeforeParent(msg, wparam, lparam);

        default: break;
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << L"Error: " << e.what();
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    return 0;
}
