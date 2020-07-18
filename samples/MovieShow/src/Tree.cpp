///////////////////////////////////////////////////
// Tree.cpp - Definitions for the CViewTree and CDockTree classes


#include "stdafx.h"
#include "Tree.h"
#include "MovieShowApp.h"
#include "resource.h"


///////////////////////////////////////////////
// CViewTree functions
CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
    if (IsWindow()) DeleteAllItems();
}

// Adds an item to the treeview and updates the item map.
HTREEITEM CViewTree::AddItem(HTREEITEM parent, LPCTSTR text, int imageIndex)
{
    TVITEM tvi;
    ZeroMemory(&tvi, sizeof(TVITEM));
    tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
    tvi.iImage = imageIndex;
    tvi.iSelectedImage = imageIndex;

    // Make a local copy of the item's text
    m_itemsText.push_back(text);
    CString* pString = &m_itemsText.back();
    tvi.pszText = const_cast<LPTSTR>(pString->c_str());

    TVINSERTSTRUCT tvis;
    ZeroMemory(&tvis, sizeof(TVINSERTSTRUCT));
    tvis.hParent = parent;
    tvis.item = tvi;

    HTREEITEM item = InsertItem(tvis);
    
    // Add the item and CString pointer to the map
    m_itemMap.insert(std::make_pair(item, pString));

    return item;
}

// Called when the treeview window is destroyed.
void CViewTree::OnDestroy()
{
    SetImageList(NULL, LVSIL_SMALL);
}

// Called after the treeview window is created.
// Sets the icons for the treeview.
void CViewTree::OnInitialUpdate()
{
    //set the image lists
    m_imlNormal.Create(24, 24, ILC_COLOR32, 1, 0);

    m_imlNormal.AddIcon(IDI_LIBRARY);
    m_imlNormal.AddIcon(IDI_MOVIES);
    m_imlNormal.AddIcon(IDI_BOXSET);
    m_imlNormal.AddIcon(IDI_CALENDAR);
    m_imlNormal.AddIcon(IDI_FAVOURITES);
    m_imlNormal.AddIcon(IDI_MASK);
    m_imlNormal.AddIcon(IDI_VIOLIN);
    m_imlNormal.AddIcon(IDI_SEARCH);

    SetImageList(m_imlNormal, LVSIL_NORMAL);

    // Adjust style to show lines and [+] button
    DWORD dwStyle = GetStyle();
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(dwStyle);
}

// Returns true if the text for the box set is unique.
bool CViewTree::IsBoxSetUnique(LPCTSTR text, HTREEITEM item)
{
    HTREEITEM parentItem = GetParentItem(item);
    HTREEITEM childItem = GetChild(parentItem);
    bool isUnique = true;

    while (childItem != 0)
    {
        if (0 == lstrcmp(GetItemText(childItem), text))
        {
            isUnique = false;
            break;
        }
        
        childItem = GetNextSibling(childItem);
    };

    return isUnique;
}

// Called when the WM_NOTIFY message is reflected back to CViewTree 
// by the framework.
LRESULT CViewTree::OnNotifyReflect(WPARAM, LPARAM lParam)
{
    LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;

    switch (pnmtv->hdr.code)
    {
        case TVN_SELCHANGED:       return OnSelChanged();
        case TVN_BEGINLABELEDIT:   return OnBeginLabelEdit(lParam);
        case TVN_ENDLABELEDIT:     return OnEndLabelEdit(lParam);
    }

    return 0;
}

// Called when a treview item is selected
BOOL CViewTree::OnSelChanged()
{
    CMainFrame& Frame = GetMovieShowApp()->GetMainFrame();
    Frame.OnSelectTreeItem();
    return TRUE;
}

// Allows label editing for Box Set children
BOOL CViewTree::OnBeginLabelEdit(LPARAM lparam)
{
    LPNMTVDISPINFO pTVDispInfo = (LPNMTVDISPINFO)lparam;

    HTREEITEM item = pTVDispInfo->item.hItem;
    HTREEITEM parentItem = GetParentItem(item);
    if (GetItemText(parentItem) == L"Box Sets")
    {
        // Permit label editing for Box Set children
        return FALSE;
    }

    // Suppress label editing 
    return TRUE;
}

