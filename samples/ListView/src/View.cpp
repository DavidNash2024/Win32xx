/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "UserMessages.h"

/////////////////////////////
// CView function definitions
//

// Constructor.
CView::CView() :  m_editRow(-1), m_editColumn(-1)
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
    // Set full row select to support editing subitems.
    SetExtendedStyle(LVS_EX_FULLROWSELECT);
    
    // Fill the List-View control.
    SetColumns();
    InsertItems();

    // Create the edit window. It is initially hidden.
    m_edit.Create(*this);
}

// Shows the edit window when a subitem is clicked. 
LRESULT CView::OnClick(LPARAM lparam)
{
    LPNMITEMACTIVATE itemInfo = (LPNMITEMACTIVATE)lparam;
    m_editRow = itemInfo->iItem;
    m_editColumn = itemInfo->iSubItem;

    if (m_editRow != -1 && m_editColumn != -1)
    {
        // Display the edit window.
        CRect rect;
        GetSubItemRect(m_editRow, m_editColumn, LVIR_LABEL, rect);
        m_edit.SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

        m_edit.SetFocus();

        // Set the edit window's text.
        CFont font = GetFont();
        m_edit.SetFont(font);
        CString str = GetItemText(m_editRow, m_editColumn);
        m_edit.SetWindowText(str);
    }

    return 0;
}

// Performs the customised drawing of the list-view control.
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

// Called when an item has changed. Here we prevent the item from 
// being activated to preserve our colors.
LRESULT CView::OnItemChanged(LPARAM lparam)
{
    LPNMLISTVIEW pnmv = (LPNMLISTVIEW)lparam;
    int row = pnmv->iItem;
    
    // Prevent the item from being activated.
    if (GetItemState(row, LVIS_SELECTED) != 0)
    {
        SetItemState(row, 0, LVIS_SELECTED);
    }

    return 0;
}

// Handles the WM_NOTIFY messages sent by this window.
LRESULT CView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    case NM_CUSTOMDRAW:     return OnCustomDraw(lparam);
    case NM_CLICK:          return OnClick(lparam);
    case LVN_ITEMCHANGED:   return OnItemChanged(lparam);
    }

    return 0;
}

// Called in response to a message from the edit control to update the 
// text in the subitem.
LRESULT CView::OnUpdateText()
{
    Trace("Should update text\n");
    CString text = m_edit.GetWindowText();
    SetItemText(m_editRow, m_editColumn, text);
    
    return 0;
}

// Sets the CREATESTRUCT parameters for the window before it is created. 
void CView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = LVS_REPORT | LVS_SINGLESEL | WS_CHILD;

    // The LVM_EDITLABEL style is not used. It sounds useful, but it
    // would only allow us to edit the label, not the other subitems. 
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
    TCHAR string[3][20] = { _T("Name"), _T("Size"), _T("Type") };
    for (int i = 0; i < 3; ++i)
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
        switch (msg)
        {
        case UWM_UPDATETEXT:  return OnUpdateText();
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
