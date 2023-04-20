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
    int scale = DPIScaleInt(1);
    m_smallImages.Create(16 * scale, 15 * scale, ILC_COLOR32 | ILC_MASK, 1, 0);
    CBitmap image(IDB_FILEVIEW);
    image = DPIScaleUpBitmap(image);
    m_smallImages.Add( image, RGB(255, 0, 255) );
    SetImageList(m_smallImages, LVSIL_SMALL);

    // Set the report style.
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

// Called in response to a UWM_DPICHANGED message which is sent to child windows
// when the top-level window receives a WM_DPICHANGED message. WM_DPICHANGED is
// received when the DPI changes and the application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewFiles::OnUserDPIChanged(UINT, WPARAM, LPARAM)
{
    SetDPIImages();
    SetDPIColumnWidths();
    return 0;
}

// Sets up the list view's columns.
void CViewFiles::SetColumns()
{
    // Empty the list.
    DeleteAllItems();

    // Initialize the columns.
    LV_COLUMN column;
    ZeroMemory(&column, sizeof(column));
    column.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    column.fmt = LVCFMT_LEFT;
    column.cx = 120;
    TCHAR string[3][20] = {TEXT("Name"), TEXT("Size"), TEXT("Type")};
    for(int i = 0; i < 3; ++i)
    {
        column.pszText = string[i];
        InsertColumn(i, column);
    }
}

// Adjusts the listview column widths in response to window DPI changes.
void CViewFiles::SetDPIColumnWidths()
{
    SetColumnWidth(0, DPIScaleInt(120));
    SetColumnWidth(1, DPIScaleInt(50));
    SetColumnWidth(2, DPIScaleInt(100));
}

// Adjusts the listview image sizes widths in response to window DPI changes.
void CViewFiles::SetDPIImages()
{
    // Set the image lists
    CBitmap bmImage(IDB_FILEVIEW);
    bmImage = DPIScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    m_smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    m_smallImages.Add(bmImage, RGB(255, 0, 255));
    SetImageList(m_smallImages, LVSIL_SMALL);
}

// Process the list-view's window messages.
LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
