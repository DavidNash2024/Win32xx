///////////////////////////////////////////////////
// List.cpp - Definitions for the CViewList and CDockList classes


#include "stdafx.h"
#include "List.h"
#include "resource.h"
#include "MovieShowApp.h"


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
        int item = GetItemCount();
        LVITEM lvi;
        ZeroMemory(&lvi, sizeof(LVITEM));
        lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM;
        lvi.iImage = nImage;
        lvi.pszText = const_cast<LPTSTR>(mi.movieName.c_str());
        lvi.iItem = item;
        lvi.lParam = reinterpret_cast<LPARAM>(&mi);

        InsertItem(lvi);
        SetItemText(item, 1, mi.releaseDate);
        SetItemText(item, 2, mi.genre);
        SetItemText(item, 3, mi.fileName);
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

    if (lp1 == 0 || lp2 == 0 || !pSortViewItems)
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
                compare = pSort->m_isSortDown ? 1 : -1;

            if (pmi1->movieName < pmi2->movieName)
                compare = pSort->m_isSortDown ? -1 : 1;
            break;
        }
    case 1:
        {
            if (pmi1->releaseDate > pmi2->releaseDate)
                compare = pSort->m_isSortDown ? 1 : -1;

            if (pmi1->releaseDate < pmi2->releaseDate)
                compare = pSort->m_isSortDown ? -1 : 1;
            break;
        }
    case 2:
        {
            if (pmi1->genre > pmi2->genre)
                compare = pSort->m_isSortDown ? 1 : -1;

            if (pmi1->genre < pmi2->genre)
                compare = pSort->m_isSortDown ? -1 : 1;
            break;
        }
    case 3:
        {
            if (pmi1->fileName > pmi2->fileName)
                compare = pSort->m_isSortDown ? 1 : -1;

            if (pmi1->fileName < pmi2->fileName)
                compare = pSort->m_isSortDown ? -1 : 1;
            break;
        }
    }

    return compare;
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
    SetImageList(m_small, LVSIL_SMALL);

    // Set the report style
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);

    SetExtendedStyle( LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES
        | LVS_EX_HEADERDRAGDROP | LVS_EX_DOUBLEBUFFER/* | LVS_EX_TRANSPARENTBKGND*/);

    SetColumn();
}

// Called when the listview window is destroyed.
void CViewList::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Called after the listview window is created.
void CViewList::OnInitialUpdate()
{
    // Attach's ListView's header window to the CLVHeader object
    m_header.Attach(GetHeader());
}

// Called when the framework reflects the WM_NOTIFY message
// back to CViewList.
LRESULT CViewList::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMITEMACTIVATE pnmitem = (LPNMITEMACTIVATE)lparam;

    switch (pnmitem->hdr.code)
    {
    case NM_RCLICK:
    {
        CMainFrame& Frame = GetMovieShowApp()->GetMainFrame();
        if (GetNextItem(-1, LVNI_SELECTED) >= 0)
            Frame.OnRClickListItem();

        return 0;
    }

    case LVN_ITEMACTIVATE:
    {
        CMainFrame& frame = GetMovieShowApp()->GetMainFrame();
        int item = pnmitem->iItem;

        MovieInfo* pmi = (MovieInfo*)GetItemData(item);
        assert(pmi);
        if (pmi)
            frame.PlayMovie(pmi->fileName);

        return 0;
    }

    case LVN_ITEMCHANGED:
    {
        CMainFrame& frame = GetMovieShowApp()->GetMainFrame();
        int item = pnmitem->iItem;

        MovieInfo* pmi = reinterpret_cast<MovieInfo*>(GetItemData(item));
        frame.OnSelectListItem(pmi);

        return 0;
    }

    case LVN_COLUMNCLICK:
    {
        int column = pnmitem->iSubItem;

        // Determine the required sort order.
        HDITEM  hdrItem;
        ZeroMemory(&hdrItem, sizeof(hdrItem));
        hdrItem.mask = HDI_FORMAT;
        VERIFY(Header_GetItem(GetHeader(), column, &hdrItem));
        bool isSortDown = (hdrItem.fmt & HDF_SORTDOWN) ? false : true;

        // Perform the sort.
        SortColumn(column, isSortDown);
    }

    }
    return 0;
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

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( push )
#pragma warning ( disable : 26812 )       // enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

    SetHeaderSortImage(column, isSortDown ? SHOW_DOWN_ARROW : SHOW_UP_ARROW);

#if defined (_MSC_VER) && (_MSC_VER >= 1400)
#pragma warning ( pop )  // ( disable : 26812 )    enum type is unscoped.
#endif // (_MSC_VER) && (_MSC_VER >= 1400)

    // Select the previously selected or first item
    if (GetSelectedCount() > 0)
        SetItemState(GetSelectionMark(), LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
    else
        SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

// Sets the CREATESTRUCT parameters prior to the window's creation.
void CViewList::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    cs.style = LVS_SHOWSELALWAYS | /*LVS_SINGLESEL | TVS_NOTOOLTIPS |*/ WS_CHILD | LVS_ALIGNLEFT;
}

// Configures the columns in the list view's header.
void CViewList::SetColumn()
{
    // Initialise the columns
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
    lvColumn.pszText = const_cast<LPTSTR>(L"FileName");
    InsertColumn(3, lvColumn);
}

// Sets the up and down sort arrows in the listview's header.
BOOL CViewList::SetHeaderSortImage(int  columnIndex, SHOW_ARROW showArrow)
{
    HWND    hHeader = NULL;
    HDITEM  hdrItem = { 0 };

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
    int remainingWidth = GetClientRect().Width();
    int lastCol = m_header.GetItemCount() - 1;
    for (int i = 0; i < lastCol; i++)
    {
        remainingWidth -= GetColumnWidth(i);
    }

    SetColumnWidth(lastCol, MAX(remainingWidth, 200));
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

        LVITEM lvi;
        ZeroMemory(&lvi, sizeof(LVITEM));
        lvi.mask = LVIF_IMAGE;
        lvi.iImage = nImage;
        lvi.iItem = item;

        SetItem(lvi);
    }
}

