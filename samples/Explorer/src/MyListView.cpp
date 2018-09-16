//////////////////////////////////////////////////////////
// MyListView.cpp
//  Definitions for the CMyListView and ListItemData classes

#include "stdafx.h"
#include "ExplorerApp.h"
#include "MyListView.h"
#include "resource.h"

////////////////////////////////
//CMyListView function definitions
int CALLBACK CMyListView::CompareProc(LPARAM param1, LPARAM param2, LPARAM paramSort)
{
    UNREFERENCED_PARAMETER(paramSort);
    ListItemData*  pItem1 = reinterpret_cast<ListItemData*>(param1);
    ListItemData*  pItem2 = reinterpret_cast<ListItemData*>(param2);

    HRESULT result = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelPidl(), pItem2->GetRelPidl());

    if(FAILED(result))
        return 0;

    return (short)SCODE_CODE(GetScode(result));
}

CMyListView::CMyListView()
{
}

CMyListView::~CMyListView()
{
    DeleteItems();
}

void CMyListView::DeleteItems()
{
    m_pItems.clear();
}

void CMyListView::DisplayFolder(CShellFolder& parentFolder, Cpidl& cpidlFull, Cpidl& cpidlRel)
{
    m_cpidlCurFull = cpidlFull;
    if(parentFolder.GetIShellFolder())
        parentFolder.BindToObject(cpidlRel, NULL, IID_IShellFolder, m_csfCurFolder);
    else
        m_csfCurFolder.SHGetDesktopFolder();

    DoDisplay();
}

void CMyListView::DoBackgroundMenu(CPoint& point)
{
    HRESULT result;
    if(m_csfCurFolder.GetIShellFolder())
    {
        CContextMenu ccm;
        result = m_csfCurFolder.CreateViewObject(GetParent(), IID_IContextMenu, ccm);

        if(SUCCEEDED(result))
        {
            CMenu popup;
            popup.CreatePopupMenu();
            if(popup.GetHandle())
            {
                int   i = 0;
                UINT  idCmdFirst = 0;
                UINT  idCmd;

                //find the largest ID in the menu
                while((idCmd = popup.GetMenuItemID(i)) != static_cast<UINT>(-1))
                {
                    if(idCmd > idCmdFirst)
                        idCmdFirst = idCmd;
                    i++;
                }

                result = ccm.QueryContextMenu(popup, 0, ++idCmdFirst, static_cast<UINT>(-1), CMF_NORMAL | CMF_EXPLORE);

                if(SUCCEEDED(result))
                {
                    ccm.QueryInterface(IID_IContextMenu2, m_ccm2);

                    idCmd = popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
                        point.x, point.y, *this, NULL);

                    if(idCmd)
                    {
                        CMINVOKECOMMANDINFO  cmi;
                        ZeroMemory(&cmi, sizeof(cmi));
                        cmi.cbSize = sizeof(cmi);
                        cmi.hwnd = GetParent();
                        cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - idCmdFirst);
                        cmi.nShow = SW_SHOWNORMAL;
                        ccm.InvokeCommand(cmi);

                        //The operation performed by the context menu may have
                        // changed the contents of the folder, so do a refresh.
                        DoDisplay();
                    }

                    m_ccm2.Release();
                }
            }
        }
    }
}

void CMyListView::DoContextMenu(CPoint& point)
{
    LVHITTESTINFO  lvhti;
    lvhti.pt = point;
    ScreenToClient(lvhti.pt);
    lvhti.flags = LVHT_NOWHERE;
    HitTest(lvhti);

    if(LVHT_ONITEM & lvhti.flags)
    {
        //get the selected items
        UINT  items = GetSelectedCount();
        std::vector<int> vItems(items, 0);
        int* pItemArray = &vItems.front();

        if(pItemArray)
        {
            UINT  i;
            int   curItem;

            //put the item clicked on first in the list
            pItemArray[0] = lvhti.iItem;

            for(i = 1, curItem = -1; i < items; ++i)
            {
                curItem = GetNextItem(curItem, LVNI_SELECTED);
                if(curItem != lvhti.iItem)
                    pItemArray[i] = curItem;
                else
                    --i;
            }

            DoItemMenu(pItemArray, items, point);
        }
    }
    else
        DoBackgroundMenu(point);
}