// Retrieves the item's CString* from the item map
CString* CViewTree::GetItemString(HTREEITEM item)
{
    CString* pString = 0;
    std::map<HTREEITEM, CString*>::const_iterator m;
    m = m_itemMap.find(item);
    if (m != m_itemMap.end())
        pString = m->second;

    return pString;
}

// Finalises the editing of a Box Set child's label.
BOOL CViewTree::OnEndLabelEdit(LPARAM lparam)
{
    LPNMTVDISPINFO pTVDispInfo = (LPNMTVDISPINFO)lparam;
    HTREEITEM item = pTVDispInfo->item.hItem;
    CString* pString = GetItemString(item);
    assert(pString);

    if (pString)
    {
        CString oldText = *pString;

        // Use the new text unless the user cancelled the edit
        if (pTVDispInfo->item.pszText != 0)
            pString->GetWindowText(GetEditControl());

        // Ensure the boxset name is unique. Append (2) etc. if required.
        if (!IsBoxSetUnique(pString->c_str(), item))
        {
            int start = 1;
            CString append;
            CString str2 = *pString;
            do
            {
                append.Format(L" (%d)", ++start);
                str2 = *pString + append;

            } while (!IsBoxSetUnique(str2, item));

            *pString += append;
        }

        // Confirm the edit, and update the item text.
        pTVDispInfo->item.pszText = (LPTSTR)pString->c_str();       // required for user initiated editing
        SetItemText(item, *pString);                                // required for software initiated editing

        if (oldText != *pString)
        {
            CMainFrame& frame = GetMovieShowApp()->GetMainFrame();
            std::list<MovieInfo>::iterator it;
            std::list<MovieInfo>& data = frame.SetMoviesData();
            for (it = data.begin(); it != data.end(); ++it)
            {
                if ((*it).boxset == oldText)
                    (*it).boxset = *pString;
            }
        }
    }
    
    return TRUE;
}

// Set the treeview styles before the window is created.
void CViewTree::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    cs.style = TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS | TVS_EDITLABELS | TVS_FULLROWSELECT | WS_CHILD;
}

// Removes the specified treeview item.
void CViewTree::RemoveItem(HTREEITEM item)
{
    const std::map<HTREEITEM, CString*>::iterator it = m_itemMap.find(item);
    assert(it != m_itemMap.end());
    if (it != m_itemMap.end())
        m_itemMap.erase(it);
    
    DeleteItem(item);
}

// Swaps the two specified treeview items.
void CViewTree::Swap(HTREEITEM item1, HTREEITEM item2)
{
    TVITEM tv1;
    ZeroMemory(&tv1, sizeof(tv1));
    tv1.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_TEXT;
    tv1.hItem = item1;
    GetItem(tv1);
    CString* pString1 = GetItemString(item1);
    assert(pString1);

    if (pString1)
    {
        tv1.cchTextMax = pString1->GetLength();
        tv1.pszText = const_cast<LPTSTR>(pString1->c_str());

        TVITEM tv2;
        ZeroMemory(&tv2, sizeof(tv2));
        tv2.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_TEXT;
        tv2.hItem = item2;
        GetItem(tv2);
        CString* pString2 = GetItemString(item2);
        assert(pString2);

        if (pString2)
        {
            tv2.cchTextMax = pString2->GetLength();
            tv2.pszText = const_cast<LPTSTR>(pString2->c_str());

            tv1.hItem = item2;
            tv2.hItem = item1;

            // swap the items in the treeview
            SetItem(tv1);
            SetItem(tv2);

            // Swap the values in the map
            const std::map<HTREEITEM, CString*>::iterator it1 = m_itemMap.find(item1);
            const std::map<HTREEITEM, CString*>::iterator it2 = m_itemMap.find(item2);
            if ((it1 != m_itemMap.end()) && (it2 != m_itemMap.end()))
                std::swap(it1->second, it2->second);
        }
    }
}

// Process the messages for the treeview window.
LRESULT CViewTree::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_RBUTTONDOWN:
        return DefWindowProc(msg, wparam, lparam);

    case WM_RBUTTONUP:
        {
            CMainFrame& Frame = GetMovieShowApp()->GetMainFrame();
            Frame.OnRClickTreeItem();
            return DefWindowProc(msg, wparam, lparam);
        }
    }
    
    return WndProcDefault(msg, wparam, lparam);
}



/////////////////////////////////////////////////
//  Definitions for the CDockTree class
CDockTree::CDockTree()
{
    SetView(m_treeView);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

