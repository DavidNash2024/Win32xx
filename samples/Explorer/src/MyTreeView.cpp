//////////////////////////////////////////////////////////
// MyTreeView.cpp
//  Definitions for the CMyTreeView and TreeItemData classes

#include "stdafx.h"
#include "MyTreeView.h"
#include "ExplorerApp.h"
#include "resource.h"

////////////////////////////////
//CMyTreeView function definitions
CMyTreeView::CMyTreeView()
{
}

CMyTreeView::~CMyTreeView()
{
    DeleteItems();
}

int CALLBACK CMyTreeView::CompareProc(LPARAM param1, LPARAM param2, LPARAM paramSort)
{
    UNREFERENCED_PARAMETER(paramSort);
    TreeItemData* pItem1 = reinterpret_cast<TreeItemData*>(param1);
    TreeItemData* pItem2 = reinterpret_cast<TreeItemData*>(param2);
    HRESULT result;

    result = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());
    if(FAILED(result))
        return 0;

    return (short)SCODE_CODE(GetScode(result));
}

void CMyTreeView::DeleteItems()
{
    m_pItems.clear();
}

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
            sf.SHGetDesktopFolder();

        if(sf.GetIShellFolder())
        {
            LPCITEMIDLIST pidl = pInfo->GetRelCpidl();
            result = sf.GetUIObjectOf(*this, 1, &pidl, IID_IContextMenu, cm);

            if(SUCCEEDED(result))
            {
                CMenu popup;
                popup.CreatePopupMenu();
                if(popup.GetHandle())
                {
                    result = cm.QueryContextMenu(popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);

                    if(SUCCEEDED(result))
                    {
                        cm.QueryInterface(IID_IContextMenu2, m_ccm2);

                        UINT idCmd = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON ,
                            point.x, point.y, *this, NULL);

                        //A Treeview control sometimes requires this to end the
                        // TrackPopupMenu properly
                        GetParent().PostMessage(WM_CANCELMODE, 0, 0);

                        if(idCmd)
                        {
                            CMINVOKECOMMANDINFO  cmi;
                            ZeroMemory(&cmi, sizeof(cmi));
                            cmi.cbSize = sizeof(cmi);
                            cmi.hwnd = GetHwnd();
                            cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                            cmi.nShow = SW_SHOWNORMAL;
                            cm.InvokeCommand(cmi);
                        }

                        m_ccm2.Release();
                    }
                }
            }
        }
    }
}

LRESULT CMyTreeView::OnNMRClick(LPNMHDR pNMHDR)
{
    UNREFERENCED_PARAMETER(pNMHDR);

    CPoint pos = GetCursorPos();
    DoContextMenu(pos);

    return 0;
}

LRESULT CMyTreeView::OnTVNGetDispInfo(LPNMTVDISPINFO pDI)
{
    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(pDI->item.lParam);

    //do we need to supply the text?
    if(pDI->item.mask & TVIF_TEXT)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        //get the display name of the item
        if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
            StrCopy(pDI->item.pszText, sfi.szDisplayName, pDI->item.cchTextMax);
    }

    //do we need to supply the unselected image?
    if(pDI->item.mask & TVIF_IMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        //get the unselected image for this item
        if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
            pDI->item.iImage = sfi.iIcon;
    }

    //do we need to supply the selected image?
    if(pDI->item.mask & TVIF_SELECTEDIMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        //get the selected image for this item
        if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON))
            pDI->item.iSelectedImage = sfi.iIcon;
    }

    return 0;
}


LRESULT CMyTreeView::OnTVNExpanding(LPNMTREEVIEW pNMTV)
{
    switch(pNMTV->action)
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

LRESULT CMyTreeView::OnTVNSelChanged(LPNMTREEVIEW pNMTV)
{
    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(pNMTV->itemNew.lParam);

    CMyListView& LeftView = GetExplorerApp()->GetMainFrame().GetListView();
    LeftView.DisplayFolder(pItem->GetParentFolder(), pItem->GetFullCpidl(), pItem->GetRelCpidl());

    return 0;
}

LRESULT CMyTreeView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR  lpnmh = (LPNMHDR)lparam;

    switch(lpnmh->code)
    {
    case NM_RCLICK:         return OnNMRClick(lpnmh);
    case TVN_GETDISPINFO:   return OnTVNGetDispInfo((LPNMTVDISPINFO)lparam);
    case TVN_ITEMEXPANDING: return OnTVNExpanding((LPNMTREEVIEW)lparam);
    case TVN_SELCHANGED:    return OnTVNSelChanged((LPNMTREEVIEW)lparam);
    }

    return 0;
}