void CMyListView::DoDefault(int item)
{
    LVITEM   lvItem;
    ZeroMemory(&lvItem, sizeof(LVITEM));
    lvItem.mask = LVIF_PARAM;
    lvItem.iItem = item;

    if(GetItem(lvItem))
    {
        HRESULT        result;
        ListItemData*  pInfo = reinterpret_cast<ListItemData*>(lvItem.lParam);
        CShellFolder   folder;
        CContextMenu ccm;
        folder = pInfo->GetParentFolder();

        if (!folder.GetIShellFolder())
            folder.SHGetDesktopFolder();

        if(folder.GetIShellFolder())
        {
            Cpidl* pCpidl = &(pInfo->GetRelPidl());
            result = folder.GetUIObjectOf(*this, 1, pCpidl, IID_IContextMenu, 0, ccm);

            if(SUCCEEDED(result))
            {
                CMenu popup;
                popup.CreatePopupMenu();
                if(popup.GetHandle())
                {
                    result = ccm.QueryContextMenu(popup, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE);

                    if(SUCCEEDED(result))
                    {
                        UINT idCmd = popup.GetMenuItemID(0);
                        if(idCmd && (idCmd != static_cast<UINT>(-1)))
                        {
                            //determine if the item is a folder
                            ULONG ulAttr = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
                            folder.GetAttributesOf(1, pInfo->GetRelPidl(), ulAttr);

                            if ((ulAttr & SFGAO_HASSUBFOLDER) || (ulAttr &SFGAO_FOLDER))
                            {
                                CMainFrame& mainFrame = GetExplorerApp().GetMainFrame();
                                mainFrame.GetTreeView().SelectFromListView(pInfo->GetFullPidl());
                            }
                            else
                            {
                                CMINVOKECOMMANDINFO  cmi;
                                ZeroMemory(&cmi, sizeof(CMINVOKECOMMANDINFO));
                                cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                                cmi.hwnd = GetParent();
                                cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                                cmi.nShow = SW_SHOWNORMAL;
                                ccm.InvokeCommand(cmi);
                            }
                        }
                    }
                }
            }
        }
    }
}

void CMyListView::DoDisplay()
{
    DeleteAllItems();
    DeleteItems();

    if(m_csfCurFolder.GetIShellFolder())
    {
        HCURSOR  hCur = ::LoadCursor(NULL, IDC_WAIT);
        hCur = ::SetCursor(hCur);

        //turn redawing off in the ListView. This will speed things up as we add items
        SendMessage(WM_SETREDRAW, FALSE, 0);

        EnumObjects(m_csfCurFolder, m_cpidlCurFull);
        SortItems(CompareProc, 0);

        //turn redawing back on
        SendMessage(WM_SETREDRAW, TRUE, 0);
        ::SetCursor(hCur);
    }
}

