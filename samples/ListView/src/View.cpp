/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"

/////////////////////////////
// CView function definitions
//

// Constructor.
CView::CView()
{
}

// Destructor.
CView::~CView()
{
    if (IsWindow()) DeleteAllItems();
}

// Insert 4 list view items.
void CView::InsertItems()
{
    // Add 4th item
    int item = InsertItem(0, _T("ListViewApp.h"), 2);
    SetItemText(item, 1, _T("1 KB"));
    SetItemState(1, LVIS_SELECTED, LVIF_STATE);
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
void CView::OnAttach()
{
    // Set the report style.
    DWORD style = GetStyle();
    SetStyle((style & ~LVS_TYPEMASK) | LVS_REPORT | LVS_SINGLESEL);
    SetExtendedStyle(LVS_EX_FULLROWSELECT);
    SetColumns();
    InsertItems();
}

LRESULT CView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    case NM_CUSTOMDRAW:  return OnCustomDraw(lparam);
    }

    return 0;
}

LRESULT CView::OnCustomDraw(LPARAM lParam)
{
    LPNMLVCUSTOMDRAW lplvcd = (LPNMLVCUSTOMDRAW)lParam;

    switch (lplvcd->nmcd.dwDrawStage)
    {
    case CDDS_PREPAINT: // Before the paint cycle begins.
        // Request notifications for individual listview items.
        return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT: //Before an item is drawn
        // Request notifications with dwDrawStage set to CDDS_ITEMPREPAINT | CDDS_SUBITEM
        // before each list-view subitem is drawn.
        return CDRF_NOTIFYSUBITEMDRAW;

    case CDDS_SUBITEM | CDDS_ITEMPREPAINT: // Before a subitem is drawn.
    {
        switch (lplvcd->iSubItem)
        {
        case 0: // 1st column
        {
            if (lplvcd->nmcd.dwItemSpec == 0)   // first row only
            {
                lplvcd->clrText = RGB(255, 255, 255);
                lplvcd->clrTextBk = RGB(240, 55, 23);
            }
            else
            {
                lplvcd->clrText = RGB(0, 0, 0);
                lplvcd->clrTextBk = RGB(255, 255, 255);
            }
        }
        break;

        case 1: // 2nd column
        {
            if (lplvcd->nmcd.dwItemSpec % 2)  // odd numbered rows
            {
                lplvcd->clrText = RGB(255, 255, 0);
                lplvcd->clrTextBk = RGB(0, 0, 0);
            }
            else
            {
                lplvcd->clrText = RGB(0, 0, 0);
                lplvcd->clrTextBk = RGB(255, 255, 255);
            }
        }
        break;

        case 2:  // 3rd column
        {
            // all rows
            lplvcd->clrText = RGB(20, 26, 158);
            lplvcd->clrTextBk = RGB(200, 200, 10);
        }
        break;
        }  // switch (lplvcd->iSubItem)

        return CDRF_DODEFAULT;
    }  // case CDDS_SUBITEM | CDDS_ITEMPREPAINT
    }  // switch (lplvcd->nmcd.dwDrawStage)
    return CDRF_DODEFAULT;
}
// Configures the list-view's columns (its header control).
void CView::SetColumns()
{
    // empty the list
    DeleteAllItems();

    // initialize the columns
    LV_COLUMN lvColumn;
    ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;
    lvColumn.cx = 120;
    TCHAR string[3][20] = {_T("Name"), _T("Size"), _T("Type")};
    for(int i = 0; i < 3; ++i)
    {
        lvColumn.pszText = string[i];
        InsertColumn(i, lvColumn);
    }
}

// Process the list-view's window messages.
LRESULT CView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
     //   switch (msg)
     //   {
     //   }

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
