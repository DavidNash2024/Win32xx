/////////////////////////////
// MyTreeView.cpp
//

#include "stdafx.h"
#include "MyTreeView.h"
#include "ExplorerApp.h"
#include "resource.h"

//////////////////////////////////
// CMyTreeView function definitions
//

// Constructor.
CMyTreeView::CMyTreeView()
{
}

// Destructor.
CMyTreeView::~CMyTreeView()
{
}

// Compares param1 and param2. Used for sorting.
int CALLBACK CMyTreeView::CompareFunction(LPARAM param1, LPARAM param2, LPARAM)
{
    TreeItemData* pItem1 = reinterpret_cast<TreeItemData*>(param1);
    TreeItemData* pItem2 = reinterpret_cast<TreeItemData*>(param2);
    HRESULT result;

    result = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());
    if(FAILED(result))
        return 0;

    return (short)SCODE_CODE(GetScode(result));
}

// Identifies the tree view item for the point, and calls DoItemMenu.
void CMyTreeView::DoContextMenu(CPoint& point)
{
    TVHITTESTINFO  htInfo;
    htInfo.pt = point;
    ScreenToClient(htInfo.pt);
    htInfo.flags = LVHT_NOWHERE;
    HitTest(htInfo);

    if(TVHT_ONITEM & htInfo.flags)
        DoItemMenu(htInfo.hItem , point);
}

// Displays the appropriate context menu, positioned at the specified point.
void CMyTreeView::DoItemMenu(HTREEITEM item, CPoint& point)
{
    TVITEM itemInfo;
    ZeroMemory(&itemInfo, sizeof(itemInfo));
    itemInfo.mask = TVIF_PARAM;
    itemInfo.hItem = item;

    if(GetItem(itemInfo))
    {
        HRESULT        result;
        TreeItemData*  pInfo = reinterpret_cast<TreeItemData*>(itemInfo.lParam);
        CContextMenu cm;
        CShellFolder sf;

        if (pInfo->GetParentFolder().GetIShellFolder() != NULL)
            sf = pInfo->GetParentFolder();
        else
            sf.DesktopFolder();

        if(sf.GetIShellFolder())
        {
            LPCITEMIDLIST pidl = pInfo->GetRelCpidl();
            result = sf.GetContextMenu(*this, 1, &pidl, cm);

            if(SUCCEEDED(result))
            {
                CMenu popup;
                popup.CreatePopupMenu();
                if(popup.GetHandle())
                {
                    result = cm.QueryContextMenu(popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);

                    if(SUCCEEDED(result))
                    {
                        cm.GetContextMenu2(m_ccm2);

                        UINT idCmd = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON ,
                            point.x, point.y, *this, NULL);

                        // A Treeview control sometimes requires this to end the
                        // TrackPopupMenu properly.
                        GetParent().PostMessage(WM_CANCELMODE, 0, 0);

                        if(idCmd)
                        {
                            CMINVOKECOMMANDINFO  cmi;
                            ZeroMemory(&cmi, sizeof(cmi));
                            cmi.cbSize = sizeof(cmi);
                            cmi.hwnd = GetHwnd();
                            cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                            cmi.nShow = SW_SHOWNORMAL;
                            cm.Invoke(cmi);
                        }

                        m_ccm2.Release();
                    }
                }
            }
        }
    }
}

// Enumerates a child folder, and adds the new entries to the tree view.
void CMyTreeView::EnumObjects(HTREEITEM parentItem, CShellFolder& parentFolder, Cpidl& cpidlParent)
{
    CEnumIDList list;
    if(SUCCEEDED(parentFolder.GetEnumIDList(0, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, list)))
    {
        ULONG fetched = 1;
        Cpidl cpidlRel;

        // Enumerate the item's PIDLs.
        while(S_OK == (list.Next(1, cpidlRel, fetched)) && fetched)
        {
            TVITEM         itemInfo;
            ULONG          attribs;
            ZeroMemory(&itemInfo, sizeof(itemInfo));

            // Fill in the TV_ITEM structure for this item.
            itemInfo.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

            // Store a pointer to the TreeItemData in the lParam and m_pItems.
            TreeItemDataPtr pItem(new TreeItemData(cpidlParent, cpidlRel, parentFolder));
            itemInfo.lParam = reinterpret_cast<LPARAM>(pItem.get());

            // m_pItems is a vector of smart pointers. The memory allocated by
            // new is automatically deleted when the vector goes out of scope.
            m_pItems.push_back(pItem);

            // Text and images are done on a callback basis.
            itemInfo.pszText = LPSTR_TEXTCALLBACK;
            itemInfo.iImage = itemInfo.iSelectedImage = I_IMAGECALLBACK;

            // Determine if the item has children or shared.
            attribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_SHARE;
            parentFolder.GetAttributes(1, cpidlRel, attribs);
            itemInfo.cChildren = (attribs & SFGAO_HASSUBFOLDER);

            // Determine if the item is shared.
            if(attribs & SFGAO_SHARE)
            {
                itemInfo.mask |= TVIF_STATE;
                itemInfo.stateMask |= TVIS_OVERLAYMASK;
                itemInfo.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
            }

            // Fill in the TVINSERTSTRUCT structure for this item.
            TVINSERTSTRUCT insertInfo;
            ZeroMemory(&insertInfo, sizeof(insertInfo));
            insertInfo.item = itemInfo;
            insertInfo.hInsertAfter = TVI_LAST;
            insertInfo.hParent = parentItem;

            InsertItem(insertInfo);
            fetched = 0;
        }
    }
}

