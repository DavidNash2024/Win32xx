/////////////////////////////
// Files.cpp
//

#include "stdafx.h"
#include "Files.h"
#include "resource.h"

///////////////////////////////////
// CViewFiles function definitions.
//
CViewFiles::CViewFiles()
{
}

CViewFiles::~CViewFiles()
{
    if (IsWindow()) DeleteAllItems();
}

// Insert 4 list view items.
void CViewFiles::InsertItems()
{
    // Add 4th item.
    int item = InsertItem(0, _T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C Header file"));

    // Add 3rd item.
    item = InsertItem(item, _T("ListViewApp.cpp"), 1);
    SetItemText(item, 1, _T("3 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // Add 2nd item.
    item = InsertItem(item, _T("main.cpp"), 1);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // Add 1st item.
    item = InsertItem(item, _T("ListView"), 0);
    SetItemText(item, 2, _T("Folder"));
}

// Called when the window is created and attached to the CWnd.
void CViewFiles::OnAttach()
{
    // Set the image lists.
    SetDPIImages();

    // Set the report style.
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

// Called in response to a WM_DPICHANGED_BEFOREPARENT message that is sent to child
// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewFiles::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetDPIImages();
    SetDPIColumnWidths();
    return FinalWindowProc(msg, wparam, lparam);
}

// Sets up the list view's columns.
void CViewFiles::SetColumns()
{
    // Empty the list.
    DeleteAllItems();

    // Add the column items.
    InsertColumn(0, _T("Name"));
    InsertColumn(1, _T("Size"));
    InsertColumn(2, _T("Type"));
    SetDPIColumnWidths();
}

// Adjusts the listview column widths in response to window DPI changes.
void CViewFiles::SetDPIColumnWidths()
{
    SetColumnWidth(0, DpiScaleInt(120));
    SetColumnWidth(1, DpiScaleInt(50));
    SetColumnWidth(2, DpiScaleInt(100));
}

// Adjusts the listview image sizes widths in response to window DPI changes.
void CViewFiles::SetDPIImages()
{
    // Set the image lists
    CBitmap bmImage(IDB_FILEVIEW);
    bmImage = DpiScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    CImageList smallImages;
    smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    smallImages.Add(bmImage, RGB(255, 0, 255));
    SetImageList(smallImages, LVSIL_SMALL);
}

// Process the list-view's window messages.
LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DPICHANGED_BEFOREPARENT:  return OnDpiChangedBeforeParent(msg, wparam, lparam);
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
