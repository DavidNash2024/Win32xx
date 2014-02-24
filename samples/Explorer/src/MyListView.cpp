//////////////////////////////////////////////////////////
// MyListView.cpp
//  Definitions for the CMyListView and ListItemData classes

#include "stdafx.h"
#include "ExplorerApp.h"
#include "MyListView.h"
#include "resource.h"

////////////////////////////////
//CMyListView function definitions
int CALLBACK CMyListView::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNREFERENCED_PARAMETER(lParamSort);
	ListItemData*  pItem1 = (ListItemData*)lParam1;
	ListItemData*  pItem2 = (ListItemData*)lParam2;

	HRESULT hr = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelPidl(), pItem2->GetRelPidl());

	if(FAILED(hr))
		return 0;

	return (short)SCODE_CODE(GetScode(hr));
}

CMyListView::CMyListView()
{
	SetImageLists();
}

CMyListView::~CMyListView()
{
	DeleteItems();
}

void CMyListView::DeleteItems()
{
	m_pItems.clear();
}

void CMyListView::DisplayFolder(CShellFolder& cParentFolder, Cpidl& cpidlFull, Cpidl& cpidlRel)
{
	m_cpidlCurFull = cpidlFull;
	if(cParentFolder.GetIShellFolder())
		cParentFolder.BindToObject(cpidlRel, NULL, IID_IShellFolder, m_csfCurFolder);
	else
		m_csfCurFolder.SHGetDesktopFolder();

	DoDisplay();
}

