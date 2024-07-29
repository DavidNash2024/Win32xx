///////////////////////////////////////////////////
// Tree.cpp - Definitions for the CViewTree and CDockTree classes


#include "stdafx.h"
#include "Tree.h"
#include "MovieInfo.h"
#include "resource.h"
#include "UserMessages.h"


/////////////////////////////////
// CViewTree function definitions
//

// Constructor.
CViewTree::CViewTree()
{
}

// Destructor.
CViewTree::~CViewTree()
{
    if (IsWindow()) DeleteAllItems();
}

// Returns true if the text for the box set is unique.
bool CViewTree::IsBoxSetUnique(LPCTSTR text, HTREEITEM item)
{
    HTREEITEM parentItem = GetParentItem(item);
    HTREEITEM childItem = GetChild(parentItem);
    bool isUnique = true;

    while (childItem != 0)
    {
        if (childItem != item)
        {
            if (lstrcmp(GetItemText(childItem), text) == 0)
            {
                isUnique = false;
                break;
            }
        }

        childItem = GetNextSibling(childItem);
    };

    return isUnique;
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

// Called in response to a WM_DPICHANGED_BEFOREPARENT message that is sent to child
// windows after a DPI change. A WM_DPICHANGED_BEFOREPARENT is only received when the
// application is DPI_AWARENESS_PER_MONITOR_AWARE.
LRESULT CViewTree::OnDpiChangedBeforeParent(UINT msg, WPARAM wparam, LPARAM lparam)
{
    SetDPIImages();
    return FinalWindowProc(msg, wparam, lparam);
}

// Finalizes the editing of a Box Set child's label.
BOOL CViewTree::OnEndLabelEdit(LPARAM lparam)
{
    LPNMTVDISPINFO pTVDispInfo = (LPNMTVDISPINFO)lparam;
    HTREEITEM item = pTVDispInfo->item.hItem;
    m_itemText = GetItemText(item);

    CString oldText = m_itemText;

    // Use the new text unless the user cancelled the edit
    LPTSTR text = pTVDispInfo->item.pszText;
    if ((text != nullptr) && (lstrlen(text) != 0))
        m_itemText.GetWindowText(GetEditControl());

    // Ensure the boxset name is unique. Append (2) etc. if required.
    if (!IsBoxSetUnique(m_itemText, item))
    {
        int start = 1;
        CString append;
        CString str2 = m_itemText;
        do
        {
            append.Format(L" (%d)", ++start);
            str2 = m_itemText + append;

        } while (!IsBoxSetUnique(str2, item));

        m_itemText += append;
    }

    // Confirm the edit, and update the item text.
    SetItemText(item, m_itemText);
    pTVDispInfo->item.pszText = (LPTSTR)m_itemText.c_str();

    if (oldText != m_itemText)
    {
        std::list<MovieInfo>* data;
        data = (std::list<MovieInfo>*)GetAncestor().SendMessage(UWM_GETMOVIESDATA, 0, 0);
        assert(data);
        for (auto& i : *data)
        {
            if (i.boxset == oldText)
                i.boxset = m_itemText;
        }
    }

    GetAncestor().SendMessage(UWM_BOXSETCHANGED, 0, 0);

    return TRUE;
}

// Called after the treeview window is created.
// Sets the icons and style for the treeview.
void CViewTree::OnInitialUpdate()
{
    SetDPIImages();

    // Adjust style to show lines and [+] button
    DWORD dwStyle = GetStyle();
    dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
    SetStyle(dwStyle);
}

// Called when the WM_NOTIFY message is reflected back to CViewTree
// by the framework.
LRESULT CViewTree::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMTREEVIEW pTreeView = (LPNMTREEVIEW)lparam;
    switch (pTreeView->hdr.code)
    {
        case TVN_SELCHANGED:       return OnSelChanged();
        case TVN_BEGINLABELEDIT:   return OnBeginLabelEdit(lparam);
        case TVN_ENDLABELEDIT:     return OnEndLabelEdit(lparam);
    }

    return 0;
}