void CMyListView::DoItemMenu(LPINT pItems, UINT cbItems, CPoint& point)
{
    std::vector<Cpidl> vpidl(cbItems);
    Cpidl* pidlArray = &vpidl.front();

    for(UINT i = 0; i < cbItems; ++i)
    {
        LVITEM lvItem;
        ZeroMemory(&lvItem, sizeof(LVITEM));
        lvItem.mask = LVIF_PARAM;
        lvItem.iItem = pItems[i];
        if(GetItem(lvItem))
        {
            ListItemData*  pInfo = reinterpret_cast<ListItemData*>(lvItem.lParam);
            pidlArray[i] = pInfo->GetRelPidl();
        }
    }

    if(pidlArray[0].GetPidl())
    {
        HRESULT        result;
        CContextMenu ccm;

        if(m_csfCurFolder.GetIShellFolder())
        {
            result = m_csfCurFolder.GetUIObjectOf(*this, cbItems, pidlArray, IID_IContextMenu, 0, ccm);

            if(SUCCEEDED(result))
            {
                CMenu Popup;
                Popup.CreatePopupMenu();
                if(Popup.GetHandle())
                {
                    result = ccm.QueryContextMenu(Popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
                    if(SUCCEEDED(result))
                    {
                        ccm.QueryInterface(IID_IContextMenu2, m_ccm2);
                        UINT  idCmd;
                        idCmd = Popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
                                    point.x, point.y, *this, NULL);

                        if(idCmd)
                        {
                            CMINVOKECOMMANDINFO  cmi;
                            ZeroMemory(&cmi, sizeof(CMINVOKECOMMANDINFO));
                            cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                            cmi.hwnd = GetParent();
                            cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                            cmi.nShow = SW_SHOWNORMAL;
                            ccm.InvokeCommand(cmi);

                        //  The operation performed by the context menu may have changed
                        //  the contents of the folder, so do a refresh.
                            DoDisplay();
                        }

                        m_ccm2.Release();
                    }
                }
            }
        }
    }
}

LRESULT CMyListView::OnNMRClick(LPNMHDR pNMHDR)
{
    UNREFERENCED_PARAMETER(pNMHDR);

    CPoint ptScreen;
    ::GetCursorPos(&ptScreen);
    DoContextMenu(ptScreen);

    return 0;
}

LRESULT CMyListView::OnLVNDispInfo(NMLVDISPINFO* pdi)
{
    ListItemData*   pItem = reinterpret_cast<ListItemData*>(pdi->item.lParam);

    //do we need to supply the text?
    if(pdi->item.mask & LVIF_TEXT)
    {
        TCHAR szFileName[MAX_PATH];
        GetFullFileName(pItem->GetFullPidl().GetPidl(), szFileName);

        ULONG attr = SFGAO_CANDELETE | SFGAO_FOLDER;
        pItem->GetParentFolder().GetAttributesOf(1, pItem->GetRelPidl(), attr);

        HANDLE hFile;

        //A trick to help get a quick response from CreateFile
        if (attr & SFGAO_CANDELETE)
            hFile = ::CreateFile (szFileName, 0, FILE_SHARE_READ, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);
        else
            hFile = ::CreateFile (szFileName, 0, FILE_SHARE_READ, NULL,
                0, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

        TCHAR text[32];
        int nMaxLength = sizeof(text)/sizeof(text[0])-1;

        switch(pdi->item.iSubItem)
        {
        case 0:  //name
            {
                SHFILEINFO sfi;
                ZeroMemory(&sfi, sizeof(sfi));
                //get the display name of the item
                if (pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
                    StrCopy(pdi->item.pszText, sfi.szDisplayName, pdi->item.cchTextMax -1);
            }
            break;
        case 1: //Size
            {
                TCHAR szSize[32];

                //report the size files and not folders
                if ((hFile != INVALID_HANDLE_VALUE)&&(~attr & SFGAO_FOLDER))
                {
                    GetFileSizeText(hFile, szSize);
                    StrCopy(pdi->item.pszText, szSize, nMaxLength -1);
                }
                else
                    StrCopy(pdi->item.pszText, _T(""), 1);
            }
            break;
        case 2: //Type
            {
                SHFILEINFO sfi;
                ZeroMemory(&sfi, sizeof(SHFILEINFO));
                if(pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_TYPENAME))
                    StrCopy(pdi->item.pszText, sfi.szTypeName, pdi->item.cchTextMax -1);
            }
            break;
        case 3: //Modified
            {
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    GetLastWriteTime(hFile, text);
                    StrCopy(pdi->item.pszText, text, nMaxLength -1);
                }
                else
                    StrCopy(pdi->item.pszText, _T(""), 1);
            }
            break;
        }
        if (hFile != INVALID_HANDLE_VALUE)
            ::CloseHandle(hFile);
    }

    //do we need to supply the unselected image?
    if(pdi->item.mask & LVIF_IMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        //get the unselected image for this item
        if(pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
            pdi->item.iImage = sfi.iIcon;
    }

    return 0;
}

LRESULT CMyListView::OnNMReturn(LPNMHDR pNMHDR)
{
    UNREFERENCED_PARAMETER(pNMHDR);

    //get the item that has the focus
    int item = static_cast<int>(SendMessage(LVM_GETNEXTITEM, static_cast<WPARAM>(-1), MAKELPARAM (LVNI_FOCUSED, 0)));

    if(item != -1)
        DoDefault(item);

    return 0;
}

LRESULT CMyListView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR  pNMHDR = (LPNMHDR)lparam;

    switch(pNMHDR->code)
    {
    case NM_RCLICK:         return OnNMRClick(pNMHDR);
    case LVN_GETDISPINFO:   return OnLVNDispInfo(reinterpret_cast<NMLVDISPINFO*>(lparam));
    case NM_DBLCLK:         return OnNMReturn(pNMHDR);
    case NM_RETURN:         return OnNMReturn(pNMHDR);
    }
    return 0;
}

