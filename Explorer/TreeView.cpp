//////////////////////////////////////////////////////////
// TreeView.cpp
//  Definitions for the CTreeView and TreeItemData classes


#include "ShellApp.h"
#include "Mainfrm.h"
#include "MainView.h"
#include "ListView.h"
#include "TreeView.h"
#include "Resource.h"

////////////////////////////////
//CTreeView function definitions
CTreeView::CTreeView()
{
	try
	{
		HRESULT hr = ::CoInitialize(NULL);
		if (!((hr == S_OK) || (hr == S_FALSE)))
			throw CWinException(_T("Problem Initializing COM"));;

		SetImageLists();
	}

	catch (CWinException &e)
	{
		e.MessageBox();
	}

	catch (...)
	{
		DebugErrMsg(_T("Exception in CShellWin construction"));
	}
}

CTreeView::~CTreeView()
{
	std::vector<TreeItemData*>::iterator Iter;
	for (Iter = m_pItems.begin(); Iter != m_pItems.end(); Iter++)
	{
		//free up the TreeItemData objects
		delete (*Iter);
	}

	::CoUninitialize(); // Shut down COM
}

int CALLBACK CTreeView::CompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	UNREFERENCED_PARAMETER(lParamSort);
	TreeItemData* pItem1 = (TreeItemData*)lParam1;
	TreeItemData* pItem2 = (TreeItemData*)lParam2;
	HRESULT hr;

	hr = pItem1->GetParentFolder().CompareIDs(0, pItem1->GetRelCpidl(), pItem2->GetRelCpidl());
	if(FAILED(hr))
		return 0;

	return (short)SCODE_CODE(GetScode(hr));
}

void CTreeView::DoContextMenu(LPPOINT pptScreen)
{
	TVHITTESTINFO  tvhti;
	tvhti.pt = *pptScreen;
	ScreenToClient(m_hWnd, &tvhti.pt);
	tvhti.flags = LVHT_NOWHERE;
	TreeView_HitTest(m_hWnd, &tvhti);

	if(TVHT_ONITEM & tvhti.flags)
		DoItemMenu(tvhti.hItem , pptScreen);
}

void CTreeView::DoItemMenu(HTREEITEM hItem, LPPOINT pptScreen)
{
	TVITEM tvItem = {0};
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hItem;

	if(TreeView_GetItem(m_hWnd, &tvItem))
	{
		HRESULT        hr;
		TreeItemData*  pInfo = (TreeItemData*)tvItem.lParam;
		CContextMenu cm;
		CShellFolder sf;

		if (pInfo->GetParentFolder().GetIShellFolder() != NULL)
			sf = pInfo->GetParentFolder();
		else
			sf.SHGetDesktopFolder();

		if(sf.GetIShellFolder())
		{
			hr = sf.GetUIObjectOf(m_hWnd, 1, &(pInfo->GetRelCpidl()), IID_IContextMenu, 0, cm);

			if(SUCCEEDED(hr))
			{
				HMENU hPopup;

				hPopup = CreatePopupMenu();
				if(hPopup)
				{
					hr = cm.QueryContextMenu(hPopup, 0, 1, 0x7fff, CMF_NORMAL | CMF_EXPLORE);

					if(SUCCEEDED(hr))
					{
						cm.QueryInterface(IID_IContextMenu2, m_ccm2);

						UINT idCmd = ::TrackPopupMenu(hPopup, TPM_LEFTALIGN | TPM_RETURNCMD | TPM_RIGHTBUTTON ,
									pptScreen->x, pptScreen->y, 0, m_hWnd, NULL);

						//A Treeview control sometimes requires this to end the
						// TrackPopupMenu properly
						::PostMessage(m_hWndParent, WM_CANCELMODE, 0, 0);

						if(idCmd)
						{
							CMINVOKECOMMANDINFO  cmi = {0};
							cmi.cbSize = sizeof(CMINVOKECOMMANDINFO);
							cmi.hwnd = m_hWnd;
							cmi.lpVerb = (LPCSTR)(INT_PTR)(idCmd - 1);
							cmi.nShow = SW_SHOWNORMAL;
							cm.InvokeCommand(cmi);
						}
					}
				}
				::DestroyMenu(hPopup);
			}
		}
	}
}

