#ifndef TREEVIEW_H
#define TREEVIEW_H

#include "../Win32++/WinCore.h"
#include "Commctrl.h"


namespace Win32xx
{

	class CTreeView : public CWnd
	{
	public:
		CTreeView() {}
		virtual ~CTreeView() {}
		
		virtual void PreCreate(CREATESTRUCT &cs) 
		{ cs.lpszClass = WC_TREEVIEW; }

// Attributes
		COLORREF GetBkColor() const
		{ return TreeView_GetBkColor( m_hWnd ); }
		
		HTREEITEM GetChildItem(HTREEITEM hItem) const
		{ return TreeView_GetChild(m_hWnd, hItem); }
		
		UINT  GetCount() const
		{ return TreeView_GetCount( m_hWnd ); }
		
		HTREEITEM GetDropHiLightItem() const 
		{ return TreeView_GetDropHilight(m_hWnd); }
		
		HWND GetEditControl() const
		{ return TreeView_GetEditControl( m_hWnd ); }
		
		HTREEITEM GetFirstVisibleItem() const
		{ return TreeView_GetFirstVisible(m_hWnd); }
		
		HIMAGELIST GetImageList(int iImageType) const
		{ return TreeView_GetImageList( m_hWnd, iImageType ); }
		
		UINT  GetIndent() const
		{ return TreeView_GetIndent( m_hWnd ); }
		
		COLORREF GetInsertMarkColor() const 
		{ return TreeView_GetInsertMarkColor( m_hWnd ); }
		
		BOOL GetItem(TVITEM* pItem) const
		{ return TreeView_GetItem( m_hWnd, pItem ); }
		
		DWORD_PTR GetItemData(HTREEITEM hItem) const 
		{
			TVITEM tvi = {0};
			tvi.mask = TVIF_PARAM;
			tvi.hItem = hItem;
			return TreeView_GetItem( m_hWnd, hItem );
		}
		
		int  GetItemHeight() const
		{ return TreeView_GetItemHeight( m_hWnd ); }
		
		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const
		{
			TVITEM tvi = {0};
			tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			tvi.hItem = hItem;
			BOOL bResult = TreeView_GetItem( m_hWnd, &tvi );
			nImage = tvi.iImage;
			nSelectedImage = tvi.iSelectedImage;
			return bResult;	
		}
		
		BOOL GetItemRect(HTREEITEM hItem, LPRECT pRect, BOOL bTextOnly) const
		{ return TreeView_GetItemRect( m_hWnd, hItem, pRect, bTextOnly ); }
		
		tString GetItemText(HTREEITEM hItem) const
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
		
		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const
		{ return TreeView_GetNextItem( m_hWnd, hItem, nCode); }
		
		HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const
		{ return TreeView_GetNextSibling(m_hWnd, hItem); }
		
		HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const
		{ return TreeView_GetNextVisible(m_hWnd, hItem); }
		
		HTREEITEM GetParentItem(HTREEITEM hItem) const
		{ return TreeView_GetParent(m_hWnd, hItem); }
		
		HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const
		{ return TreeView_GetPrevSibling(m_hWnd, hItem); }
		
		HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const
		{ return TreeView_GetPrevSibling(m_hWnd, hItem); }
		
		HTREEITEM GetRootItem() const
		{TreeView_GetRoot(m_hWnd); }
		
		int GetScrollTime() const
		{ return TreeView_GetScrollTime( m_hWnd ); }
		
		HTREEITEM GetSelectedItem() const 
		{ return TreeView_GetSelection(m_hWnd); }
		
		COLORREF GetTextColor() const
		{ return TreeView_GetTextColor( m_hWnd ); }
		
		HWND GetToolTips() const
		{ return TreeView_GetToolTips( m_hWnd ); }
		
		UINT GetVisibleCount() const
		{ return TreeView_GetVisibleCount( m_hWnd ); }
		
		BOOL ItemHasChildren(HTREEITEM hItem) const 
		{ return (BOOL)TreeView_GetChild( m_hWnd, hItem ); }
		
		COLORREF SetBkColor(COLORREF clrBk) 
		{ return TreeView_SetBkColor( m_hWnd, clrBk ); }
				
		HIMAGELIST SetImageList(HIMAGELIST himl, int nType) 
		{ return TreeView_SetImageList( m_hWnd, himl, nType ); }