void CMyListView::DoBackgroundMenu(CPoint& ptScreen)
{
	HRESULT        hr;
	if(m_csfCurFolder.GetIShellFolder())
	{
		CContextMenu ccm;
		hr = m_csfCurFolder.CreateViewObject(GetParent()->GetHwnd(), IID_IContextMenu, ccm);

		if(SUCCEEDED(hr))
		{
			CMenu Popup;
			Popup.CreatePopupMenu();
			if(Popup.GetHandle())
			{
				int   i = 0;
				UINT  idCmdFirst = 0;
				UINT  idCmd;

				//find the largest ID in the menu
				while((idCmd = Popup.GetMenuItemID(i)) != (UINT)-1)
				{
					if(idCmd > idCmdFirst)
						idCmdFirst = idCmd;
					i++;
				}

				hr = ccm.QueryContextMenu(Popup, 0, ++idCmdFirst, (UINT)-1, CMF_NORMAL | CMF_EXPLORE);

				if(SUCCEEDED(hr))
				{
					ccm.QueryInterface(IID_IContextMenu2, m_ccm2);

					idCmd = Popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
						ptScreen.x, ptScreen.y, this, NULL);

					if(idCmd)
					{
						CMINVOKECOMMANDINFO  cmi = {0};
						cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
						cmi.hwnd = ::GetParent(m_hWnd);
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

void CMyListView::DoContextMenu(CPoint& ptScreen)
{
	LVHITTESTINFO  lvhti;
	lvhti.pt = ptScreen;
	ScreenToClient(lvhti.pt);
	lvhti.flags = LVHT_NOWHERE;
	HitTest(lvhti);

	if(LVHT_ONITEM & lvhti.flags)
	{
		//get the selected items
		UINT  nItems = GetSelectedCount();
		std::vector<int> vItems(nItems, 0);
		int* pItemArray = &vItems.front();

		if(pItemArray)
		{
			UINT  i;
			int   nCurItem;

			//put the item clicked on first in the list
			pItemArray[0] = lvhti.iItem;

			for(i = 1, nCurItem = -1; i < nItems; ++i)
			{
				nCurItem = GetNextItem(nCurItem, LVNI_SELECTED);
				if(nCurItem != lvhti.iItem)
					pItemArray[i] = nCurItem;
				else
					--i;
			}

			DoItemMenu(pItemArray, nItems, ptScreen);
		}
	}
	else
		DoBackgroundMenu(ptScreen);
}

void CMyListView::DoDefault(int iItem)
{
	LVITEM   lvItem = {0};
	lvItem.mask = LVIF_PARAM;
	lvItem.iItem = iItem;

	if(GetItem(lvItem))
	{
		HRESULT        hr;
		ListItemData*  pInfo = (ListItemData*)lvItem.lParam;
		CShellFolder csFolder;
		CContextMenu ccm;
		csFolder = pInfo->GetParentFolder();

		if (!csFolder.GetIShellFolder())
			csFolder.SHGetDesktopFolder();

		if(csFolder.GetIShellFolder())
		{
			Cpidl* pCpidl = &(pInfo->GetRelPidl());
			hr = csFolder.GetUIObjectOf(m_hWnd, 1, pCpidl, IID_IContextMenu, 0, ccm);

			if(SUCCEEDED(hr))
			{
				CMenu Popup;
				Popup.CreatePopupMenu();
				if(Popup.GetHandle())
				{
					hr = ccm.QueryContextMenu(Popup, 0, 1, 0x7fff, CMF_DEFAULTONLY | CMF_EXPLORE);

					if(SUCCEEDED(hr))
					{
						UINT idCmd = Popup.GetMenuItemID(0);
						if(idCmd && (idCmd != (UINT)-1))
						{
							//determine if the item is a folder
							ULONG ulAttr = SFGAO_HASSUBFOLDER | SFGAO_FOLDER;
							csFolder.GetAttributesOf(1, pInfo->GetRelPidl(), ulAttr);

							if ((ulAttr & SFGAO_HASSUBFOLDER) || (ulAttr &SFGAO_FOLDER))
							{
								GetExplorerApp()->GetMainFrame()->GetTreeView()->SelectFromListView(pInfo->GetFullPidl());
							}
							else
							{
								CMINVOKECOMMANDINFO  cmi = {0};
								cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
								cmi.hwnd = ::GetParent(m_hWnd);
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

void CMyListView::DoItemMenu(LPINT piItems, UINT cbItems, CPoint& ptScreen)
{
	std::vector<Cpidl> vpidl(cbItems);
	Cpidl* pidlArray = &vpidl.front();

	for(UINT i = 0; i < cbItems; ++i)
	{
		LVITEM lvItem = {0};
		lvItem.mask = LVIF_PARAM;
		lvItem.iItem = piItems[i];
		if(GetItem(lvItem))
		{
			ListItemData*  pInfo = (ListItemData*)lvItem.lParam;
			pidlArray[i] = pInfo->GetRelPidl();
		}
	}

	if(pidlArray[0].GetPidl())
	{
		HRESULT        hr;
		CContextMenu ccm;

		if(m_csfCurFolder.GetIShellFolder())
		{
			hr = m_csfCurFolder.GetUIObjectOf(m_hWnd, cbItems, pidlArray, IID_IContextMenu, 0, ccm);

			if(SUCCEEDED(hr))
			{
				CMenu Popup;
				Popup.CreatePopupMenu();
				if(Popup.GetHandle())
				{
					hr = ccm.QueryContextMenu(Popup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);
					if(SUCCEEDED(hr))
					{
						ccm.QueryInterface(IID_IContextMenu2, m_ccm2);
						UINT  idCmd;
						idCmd = Popup.TrackPopupMenu(TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON,
									ptScreen.x, ptScreen.y, this, NULL);

						if(idCmd)
						{
							CMINVOKECOMMANDINFO  cmi = {0};
							cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
							cmi.hwnd = ::GetParent(m_hWnd);
							cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
							cmi.nShow = SW_SHOWNORMAL;
							ccm.InvokeCommand(cmi);

						//	The operation performed by the context menu may have changed
						//	the contents of the folder, so do a refresh.
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

	return 0L;
}

LRESULT CMyListView::OnLVNDispInfo(NMLVDISPINFO* pdi)
{
	ListItemData*   pItem = (ListItemData*)pdi->item.lParam;

	//do we need to supply the text?
	if(pdi->item.mask & LVIF_TEXT)
	{
		TCHAR szFileName[MAX_PATH];
		GetFullFileName(pItem->GetFullPidl().GetPidl(), szFileName);

		ULONG ulAttr = SFGAO_CANDELETE | SFGAO_FOLDER;
		pItem->GetParentFolder().GetAttributesOf(1, pItem->GetRelPidl(), ulAttr);

		HANDLE hFile;

		//A trick to help get a quick response from CreateFile
		if (ulAttr & SFGAO_CANDELETE)
			hFile = ::CreateFile (szFileName, 0, FILE_SHARE_READ, NULL,
				OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);
		else
			hFile = ::CreateFile (szFileName, 0, FILE_SHARE_READ, NULL,
				0, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_BACKUP_SEMANTICS, NULL);

		TCHAR szText[32];
		int nMaxLength = sizeof(szText)/sizeof(szText[0])-1;

		switch(pdi->item.iSubItem)
		{
		case 0:  //name
			{
				SHFILEINFO sfi = {0};
				//get the display name of the item
				if (pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
					::lstrcpyn(pdi->item.pszText, sfi.szDisplayName, pdi->item.cchTextMax -1);
			}
			break;
		case 1: //Size
			{
				TCHAR szSize[32];

				//report the size files and not folders
				if ((hFile != INVALID_HANDLE_VALUE)&&(~ulAttr & SFGAO_FOLDER))
				{
					GetFileSizeText(hFile, szSize);
					::lstrcpyn(pdi->item.pszText, szSize, nMaxLength -1);
				}
				else
					::lstrcpy(pdi->item.pszText, _T(""));
			}
			break;
		case 2: //Type
			{
				SHFILEINFO sfi = {0};
				if(pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_TYPENAME))
					::lstrcpyn(pdi->item.pszText, sfi.szTypeName, pdi->item.cchTextMax -1);
			}
			break;
		case 3: //Modified
			{
				if (hFile != INVALID_HANDLE_VALUE)
				{
					GetLastWriteTime(hFile, szText);
					::lstrcpyn(pdi->item.pszText, szText, nMaxLength -1);
				}
				else
					::lstrcpy(pdi->item.pszText, _T(""));
			}
			break;
		}
		if (hFile != INVALID_HANDLE_VALUE)
			::CloseHandle(hFile);
	}

	//do we need to supply the unselected image?
	if(pdi->item.mask & LVIF_IMAGE)
	{
		SHFILEINFO sfi = {0};

		//get the unselected image for this item
		if(pItem->GetFullPidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON))
			pdi->item.iImage = sfi.iIcon;
	}

	return 0L;
}

LRESULT CMyListView::OnNMReturn(LPNMHDR pNMHDR)
{
	UNREFERENCED_PARAMETER(pNMHDR);

	//get the item that has the focus
	int nItem = (int)::SendMessage(m_hWnd, LVM_GETNEXTITEM, (WPARAM) -1, (LPARAM) MAKELPARAM (LVNI_FOCUSED, 0));

	if(nItem != -1)
		DoDefault(nItem);

	return 0L;
}

LRESULT CMyListView::OnNotifyReflect(WPARAM, LPARAM lParam)
{
	LPNMHDR  pNMHDR = (LPNMHDR) lParam;

	switch(pNMHDR->code)
	{
	case NM_RCLICK:			return OnNMRClick(pNMHDR);
	case LVN_GETDISPINFO:	return OnLVNDispInfo((NMLVDISPINFO*)lParam);
 	case NM_DBLCLK:			return OnNMReturn(pNMHDR);
	case NM_RETURN:			return OnNMReturn(pNMHDR);
	}
	return 0;
}

void CMyListView::EnumObjects(CShellFolder& cPFolder, Cpidl& cpidlParent)
{
	CEnumIDList cEnum;

	int grFlags = SHCONTF_FOLDERS | SHCONTF_NONFOLDERS;
	if ( GetExplorerApp()->GetMainFrame()->GetShowHidden() )
		grFlags |= SHCONTF_INCLUDEHIDDEN;

	if(SUCCEEDED(cPFolder.EnumObjects(NULL, grFlags, cEnum)))
	{
		ULONG ulFetched = 1;
		Cpidl cpidlRel;

		//enumerate the item's PIDLs
		while(S_OK == (cEnum.Next(1, cpidlRel, ulFetched)) && ulFetched)
		{
			LVITEM lvItem = {0};
			ULONG  ulAttr;

			//fill in the TV_ITEM structure for this item
			lvItem.mask = LVIF_PARAM | LVIF_TEXT | LVIF_IMAGE | LVIF_STATE;

			//Store a pointer to the ListItemData in the lParam and m_pItems
			ListItemData* pItem = new ListItemData(cpidlParent, cpidlRel, cPFolder);
			lvItem.lParam = (LPARAM)pItem;
			m_pItems.push_back(pItem);

			//text and images are done on a callback basis
			lvItem.pszText = LPSTR_TEXTCALLBACK;
			lvItem.iImage = I_IMAGECALLBACK;

			//determine if the item's icon characteristics
			ulAttr = SFGAO_SHARE | SFGAO_LINK | SFGAO_GHOSTED;
			cPFolder.GetAttributesOf(1, cpidlRel, ulAttr);

			if(ulAttr & SFGAO_SHARE)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(1); //1 is the index for the shared overlay image
			}

			if (ulAttr & SFGAO_LINK)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_OVERLAYMASK;
				lvItem.state |= INDEXTOOVERLAYMASK(2); //2 is the index for the link overlay image
			}

			if(ulAttr & SFGAO_GHOSTED)
			{
				lvItem.mask |= LVIF_STATE;
				lvItem.stateMask |= LVIS_CUT;
				lvItem.state |= LVIS_CUT;
			}

			InsertItem(lvItem);
			ulFetched = 0;
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
	lstrcpyn(lpszSize, strPostFormat, nMaxSize);
	return TRUE;
}

BOOL CMyListView::GetLastWriteTime(HANDLE hFile, LPTSTR lpszString)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stLocal;
	FILETIME ftLocal;
	TCHAR szTime[32];
	TCHAR szDate[32];

	// Retrieve the file times for the file.
	if (!::GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	// Convert the last-write time to local time.
	::FileTimeToLocalFileTime(&ftWrite, &ftLocal);
	::FileTimeToSystemTime(&ftLocal, &stLocal);

	// Build a string showing the date and time with regional settings.
	::GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &stLocal, NULL, szDate, 31);
	::GetTimeFormat(LOCALE_USER_DEFAULT, TIME_NOSECONDS, &stLocal, NULL, szTime, 31);

	::lstrcpy(lpszString, szDate);
	::lstrcat(lpszString, _T(" "));
	::lstrcat(lpszString, szTime);

	return TRUE;
}

HIMAGELIST CMyListView::GetImageList(BOOL bLarge)
{
	if (bLarge)
		return m_hLargeImageList;
	else
		return m_hSmallImageList;
}

void CMyListView::OnDestroy()
{
	m_pItems.clear();
	m_csfCurFolder.Delete();
}

void CMyListView::OnInitialUpdate()
{
	//Set the image lists
	SendMessage(LVM_SETIMAGELIST, LVSIL_NORMAL, (LPARAM) GetImageList(TRUE));
	SendMessage(LVM_SETIMAGELIST, LVSIL_SMALL, (LPARAM) GetImageList(FALSE));

	//Set up the colmns for report mode
	TCHAR szText[256];
	LVCOLUMN lvc = {0};
    lvc.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	int iColumns = 4;	//Number of columns
	int ColSize[4] = {150, 70, 100, 120}; // width of columns in pixels

    for (int iCol = 0; iCol < iColumns; iCol++)
	{
		lvc.iSubItem = iCol;
		lvc.pszText = szText;
		lvc.cx = ColSize[iCol];

		if (1 == iCol) lvc.fmt = LVCFMT_RIGHT; // right-aligned column
		else lvc.fmt = LVCFMT_LEFT;		//left-aligned column

		::LoadString(GetApp()->GetInstanceHandle(), IDS_COLUMN1 + iCol,
			szText, sizeof(szText)/sizeof(szText[0]));

		InsertColumn(iCol, lvc);
    }

	//Set initial the view style as report
	ViewReport();
}

void CMyListView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | LVS_AUTOARRANGE |
            LVS_ICON | LVS_SHAREIMAGELISTS | LVS_SHOWSELALWAYS;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
}

void CMyListView::SetImageLists()
{
	SHFILEINFO  sfi;

	// Get the system image list
	m_hLargeImageList = (HIMAGELIST)::SHGetFileInfo(_T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);

	m_hSmallImageList = (HIMAGELIST)::SHGetFileInfo(_T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
}

void CMyListView::ViewLargeIcons()
{
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_ICON );
}

void CMyListView::ViewSmallIcons()
{
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_SMALLICON);
}

void CMyListView::ViewList()
{
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_LIST);
}

void CMyListView::ViewReport()
{
	DWORD dwStyle = (DWORD)GetWindowLongPtr(GWL_STYLE);
	SetWindowLongPtr(GWL_STYLE, (dwStyle & ~LVS_TYPEMASK) | LVS_REPORT);
}

LRESULT CMyListView::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
//ListItemData function definitions
CMyListView::ListItemData::ListItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder)
{
	m_cParentFolder = cParentFolder;
	m_cpidlFull     = cpidlParent + cpidlRel;
	m_cpidlRel      = cpidlRel;
}

CMyListView::ListItemData::~ListItemData()
{
}
