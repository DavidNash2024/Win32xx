/////////////////////////////
// MyListView.cpp
//

#include "stdafx.h"
#include "ExplorerApp.h"
#include "MyListView.h"
#include "resource.h"

#ifndef HDF_SORTUP
#define HDF_SORTUP              0x0400
#define HDF_SORTDOWN            0x0200
#endif


//////////////////////////////////
// CMyListView function definitions
//

// Constructor.
CMyListView::CMyListView()
{
}

// Destructor.
CMyListView::~CMyListView()
{
}

// Compares param1 and param2. Used to sort items in a list view.
int CALLBACK CMyListView::CompareFunction(LPARAM param1, LPARAM param2, LPARAM pSortViewItems)
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

    switch (pSort->m_column)
    {
    case 0:  // Sort by file name using IShellFolder::CompareIDs.
    {
        HRESULT result = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());
        if (FAILED(result))
            break;

        short value = (short)SCODE_CODE(GetScode(result));
        compare = pSort->m_isSortDown ? value : -value;
        break;
    }
    case 1:  // Sort by file size.
    {
        if (pItem1->m_fileSize > pItem2->m_fileSize)
            compare = pSort->m_isSortDown ? 1 : -1;

        if (pItem1->m_fileSize < pItem2->m_fileSize)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (pItem1->m_isFolder && !pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (!pItem1->m_isFolder && pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? 1 : -1;

        break;
    }
    case 2:  // Sort by file type.
    {
        if (pItem1->m_fileType > pItem2->m_fileType)
            compare = pSort->m_isSortDown ? 1 : -1;

        if (pItem1->m_fileType < pItem2->m_fileType)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (pItem1->m_isFolder && !pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (!pItem1->m_isFolder && pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? 1 : -1;

        break;
    }
    case 3:  // Sort by modified time
    {
        ULONGLONG t1 = FileTimeToULL(pItem1->m_fileTime);
        ULONGLONG t2 = FileTimeToULL(pItem2->m_fileTime);

        if (t1 > t2)
            compare = pSort->m_isSortDown ? 1 : -1;

        if (t1 < t2)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (pItem1->m_isFolder && !pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? -1 : 1;

        if (!pItem1->m_isFolder && pItem2->m_isFolder)
            compare = pSort->m_isSortDown ? 1 : -1;

        break;
    }
    }

    return compare;
}

// Displays the contents of the specified folder.
void CMyListView::DisplayFolder(CShellFolder& parentFolder, Cpidl& cpidlFull, Cpidl& cpidlRel)
{
    m_cpidlCurFull = cpidlFull;
    if(parentFolder.GetIShellFolder())
        parentFolder.GetSubFolder(cpidlRel, m_csfCurFolder);
    else
        m_csfCurFolder.DesktopFolder();

    DoDisplay();
}

// Initiates the popup menu in response to a right mouse click on the list-view background.
void CMyListView::DoBackgroundMenu(CPoint& point)
{
    HRESULT result;
    if(m_csfCurFolder.GetIShellFolder())
    {
        CContextMenu ccm;
        result = m_csfCurFolder.GetContextMenu(*this, ccm);

        if(SUCCEEDED(result))
        {
            CMenu popup;
            popup.CreatePopupMenu();
            if(popup.GetHandle())
            {
                int   i = 0;
                UINT  idCmdFirst = 0;
                UINT  idCmd;

                // Find the largest ID in the menu.
                while((idCmd = popup.GetMenuItemID(i)) != static_cast<UINT>(-1))
                {
                    if(idCmd > idCmdFirst)
                        idCmdFirst = idCmd;
                    i++;
                }

                result = ccm.QueryContextMenu(popup, 0, ++idCmdFirst, static_cast<UINT>(-1), CMF_NORMAL | CMF_EXPLORE);

                if(SUCCEEDED(result))
                {
                    ccm.GetContextMenu2(m_ccm2);

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
                        ccm.Invoke(cmi);

                        // The operation performed by the context menu may have
                        // changed the contents of the folder, so do a refresh.
                        DoDisplay();
                    }

                    m_ccm2.Release();
                }
            }
        }
    }
}

// Initiates the popup menu in response to a right mouse click on the list-view.
// Calls either DoItemMenu or DoBackgroundMenu.
void CMyListView::DoContextMenu(CPoint& point)
{
    LVHITTESTINFO  lvhti;
    lvhti.pt = point;
    ScreenToClient(lvhti.pt);
    lvhti.flags = LVHT_NOWHERE;
    HitTest(lvhti);

    if(LVHT_ONITEM & lvhti.flags)
    {
        // Get the selected items.
        UINT  items = GetSelectedCount();
        std::vector<int> vItems(items, 0);
        int* pItemArray = &vItems.front();

        if(pItemArray)
        {
            UINT  i;
            int   curItem;

            // Put the item clicked on first in the list.
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

// Performs the default action on the list-view item.
// Typically that would open a folder or run a file.
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
            folder.DesktopFolder();

        if(folder.GetIShellFolder())
        {
            LPCITEMIDLIST pidl = pInfo->GetRelCpidl();
            result = folder.GetContextMenu(*this, 1, &pidl, ccm);

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
                            folder.GetAttributes(1, pInfo->GetRelCpidl(), ulAttr);

                            if ((ulAttr & SFGAO_HASSUBFOLDER) || (ulAttr &SFGAO_FOLDER))
                            {
                                CMainFrame& mainFrame = GetExplorerApp()->GetMainFrame();
                                mainFrame.GetTreeView().SelectFromListView(pInfo->GetFullCpidl());
                            }
                            else
                            {
                                CMINVOKECOMMANDINFO  cmi;
                                ZeroMemory(&cmi, sizeof(CMINVOKECOMMANDINFO));
                                cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
                                cmi.hwnd = GetParent();
                                cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
                                cmi.nShow = SW_SHOWNORMAL;
                                ccm.Invoke(cmi);
                            }
                        }
                    }
                }
            }
        }
    }
}