		void SetIndent(int indent) 
		{ TreeView_SetIndent( m_hWnd, indent ); }
		
		BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE) 
		{ return TreeView_SetInsertMark( m_hWnd, hItem, fAfter ); }
		
		COLORREF SetInsertMarkColor(COLORREF clrInsertMark) 
		{ return TreeView_SetInsertMarkColor( m_hWnd, clrInsertMark ); }
		
		BOOL SetItem(TVITEM tvItem) 
		{ return TreeView_SetItem( m_hWnd, &tvItem ); }
		
		BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR szText, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.mask  = nMask;
			tvi.pszText = (LPTSTR)szText;
			tvi.iImage  = nImage;
			tvi.iSelectedImage = nSelectedImage;
			tvi.state = nState;
			tvi.stateMask = nStateMask;
			tvi.lParam = lParam;
			return TreeView_SetItem( m_hWnd, &tvi );
		}
		
		BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData) 
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.mask = TVIF_PARAM;
			tvi.lParam = dwData;
			return TreeView_SetItem( m_hWnd, &tvi );
		}
		int  SetItemHeight(SHORT cyItem) 
		{ return TreeView_SetItemHeight( m_hWnd, cyItem ); }
		
		BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.iImage = nImage;
			tvi.iSelectedImage = nSelectedImage;
			tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
			return TreeView_SetItem(m_hWnd, &tvi );
		}
		
		BOOL SetItemText(HTREEITEM hItem, LPCTSTR szText) 
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.pszText = (LPTSTR)szText;
			tvi.mask = TVIF_TEXT;
			return TreeView_SetItem(m_hWnd, &tvi );
		}
		
		UINT SetScrollTime(UINT uScrollTime) 
		{ return TreeView_SetScrollTime( m_hWnd, uScrollTime ); }
		
		COLORREF SetTextColor(COLORREF clrText) 
		{ return TreeView_SetTextColor( m_hWnd, clrText ); }
		
		HWND SetToolTips(HWND hwndTooltip) 
		{ return TreeView_SetToolTips( m_hWnd, hwndTooltip ); }

// Operations
		HIMAGELIST CreateDragImage(HTREEITEM hItem) 
		{ return TreeView_CreateDragImage( m_hWnd, hItem ); }
		
		BOOL DeletAllItems() 
		{return TreeView_DeleteAllItems( m_hWnd ); }
		
		BOOL DeleteItem(HTREEITEM hItem) 
		{ return TreeView_DeleteItem( m_hWnd, hItem ); }
		
		HWND EditLabel(HTREEITEM hItem) 
		{ return TreeView_EditLabel( m_hWnd, hItem ); }
		
		BOOL EnsureVisible(HTREEITEM hItem) 
		{ return TreeView_EnsureVisible( m_hWnd, hItem ); }
		
		BOOL Expand(HTREEITEM hItem, UINT nCode) 
		{ return TreeView_Expand( m_hWnd, hItem, nCode ); }
		
		HTREEITEM HitTest(LPTVHITTESTINFO pht) 
		{ return TreeView_HitTest( m_hWnd, pht ); }
		
		HTREEITEM InsertItem(LPTVINSERTSTRUCT pis) 
		{ return TreeView_InsertItem( m_hWnd, pis ); }
		
		BOOL Select(HTREEITEM hitem, UINT flag) 
		{ return TreeView_Select(m_hWnd, hitem, flag ); }
		
		BOOL SelectDropTarget(HTREEITEM hItem) 
		{ return TreeView_SelectDropTarget(m_hWnd, hItem); }
		
		BOOL SelectItem(HTREEITEM hItem, DWORD dwFlag = 0) 
		{ return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, (WPARAM)dwFlag, (LPARAM)hItem ); }
		
		BOOL SelectSetFirstVisible(HTREEITEM hItem) 
		{ return TreeView_SelectSetFirstVisible(m_hWnd, hItem); }
		
		BOOL SortChildren(HTREEITEM hItem, BOOL fRecurse) 
		{ return TreeView_SortChildren( m_hWnd, hItem, fRecurse ); }
		
		BOOL SortChildrenCB(LPTVSORTCB psort, BOOL fRecurse) 
		{ return TreeView_SortChildrenCB( m_hWnd, psort, fRecurse ); }
	};

} // namespace Win32xx

#endif // #ifndef TREEVIEW_H

