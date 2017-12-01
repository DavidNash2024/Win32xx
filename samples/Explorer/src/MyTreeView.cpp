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

int CALLBACK CMyTreeView::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
    UNREFERENCED_PARAMETER(lParamSort);
    TreeItemData* pItem1 = reinterpret_cast<TreeItemData*>(lParam1);
    TreeItemData* pItem2 = reinterpret_cast<TreeItemData*>(lParam2);
    HRESULT hr;

    hr = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());
    if(FAILED(hr))
        return 0;

    return (short)SCODE_CODE(GetScode(hr));
}

void CMyTreeView::DeleteItems()
{
    m_pItems.clear();
}

void CMyTreeView::DoContextMenu(CPoint& ptScreen)
{
    TVHITTESTINFO  tvhti;
    tvhti.pt = ptScreen;
    ScreenToClient(tvhti.pt);
    tvhti.flags = LVHT_NOWHERE;
    HitTest(tvhti);

    if(TVHT_ONITEM & tvhti.flags)
        DoItemMenu(tvhti.hItem , ptScreen);
}

void CMyTreeView::DoItemMenu(HTREEITEM hItem, CPoint& ptScreen)
{
    TVITEM tvItem;
    ZeroMemory(&tvItem, sizeof(TVITEM));
    tvItem.mask = TVIF_PARAM;
    tvItem.hItem = hItem;

    if(GetItem(tvItem))
    {
        HRESULT        hr;
        TreeItemData*  pInfo = reinterpret_cast<TreeItemData*>(tvItem.lParam);
        CContextMenu cm;
        CShellFolder sf;

        if (pInfo->GetParentFolder().GetIShellFolder() != NULL)
            sf = pInfo->GetParentFolder();
        else
            sf.SHGetDesktopFolder();

        if(sf.GetIShellFolder())
        {
            hr = sf.GetUIObjectOf(*this, 1, &(pInfo->GetRelCpidl()), IID_IContextMenu, 0, cm);

            if(SUCCEEDED(hr))
            {
                CMenu Popup;
                Popup.CreatePopupMenu();
                if(Popup.GetHandle())
                {
                    hr = cm.QueryContextMenu(Popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);

                    if(SUCCEEDED(hr))
                    {
                        cm.QueryInterface(IID_IContextMenu2, m_ccm2);

                        UINT idCmd = Popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON ,
                            ptScreen.x, ptScreen.y, *this, NULL);

                        //A Treeview control sometimes requires this to end the
                        // TrackPopupMenu properly
                        GetParent().PostMessage(WM_CANCELMODE, 0, 0);

                        if(idCmd)
                        {
                            CMINVOKECOMMANDINFO  cmi;
                            ZeroMemory(&cmi, sizeof(CMINVOKECOMMANDINFO));
                            cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
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

    CPoint ptScreen = GetCursorPos();
    DoContextMenu(ptScreen);

    return 0L;
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
            ::lstrcpyn(pDI->item.pszText, sfi.szDisplayName, pDI->item.cchTextMax -1);
    }

    //do we need to supply the unselected image?
    if(pDI->item.mask & TVIF_IMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        //get the unselected image for this item
        if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
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

    return 0L;
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

    return 0L;
}

LRESULT CMyTreeView::OnTVNSelChanged(LPNMTREEVIEW pNMTV)
{
    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(pNMTV->itemNew.lParam);

    CMyListView& LeftView = GetExplorerApp().GetMainFrame().GetListView();
    LeftView.DisplayFolder(pItem->GetParentFolder(), pItem->GetFullCpidl(), pItem->GetRelCpidl());

    return 0L;
}

LRESULT CMyTreeView::OnNotifyReflect(WPARAM, LPARAM lParam)
{
    LPNMHDR  lpnmh = (LPNMHDR) lParam;

    switch(lpnmh->code)
    {
    case NM_RCLICK:         return OnNMRClick(lpnmh);
    case TVN_GETDISPINFO:   return OnTVNGetDispInfo((LPNMTVDISPINFO)lParam);
    case TVN_ITEMEXPANDING: return OnTVNExpanding((LPNMTREEVIEW)lParam);
    case TVN_SELCHANGED:    return OnTVNSelChanged((LPNMTREEVIEW)lParam);
    }

    return 0;
}

void CMyTreeView::EnumObjects(HTREEITEM hParentItem, CShellFolder& cParentFolder, Cpidl& cpidlParent)
{
    CEnumIDList cEnum;
    if(SUCCEEDED(cParentFolder.EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, cEnum)))
    {
        ULONG ulFetched = 1;
        Cpidl cpidlRel;

        //enumerate the item's PIDLs
        while(S_OK == (cEnum.Next(1, cpidlRel, ulFetched)) && ulFetched)
        {
            TVITEM         tvItem;
            TVINSERTSTRUCT tvInsert;
            ULONG          ulAttribs;
            ZeroMemory(&tvItem, sizeof(TVITEM));            
            ZeroMemory(&tvInsert, sizeof(TVINSERTSTRUCT));

            //fill in the TV_ITEM structure for this item
            tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

            //Store a pointer to the TreeItemData in the lParam and m_pItems
            TreeItemData* pItem = new TreeItemData(cpidlParent, cpidlRel, cParentFolder);
            tvItem.lParam = reinterpret_cast<LPARAM>(pItem);
            m_pItems.push_back(pItem);

            //text and images are done on a callback basis
            tvItem.pszText = LPSTR_TEXTCALLBACK;
            tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;

            //determine if the item has children or shared
            ulAttribs = SFGAO_HASSUBFOLDER | SFGAO_FOLDER | SFGAO_SHARE;
            cParentFolder.GetAttributesOf(1, cpidlRel, ulAttribs);
            tvItem.cChildren = (ulAttribs & SFGAO_HASSUBFOLDER);

            //determine if the item is shared
            if(ulAttribs & SFGAO_SHARE)
            {
                tvItem.mask |= TVIF_STATE;
                tvItem.stateMask |= TVIS_OVERLAYMASK;
                tvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
            }

            //fill in the TV_INSERTSTRUCT structure for this item
            tvInsert.item = tvItem;
            tvInsert.hInsertAfter = TVI_LAST;
            tvInsert.hParent = hParentItem;

            InsertItem(tvInsert);
            ulFetched = 0;
        }
    }
}

BOOL CMyTreeView::GetChildItems(HTREEITEM hParentItem)
{
    TVITEM tvItem;
    ZeroMemory(&tvItem, sizeof(TVITEM));
    tvItem.mask = TVIF_PARAM;
    tvItem.hItem = hParentItem;
    if (!GetItem(tvItem))
        return FALSE;

    //change the cursor
    HCURSOR hCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

    //turn redawing off in the TreeView. This will speed things up as we add items
    SendMessage(WM_SETREDRAW, FALSE, 0);

    TreeItemData* pItem = reinterpret_cast<TreeItemData*>(tvItem.lParam);
    CShellFolder cParentFolder;
    HRESULT        hr;

    //if the parent folder is NULL, then we are at the root of the namespace, so the parent of this item is the desktop folder
    if(!pItem->GetParentFolder().GetIShellFolder())
        hr = cParentFolder.SHGetDesktopFolder();

    //otherwise we need to get the IShellFolder for this item
    else
        hr = pItem->GetParentFolder().BindToObject(pItem->GetRelCpidl(), NULL, IID_IShellFolder, cParentFolder);

    if(FAILED(hr))
        return FALSE;

    EnumObjects(hParentItem, cParentFolder, pItem->GetFullCpidl());

    //sort the new items
    TV_SORTCB tvSort;
    tvSort.hParent = hParentItem;
    tvSort.lpfnCompare = CompareProc;
    tvSort.lParam = 0;
    SortChildrenCB(tvSort, 0);

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
        TV_ITEM           tvItem;
        TV_INSERTSTRUCT   tvInsert;
        HTREEITEM         hParentItem;
        ZeroMemory(&tvItem, sizeof(TV_ITEM));
        ZeroMemory(&tvInsert, sizeof(TV_INSERTSTRUCT));             

        //fill in the TVITEM structure for this item
        tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

        //Store a pointer to the TreeItemData in the lParam and m_pItems
        TreeItemData* pItem = new TreeItemData(cpidlDesk);
        tvItem.lParam = reinterpret_cast<LPARAM>(pItem);
        m_pItems.push_back(pItem);

        //text and images are done on a callback basis
        tvItem.pszText = LPSTR_TEXTCALLBACK;
        tvItem.iImage = tvItem.iSelectedImage = I_IMAGECALLBACK;

        //assume the desktop has children
        tvItem.cChildren = TRUE;

        //fill in the TV_INSERTSTRUCT structure for this item
        tvInsert.item = tvItem;
        tvInsert.hInsertAfter = TVI_LAST;
        tvInsert.hParent = TVI_ROOT;

        SetFocus();

        //add the item
        hParentItem = InsertItem(tvInsert);

        //go ahead and expand this item
        Expand(hParentItem, TVE_EXPAND);

        //Select the next item
        HTREEITEM hNext = GetNextItem(hParentItem, TVGN_CHILD);

        //Expand this item
        Expand(hNext, TVE_EXPAND);

        //Select this item
        SelectItem(hNext);

        //Scroll this item into view
        SelectSetFirstVisible(hNext);

        return TRUE;
    }

    return FALSE;
}

void CMyTreeView::OnAttach()
{
    // Get a copy of the system image lists
    SHFILEINFO  sfi;

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
    HTREEITEM hItem = GetSelection();

    //Set parent item's has Children flag
    TVITEM tvItem;
    ZeroMemory(&tvItem, sizeof(TVITEM));
    tvItem.mask = TVIF_CHILDREN;
    tvItem.cChildren = 1;
    tvItem.hItem = hItem;
    SetItem(tvItem);

    //Expand the tree item
    Expand(hItem, TVE_EXPAND);

    //First Child
    HTREEITEM hChild = GetChild(hItem);

    while (hChild != NULL)
    {
        //Get the TVITEM structure for this item
        ZeroMemory(&tvItem, sizeof(TVITEM));
        tvItem.mask = TVIF_PARAM;
        tvItem.hItem = hChild;
        if(!GetItem(tvItem))
            return FALSE;

        //Get the TreeItemData pointer from the item's lParam
        TreeItemData* pTD = reinterpret_cast<TreeItemData*>(tvItem.lParam);

        //Compare the pidls
        if (pTD->GetFullCpidl() == cpidlFull)
        {
            SelectItem(hChild);
            return TRUE;
        }

        //Next Sibling
        hChild = GetNextSibling(hChild);
    }
    TRACE("Item NOT found\n");

    return FALSE;
}

LRESULT CMyTreeView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
        case WM_DRAWITEM:
        case WM_MENUCHAR:
        case WM_MEASUREITEM:
        case WM_INITMENUPOPUP:
        {
            if(m_ccm2.GetIContextMenu2())
                m_ccm2.HandleMenuMsg(uMsg, wParam, lParam);
        }
        break;
    }

    return WndProcDefault(uMsg, wParam, lParam);
}

///////////////////////////////////
//TreeItemData function definitions
CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidl)
{
    m_cpidlRel  = cpidl;
    m_cpidlFull = cpidl;
}

CMyTreeView::TreeItemData::TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder)
{
    m_cParentFolder = cParentFolder;
    m_cpidlFull     = cpidlParent + cpidlRel;
    m_cpidlRel      = cpidlRel;
}

CMyTreeView::TreeItemData::~TreeItemData()
{
}