// Enumerates the items in the currently selected folder and displays them.
void CMyListView::DoDisplay()
{
    DeleteAllItems();
    m_pItems.clear();

    if(m_csfCurFolder.GetIShellFolder())
    {
        HCURSOR  hCur = ::LoadCursor(0, IDC_WAIT);
        hCur = ::SetCursor(hCur);

        // Turn redrawing off in the ListView.
        // This will speed things up as we add items.
        SetRedraw(FALSE);

        EnumObjects(m_csfCurFolder, m_cpidlCurFull);
        SortViewItems sort(0, TRUE);
        SortItems(CompareFunction, (LPARAM)&sort);

        // Turn redrawing back on.
        SetRedraw(TRUE);
        ::SetCursor(hCur);
    }
}

// Initiates the popup menu in response to a right mouse click on a list-view item.
void CMyListView::DoItemMenu(LPINT pItems, UINT items, CPoint& point)
{
    std::vector<LPCITEMIDLIST> vpidl(items);
    LPCITEMIDLIST* pidlArray = &vpidl.front();

    for(UINT i = 0; i < items; ++i)
    {
        LVITEM lvItem;
        ZeroMemory(&lvItem, sizeof(LVITEM));
        lvItem.mask = LVIF_PARAM;
        lvItem.iItem = pItems[i];
        if(GetItem(lvItem))
        {
            ListItemData*  pInfo = reinterpret_cast<ListItemData*>(lvItem.lParam);
            pidlArray[i] = pInfo->GetRelCpidl().GetPidl();
        }
    }

    if(pidlArray[0])
    {
        HRESULT        result;
        CContextMenu ccm;

        if(m_csfCurFolder.GetIShellFolder())
        {
            result = m_csfCurFolder.GetContextMenu(*this, items, pidlArray, ccm);

            if(SUCCEEDED(result))
            {
                CMenu Popup;
                Popup.CreatePopupMenu();
                if(Popup.GetHandle())
                {
                    result = ccm.QueryContextMenu(Popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
                    if(SUCCEEDED(result))
                    {
                        ccm.GetContextMenu2(m_ccm2);
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
                            ccm.Invoke(cmi);

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

// Enumerates the items in the specified folder, and inserts each item into
// the list-view. The LVITEM param parameter holds a pointer to the
// ListItemData.
void CMyListView::EnumObjects(CShellFolder& folder, Cpidl& cpidlParent)
{
    CEnumIDList list;

    int flags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
    if ( GetExplorerApp()->GetMainFrame().GetShowHidden() )
        flags |= SHCONTF_INCLUDEHIDDEN;

    if(SUCCEEDED(folder.GetEnumIDList(0, flags, list)))
    {
        ULONG fetched = 1;
        Cpidl cpidlRel;

        // Enumerate the item's PIDLs.
        while(S_OK == (list.Next(1, cpidlRel, fetched)) && fetched)
        {
            try
            {
                LVITEM lvItem;
                ZeroMemory(&lvItem, sizeof(lvItem));

                // Fill in the TV_ITEM structure for this item.
                lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

                // Store a pointer to the ListItemData in the lParam and m_pItems.
                ListItemDataPtr pItem(new ListItemData(cpidlParent, cpidlRel, folder));
                lvItem.lParam = reinterpret_cast<LPARAM>(pItem.get());

                TCHAR szFileName[MAX_PATH];
                GetFullFileName(pItem->GetFullCpidl().GetPidl(), szFileName);

                ULONG attr = SFGAO_CANDELETE | SFGAO_FOLDER;
                pItem->GetParentFolder().GetAttributes(1, pItem->GetRelCpidl(), attr);
                pItem->m_isFolder = (attr & SFGAO_FOLDER) != 0;

                // Open the existing file.
                CFile file;
                UINT openFlags = OPEN_EXISTING | CFile::shareDenyWrite;
                DWORD attributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS;
                if (attr & SFGAO_CANDELETE)
                {
                    file.Open(szFileName, openFlags, attributes);
                }

                // Retrieve the file size.
                DWORD fileSizeHi;
                DWORD fileSizeLo = ::GetFileSize(file, &fileSizeHi);
                ULONGLONG fileSize = ((ULONGLONG)fileSizeHi) << 32 | fileSizeLo;
                pItem->m_fileSize = fileSize;

                // Retrieve the file type.
                SHFILEINFO sfi;
                ZeroMemory(&sfi, sizeof(SHFILEINFO));
                if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_TYPENAME))
                {
                    pItem->m_fileType = sfi.szTypeName;
                }

                // Retrieve the modified file time for the file.
                FILETIME modified;
                ::GetFileTime(file, NULL, NULL, &modified);
                pItem->m_fileTime = modified;

                // m_pItems is a vector of smart pointers. The memory allocated by
                // new is automatically deleted when the vector goes out of scope.
                m_pItems.push_back(pItem);

                // Text and images are done on a callback basis.
                lvItem.pszText = LPSTR_TEXTCALLBACK;
                lvItem.iImage = I_IMAGECALLBACK;

                // Determine if the item's icon characteristics
                attr = SFGAO_SHARE | SFGAO_LINK | SFGAO_GHOSTED;
                folder.GetAttributes(1, cpidlRel, attr);

                if (attr & SFGAO_SHARE)
                {
                    lvItem.mask |= LVIF_STATE;
                    lvItem.stateMask |= LVIS_OVERLAYMASK;
                    lvItem.state |= INDEXTOOVERLAYMASK(1); // 1 is the index for the shared overlay image
                }

                if (attr & SFGAO_LINK)
                {
                    lvItem.mask |= LVIF_STATE;
                    lvItem.stateMask |= LVIS_OVERLAYMASK;
                    lvItem.state |= INDEXTOOVERLAYMASK(2); // 2 is the index for the link overlay image
                }

                if (attr & SFGAO_GHOSTED)
                {
                    lvItem.mask |= LVIF_STATE;
                    lvItem.stateMask |= LVIS_CUT;
                    lvItem.state |= LVIS_CUT;
                }

                InsertItem(lvItem);
                fetched = 0;
            }

            // Some enumerated IShellFolder objects aren't actually files.
            // Discard any file exceptions.
            catch (const CFileException&)
            {
            }
        }

        // Sort by the first column, sorting down.
        SortColumn(0, TRUE);
    }
}

// Converts a filetime to ULONGLONG.
ULONGLONG CMyListView::FileTimeToULL(FILETIME ft)
{
    return static_cast<ULONGLONG>(ft.dwHighDateTime) << 32 | ft.dwLowDateTime;
}

// Retrieves the file's size and stores the text in string.
BOOL CMyListView::GetFileSizeText(ULONGLONG fileSize, LPTSTR string)
{
    // Convert the fileSize to a string using Locale information.
    CString preFormat;
    preFormat.Format(_T("%d"), ((1023 + fileSize) >> 10));
    CString postFormat;
    const int maxSize = 31;
    ::GetNumberFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, preFormat, NULL, postFormat.GetBuffer(maxSize), maxSize);
    postFormat.ReleaseBuffer();

    // Get our decimal point character from Locale information.
    int buffLen = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0 );
    assert(buffLen > 0);
    CString decimal;
    ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, decimal.GetBuffer(buffLen), buffLen);
    decimal.ReleaseBuffer();

    // Truncate at the "decimal" point.
    int pos = postFormat.Find(decimal);
    if (pos > 0)
        postFormat = postFormat.Left(pos);

    postFormat += _T(" KB");
    StrCopy(string, postFormat, maxSize);
    return TRUE;
}

// Retrieves the file's last write time and stores the text in string.
BOOL CMyListView::GetLastWriteTime(FILETIME modified, LPTSTR string)
{
    // Convert the last-write time to local time.
    SYSTEMTIME localSysTime;
    FILETIME localFileTime;
    ::FileTimeToLocalFileTime(&modified, &localFileTime);
    ::FileTimeToSystemTime(&localFileTime, &localSysTime);

    // Build a string showing the date and time with regional settings.
    const int maxChars = 32;
    TCHAR time[maxChars];
    TCHAR date[maxChars];
    ::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &localSysTime, NULL, date, maxChars-1);
    ::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &localSysTime, NULL, time, maxChars-1);

    StrCopy(string, date, maxChars);
    ::lstrcat(string, _T(" "));
    ::lstrcat(string, time);

    return TRUE;
}

