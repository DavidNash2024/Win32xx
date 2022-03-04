///////////////////////////////////////////////////
// List.cpp - Definitions for the CViewList and CDockList classes


#include "stdafx.h"
#include <uxtheme.h>
#include "List.h"
#include "resource.h"
#include "MovieInfo.h"
#include "UserMessages.h"


/////////////////////////////////
// CViewList function definitions
//

// Constructor
CViewList::CViewList()
{
}

// Destructor.
CViewList::~CViewList()
{
    if (IsWindow()) DeleteAllItems();
}

// Adds an item to the list view.
void CViewList::AddItem(const MovieInfo& mi)
{
    bool isFound = false;
    for (int i = 0; i < GetItemCount(); i++)
        if (GetItemData(i) == (DWORD_PTR)&mi)
            isFound = true;

    if (!isFound)
    {
        int nImage = 0;
        if (mi.videoType == L"Live Performances")
            nImage = 1;
        if (!mi.boxset.IsEmpty())
            nImage = 2;
        if (mi.flags & 0x0001)
            nImage = 3;
        if (mi.flags & 0x0002)
            nImage = 4;
        int item = GetItemCount();

        UINT mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        LPTSTR text = const_cast<LPTSTR>(mi.movieName.c_str());
        LPARAM lparam = reinterpret_cast<LPARAM>(&mi);
        InsertItem(mask, item, text, 0, 0, nImage, lparam);

        SetItemText(item, 1, mi.releaseDate);
        SetItemText(item, 2, mi.genre);
        SetItemText(item, 3, mi.fileName);
        SetItemText(item, 4, GetFileTime(mi.lastModifiedTime));
    }
}

// Compares lp1 and lp2. Used for sorting.
int CALLBACK CViewList::CompareFunction(LPARAM lp1, LPARAM lp2, LPARAM pSortViewItems)
{
    //  The comparison function must return a negative value if the first
    //  item should precede the second, a positive value if the first item
    //  should follow the second, or zero if the two items are equivalent.

    assert(lp1);
    assert(lp2);
    assert(pSortViewItems);

    if (lp1 == 0 || lp2 == 0 || pSortViewItems == 0)
        return 0;

    SortViewItems* pSort = reinterpret_cast<SortViewItems*>(pSortViewItems);
    MovieInfo* pmi1 = (MovieInfo*)lp1;
    MovieInfo* pmi2 = (MovieInfo*)lp2;

    int compare = 0;

    switch (pSort->m_column)
    {
    case 0:
        {
            if (pmi1->movieName > pmi2->movieName)
                compare = pSort->m_isSortDown ? -1 : 1;

            if (pmi1->movieName < pmi2->movieName)
                compare = pSort->m_isSortDown ? 1 : -1;
            break;
        }
    case 1:
        {
            if (pmi1->releaseDate > pmi2->releaseDate)
                compare = pSort->m_isSortDown ? -1 : 1;

            if (pmi1->releaseDate < pmi2->releaseDate)
                compare = pSort->m_isSortDown ? 1 : -1;
            break;
        }
    case 2:
        {
            if (pmi1->genre > pmi2->genre)
                compare = pSort->m_isSortDown ? -1 : 1;

            if (pmi1->genre < pmi2->genre)
                compare = pSort->m_isSortDown ? 1 : -1;
            break;
        }
    case 3:
        {
            if (pmi1->fileName > pmi2->fileName)
                compare = pSort->m_isSortDown ? -1 : 1;

            if (pmi1->fileName < pmi2->fileName)
                compare = pSort->m_isSortDown ? 1 : -1;
            break;
        }
    case 4:
    {
        if (CTime(pmi1->lastModifiedTime) > CTime(pmi2->lastModifiedTime))
            compare = pSort->m_isSortDown ? -1 : 1;

        if (CTime(pmi1->lastModifiedTime) < CTime(pmi2->lastModifiedTime))
            compare = pSort->m_isSortDown ? 1 : -1;
        break;
    }
    }

    return compare;
}

// Returns a CString containing the file's date and time.
CString CViewList::GetFileTime(FILETIME fileTime)
{
    // Convert the file time to local time.
    FILETIME localFileTime;
    SYSTEMTIME localSysTime;
    ::FileTimeToLocalFileTime(&fileTime, &localFileTime);
    ::FileTimeToSystemTime(&localFileTime, &localSysTime);

    // Build strings with the date and time with regional settings.
    const int maxChars = 32;
    WCHAR time[maxChars];
    WCHAR date[maxChars];
    ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &localSysTime, NULL, date, maxChars - 1);
    ::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &localSysTime, NULL, time, maxChars - 1);

    // Combine the date and time in a CString.
    CString str = date;
    str += L" ";
    str += time;
    return str;
}