void CMyTreeView::EnumObjects(HTREEITEM parentItem, CShellFolder& parentFolder, Cpidl& cpidlParent)
{
    CEnumIDList list;
    if(SUCCEEDED(parentFolder.EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, list)))
    {
        ULONG fetched = 1;
        Cpidl cpidlRel;

        //enumerate the item's PIDLs
        while(S_OK == (list.Next(1, cpidlRel, fetched)) && fetched)
        {
            TVITEM         itemInfo;
            ULONG          attribs;
            ZeroMemory(&itemInfo, sizeof(itemInfo));

            //fill in the TV_ITEM structure for this item
            itemInfo.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

            //Store a pointer to the TreeItemData in the lParam and m_pItems
            TreeItemData* pItem = new TreeItemData(cpidlParent, cpidlRel, parentFolder);
            itemInfo.lParam = reinterpret_cast<LPARAM>(pItem);
            m_pItems.push_back(pItem);

            //text and images are done on a callback basis
            itemInfo.pszText = LPSTR_TEXTCALLBACK;
            itemInfo.iImage = itemInfo.iSelectedImage = I_IMAGECALLBACK;

            //determine if the item has children or shared
            attribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_SHARE;
            parentFolder.GetAttributesOf(1, cpidlRel, attribs);
            itemInfo.cChildren = (attribs & SFGAO_HASSUBFOLDER);

            //determine if the item is shared
            if(attribs & SFGAO_SHARE)
            {
                itemInfo.mask |= TVIF_STATE;
                itemInfo.stateMask |= TVIS_OVERLAYMASK;
                itemInfo.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
            }

            //fill in the TVINSERTSTRUCT structure for this item
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

BOOL CMyTreeView::GetChildItems(HTREEITEM parentItem)
{
    TVITEM itemInfo;
    ZeroMemory(&itemInfo, sizeof(itemInfo));
    itemInfo.mask = TVIF_PARAM;
    itemInfo.hItem = parentItem;
    if (!GetItem(itemInfo))
        return FALSE;

    //change the cursor
    HCURSOR hCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

    //turn redawing off in the TreeView. This will speed things up as we add items
    SendMessage(WM_SETREDRAW, FALSE, 0);

    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(itemInfo.lParam);
    CShellFolder parentFolder;
    HRESULT      result;

    //if the parent folder is NULL, then we are at the root of the namespace, so the parent of this item is the desktop folder
    if(!pItem->GetParentFolder().GetIShellFolder())
        result = parentFolder.SHGetDesktopFolder();

    //otherwise we need to get the IShellFolder for this item
    else
        result = pItem->GetParentFolder().BindToObject(pItem->GetRelCpidl(), NULL, IID_IShellFolder, parentFolder);

    if(FAILED(result))
        return FALSE;

    EnumObjects(parentItem, parentFolder, pItem->GetFullCpidl());

    //sort the new items
    TV_SORTCB sortInfo;
    sortInfo.hParent = parentItem;
    sortInfo.lpfnCompare = CompareProc;
    sortInfo.lParam = 0;
    SortChildrenCB(&sortInfo, 0);

    //turn redawing back on in the TreeView
    SendMessage(WM_SETREDRAW, TRUE, 0);

    UpdateWindow();
    ::SetCursor(hCursor);

    return TRUE;
}

BOOL CMyTreeView::GetRootItems()
{
    DeleteAllItems();

    Cpidl cpidlDesk;
    cpidlDesk.SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP);
    if (cpidlDesk.GetPidl())
    {
        //fill in the TVITEM structure for this item
        TV_ITEM           itemInfo;
        ZeroMemory(&itemInfo, sizeof(itemInfo));
        itemInfo.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

        //Store a pointer to the TreeItemData in the lParam and m_pItems
        TreeItemData* pItem = new TreeItemData(cpidlDesk);
        itemInfo.lParam = reinterpret_cast<LPARAM>(pItem);
        m_pItems.push_back(pItem);

        //text and images are done on a callback basis
        itemInfo.pszText = LPSTR_TEXTCALLBACK;
        itemInfo.iImage = itemInfo.iSelectedImage = I_IMAGECALLBACK;

        //assume the desktop has children
        itemInfo.cChildren = TRUE;

        //fill in the TVINSERTSTRUCT structure for this item
        TVINSERTSTRUCT   insertInfo;
        ZeroMemory(&insertInfo, sizeof(insertInfo));
        insertInfo.item = itemInfo;
        insertInfo.hInsertAfter = TVI_LAST;
        insertInfo.hParent = TVI_ROOT;

        SetFocus();

        //add the item
        HTREEITEM parentItem;
        parentItem = InsertItem(insertInfo);

        //go ahead and expand this item
        Expand(parentItem, TVE_EXPAND);

        //Select the next item
        HTREEITEM nextItem = GetNextItem(parentItem, TVGN_CHILD);

        //Expand this item
        Expand(nextItem, TVE_EXPAND);

        //Select this item
        SelectItem(nextItem);

        //Scroll this item into view
        SelectSetFirstVisible(nextItem);

        return TRUE;
    }

    return FALSE;
}

void CMyTreeView::OnAttach()
{
    // Get a copy of the system image lists
    SHFILEINFO  sfi;
    ZeroMemory(&sfi, sizeof(sfi));

    HIMAGELIST hSmall = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0,
        &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    SetImageList(hSmall, LVSIL_NORMAL);
}

void CMyTreeView::OnDestroy()
{
    m_pItems.clear();
}

void CMyTreeView::PreCreate(CREATESTRUCT& cs)
{
    cs.dwExStyle = WS_EX_CLIENTEDGE;
    cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | TVS_HASLINES |
                    TVS_HASBUTTONS | TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS ;
}

BOOL CMyTreeView::SelectFromListView(Cpidl& cpidlFull)
{
    HTREEITEM item = GetSelection();

    //Set parent item's has Children flag
    TVITEM itemInfo;
    ZeroMemory(&itemInfo, sizeof(itemInfo));
    itemInfo.mask = TVIF_CHILDREN;
    itemInfo.cChildren = 1;
    itemInfo.hItem = item;
    SetItem(itemInfo);

    //Expand the tree item
    Expand(item, TVE_EXPAND);

    //First Child
    HTREEITEM childItem = GetChild(item);

    while (childItem != NULL)
    {
        //Get the TVITEM structure for this item
        ZeroMemory(&itemInfo, sizeof(itemInfo));
        itemInfo.mask = TVIF_PARAM;
        itemInfo.hItem = childItem;
        if(!GetItem(itemInfo))
            return FALSE;

        //Get the TreeItemData pointer from the item's lParam
        TreeItemData* pTD = reinterpret_cast<TreeItemData*>(itemInfo.lParam);

        //Compare the pidls
        if (pTD->GetFullCpidl() == cpidlFull)
        {
            SelectItem(childItem);
            return TRUE;
        }

        //Next Sibling
        childItem = GetNextSibling(childItem);
    }
    TRACE("Item NOT found\n");

    return FALSE;
}

LRESULT CMyTreeView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
        case WM_DRAWITEM:
        case WM_MENUCHAR:
        case WM_MEASUREITEM:
        case WM_INITMENUPOPUP:
        {
            if(m_ccm2.GetIContextMenu2())
                m_ccm2.HandleMenuMsg(msg, wparam, lparam);
        }
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

///////////////////////////////////
//TreeItemData function definitions
CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidl)
{
    m_cpidlRel  = cpidl;
    m_cpidlFull = cpidl;
}

CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder)
{
    m_parentFolder = parentFolder;
    m_cpidlFull     = cpidlParent + cpidlRel;
    m_cpidlRel      = cpidlRel;
}

CMyTreeView::TreeItemData::~TreeItemData()
{
}
