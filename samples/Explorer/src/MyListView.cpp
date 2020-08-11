/////////////////////////////
// MyListView.cpp
//

#include "stdafx.h"
#include "ExplorerApp.h"
#include "MyListView.h"
#include "resource.h"

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
    DeleteItems();
}

// Compares param1 and param2. Used to sort items in a list view.
int CALLBACK CMyListView::CompareProc(LPARAM param1, LPARAM param2, LPARAM paramSort)
{
    UNREFERENCED_PARAMETER(paramSort);
    ListItemData* pItem1 = reinterpret_cast<ListItemData*>(param1);
    ListItemData* pItem2 = reinterpret_cast<ListItemData*>(param2);

    HRESULT result = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());

    if (FAILED(result))
        return 0;

    return (short)SCODE_CODE(GetScode(result));
}

// Deletes all the items from the list-view.
void CMyListView::DeleteItems()
{
    m_pItems.clear();
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
    DeleteItems();

    if(m_csfCurFolder.GetIShellFolder())
    {
        HCURSOR  hCur = ::LoadCursor(NULL, IDC_WAIT);
        hCur = ::SetCursor(hCur);

        // Turn redawing off in the ListView. 
        // This will speed things up as we add items.
        SetRedraw(FALSE);

        EnumObjects(m_csfCurFolder, m_cpidlCurFull);
        SortItems(CompareProc, 0);

        // Turn redawing back on.
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

// Respond to a right mouse click on the window.
LRESULT CMyListView::OnNMRClick(LPNMHDR pNMHDR)
{
    UNREFERENCED_PARAMETER(pNMHDR);

    CPoint ptScreen;
    ::GetCursorPos(&ptScreen);
    DoContextMenu(ptScreen);

    return 0;
}

// Called in response to a LVN_GETDISPINFO notification.
// Updates the list view item with the relevant file information.
LRESULT CMyListView::OnLVNDispInfo(NMLVDISPINFO* pdi)
{
    ListItemData*   pItem = reinterpret_cast<ListItemData*>(pdi->item.lParam);

    // Add text if available.
    if(pdi->item.mask & LVIF_TEXT)
    {
        TCHAR szFileName[MAX_PATH];
        GetFullFileName(pItem->GetFullCpidl().GetPidl(), szFileName);

        ULONG attr = SFGAO_CANDELETE | SFGAO_FOLDER;
        pItem->GetParentFolder().GetAttributes(1, pItem->GetRelCpidl(), attr);

        HANDLE hFile = INVALID_HANDLE_VALUE;

        // Retrieve the file handle for an existing file
        if (attr & SFGAO_CANDELETE)
            hFile = ::CreateFile (szFileName, 0, FILE_SHARE_READ, NULL,
                OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

        const int maxLength = 32;
        TCHAR text[maxLength];

        switch(pdi->item.iSubItem)
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

                // Report the size files and not folders
                if ((hFile != INVALID_HANDLE_VALUE)&&(~attr & SFGAO_FOLDER))
                {
                    GetFileSizeText(hFile, szSize);
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
                if(pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_TYPENAME))
                    StrCopy(pdi->item.pszText, sfi.szTypeName, pdi->item.cchTextMax);
            }
            break;
        case 3: // Modified
            {
                if (hFile != INVALID_HANDLE_VALUE)
                {
                    GetLastWriteTime(hFile, text);
                    StrCopy(pdi->item.pszText, text, maxLength);
                }
                else
                    StrCopy(pdi->item.pszText, _T(""), 1);
            }
            break;
        }
        if (hFile != INVALID_HANDLE_VALUE)
            ::CloseHandle(hFile);
    }

    // Add the unselected image.
    if(pdi->item.mask & LVIF_IMAGE)
    {
        SHFILEINFO sfi;
        ZeroMemory(&sfi, sizeof(SHFILEINFO));

        // Get the unselected image for this item
        if(pItem->GetFullCpidl().GetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
            pdi->item.iImage = sfi.iIcon;
    }

    return 0;
}

// Called when the list-view has focus and the Enter key is pressed.
LRESULT CMyListView::OnNMReturn(LPNMHDR pNMHDR)
{
    UNREFERENCED_PARAMETER(pNMHDR);

    // Get the item that has the focus.
    int item = GetNextItem(-1, LVNI_FOCUSED);

    if (item != -1)
        DoDefault(item);

    return 0;
}

// Process notification messages (WM_NOTIFY) reflected back from the parent window.
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

// Enumerates the items in the specified folder, and inerts each item into
// the list-view. The LVITEM param parameter holds a pointer to the 
// ListItemData.
void CMyListView::EnumObjects(CShellFolder& folder, Cpidl& cpidlParent)
{
    CEnumIDList list;

    int flags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
    if ( GetExplorerApp()->GetMainFrame().GetShowHidden() )
        flags |= SHCONTF_INCLUDEHIDDEN;

    if(SUCCEEDED(folder.GetEnumIDList(NULL, flags, list)))
    {
        ULONG fetched = 1;
        Cpidl cpidlRel;

        // Enumerate the item's PIDLs.
        while(S_OK == (list.Next(1, cpidlRel, fetched)) && fetched)
        {
            LVITEM lvItem;
            ZeroMemory(&lvItem, sizeof(lvItem));
            ULONG  attr;

            // Fill in the TV_ITEM structure for this item.
            lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

            // Store a pointer to the ListItemData in the lParam and m_pItems.
            ListItemData* pItem = new ListItemData(cpidlParent, cpidlRel, folder);
            lvItem.lParam = reinterpret_cast<LPARAM>(pItem);
            m_pItems.push_back(pItem);

            // Text and images are done on a callback basis.
            lvItem.pszText = LPSTR_TEXTCALLBACK;
            lvItem.iImage = I_IMAGECALLBACK;

            // Determine if the item's icon characteristics
            attr = SFGAO_SHARE | SFGAO_LINK | SFGAO_GHOSTED;
            folder.GetAttributes(1, cpidlRel, attr);

            if(attr & SFGAO_SHARE)
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

// Retrieves the file's size and stores the text in string.
BOOL CMyListView::GetFileSizeText(HANDLE file, LPTSTR string)
{
    DWORD dwFileSizeLo;
    DWORD dwFileSizeHi;
    DWORDLONG ldwSize;
    int nMaxSize = 31;
    CString strPreFormat;
    CString strPostFormat;

    dwFileSizeLo = ::GetFileSize (file, &dwFileSizeHi);
    ldwSize = ((DWORDLONG) dwFileSizeHi)<<32 | dwFileSizeLo;
    strPreFormat.Format(_T("%d"), ((1023 + ldwSize)>>10));

    // Convert our number string using Locale information.
    ::GetNumberFormat(LOCALE_USER_DEFAULT, LOCALE_NOUSEROVERRIDE, strPreFormat, NULL, strPostFormat.GetBuffer(nMaxSize), nMaxSize);
    strPostFormat.ReleaseBuffer();

    // Get our decimal point character from Locale information.
    int nBuffLen = ::GetLocaleInfo( LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0 );
    assert(nBuffLen > 0);
    CString Decimal;
    ::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, Decimal.GetBuffer(nBuffLen), nBuffLen);
    Decimal.ReleaseBuffer();

    // Truncate at the "decimal" point.
    int nPos = strPostFormat.Find(Decimal);
    if (nPos > 0)
        strPostFormat = strPostFormat.Left(nPos);

    strPostFormat += _T(" KB");
    StrCopy(string, strPostFormat, nMaxSize);
    return TRUE;
}

// Retrieves the file's last write time and stores the text in string.
BOOL CMyListView::GetLastWriteTime(HANDLE file, LPTSTR string)
{
    FILETIME create, access, write;
    SYSTEMTIME localSysTime;
    FILETIME localFileTime;
    const int maxChars = 32;
    TCHAR time[maxChars];
    TCHAR date[maxChars];

    // Retrieve the file times for the file.
    if (!::GetFileTime(file, &create, &access, &write))
        return FALSE;

    // Convert the last-write time to local time.
    ::FileTimeToLocalFileTime(&write, &localFileTime);
    ::FileTimeToSystemTime(&localFileTime, &localSysTime);

    // Build a string showing the date and time with regional settings.
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

    // Set up the colmns for report mode.
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
    m_pItems.clear();
}

// Sets the CREATESTRUCT parameters before the window is created.
void CMyListView::PreCreate(CREATESTRUCT& cs)
{
    cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE |
            LVS_ICON | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
    cs.dwExStyle = WS_EX_CLIENTEDGE;
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
    switch (msg)
    {
    case WM_DRAWITEM:
    case WM_MENUCHAR:
    case WM_MEASUREITEM:
    case WM_INITMENUPOPUP:
        {
            // Add features implemented via IContextMenu2 such as 'New'.
            if(m_ccm2.GetIContextMenu2())
                m_ccm2.HandleMenuMsg(msg, wparam, lparam);
        }
        break;
    }

    return WndProcDefault(msg, wparam, lparam);
}

///////////////////////////////////
// ListItemData function definitions
// ListItemData is a nested class of CMyListView.
//

// Constructor.
CMyListView::ListItemData::ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder)
{
    m_parentFolder = cParentFolder;
    m_cpidlFull     = cpidlParent + cpidlRel;
    m_cpidlRel      = cpidlRel;
}

// Destructor.
CMyListView::ListItemData::~ListItemData()
{
}
