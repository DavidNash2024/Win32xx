/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "UserMessages.h"

#ifndef HDF_SORTUP
#define HDF_SORTUP              0x0400
#define HDF_SORTDOWN            0x0200
#endif

/////////////////////////////
// CView function definitions
//

// Constructor.
CView::CView() :  m_row(-1), m_column(-1)
{
}

// Destructor.
CView::~CView()
{
    if (IsWindow()) DeleteAllItems();
}

void CView::AddItem(CString subItem0, CString subItem1, CString subItem2)
{
    // Create the itemData smart pointer.
    ListItemDataPtr itemData(new ListItemData(subItem0, subItem1, subItem2));
    m_allListItemData.push_back(itemData);

    // Set the text for the all the subItems belonging to the item.
    int item = GetItemCount();
    InsertItem(item, LPSTR_TEXTCALLBACK);
    SetItemText(item, 1, LPSTR_TEXTCALLBACK);
    SetItemText(item, 2, LPSTR_TEXTCALLBACK);

    // Set the item's lparam.
    // The item's lparam is used for sorting.
    LPARAM lparam = reinterpret_cast<LPARAM>(itemData.get());
    SetItemData(item, lparam);
}

// Compares two items using their lparam values.
// The item's lparam contains a pointer to its ListItemData.
int CALLBACK CView::CompareFunction(LPARAM param1, LPARAM param2, LPARAM pSortViewItems)
{
    assert(param1);
    assert(param2);
    assert(pSortViewItems);

    if (param1 == 0 || param2 == 0 || pSortViewItems == 0)
        return 0;

    ListItemData* pItem1 = reinterpret_cast<ListItemData*>(param1);
    ListItemData* pItem2 = reinterpret_cast<ListItemData*>(param2);
    SortViewItems* pSort = reinterpret_cast<SortViewItems*>(pSortViewItems);

    int compare = 0;
    int subItem = pSort->m_column;
    if (pItem1->m_subItemText[subItem] > pItem2->m_subItemText[subItem])
        compare = pSort->m_isSortDown ? 1 : -1;

    if (pItem1->m_subItemText[subItem] < pItem2->m_subItemText[subItem])
        compare = pSort->m_isSortDown ? -1 : 1;

    return compare;
}

// Insert 4 list view items.
void CView::AddAllItems()
{
    AddItem(_T("ListViewApp.h"),   _T("1 KB"), _T("C++ Header file"));
    AddItem(_T("main.cpp"),        _T("1 KB"), _T("C++ Source file"));
    AddItem(_T("ListViewApp.cpp"), _T("3 KB"), _T("C++ Source file"));
    AddItem(_T("Resource.rc"),     _T("2 KB"), _T("C++ Resource Script"));
    AddItem(_T("Readme.txt"),      _T("4 KB"), _T("Text file"));
}

// The window handle (HWND) is attached to CView when it is created.
void CView::OnAttach()
{
    // The extended ListView styles must be set after the window is created.
    // Full row select is required to support the editing of subitems.
    SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    // Fill the List-View control.
    SetColumns();
    AddAllItems();

    // Sort down based on the first column.
    SortColumn(0, true);

    // Create the edit window. It is initially hidden.
    m_edit.Create(*this);
}

// Shows the edit window when a subitem is clicked.
LRESULT CView::OnClick(LPNMLISTVIEW pListView)
{
    m_row = pListView->iItem;
    m_column = pListView->iSubItem;

    if (m_row != -1 && m_column != -1)
    {
        // Display the edit window.
        CRect rect;
        GetSubItemRect(m_row, m_column, LVIR_LABEL, rect);
        m_edit.SetWindowPos(HWND_TOP, rect, SWP_SHOWWINDOW);

        m_edit.SetFocus();

        // Set the edit window's text.
        CFont font = GetFont();
        m_edit.SetFont(font);
        CString str = GetItemText(m_row, m_column);
        m_edit.SetWindowText(str);
    }

    return 0;
}