// Called when the window handle (HWND) is attached to CMyListView.
void CMyListView::OnAttach()
{
    // Set the image lists.
    SetImageLists();

    // Set up the columns for report mode.
    LVCOLUMN lvc;
    ZeroMemory(&lvc, sizeof(LVCOLUMN));
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    int columns = 4;   // Number of columns
    int colSizes[4] = {150, 70, 100, 120}; // Width of columns in pixels

    for (int i = 0; i < columns; i++)
    {
        CString str = LoadString(IDS_COLUMN1 + i);
        lvc.iSubItem = i;
        lvc.pszText = const_cast<LPTSTR>(str.c_str());
        lvc.cx = colSizes[i];

        if (i == 1) lvc.fmt = LVCFMT_RIGHT; // right-aligned column
        else lvc.fmt = LVCFMT_LEFT;         // left-aligned column

        InsertColumn(i, lvc);
    }

    // Set initial the view style as report
    ViewReport();
}

// Called when the window is destroyed.
void CMyListView::OnDestroy()
{
    // Cleanup required by Win2000
    m_pItems.clear();
    m_csfCurFolder.Release();
}

// Called when a list view column is clicked.
LRESULT CMyListView::OnLVColumnClick(LPNMITEMACTIVATE pnmitem)
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
// Updates the list view item with the relevant file information.
LRESULT CMyListView::OnLVNDispInfo(NMLVDISPINFO* pdi)
{
    ListItemData*   pItem = reinterpret_cast<ListItemData*>(pdi->item.lParam);

    try
    {
        // Add text if available.
        if (pdi->item.mask & LVIF_TEXT)
        {
            TCHAR szFileName[MAX_PATH];
            GetFullFileName(pItem->GetFullCpidl().GetPidl(), szFileName);

            ULONG attr = SFGAO_CANDELETE | SFGAO_FOLDER;
            pItem->GetParentFolder().GetAttributes(1, pItem->GetRelCpidl(), attr);

            // Open the existing file.
            CFile file;
            UINT openFlags = OPEN_EXISTING | CFile::shareDenyWrite;
            DWORD attributes = FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS;
            if (attr & SFGAO_CANDELETE)
            {
                file.Open(szFileName, openFlags, attributes);
            }

            const int maxLength = 32;
            TCHAR text[maxLength];

            switch (pdi->item.iSubItem)
            {
            case 0:  // Name
            {
                SHFILEINFO sfi;
                ZeroMemory(&sfi, sizeof(sfi));
                // Get the display name of the item.
                if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
                    StrCopy(pdi->item.pszText, sfi.szDisplayName, pdi->item.cchTextMax);
            }
            break;
            case 1: // Size
            {
                TCHAR szSize[maxLength];

                // Report the size files and not folders.
                if (~attr & SFGAO_FOLDER)
                {
                    // Retrieve the file size.
                    GetFileSizeText(pItem->m_fileSize, szSize);
                    StrCopy(pdi->item.pszText, szSize, maxLength);
                }
                else
                    StrCopy(pdi->item.pszText, _T(""), 1);
            }
            break;
            case 2: // Type
            {
                SHFILEINFO sfi;
                ZeroMemory(&sfi, sizeof(SHFILEINFO));
                if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_TYPENAME))
                {
                    StrCopy(pdi->item.pszText, sfi.szTypeName, pdi->item.cchTextMax);
                }
            }
            break;
            case 3: // Modified
            {
                // Retrieve the modified file time for the file.
                if (pItem->m_fileTime.dwHighDateTime != 0 && pItem->m_fileTime.dwLowDateTime != 0)
                {
                    GetLastWriteTime(pItem->m_fileTime, text);
                    StrCopy(pdi->item.pszText, text, maxLength);
                }
                else
                    StrCopy(pdi->item.pszText, _T(""), 1);
            }
            break;
            }
        }

        // Add the unselected image.
        if (pdi->item.mask & LVIF_IMAGE)
        {
            SHFILEINFO sfi;
            ZeroMemory(&sfi, sizeof(SHFILEINFO));

            // Get the unselected image for this item.
            if (pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
                pdi->item.iImage = sfi.iIcon;
        }
    }

    // Some enumerated IShellFolder objects aren't actually files.
    // Discard any file exceptions.
    catch (const CFileException&)
    {}

    return 0;
}