// Called when the listview window is attached to CViewList during Create.
void CViewList::OnAttach()
{
    SetWindowTheme(L"Explorer", NULL);

    // Set the image lists
    m_small.Create(16, 16, ILC_COLOR32, 1, 0);
    m_small.AddIcon(IDI_MOVIES);
    m_small.AddIcon(IDI_VIOLIN);
    m_small.AddIcon(IDI_BOXSET);
    m_small.AddIcon(IDI_FAVOURITES);
    m_small.AddIcon(IDI_EYE);
    SetImageList(m_small, LVSIL_SMALL);

    // Set the report style
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
                      | LVS_EX_HEADERDRAGDROP | LVS_EX_DOUBLEBUFFER );

    SetColumn();
}

// Called when the listview window is destroyed.
void CViewList::OnDestroy()
{
    SetImageList(0, LVSIL_SMALL);
}

// Called after the listview window is created.
void CViewList::OnInitialUpdate()
{
}

// Called with a double click left mouse button or press the Enter key
// on a list view item.
LRESULT CViewList::OnItemActivate(LPNMITEMACTIVATE pnmitem)
{
    int item = pnmitem->iItem;
    MovieInfo* pmi = reinterpret_cast<MovieInfo*>(GetItemData(item));
    assert(pmi);
    if (pmi)
    {
        LPCTSTR movie = pmi->fileName.c_str();
        GetAncestor().SendMessage(UWM_PLAYMOVIE, (WPARAM)movie, 0);
    }

    return 0;
}

// Called when a list view item is selected.
LRESULT CViewList::OnItemChanged(LPNMITEMACTIVATE pnmitem)
{
    int item = pnmitem->iItem;
    MovieInfo* pmi = reinterpret_cast<MovieInfo*>(GetItemData(item));
    GetAncestor().SendMessage(UWM_ONSELECTLISTITEM, (WPARAM)pmi, 0);

    return 0;
}

// Called when a list view column is clicked.
LRESULT CViewList::OnLVColumnClick(LPNMITEMACTIVATE pnmitem)
{
    // Determine the required sort order.
    HDITEM  hdrItem;
    ZeroMemory(&hdrItem, sizeof(hdrItem));
    hdrItem.mask = HDI_FORMAT;
    int column = pnmitem->iSubItem;
    VERIFY(Header_GetItem(GetHeader(), column, &hdrItem));
    bool isSortDown = (hdrItem.fmt & HDF_SORTUP) ? true : false;

    // Perform the sort.
    SortColumn(column, isSortDown);

    return 0;
}

// Call to perform custom drawing.
LRESULT CViewList::OnCustomDraw(LPNMCUSTOMDRAW pnmitem)
{
    switch (pnmitem->dwDrawStage)
    {
    case CDDS_PREPAINT: // Before the paint cycle begins.
        // Request notifications for individual header items.
        return CDRF_NOTIFYITEMDRAW;

    case CDDS_ITEMPREPAINT: // Before an item is drawn
    {
        // Get an appropriate color for the header
        COLORREF color = GetSysColor(COLOR_BTNFACE);
        HWND hFrame = GetAncestor();
        ReBarTheme* pTheme = reinterpret_cast<ReBarTheme*>(::SendMessage(hFrame, UWM_GETRBTHEME, 0, 0));
        if (pTheme && pTheme->UseThemes && pTheme->clrBand2 != 0)
            color = pTheme->clrBkgnd2;

        // Set the background color of the header
        CBrush br(color);
        ::FillRect(pnmitem->hdc, &pnmitem->rc, br);

        // Also set the text background color
        ::SetBkColor(pnmitem->hdc, color);

        return CDRF_DODEFAULT;
    }
    }

    return CDRF_DODEFAULT;
}

// Respond to notifications from child windows,
LRESULT CViewList::OnNotify(WPARAM, LPARAM lparam)
{
    LPNMCUSTOMDRAW  pCustomDraw = (LPNMCUSTOMDRAW)lparam;
    switch (pCustomDraw->hdr.code)
    {
    case NM_CUSTOMDRAW:          return OnCustomDraw(pCustomDraw);
    }

    return 0;
}