// Called when the right mouse button is pressed.
LRESULT CViewTree::OnRButtonDown(UINT msg, WPARAM wparam, LPARAM lparam)
{
    return DefWindowProc(msg, wparam, lparam);
}

// Called when the right mouse button is released.
LRESULT CViewTree::OnRButtonUp(UINT msg, WPARAM wparam, LPARAM lparam)
{
    // Displays the popup menus for the box sets.
    GetAncestor().SendMessage(UWM_ONRCLICKTREEITEM, 0, 0);

    return DefWindowProc(msg, wparam, lparam);
}

// Called when a treeview item is selected
BOOL CViewTree::OnSelChanged()
{
    // Send the message to CMainFrame.
    GetAncestor().SendMessage(UWM_ONSELECTTREEITEM, 0, 0);
    return TRUE;
}

// Set the treeview styles before the window is created.
void CViewTree::PreCreate(CREATESTRUCT& cs)
{
    CTreeView::PreCreate(cs);
    cs.dwExStyle |= WS_EX_CLIENTEDGE;
    cs.style |= TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS | TVS_EDITLABELS | TVS_FULLROWSELECT;
}

// Adjusts the listview image sizes in response to window DPI changes.
void CViewTree::SetDPIImages()
{
    //set the image lists
    int size = DpiScaleInt(24);
    CImageList normalImages;
    normalImages.Create(size, size, ILC_COLOR32, 1, 0);

    normalImages.AddIcon(IDI_LIBRARY);
    normalImages.AddIcon(IDI_MOVIES);
    normalImages.AddIcon(IDI_BOXSET);
    normalImages.AddIcon(IDI_CALENDAR);
    normalImages.AddIcon(IDI_FAVOURITES);
    normalImages.AddIcon(IDI_MASK);
    normalImages.AddIcon(IDI_VIOLIN);
    normalImages.AddIcon(IDI_SEARCH);
    normalImages.AddIcon(IDI_EYE);

    SetImageList(normalImages, TVSIL_NORMAL);

    // Reset the item indentation.
    int imageWidth = size;
    SetIndent(imageWidth);
}

// Swaps the two specified treeview items.
void CViewTree::Swap(HTREEITEM item1, HTREEITEM item2)
{
    TVITEM tv1{};
    tv1.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_TEXT;
    tv1.hItem = item1;
    GetItem(tv1);
    CString str1 = GetItemText(item1);

    tv1.cchTextMax = str1.GetLength();
    tv1.pszText = const_cast<LPTSTR>(str1.c_str());

    TVITEM tv2{};
    tv2.mask = TVIF_HANDLE | TVIF_IMAGE | TVIF_TEXT;
    tv2.hItem = item2;
    GetItem(tv2);
    CString str2 = GetItemText(item2);

    tv2.cchTextMax = str2.GetLength();
    tv2.pszText = const_cast<LPTSTR>(str2.c_str());

    tv1.hItem = item2;
    tv2.hItem = item1;

    // swap the items in the treeview
    SetItem(tv1);
    SetItem(tv2);
}

// Process the messages for the treeview window.
LRESULT CViewTree::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_RBUTTONDOWN:            return OnRButtonDown(msg, wparam, lparam);
        case WM_RBUTTONUP:              return OnRButtonUp(msg, wparam, lparam);
        case WM_DPICHANGED_BEFOREPARENT: return OnDpiChangedBeforeParent(msg, wparam, lparam);
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
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}

/////////////////////////////////
// CDockTree function definitions
//

// Constructor.
CDockTree::CDockTree()
{
    SetView(m_treeView);

    // Set the width of the splitter bar
    SetBarWidth(8);
}

// Handle messages for the docktree window.
LRESULT CDockTree::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
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
        ::MessageBox(nullptr, str1, str2, MB_ICONERROR);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        ::MessageBox(nullptr, str1, _T("Error: std::exception"), MB_ICONERROR);
    }

    return 0;
}