LRESULT CTreeView::OnNotifyReflect(WPARAM, LPARAM lParam)
{
	LPNMHDR  lpnmh = (LPNMHDR) lParam;

	switch(lpnmh->code)
	{
	case NM_RCLICK:
		{
			POINT ptScreen;
			::GetCursorPos(&ptScreen);
			DoContextMenu(&ptScreen);
		}
		break;
	case TVN_GETDISPINFO:
		{
			LPNMTVDISPINFO lpdi = (LPNMTVDISPINFO)lParam;
			TreeItemData* pItem = (TreeItemData*)lpdi->item.lParam;

			//do we need to supply the text?
			if(lpdi->item.mask & TVIF_TEXT)
			{
				SHFILEINFO sfi = {0};

				//get the display name of the item
				if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_DISPLAYNAME))
					::lstrcpyn(lpdi->item.pszText, sfi.szDisplayName, lpdi->item.cchTextMax -1);
			}

			//do we need to supply the unselected image?
			if(lpdi->item.mask & TVIF_IMAGE)
			{
				SHFILEINFO sfi = {0};

				//get the unselected image for this item
				if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_LINKOVERLAY))
					lpdi->item.iImage = sfi.iIcon;
			}

			//do we need to supply the selected image?
			if(lpdi->item.mask & TVIF_SELECTEDIMAGE)
			{
				SHFILEINFO sfi = {0};

				//get the selected image for this item
				if(pItem->GetFullCpidl().SHGetFileInfo(0, sfi, SHGFI_PIDL | SHGFI_SYSICONINDEX | SHGFI_SMALLICON | SHGFI_OPENICON))
					lpdi->item.iSelectedImage = sfi.iIcon;
			}
		}
		break;

	case TVN_ITEMEXPANDING:
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;

			switch(pnmtv->action)
			{
			case TVE_EXPAND:
				{
					UINT ExpandedOnce = pnmtv->itemNew.state & TVIS_EXPANDEDONCE;
					if (!ExpandedOnce)
						GetChildItems(pnmtv->itemNew.hItem);
				}
				break;
			}
		}
		break;

	case TVN_SELCHANGED:
		{
			LPNMTREEVIEW pnmtv = (LPNMTREEVIEW)lParam;
			TreeItemData* pItem = (TreeItemData*)pnmtv->itemNew.lParam;
			CMainFrame* pFrame = (CMainFrame*) GetApp()->GetFrame();

			pFrame->GetMainView().GetListView().DisplayFolder(pItem->GetParentFolder(),
				pItem->GetFullCpidl(), pItem->GetRelCpidl());
		}
		break;
	} // switch(lpnmh->code)
	return 0;
}

void CTreeView::EnumObjects(HTREEITEM hParentItem, CShellFolder& cParentFolder, Cpidl& cpidlParent)
{
	CEnumIDList cEnum;
	if(SUCCEEDED(cParentFolder.EnumObjects(NULL, SHCONTF_FOLDERS | SHCONTF_INCLUDEHIDDEN, cEnum)))
	{
		ULONG ulFetched = 1;
		Cpidl cpidlRel;

		//enumerate the item's PIDLs
		while(S_OK == (cEnum.Next(1, cpidlRel, ulFetched)) && ulFetched)
		{
			TVITEM         tvItem = {0};
			TVINSERTSTRUCT tvInsert = {0};
			ULONG          ulAttribs;

			//fill in the TV_ITEM structure for this item
			tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

			//Store a pointer to the TreeItemData in the lParam and m_pItems
			TreeItemData* pItem = new TreeItemData(cpidlParent, cpidlRel, cParentFolder);
			tvItem.lParam = (LPARAM)pItem;
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

			TreeView_InsertItem(m_hWnd, &tvInsert);
			ulFetched = 0;
		}
	}
}

BOOL CTreeView::GetChildItems(HTREEITEM hParentItem)
{
	TVITEM tvItem = {0};
	tvItem.mask = TVIF_PARAM;
	tvItem.hItem = hParentItem;
	if(!TreeView_GetItem(m_hWnd, &tvItem))
		return FALSE;

	//change the cursor
	HCURSOR hCursor = ::SetCursor(LoadCursor(NULL, IDC_WAIT));

	//turn redawing off in the TreeView. This will speed things up as we add items
	::SendMessage(m_hWnd, WM_SETREDRAW, FALSE, 0);

	TreeItemData* pItem = (TreeItemData*)tvItem.lParam;
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
	TreeView_SortChildrenCB(m_hWnd, &tvSort, 0);

	//turn redawing back on in the TreeView
	::SendMessage(m_hWnd, WM_SETREDRAW, TRUE, 0);

	::UpdateWindow(m_hWnd);
	::SetCursor(hCursor);

	return TRUE;
}

HIMAGELIST CTreeView::GetImageList(BOOL bLarge)
{
	if (bLarge)
		return m_hLargeImageList;
	else
		return m_hSmallImageList;
}

