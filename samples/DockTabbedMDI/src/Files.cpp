//////////////////////////////////////////////
// Files.cpp - Definitions for CViewFiles, CContainFiles
//             and DockFiles classes
//

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
    // Add 4th item
    int item = InsertItem(0, _T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C Header file"));

    // add 3rd item
    item = InsertItem(item, _T("ListViewApp.cpp"), 1);
    SetItemText(item, 1, _T("3 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 2nd item
    item = InsertItem(item, _T("main.cpp"), 1);
    SetItemText(item, 1, _T("1 KB"));
    SetItemText(item, 2, _T("C++ Source file"));

    // add 1st item
    item = InsertItem(item, _T("ListView"), 0);
    SetItemText(item, 2, _T("Folder"));
}

// Called when a window handle (HWND) is attached to CViewFiles.
void CViewFiles::OnAttach()
{
    SetDPIImages();

    // Set the report style
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT);

    SetColumns();
    InsertItems();

#ifndef LVS_EX_DOUBLEBUFFER
  #define LVS_EX_DOUBLEBUFFER     0x00010000
#endif

    // Set the extended style to double buffer.
    SetExtendedStyle(LVS_EX_DOUBLEBUFFER);
}

// Respond to a mouse click on the window
LRESULT CViewFiles::OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Set window focus. The docker will now report this as active.
    SetFocus();
    return FinalWindowProc(msg, wparam, lparam);
}

// Called in response to a WM_DPICHANGED_BEFOREPARENT message which is sent to child
// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewFiles::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetDPIImages();
    SetDPIColumnWidths();
    return FinalWindowProc(msg, wparam, lparam);
}

// Configures the columns (header control) of the list view.
void CViewFiles::SetColumns()
{
    // empty the list
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

// Adjusts the listview image sizes in response to window DPI changes.
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
        case WM_MOUSEACTIVATE:      return OnMouseActivate(msg, wparam, lparam);
        case WM_DPICHANGED_BEFOREPARENT:   return OnDpiChangedBeforeParent(msg, wparam, lparam);
        }

        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}


/////////////////////////////////////
// CContainFiles function definitions
//

// Constructor.
CContainFiles::CContainFiles()
{
    SetTabText(_T("FileView"));
    SetTabIcon(IDI_FILEVIEW);
    SetDockCaption (_T("File View - Docking container"));
    SetView(m_viewFiles);
}

// Handle the window's messages.
LRESULT CContainFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}


//////////////////////////////////
// CDockFiles function definitions
//

// Constructor.
CDockFiles::CDockFiles()
{
    SetView(m_files);

    // Set the width of the splitter bar.
    SetBarWidth(8);
}

// This function overrides CDocker::RecalcDockLayout to elimate jitter
// when the dockers are resized. The technique used here is is most
// appropriate for a complex arrangement of dockers.  It might not suite
// other docking applications. To support this technique the
// WS_EX_COMPOSITED extended style has been added to some view windows.
void CDockFiles::RecalcDockLayout()
{
    if (GetWinVersion() >= 3000)  // Windows 10 or later.
    {
        if (GetDockAncestor()->IsWindow())
        {
            GetTopmostDocker()->LockWindowUpdate();
            CRect rc = GetTopmostDocker()->GetViewRect();
            GetTopmostDocker()->RecalcDockChildLayout(rc);
            GetTopmostDocker()->UnlockWindowUpdate();
            GetTopmostDocker()->UpdateWindow();
        }
    }
    else
        CDocker::RecalcDockLayout();
}

// Handle the window's messages.
LRESULT CDockFiles::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass unhandled messages on for default processing.
        return WndProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << _T("\n") << e.GetErrorString();
        CString str2;
        str2 << "Error: " << e.what();
        ::MessageBox(NULL, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(NULL, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}