// Called when a tree view item is expanded.
BOOL CMyTreeView::GetChildItems(HTREEITEM parentItem)
{
    TVITEM itemInfo;
    ZeroMemory(&itemInfo, sizeof(itemInfo));
    itemInfo.mask = TVIF_PARAM;
    itemInfo.hItem = parentItem;
    if (!GetItem(itemInfo))
        return FALSE;

    // Change the cursor.
    HCURSOR hCursor = ::SetCursor(LoadCursor(0, IDC_WAIT));

    // Turn redrawing off in the TreeView.
    // This will speed things up as we add items.
    SetRedraw(FALSE);

    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(itemInfo.lParam);
    CShellFolder parentFolder;
    HRESULT      result;

    if (!pItem->GetParentFolder().GetIShellFolder())
        result = parentFolder.DesktopFolder();
    else
        result = pItem->GetParentFolder().GetSubFolder(pItem->GetRelCpidl(), parentFolder);

    if (FAILED(result))
        return FALSE;

    EnumObjects(parentItem, parentFolder, pItem->GetFullCpidl());

    // Sort the new items.
    TV_SORTCB sortInfo;
    sortInfo.hParent = parentItem;
    sortInfo.lpfnCompare = CompareFunction;
    sortInfo.lParam = 0;
    SortChildrenCB(&sortInfo, 0);

    // Turn redrawing back on in the TreeView.
    SetRedraw(TRUE);

    UpdateWindow();
    ::SetCursor(hCursor);

    return TRUE;
}

// Performs the initial loading of the tree view from the root folder (desktop).
BOOL CMyTreeView::GetRootItems()
{
    DeleteAllItems();

    Cpidl cpidlDesk;
    cpidlDesk.GetSpecialFolderLocation(0, CSIDL_DESKTOP);
    if (cpidlDesk.GetPidl())
    {
        // Fill in the TVITEM structure for this item.
        TV_ITEM           itemInfo;
        ZeroMemory(&itemInfo, sizeof(itemInfo));
        itemInfo.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

        // Store a pointer to the TreeItemData in the lParam and m_pItems.
        TreeItemDataPtr pItem(new TreeItemData(cpidlDesk));
        itemInfo.lParam = reinterpret_cast<LPARAM>(pItem.get());

        // m_pItems is a vector of smart pointers. The memory allocated by
        // new is automatically deleted when the vector goes out of scope.
        m_pItems.push_back(pItem);

        // Text and images are done on a callback basis.
        itemInfo.pszText = LPSTR_TEXTCALLBACK;
        itemInfo.iImage = itemInfo.iSelectedImage = I_IMAGECALLBACK;

        // Assume the desktop has children.
        itemInfo.cChildren = TRUE;

        // Fill in the TVINSERTSTRUCT structure for this item.
        TVINSERTSTRUCT   insertInfo;
        ZeroMemory(&insertInfo, sizeof(insertInfo));
        insertInfo.item = itemInfo;
        insertInfo.hInsertAfter = TVI_LAST;
        insertInfo.hParent = TVI_ROOT;

        SetFocus();

        // Add the item.
        HTREEITEM parentItem;
        parentItem = InsertItem(insertInfo);

        // Go ahead and expand this item.
        Expand(parentItem, TVE_EXPAND);

        // Find and expand the drives folder.
        Cpidl cpidlDrives;
        cpidlDrives.GetSpecialFolderLocation(0, CSIDL_DRIVES);
        HTREEITEM nextItem = GetNextItem(parentItem, TVGN_CHILD);
        while (nextItem != 0)
        {
            TreeItemData* data = reinterpret_cast<TreeItemData*>(GetItemData(nextItem));
            assert(data);
            if (data->GetFullCpidl() == cpidlDrives)
            {
                // Expand the drives folder item.
                Expand(nextItem, TVE_EXPAND);
                break;
            }

            nextItem = GetNextItem(nextItem, TVGN_NEXT);
        }

        SelectItem(parentItem);
        return TRUE;
    }

    return FALSE;
}

// Called when a window handle (HWND) is attached to CMyTreeView.
void CMyTreeView::OnAttach()
{
    // Get a copy of the system image lists
    SHFILEINFO  sfi;
    ZeroMemory(&sfi, sizeof(sfi));

    HIMAGELIST hSmall = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0,
        &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    SetImageList(hSmall, LVSIL_NORMAL);
}

// Called when the window is destroyed.
void CMyTreeView::OnDestroy()
{
    // Cleanup required by Win2000
    DeleteAllItems();
    m_pItems.clear();
}