void CMyListView::EnumObjects(CShellFolder& folder, Cpidl& cpidlParent)
{
    CEnumIDList list;

    int flags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
    if ( GetExplorerApp().GetMainFrame().GetShowHidden() )
        flags |= SHCONTF_INCLUDEHIDDEN;

    if(SUCCEEDED(folder.EnumObjects(NULL, flags, list)))
    {
        ULONG fetched = 1;
        Cpidl cpidlRel;

        //enumerate the item's PIDLs
        while(S_OK == (list.Next(1, cpidlRel, fetched)) && fetched)
        {
            LVITEM lvItem;
            ZeroMemory(&lvItem, sizeof(lvItem));
            ULONG  attr;

            //fill in the TV_ITEM structure for this item
            lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

            //Store a pointer to the ListItemData in the lParam and m_pItems
            ListItemData* pItem = new ListItemData(cpidlParent, cpidlRel, folder);
            lvItem.lParam = reinterpret_cast<LPARAM>(pItem);
            m_pItems.push_back(pItem);

            //text and images are done on a callback basis
            lvItem.pszText = LPSTR_TEXTCALLBACK;
            lvItem.iImage = I_IMAGECALLBACK;

            //determine if the item's icon characteristics
            attr = SFGAO_SHARE | SFGAO_LINK | SFGAO_GHOSTED;
            folder.GetAttributesOf(1, cpidlRel, attr);

            if(attr & SFGAO_SHARE)
            {
                lvItem.mask |= LVIF_STATE;
                lvItem.stateMask |= LVIS_OVERLAYMASK;
                lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
            }

            if (attr & SFGAO_LINK)
            {
                lvItem.mask |= LVIF_STATE;
                lvItem.stateMask |= LVIS_OVERLAYMASK;
                lvItem.state |= INDEXTOOVERLAYMASK(2); //2 is the index for the link overlay image
            }

            if(attr & SFGAO_GHOSTED)
            {
                lvItem.mask |= LVIF_STATE;
                lvItem.stateMask |= LVIS_CUT;
                lvItem.state |= LVIS_CUT;
            }

            InsertItem(lvItem);
            fetched = 0;
        }
    }
}

