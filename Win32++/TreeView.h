#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "../Win32++/WinCore.h"


namespace Win32xx
{

	class CTreeView : public CWnd
	{
	public:
		CTreeView() {}
		virtual ~CTreeView() {}
		virtual void PreCreate(CREATESTRUCT &cs) { cs.lpszClass = WC_TREEVIEW; }

		COLORREF GetBkColor() { return (COLORREF)::SendMessage(m_hWnd, TVM_GETBKCOLOR, 0, 0); }
		
		int GetCheckState(HTREEITEM hItem) { return TreeView_GetCheckState(m_hWnd, hItem); }
		
		HTREEITEM GetChildItem(HTREEITEM hItem) { return TreeView_GetChild(m_hWnd, hItem); }
		int  GetCount() { return (int)::SendMessage(m_hWnd, TVM_GETCOUNT, 0, 0); }
		HTREEITEM GetDropHiLightItem() { return TreeView_GetDropHilight(m_hWnd); }
		HWND GetEditControl() { return (HWND)::SendMessage(m_hWnd, TVM_GETEDITCONTROL, 0, 0); }
		HTREEITEM GetFirstVisibleItem() { return TreeView_GetFirstVisible(m_hWnd); }
		HIMAGELIST GetImageList(int iType) { return (HIMAGELIST)::SendMessage(m_hWnd, TVM_GETIMAGELIST, (WPARAM) iType, 0); }
		int  GetIndent() { return (int)::SendMessage(m_hWnd, TVM_GETINDENT, 0, 0); }
		COLORREF GetInsertMarkColor() { return (COLORREF)::SendMessage(m_hWnd, TVM_GETINSERTMARKCOLOR, 0, 0); }
		BOOL GetItem(TVITEM* pItem) { return (BOOL)::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM) pItem); }
		DWORD_PTR GetItemData(HTREEITEM hItem) 
		{
			TVITEM tvi = {0};
			tvi.mask = TVIF_PARAM;
			tvi.hItem = hItem;
			::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&tvi);
			return tvi.lParam;
		}
		int  GetItemHeight() { return (int)::SendMessage(m_hWnd, TVM_GETITEMHEIGHT, 0, 0); }
		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) 
		{
			TVITEM tvi = {0};
			tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.hItem = hItem;
			BOOL bResult = ::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&tvi);
			nImage = tvi.iImage;
			nSelectedImage = tvi.iSelectedImage;
			return bResult;	
		}
		BOOL GetItemRect(HTREEITEM hItem, LPRECT pRect, BOOL bTextOnly) 
		{ 
			*(HTREEITEM*)pRect = hItem;
			return (BOOL)::SendMessage(m_hWnd, TVM_GETITEMRECT, (WPARAM) bTextOnly, (LPARAM)pRect ); 
		}
		UINT GetItemState(HTREEITEM hItem, UINT nStateMask) { return (UINT)::SendMessage(m_hWnd, TVM_GETITEMSTATE, (WPARAM)hItem, (LPARAM)nStateMask); }
		tString GetItemText(HTREEITEM hItem)
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.mask = TVIF_TEXT;
			tvi.cchTextMax = 260;
			TCHAR szText[261];
			tvi.pszText = szText;
			::SendMessage(m_hWnd, TVM_GETITEM, 0, (LPARAM)&tvi);
			tString t = tvi.pszText;
			return t;
		}
		COLORREF GetLineColor() { return (COLORREF)::SendMessage(m_hWnd, TVM_GETLINECOLOR, 0, 0); }
		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) { return (HTREEITEM)::SendMessage(m_hWnd, TVM_GETNEXTITEM, (WPARAM)nCode, (LPARAM)hItem); }
		HTREEITEM GetNextSiblingItem(HTREEITEM hItem) { return TreeView_GetNextSibling(m_hWnd, hItem); }
		HTREEITEM GetNextVisibleItem(HTREEITEM hItem) { return TreeView_GetNextVisible(m_hWnd, hItem); }
		HTREEITEM GetParentItem(HTREEITEM hItem) { return TreeView_GetParent(m_hWnd, hItem); }
		HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) { return TreeView_GetPrevSibling(m_hWnd, hItem); }
		HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) { return TreeView_GetPrevSibling(m_hWnd, hItem); }
		HTREEITEM GetRootItem() {TreeView_GetRoot(m_hWnd); }
		int GetScrollTime() { return (int)::SendMessage(m_hWnd, TVM_GETSCROLLTIME, 0, 0); }
		HTREEITEM GetSelectedItem() { return TreeView_GetSelection(m_hWnd); }
		COLORREF GetTextColor() { return (COLORREF)::SendMessage(m_hWnd, TVM_GETTEXTCOLOR, 0, 0); }
		HWND GetToolTips() { return (HWND)::SendMessage(m_hWnd, TVM_GETTOOLTIPS, 0, 0); }
		int GetVisibleCount() { return (int)::SendMessage(m_hWnd, TVM_GETVISIBLECOUNT, 0, 0); }
		BOOL ItemHasChildren(HTREEITEM hItem) { return (BOOL)TreeView_GetItemState(m_hWnd, hItem, TVIF_CHILDREN); }
		COLORREF SetBkColor(COLORREF clrBk) { return (COLORREF)::SendMessage(m_hWnd, TVM_SETBKCOLOR, 0, (LPARAM)clrBk); }
		void SetCheckState(HTREEITEM hItem, BOOL fCheck) { TreeView_SetCheckState(m_hWnd, hItem, fCheck ); }
		HIMAGELIST SetImageList(HIMAGELIST himl, int nType) { return (HIMAGELIST)::SendMessage(m_hWnd, TVM_SETIMAGELIST, (WPARAM) nType, (LPARAM) (HIMAGELIST) himl); }
		void SetIndent(int indent) { ::SendMessage(m_hWnd, TVM_SETINDENT, (WPARAM) indent, 0); }
		BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE) { return (BOOL)::SendMessage(m_hWnd, TVM_SETINSERTMARK, (WPARAM)fAfter, (LPARAM)hItem); }
		COLORREF SetInsertMarkColor(COLORREF clrInsertMark) { return (COLORREF)::SendMessage(m_hWnd, TVM_SETINSERTMARKCOLOR, 0, (LPARAM) clrInsertMark); }
		BOOL SetItem(TVITEM tvItem) { return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&tvItem ); }
		BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData) 
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.mask = TVIF_PARAM;
			tvi.lParam = dwData;
			return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&tvi );
		}
		int  SetItemHeight(SHORT cyItem) { return (int)::SendMessage(m_hWnd, TVM_SETITEMHEIGHT, (WPARAM)cyItem, 0 ); }
		BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.iImage = nImage;
			tvi.iSelectedImage = nSelectedImage;
			tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&tvi );
		}
		void SetItemState(HTREEITEM hItem, UINT state, UINT stateMask) { TreeView_SetItemState(m_hWnd, hItem, state, stateMask); }
		BOOL SetItemText(HTREEITEM hItem, LPCTSTR szText) 
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.pszText = (LPTSTR)szText;
			tvi.mask = TVIF_TEXT;
			return (BOOL)::SendMessage(m_hWnd, TVM_SETITEM, 0, (LPARAM)&tvi );
		}
		COLORREF SetLineColor(COLORREF clr) { return (COLORREF)::SendMessage(m_hWnd, TVM_SETLINECOLOR, 0, (LPARAM)clr); }
		UINT SetScrollTime(UINT uScrollTime) { return (UINT)::SendMessage(m_hWnd, TVM_SETSCROLLTIME, (WPARAM)uScrollTime, 0); }
		COLORREF SetTextColor(COLORREF clrText) { return (COLORREF)::SendMessage(m_hWnd, TVM_SETTEXTCOLOR, 0, (LPARAM)clrText ); }
		HWND SetToolTips(HWND hwndTooltip) { return (HWND)::SendMessage(m_hWnd, TVM_SETTOOLTIPS, (WPARAM)hwndTooltip, 0); }

		HTREEITEM CreateDragImage(HTREEITEM hItem) { return (HTREEITEM)::SendMessage(m_hWnd, TVM_CREATEDRAGIMAGE, 0, (LPARAM)hItem); }
		BOOL DeletAllItems() {return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, 0 ); }
		BOOL DeleteItem(HTREEITEM hItem) { return (BOOL)::SendMessage(m_hWnd, TVM_DELETEITEM, 0, (LPARAM)hItem ); }
		HWND EditLabel(HTREEITEM hItem) { return (HWND)::SendMessage(m_hWnd, TVM_EDITLABEL, 0, (LPARAM)hItem ); }
		BOOL EnsureVisible(HTREEITEM hItem) { return (BOOL)::SendMessage(m_hWnd, TVM_ENSUREVISIBLE, 0, (LPARAM)hItem ); }
		BOOL Expand(HTREEITEM hItem, UINT nCode) { return (BOOL)::SendMessage(m_hWnd, TVM_EXPAND, (WPARAM)nCode, (LPARAM)hItem ); }
		HTREEITEM HitTest(LPTVHITTESTINFO pht) { return (HTREEITEM)::SendMessage(m_hWnd, TVM_HITTEST, 0, (LPARAM)pht ); }
		HTREEITEM InsertItem(LPTVINSERTSTRUCT pIS) { return (HTREEITEM)::SendMessage(m_hWnd, TVM_INSERTITEM, 0, (LPARAM)pIS ); }
		BOOL Select(HTREEITEM hitem, UINT flag) { return TreeView_Select(m_hWnd, hitem, flag ); }
		BOOL SelectDropTarget(HTREEITEM hItem) { return TreeView_SelectDropTarget(m_hWnd, hItem); }
		BOOL SelectItem(HTREEITEM hItem, DWORD dwFlag = 0) { return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, (WPARAM)dwFlag, (LPARAM)hItem ); }
		BOOL SelectSetFirstVisible(HTREEITEM hItem) { return TreeView_SelectSetFirstVisible(m_hWnd, hItem); }
		BOOL SortChildren(HTREEITEM hItem, BOOL bRecurse) { return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDREN,  (WPARAM)bRecurse, (LPARAM)hItem ); }
		BOOL SortChildrenCB(LPTVSORTCB psort) { return (BOOL)::SendMessage(m_hWnd, TVM_SORTCHILDRENCB, 0, (LPARAM)psort ); }
	};

} // namespace Win32xx

#endif // #ifndef TREEVIEW_H