// Respond to a right mouse click on the window.
LRESULT CMyListView::OnNMRClick(LPNMHDR)
{
    CPoint ptScreen;
    ::GetCursorPos(&ptScreen);
    DoContextMenu(ptScreen);

    return 0;
}

// Called when the list-view has focus and the Enter key is pressed.
LRESULT CMyListView::OnNMReturn(LPNMHDR)
{
    // Get the item that has the focus.
    int item = GetNextItem(-1, LVNI_FOCUSED);

    if (item != -1)
        DoDefault(item);

    return 0;
}

// Process notification messages (WM_NOTIFY) reflected back from the parent window.
LRESULT CMyListView::OnNotifyReflect(WPARAM, LPARAM lparam)
{
    LPNMHDR pHeader = reinterpret_cast<LPNMHDR>(lparam);
    LPNMITEMACTIVATE pnmitem = reinterpret_cast<LPNMITEMACTIVATE>(lparam);
    NMLVDISPINFO* pDispInfo = reinterpret_cast<NMLVDISPINFO*>(lparam);
    switch(pHeader->code)
    {
    case LVN_COLUMNCLICK:   return OnLVColumnClick(pnmitem);
    case LVN_GETDISPINFO:   return OnLVNDispInfo(pDispInfo);
    case NM_DBLCLK:         return OnNMReturn(pHeader);
    case NM_RCLICK:         return OnNMRClick(pHeader);
    case NM_RETURN:         return OnNMReturn(pHeader);
    }
    return 0;
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMyListView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE |
            LVS_ICON | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
    cs.dwExStyle = WS_EX_CLIENTEDGE;
}