// Called when the framework reflects the WM_NOTIFY message
// back to CViewList.
LRESULT CViewList::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMITEMACTIVATE pItemActivate = (LPNMITEMACTIVATE)lparam;

    switch (pItemActivate->hdr.code)
    {
    case LVN_COLUMNCLICK:          return OnLVColumnClick(pItemActivate);
    case LVN_ITEMACTIVATE:         return OnItemActivate(pItemActivate);
    case LVN_ITEMCHANGED:          return OnItemChanged(pItemActivate);
    case NM_RCLICK:                return OnRClick();
    }

    return 0;
}

// Called with a right mouse click on one or more list view items.
LRESULT CViewList::OnRClick()
{
    if (GetNextItem(-1, LVNI_SELECTED) >= 0)
        GetAncestor().SendMessage(UWM_ONRCLICKLISTITEM, 0, 0);

    return 0;
}

// Sets the CREATESTRUCT parameters prior to the window's creation.
void CViewList::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    cs.style = LVS_SHOWSELALWAYS | WS_CHILD | LVS_ALIGNLEFT;
}

// Configures the columns in the list view's header.
void CViewList::SetColumn()
{
    // Initialize the columns
    LV_COLUMN lvColumn;
    ZeroMemory(&lvColumn, sizeof(LV_COLUMN));
    lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    lvColumn.fmt = LVCFMT_LEFT;

    lvColumn.cx = 300;
    lvColumn.pszText = const_cast<LPTSTR>(L"Movie Title");
    InsertColumn(0, lvColumn);

    lvColumn.cx = 150;
    lvColumn.pszText = const_cast<LPTSTR>(L"Release Date");
    InsertColumn(1, lvColumn);

    lvColumn.cx = 150;
    lvColumn.pszText = const_cast<LPTSTR>(L"Genre");
    InsertColumn(2, lvColumn);

    lvColumn.cx = 200;
    lvColumn.pszText = const_cast<LPTSTR>(L"File Name");
    InsertColumn(3, lvColumn);

    lvColumn.cx = 200;
    lvColumn.pszText = const_cast<LPTSTR>(L"File Date");
    InsertColumn(4, lvColumn);
}

// Sets the up and down sort arrows in the listview's header.
BOOL CViewList::SetHeaderSortImage(int  columnIndex, int showArrow)
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

            return Header_SetItem(hHeader, columnIndex, &hdrItem);
        }
    }

    return FALSE;
}

// Sets the width of the last column to match the window width.
void CViewList::SetLastColumnWidth()
{
    if (IsWindow())
    {
        int remainingWidth = GetClientRect().Width();
        int lastCol = Header_GetItemCount(GetHeader()) - 1;
        for (int i = 0; i < lastCol; i++)
        {
            remainingWidth -= GetColumnWidth(i);
        }

        SetColumnWidth(lastCol, MAX(remainingWidth, 100));
    }
}

// Called when the user clicks on a column in the listview's header.
void CViewList::SortColumn(int column, bool isSortDown)
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

    SetHeaderSortImage(column, isSortDown ? SHOW_DOWN_ARROW : SHOW_UP_ARROW);

    // Select the previously selected or first item
    if (GetSelectedCount() > 0)
        SetItemState(GetSelectionMark(), LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
    else
        SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

// Sets the image associated with the listview item.
void CViewList::UpdateItemImage(int item)
{
    MovieInfo* mi = (MovieInfo*)GetItemData(item);

    if (mi)
    {
        int nImage = 0;
        if (mi->videoType == L"Live Performances")
            nImage = 1;
        if (!mi->boxset.IsEmpty())
            nImage = 2;
        if (mi->flags & 0x0001)
            nImage = 3;
        if (mi->flags & 0x0002)
            nImage = 4;

        LVITEM lvi;
        ZeroMemory(&lvi, sizeof(LVITEM));
        lvi.mask = LVIF_IMAGE;
        lvi.iImage = nImage;
        lvi.iItem = item;

        SetItem(lvi);
    }
}

LRESULT CViewList::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Adjust the list view's columns when the window is resized.
    if (msg == WM_WINDOWPOSCHANGED)
        SetLastColumnWidth();

    return WndProcDefault(msg, wparam, lparam);
}