BOOL CTreeView::GetRootItems()
{
	TreeView_DeleteAllItems(m_hWnd);

	Cpidl cpidlDesk;
	cpidlDesk.SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOP);
	if (cpidlDesk.GetPidl())
	{
		TV_ITEM           tvItem = {0};
		TV_INSERTSTRUCT   tvInsert = {0};
		HTREEITEM         hParentItem;

		//fill in the TVITEM structure for this item
		tvItem.mask = TVIF_PARAM | TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_CHILDREN;

		//Store a pointer to the TreeItemData in the lParam and m_pItems
		TreeItemData* pItem = new TreeItemData(cpidlDesk);
		tvItem.lParam = (LPARAM)pItem;
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

		::SetFocus(m_hWnd);

		//add the item
		hParentItem = TreeView_InsertItem(m_hWnd, &tvInsert);

		//go ahead and expand this item
		TreeView_Expand(m_hWnd, hParentItem, TVE_EXPAND);

		//Select the next item
		HTREEITEM hNext = TreeView_GetNextItem(m_hWnd, hParentItem, TVGN_CHILD);

		//Expand this item
		TreeView_Expand(m_hWnd, hNext, TVE_EXPAND);

		//Select this item
		TreeView_SelectItem(m_hWnd, hNext);

		//Scroll this item into view
		TreeView_SelectSetFirstVisible(m_hWnd, hNext);

		return TRUE;
	}

	return FALSE;
}

void CTreeView::OnInitialUpdate()
{
	TreeView_DeleteAllItems(m_hWnd);

	//Set the image list
	::SendMessage(m_hWnd, TVM_SETIMAGELIST, TVSIL_NORMAL, (LPARAM) GetImageList(FALSE));
}

void CTreeView::PreCreate(CREATESTRUCT &cs)
{
	cs.style = WS_TABSTOP | WS_CHILD | WS_VISIBLE | TVS_HASLINES |
					TVS_HASBUTTONS | TVS_NOTOOLTIPS | TVS_SHOWSELALWAYS ;
	cs.dwExStyle = WS_EX_CLIENTEDGE;
	cs.lpszClass = WC_TREEVIEW;
}

BOOL CTreeView::SelectFromListView(Cpidl& cpidlFull)
{
	HTREEITEM hItem = TreeView_GetSelection(m_hWnd);

	//Set parent item's has Children flag
	TVITEM tvItem = {0};
	tvItem.mask = TVIF_CHILDREN;
	tvItem.cChildren = 1;
	tvItem.hItem = hItem;
	TreeView_SetItem(m_hWnd, &tvItem);

	//Expand the tree item
	TreeView_Expand(m_hWnd, hItem, TVE_EXPAND);

	//First Child
	HTREEITEM hChild = TreeView_GetChild(m_hWnd, hItem);

	while (hChild != NULL)
	{
		//Get the TVITEM structure for this item
		TVITEM tvItem = {0};
		tvItem.mask = TVIF_PARAM;
		tvItem.hItem = hChild;
		if(!TreeView_GetItem(m_hWnd, &tvItem))
			return FALSE;

		//Get the TreeItemData pointer from the item's lParam
		TreeItemData* pTD = (TreeItemData*)tvItem.lParam;

		//Compare the pidls
		if (cpidlFull == pTD->GetFullCpidl())
		{
			TreeView_SelectItem(m_hWnd, hChild);
			return TRUE;
		}

		//Next Sibling
		hChild = TreeView_GetNextSibling(m_hWnd, hChild);
	}
	TRACE(_T("Item NOT found"));

	return FALSE;
}

void CTreeView::SetImageLists()
{
	SHFILEINFO  sfi;

	// Get the system image list
	m_hLargeImageList = (HIMAGELIST)::SHGetFileInfo(_T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);

	m_hSmallImageList = (HIMAGELIST)::SHGetFileInfo(_T("C:\\"), 0, &sfi,
		sizeof(SHFILEINFO), SHGFI_SYSICONINDEX | SHGFI_SMALLICON);
}

LRESULT CTreeView::WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	return 0L;
}

///////////////////////////////////
//TreeItemData function definitions
CTreeView::TreeItemData::TreeItemData(Cpidl& cpidl)
{
	m_cpidlRel  = cpidl;
	m_cpidlFull = cpidl;
}

CTreeView::TreeItemData::TreeItemData(Cpidl& cpidlParent, Cpidl& cpidlRel, CShellFolder& cParentFolder)
{
	m_cParentFolder = cParentFolder;
	m_cpidlFull     = cpidlParent + cpidlRel;
	m_cpidlRel      = cpidlRel;
}

CTreeView::TreeItemData::~TreeItemData()
{
}