// Sets the up and down sort arrows in the listview's header.
BOOL CMyListView::SetHeaderSortImage(int  columnIndex, int showArrow)
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

// Sets the image lists for the list-view control.
void CMyListView::SetImageLists()
{
    SHFILEINFO  sfi;
    ZeroMemory(&sfi, sizeof(sfi));

    // Get the system image list
    HIMAGELIST hLargeImages = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0, &sfi,
                                sizeof(SHFILEINFO), SHGFI_SYSICONINDEX));

    HIMAGELIST hSmallImages = reinterpret_cast<HIMAGELIST>(::SHGetFileInfo(_T("C:\\"), 0, &sfi,
                                sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON));

    SetImageList(hLargeImages, LVSIL_NORMAL);
    SetImageList(hSmallImages, LVSIL_SMALL);
}

// Called when the user clicks on a column in the listview's header.
void CMyListView::SortColumn(int column, bool isSortDown)
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
    if (GetSelectedCount() > 0)
        SetItemState(GetSelectionMark(), LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
    else
        SetItemState(0, LVIS_FOCUSED | LVIS_SELECTED, 0x000F);
}

// Set the view-list mode to large icons.
void CMyListView::ViewLargeIcons()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_ICON );
}

// Set the view-list mode to small icons.
void CMyListView::ViewSmallIcons()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_SMALLICON);
}

// Set the view-list mode to list.
void CMyListView::ViewList()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_LIST);
}

// Set the view-list mode to report.
void CMyListView::ViewReport()
{
    DWORD dwStyle = GetStyle();
    SetStyle((dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);
}

// Process the list-view's window messages.
LRESULT CMyListView::WndProc(UINT msg, WPARAM wparam, LPARAM lparam)
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
            // Add features implemented via IContextMenu2 such as 'New'.
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
// ListItemData function definitions
// ListItemData is a nested class of CMyListView.
//

// Constructor.
CMyListView::ListItemData::ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& parentFolder)
{
    m_parentFolder = parentFolder;
    m_cpidlFull    = cpidlParent + cpidlRel;
    m_cpidlRel     = cpidlRel;

    m_fileSize = 0;
    ZeroMemory(&m_fileTime, sizeof(m_fileTime));
    m_isFolder = false;
}

// Destructor.
CMyListView::ListItemData::~ListItemData()
{
}