// Performs the customised drawing of the list-view control.
LRESULT CView::OnCustomDraw(LPNMLVCUSTOMDRAW pLVCustomDraw)
{

    switch (pLVCustomDraw->nmcd.dwDrawStage)
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
            bool isEvenRow = ((pLVCustomDraw->nmcd.dwItemSpec % 2) == 0);
            switch (pLVCustomDraw->iSubItem)
            {
                case 0: // 1st column
                {
                    if (isEvenRow)
                    {
                        pLVCustomDraw->clrText = RGB(32, 32, 32);
                        pLVCustomDraw->clrTextBk = RGB(192, 255, 255);
                    }
                    else
                    {
                        pLVCustomDraw->clrText = RGB(32, 32, 32);
                        pLVCustomDraw->clrTextBk = RGB(255, 255, 192);
                    }
                }
                break;

                case 1: // 2nd column
                {
                    if (isEvenRow)
                    {
                        pLVCustomDraw->clrText = RGB(64, 64, 64);
                        pLVCustomDraw->clrTextBk = RGB(208, 255, 255);
                    }
                    else
                    {
                        pLVCustomDraw->clrText = RGB(64, 64, 64);
                        pLVCustomDraw->clrTextBk = RGB(255, 255, 208);
                    }
                }
                break;

                case 2:  // 3rd column
                {
                    if (isEvenRow)
                    {
                        pLVCustomDraw->clrText = RGB(96, 96, 96);
                        pLVCustomDraw->clrTextBk = RGB(224, 255, 255);
                    }
                    else
                    {
                        pLVCustomDraw->clrText = RGB(96, 96, 96);
                        pLVCustomDraw->clrTextBk = RGB(255, 255, 244);
                    }
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
LRESULT CView::OnItemChanged(LPNMLISTVIEW pListView)
{
    int row = pListView->iItem;

    // Prevent the item from being activated.
    if (GetItemState(row, LVIS_SELECTED) != 0)
    {
        SetItemState(row, 0, LVIS_SELECTED);
    }

    return 0;
}

// Called when a list view column is clicked.
LRESULT CView::OnLVColumnClick(LPNMITEMACTIVATE pnmitem)
{
    // Determine the required sort order.
    HDITEM  hdrItem;
    ZeroMemory(&hdrItem, sizeof(hdrItem));
    hdrItem.mask = HDI_FORMAT;
    int column = pnmitem->iSubItem;
    VERIFY(Header_GetItem(GetHeader(), column, &hdrItem));
    bool isSortDown = (hdrItem.fmt & HDF_SORTUP) ? false : true;

    // Perform the sort.
    SortColumn(column, isSortDown);

    return 0;
}

// Called in response to a LVN_GETDISPINFO notification.
// Updates the list view item with the relevant information.
LRESULT CView::OnLVNDispInfo(NMLVDISPINFO* pdi)
{
    ListItemData* pItem = reinterpret_cast<ListItemData*>(pdi->item.lParam);

    // Add the text for a subitem.
    if (pdi->item.mask & LVIF_TEXT)
    {
        // Copy the text from the ListItemData to the list-view's subitem.
        int i = pdi->item.iSubItem;
        StrCopy(pdi->item.pszText, pItem->m_subItemText[i], pdi->item.cchTextMax);
    }

    return 0;
}

// Handles the WM_NOTIFY messages sent by this window.
LRESULT CView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMLVCUSTOMDRAW pLVCustomDraw = (LPNMLVCUSTOMDRAW)lparam;
    LPNMLISTVIEW pListView = (LPNMLISTVIEW)lparam;
    LPNMITEMACTIVATE pnmitem = reinterpret_cast<LPNMITEMACTIVATE>(lparam);
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(lparam);

    assert(pListView);
    UINT code = pListView->hdr.code;

    switch (code)
    {
    case LVN_COLUMNCLICK:   return OnLVColumnClick(pnmitem);
    case LVN_GETDISPINFO:   return OnLVNDispInfo(pDispInfo);
    case LVN_ITEMCHANGED:   return OnItemChanged(pListView);
    case NM_CUSTOMDRAW:     return OnCustomDraw(pLVCustomDraw);
    case NM_CLICK:          return OnClick(pListView);
    }

    return 0;
}

// Called in response to a message from the edit control to update the
// text in the subitem.
LRESULT CView::OnUpdateText()
{
    CString text = m_edit.GetWindowText();

    // Update the ListItemData
    LPARAM lparam = GetItemData(m_row);
    ListItemData* pData = reinterpret_cast<ListItemData*>(lparam);
    pData->m_subItemText[m_column] = text;

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

    // initialize the LV_COLUMN struct
    LV_COLUMN lvColumn;
    ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;

    // 1st column
    lvColumn.pszText = const_cast<LPTSTR>(_T("Name"));
    lvColumn.cx = 130;
    InsertColumn(0, lvColumn);

    // 2nd column
    lvColumn.pszText = const_cast<LPTSTR>(_T("Size"));
    lvColumn.cx = 80;
    InsertColumn(1, lvColumn);

    // 3rd column
    lvColumn.pszText = const_cast<LPTSTR>(_T("Type"));
    lvColumn.cx = 160;
    InsertColumn(2, lvColumn);
}

// Sets the up and down sort arrows in the listview's header.
void CView::SetHeaderSortImage(int  columnIndex, int showArrow)
{
    HWND    hHeader = 0;
    HDITEM  hdrItem;
    ZeroMemory(&hdrItem, sizeof(hdrItem));

    hHeader = GetHeader();
    if (hHeader)
    {
        hdrItem.mask = HDI_FORMAT;

        if (Header_GetItem(hHeader, columnIndex, &hdrItem))
        {
            if (showArrow == SHOW_UP_ARROW)
            {
                hdrItem.fmt = (hdrItem.fmt & ~HDF_SORTDOWN) | HDF_SORTUP;
            }
            else if (showArrow == SHOW_DOWN_ARROW)
            {
                hdrItem.fmt = (hdrItem.fmt & ~HDF_SORTUP) | HDF_SORTDOWN;
            }
            else
            {
                hdrItem.fmt = hdrItem.fmt & ~(HDF_SORTDOWN | HDF_SORTUP);
            }

            Header_SetItem(hHeader, columnIndex, &hdrItem);
        }
    }

}

// Called when the user clicks on a column in the listview's header.
void CView::SortColumn(int column, bool isSortDown)
{
    // Perform the sort.
    SortViewItems sort(column, isSortDown);
    SortItems(CompareFunction, (LPARAM)&sort);

    // Ensure the selected item is visible after sorting.
    int itemint = GetNextItem(-1, LVNI_SELECTED);
    EnsureVisible(itemint, FALSE);

    // Add an arrow to the column header.
    for (int col = 0; col < Header_GetItemCount(GetHeader()); col++)
        SetHeaderSortImage(col, SHOW_NO_ARROW);

    SetHeaderSortImage(column, isSortDown ? SHOW_UP_ARROW : SHOW_DOWN_ARROW);

    // Select the previously selected or first item
    if (GetSelectionMark() > 0)
        SetItemState(GetSelectionMark(), LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
    else
        SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
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