BOOL CMyListView::GetFileSizeText(HANDLE hFile, LPTSTR lpszSize)
{
    DWORD dwFileSizeLo;
    DWORD dwFileSizeHi;
    DWORDLONG ldwSize;
    int nMaxSize = 31;
    CString strPreFormat;
    CString strPostFormat;

    dwFileSizeLo = ::GetFileSize (hFile, &dwFileSizeHi);
    ldwSize = ((DWORDLONG) dwFileSizeHi)<<32 | dwFileSizeLo;
    strPreFormat.Format(_T("%d"), ((1023 + ldwSize)>>10));

    //Convert our number string using Locale information
    ::GetNumberFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, strPreFormat, NULL, strPostFormat.GetBuffer(nMaxSize), nMaxSize);
    strPostFormat.ReleaseBuffer();

    //Get our decimal point character from Locale information
    int nBuffLen = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0 );
    assert(nBuffLen > 0);    
    CString Decimal;
    ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, Decimal.GetBuffer(nBuffLen), nBuffLen);
    Decimal.ReleaseBuffer();

    //Truncate at the "decimal" point
    int nPos = strPostFormat.Find(Decimal);
    if (nPos > 0)
        strPostFormat = strPostFormat.Left(nPos);

    strPostFormat += _T(" KB");
    StrCopy(lpszSize, strPostFormat, nMaxSize);
    return TRUE;
}

BOOL CMyListView::GetLastWriteTime(HANDLE hFile, LPTSTR string)
{
    FILETIME create, access, write;
    SYSTEMTIME stLocal;
    FILETIME ftLocal;
    TCHAR szTime[32];
    TCHAR szDate[32];

    // Retrieve the file times for the file.
    if (!::GetFileTime(hFile, &create, &access, &write))
        return FALSE;

    // Convert the last-write time to local time.
    ::FileTimeToLocalFileTime(&write, &ftLocal);
    ::FileTimeToSystemTime(&ftLocal, &stLocal);

    // Build a string showing the date and time with regional settings.
    ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &stLocal, NULL, szDate, 31);
    ::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &stLocal, NULL, szTime, 31);

    StrCopy(string, szDate, 32);
    ::lstrcat(string, _T(" "));
    ::lstrcat(string, szTime);

    return TRUE;
}

void CMyListView::OnAttach()
{
    //Set the image lists
    SetImageLists();

    //Set up the colmns for report mode
    TCHAR text[256];
    LVCOLUMN lvc;
    ZeroMemory(&lvc, sizeof(LVCOLUMN));
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    int columns = 4;   //Number of columns
    int colSizes[4] = {150, 70, 100, 120}; // width of columns in pixels

    for (int i = 0; i < columns; i++)
    {
        lvc.iSubItem = i;
        lvc.pszText = text;
        lvc.cx = colSizes[i];

        if (1 == i) lvc.fmt = LVCFMT_RIGHT; // right-aligned column
        else lvc.fmt = LVCFMT_LEFT;     //left-aligned column

        ::LoadString(GetApp().GetInstanceHandle(), IDS_COLUMN1 + i,
            text, sizeof(text)/sizeof(text[0]));

        InsertColumn(i, lvc);
    }

    //Set initial the view style as report
    ViewReport();
}

void CMyListView::OnDestroy()
{
    m_pItems.clear();
    m_csfCurFolder.Delete();
}

void CMyListView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE |
            LVS_ICON | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CMyListView::SetImageLists()
{
    SHFILEINFO  sfi;

    // Get the system image list
    HIMAGELIST hLargeImages = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0, &sfi,
                                sizeof(SHFILEINFO), SHGFI_SYSICONINDEX));

    HIMAGELIST hSmallImages = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0, &sfi,
                                sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    SetImageList(hLargeImages, LVSIL_NORMAL);
    SetImageList(hSmallImages, LVSIL_SMALL);
}

void CMyListView::ViewLargeIcons()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_ICON );
}

void CMyListView::ViewSmallIcons()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_SMALLICON);
}

void CMyListView::ViewList()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_LIST);
}

void CMyListView::ViewReport()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);
}

LRESULT CMyListView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
//ListItemData function definitions
CMyListView::ListItemData::ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder)
{
    m_parentFolder = cParentFolder;
    m_cpidlFull     = cpidlParent + cpidlRel;
    m_cpidlRel      = cpidlRel;
}

CMyListView::ListItemData::~ListItemData()
{
}