// Called in response to a right mouse click to open a context menu.
LRESULT CMyTreeView::OnNMRClick(LPNMHDR)
{
    CPoint pos = GetCursorPos();
    DoContextMenu(pos);

    return 0;
}

// Process notification reflected back from the parent window.
LRESULT CMyTreeView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    switch (pHeader->code)
    {
    case NM_RCLICK:         return OnNMRClick(pHeader);
    case TVN_ITEMEXPANDING: return OnTVNExpanding(reinterpret_cast<LPNMTREEVIEW>(lparam));
    case TVN_GETDISPINFO:   return OnTVNGetDispInfo(reinterpret_cast<LPNMTVDISPINFO>(lparam));
    case TVN_SELCHANGED:    return OnTVNSelChanged(reinterpret_cast<LPNMTREEVIEW>(lparam));
    }

    return 0;
}

// Called in response to a TVN_GETDISPINFO notification.
// Updates the tree-view item with file information.
LRESULT CMyTreeView::OnTVNGetDispInfo(LPNMTVDISPINFO pDI)
{
    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(pDI->item.lParam);

    // Add the item text.
    if (pDI->item.mask & TVIF_TEXT)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        // Get the display name of the item.
        if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
            StrCopy(pDI->item.pszText, sfi.szDisplayName, pDI->item.cchTextMax);
    }

    // Add the unselected image.
    if (pDI->item.mask & TVIF_IMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        // Get the unselected image for this item.
        if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
            pDI->item.iImage = sfi.iIcon;
    }

    // Add the selected image.
    if (pDI->item.mask & TVIF_SELECTEDIMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        // Get the selected image for this item.
        if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON))
            pDI->item.iSelectedImage = sfi.iIcon;
    }

    return 0;
}

// Called when the tree view item is expanded.
LRESULT CMyTreeView::OnTVNExpanding(LPNMTREEVIEW pNMTV)
{
    switch (pNMTV->action)
    {
    case TVE_EXPAND:
    {
        UINT ExpandedOnce = pNMTV->itemNew.state & TVIS_EXPANDEDONCE;
        if (!ExpandedOnce)
            GetChildItems(pNMTV->itemNew.hItem);
    }
    break;
    }

    return 0;
}

// Called when a new tree view item has been selected.
LRESULT CMyTreeView::OnTVNSelChanged(LPNMTREEVIEW pNMTV)
{
    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(pNMTV->itemNew.lParam);

    CMyListView& LeftView = GetExplorerApp()->GetMainFrame().GetListView();
    LeftView.DisplayFolder(pItem->GetParentFolder(), pItem->GetFullCpidl(), pItem->GetRelCpidl());

    return 0;
}

// Set the CREATESTRUCT parameters before the window is created.
void CMyTreeView::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | TVS_HASLINES |
                    TVS_HASBUTTONS | TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS ;
}

// Updates the tree view based on a selection in the list view.
BOOL CMyTreeView::SelectFromListView(Cpidl& cpidlFull)
{
    HTREEITEM item = GetSelection();

    // Set parent item's has Children flag.
    TVITEM itemInfo;
    ZeroMemory(&itemInfo, sizeof(itemInfo));
    itemInfo.mask = TVIF_CHILDREN;
    itemInfo.cChildren = 1;
    itemInfo.hItem = item;
    SetItem(itemInfo);

    // Expand the tree item.
    Expand(item, TVE_EXPAND);

    // First Child.
    HTREEITEM childItem = GetChild(item);

    // Search for the item displayed in the list view.
    while (childItem != 0)
    {
        // Get the TVITEM structure for this item.
        ZeroMemory(&itemInfo, sizeof(itemInfo));
        itemInfo.mask = TVIF_PARAM;
        itemInfo.hItem = childItem;
        if(!GetItem(itemInfo))
            return FALSE;

        // Get the TreeItemData pointer from the item's lParam.
        TreeItemData* pTD = reinterpret_cast<TreeItemData*>(itemInfo.lParam);

        // Compare the pidls.
        if (pTD->GetFullCpidl() == cpidlFull)
        {
            SelectItem(childItem);
            return TRUE;
        }

        // Next Sibling.
        childItem = GetNextSibling(childItem);
    }

    TRACE("Item NOT found\n");
    return FALSE;
}

// Process the tree view's window messages.
LRESULT CMyTreeView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case WM_DRAWITEM:
        case WM_MENUCHAR:
        case WM_MEASUREITEM:
        case WM_INITMENUPOPUP:
        {
            // Add features implemented via IContextMenu2.
            if (m_ccm2.GetIContextMenu2())
                m_ccm2.HandleMenuMsg(msg, wparam, lparam);
        }
        break;
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

///////////////////////////////////
//TreeItemData function definitions
//

// Constructor.
CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidl)
{
    m_cpidlRel  = cpidl;
    m_cpidlFull = cpidl;
}

// Constructor.
CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder)
{
    m_parentFolder = parentFolder;
    m_cpidlFull     = cpidlParent + cpidlRel;
    m_cpidlRel      = cpidlRel;
}

// Destructor.
CMyTreeView::TreeItemData::~TreeItemData()
{
}
