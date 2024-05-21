//////////////////////////////////////////////
// Files.cpp - Definitions for CViewFiles, CContainFiles
//             and DockFiles classes

#include "stdafx.h"
#include "Files.h"
#include "resource.h"


//////////////////////////////////
// CViewFiles function definitions
//

// Constructor.
CViewFiles::CViewFiles()
{
}

// Destructor.
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

// Called when a window handle (HWND) is attached to CViewFiles.
void CViewFiles::OnAttach()
{
    // Set the image lists.
    CBitmap bmImage(IDB_FILEVIEW);
    bmImage = DpiScaleUpBitmap(bmImage);
    int scale = bmImage.GetSize().cy / 15;
    CImageList smallImages;
    smallImages.Create(scale * 16, scale * 15, ILC_COLOR32 | ILC_MASK, 1, 0);
    smallImages.Add(bmImage, RGB(255, 0, 255));
    SetImageList(smallImages, LVSIL_SMALL);

    // Set the report style.
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();
}

// Respond to a mouse click on the window.
LRESULT CViewFiles::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Add 4 columns to the list view.
void CViewFiles::SetColumns()
{
    // Empty the list.
    DeleteAllItems();

    // Add the column items.
    InsertColumn(0, _T("Name"), 0, DpiScaleInt(120));
    InsertColumn(1, _T("Size"), 0, DpiScaleInt(50));
    InsertColumn(2, _T("Type"), 0, DpiScaleInt(120));
}

// Process the list view's window messages.
LRESULT CViewFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}




///////////////////////////////////////////////
// CContainFiles functions
CContainFiles::CContainFiles()
{
    SetTabText(_T("FileView"));
    SetTabIcon(IDI_FILEVIEW);
    SetDockCaption (_T("File View - Docking container"));
    SetView(m_viewFiles);
}

/////////////////////////////////////////////////
//  Definitions for the CDockFiles class
CDockFiles::CDockFiles()
{
    SetView(m_files);
}

int CDockFiles::OnCreate(CREATESTRUCT& cs)
{
    // Set the width of the splitter bar.
    SetBarWidth(8);

    return CDocker::OnCreate(cs);
}

