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
		virtual void PreCreate(CREATESTRUCT &cs);

// Attributes
		COLORREF GetBkColor() const;
		HTREEITEM GetChildItem(HTREEITEM hItem) const;
		UINT  GetCount() const;
		HTREEITEM GetDropHiLightItem() const;
		HWND GetEditControl() const;
		HTREEITEM GetFirstVisibleItem() const;
		HIMAGELIST GetImageList(int iImageType) const;
		UINT  GetIndent() const;
		COLORREF GetInsertMarkColor() const;
		BOOL GetItem(TVITEM* pItem) const;
		DWORD_PTR GetItemData(HTREEITEM hItem) const;
		int  GetItemHeight() const;
		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const;
		BOOL GetItemRect(HTREEITEM hItem, LPRECT pRect, BOOL bTextOnly) const;
		tString GetItemText(HTREEITEM hItem) const;
		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const;
		HTREEITEM GetNextSiblingItem(HTREEITEM hItem) const;
		HTREEITEM GetNextVisibleItem(HTREEITEM hItem) const;
		HTREEITEM GetParentItem(HTREEITEM hItem) const;
		HTREEITEM GetPrevSiblingItem(HTREEITEM hItem) const;
		HTREEITEM GetPrevVisibleItem(HTREEITEM hItem) const;
		HTREEITEM GetRootItem() const;
		int GetScrollTime() const;
		HTREEITEM GetSelectedItem() const;
		COLORREF GetTextColor() const;
		HWND GetToolTips() const;
		UINT GetVisibleCount() const;
		BOOL ItemHasChildren(HTREEITEM hItem) const;
		COLORREF SetBkColor(COLORREF clrBk);
		HIMAGELIST SetImageList(HIMAGELIST himl, int nType);
		void SetIndent(int indent);
		BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE);
		COLORREF SetInsertMarkColor(COLORREF clrInsertMark);
		BOOL SetItem(TVITEM tvItem);
		BOOL SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR szText, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam);
		BOOL SetItemData(HTREEITEM hItem, DWORD_PTR dwData);
		int  SetItemHeight(SHORT cyItem);
		BOOL SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage);
		BOOL SetItemText(HTREEITEM hItem, LPCTSTR szText);
		UINT SetScrollTime(UINT uScrollTime);
		COLORREF SetTextColor(COLORREF clrText);
		HWND SetToolTips(HWND hwndTooltip);

// Operations
		HIMAGELIST CreateDragImage(HTREEITEM hItem);
		BOOL DeletAllItems();
		BOOL DeleteItem(HTREEITEM hItem);
		HWND EditLabel(HTREEITEM hItem);
		BOOL EnsureVisible(HTREEITEM hItem);
		BOOL Expand(HTREEITEM hItem, UINT nCode);
		HTREEITEM HitTest(LPTVHITTESTINFO pht);
		HTREEITEM InsertItem(LPTVINSERTSTRUCT pis);
		BOOL Select(HTREEITEM hitem, UINT flag);
		BOOL SelectDropTarget(HTREEITEM hItem);
		BOOL SelectItem(HTREEITEM hItem, DWORD dwFlag = 0);
		BOOL SelectSetFirstVisible(HTREEITEM hItem);
		BOOL SortChildren(HTREEITEM hItem, BOOL fRecurse);
		BOOL SortChildrenCB(LPTVSORTCB psort, BOOL fRecurse);

	};


	inline void CTreeView::PreCreate(CREATESTRUCT &cs)
	{ 
		cs.lpszClass = WC_TREEVIEW; 
	}

// Attributes
	inline COLORREF CTreeView::GetBkColor() const
	{ 
		return TreeView_GetBkColor( m_hWnd ); 
	}

	inline HTREEITEM CTreeView::GetChildItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetChild(m_hWnd, hItem); 
	}

	inline UINT  CTreeView::GetCount() const
	{ 
		return TreeView_GetCount( m_hWnd ); 
	}

	inline HTREEITEM CTreeView::GetDropHiLightItem() const
	{ 
		return TreeView_GetDropHilight(m_hWnd); 
	}

	inline HWND CTreeView::GetEditControl() const
	{ 
		return TreeView_GetEditControl( m_hWnd ); 
	}

	inline HTREEITEM CTreeView::GetFirstVisibleItem() const
	{ 
		return TreeView_GetFirstVisible(m_hWnd); 
	}

	inline HIMAGELIST CTreeView::GetImageList(int iImageType) const
	{ 
		return TreeView_GetImageList( m_hWnd, iImageType ); 
	}

	inline UINT  CTreeView::GetIndent() const
	{ 
		return TreeView_GetIndent( m_hWnd ); 
	}

	inline COLORREF CTreeView::GetInsertMarkColor() const
	{ 
		return TreeView_GetInsertMarkColor( m_hWnd ); 
	}

	inline BOOL CTreeView::GetItem(TVITEM* pItem) const
	{ 
		return TreeView_GetItem( m_hWnd, pItem ); 
	}

	inline DWORD_PTR CTreeView::GetItemData(HTREEITEM hItem) const
	{
		TVITEM tvi = {0};
		tvi.mask = TVIF_PARAM;
		tvi.hItem = hItem;
		return TreeView_GetItem( m_hWnd, hItem );
	}

	inline int  CTreeView::GetItemHeight() const
	{ 
		return TreeView_GetItemHeight( m_hWnd ); 
	}

	inline BOOL CTreeView::GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const
	{
		TVITEM tvi = {0};
		tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.hItem = hItem;
		BOOL bResult = TreeView_GetItem( m_hWnd, &tvi );
		nImage = tvi.iImage;
		nSelectedImage = tvi.iSelectedImage;
		return bResult;
	}

	inline BOOL CTreeView::GetItemRect(HTREEITEM hItem, LPRECT pRect, BOOL bTextOnly) const
	{ 
		return TreeView_GetItemRect( m_hWnd, hItem, pRect, bTextOnly ); 
	}

	inline tString CTreeView::GetItemText(HTREEITEM hItem) const
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

	inline HTREEITEM CTreeView::GetNextItem(HTREEITEM hItem, UINT nCode) const
	{ 
		return TreeView_GetNextItem( m_hWnd, hItem, nCode); 
	}

	inline HTREEITEM CTreeView::GetNextSiblingItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetNextSibling(m_hWnd, hItem); 
	}

	inline HTREEITEM CTreeView::GetNextVisibleItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetNextVisible(m_hWnd, hItem); 
	}

	inline HTREEITEM CTreeView::GetParentItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetParent(m_hWnd, hItem); 
	}

	inline HTREEITEM CTreeView::GetPrevSiblingItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetPrevSibling(m_hWnd, hItem); 
	}

	inline HTREEITEM CTreeView::GetPrevVisibleItem(HTREEITEM hItem) const
	{ 
		return TreeView_GetPrevSibling(m_hWnd, hItem); 
	}

	inline HTREEITEM CTreeView::GetRootItem() const
	{ 
		return TreeView_GetRoot(m_hWnd); 
	}

	inline int CTreeView::GetScrollTime() const
	{ 
		return TreeView_GetScrollTime( m_hWnd ); 
	}

	inline HTREEITEM CTreeView::GetSelectedItem() const
	{ 
		return TreeView_GetSelection(m_hWnd); 
	}

	inline COLORREF CTreeView::GetTextColor() const
	{ 
		return TreeView_GetTextColor( m_hWnd ); 
	}

	inline HWND CTreeView::GetToolTips() const
	{ 
		return TreeView_GetToolTips( m_hWnd ); 
	}

	inline UINT CTreeView::GetVisibleCount() const
	{ 
		return TreeView_GetVisibleCount( m_hWnd ); 
	}

	inline BOOL CTreeView::ItemHasChildren(HTREEITEM hItem) const
	{ 
		return (BOOL)TreeView_GetChild( m_hWnd, hItem ); 
	}

	inline COLORREF CTreeView::SetBkColor(COLORREF clrBk)
	{ 
		return TreeView_SetBkColor( m_hWnd, clrBk ); 
	}

	inline HIMAGELIST CTreeView::SetImageList(HIMAGELIST himl, int nType)
	{ 
		return TreeView_SetImageList( m_hWnd, himl, nType ); 
	}

	inline void CTreeView::SetIndent(int indent)
	{ 
		TreeView_SetIndent( m_hWnd, indent ); 
	}

	inline BOOL CTreeView::SetInsertMark(HTREEITEM hItem, BOOL fAfter/* = TRUE*/)
	{ 
		return TreeView_SetInsertMark( m_hWnd, hItem, fAfter ); 
	}

	inline COLORREF CTreeView::SetInsertMarkColor(COLORREF clrInsertMark)
	{ 
		return TreeView_SetInsertMarkColor( m_hWnd, clrInsertMark ); 
	}

	inline BOOL CTreeView::SetItem(TVITEM tvItem)
	{ 
		return TreeView_SetItem( m_hWnd, &tvItem ); 
	}

	inline BOOL CTreeView::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR szText, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
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

	inline BOOL CTreeView::SetItemData(HTREEITEM hItem, DWORD_PTR dwData)
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.mask = TVIF_PARAM;
		tvi.lParam = dwData;
		return TreeView_SetItem( m_hWnd, &tvi );
	}
	
	inline int  CTreeView::SetItemHeight(SHORT cyItem)
	{ 
		return TreeView_SetItemHeight( m_hWnd, cyItem ); 
	}

	inline BOOL CTreeView::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.iImage = nImage;
		tvi.iSelectedImage = nSelectedImage;
		tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		return TreeView_SetItem(m_hWnd, &tvi );
	}

	inline BOOL CTreeView::SetItemText(HTREEITEM hItem, LPCTSTR szText)
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.pszText = (LPTSTR)szText;
		tvi.mask = TVIF_TEXT;
		return TreeView_SetItem(m_hWnd, &tvi );
	}

	inline UINT CTreeView::SetScrollTime(UINT uScrollTime)
	{ 
		return TreeView_SetScrollTime( m_hWnd, uScrollTime ); 
	}

	inline COLORREF CTreeView::SetTextColor(COLORREF clrText)
	{ 
		return TreeView_SetTextColor( m_hWnd, clrText ); 
	}

	inline HWND CTreeView::SetToolTips(HWND hwndTooltip)
	{ 
		return TreeView_SetToolTips( m_hWnd, hwndTooltip ); 
	}

	// Operations
	inline HIMAGELIST CTreeView::CreateDragImage(HTREEITEM hItem)
	{ 
		return TreeView_CreateDragImage( m_hWnd, hItem ); 
	}

	inline BOOL CTreeView::DeletAllItems()
	{
		return TreeView_DeleteAllItems( m_hWnd ); 
	}

	inline BOOL CTreeView::DeleteItem(HTREEITEM hItem)
	{ 
		return TreeView_DeleteItem( m_hWnd, hItem ); 
	}

	inline HWND CTreeView::EditLabel(HTREEITEM hItem)
	{ 
		return TreeView_EditLabel( m_hWnd, hItem ); 
	}

	inline BOOL CTreeView::EnsureVisible(HTREEITEM hItem)
	{ 
		return TreeView_EnsureVisible( m_hWnd, hItem ); 
	}

	inline BOOL CTreeView::Expand(HTREEITEM hItem, UINT nCode)
	{ 
		return TreeView_Expand( m_hWnd, hItem, nCode ); 
	}

	inline HTREEITEM CTreeView::HitTest(LPTVHITTESTINFO pht)
	{ 
		return TreeView_HitTest( m_hWnd, pht ); 
	}

	inline HTREEITEM CTreeView::InsertItem(LPTVINSERTSTRUCT pis)
	{ 
		return TreeView_InsertItem( m_hWnd, pis ); 
	}

	inline BOOL CTreeView::Select(HTREEITEM hitem, UINT flag)
	{ 
		return TreeView_Select(m_hWnd, hitem, flag ); 
	}

	inline BOOL CTreeView::SelectDropTarget(HTREEITEM hItem)
	{ 
		return TreeView_SelectDropTarget(m_hWnd, hItem); 
	}

	inline BOOL CTreeView::SelectItem(HTREEITEM hItem, DWORD dwFlag /*= 0*/)
	{ 
		return (BOOL)::SendMessage(m_hWnd, TVM_SELECTITEM, (WPARAM)dwFlag, (LPARAM)hItem ); 
	}

	inline BOOL CTreeView::SelectSetFirstVisible(HTREEITEM hItem)
	{ 
		return TreeView_SelectSetFirstVisible(m_hWnd, hItem); 
	}

	inline BOOL CTreeView::SortChildren(HTREEITEM hItem, BOOL fRecurse)
	{ 
		return TreeView_SortChildren( m_hWnd, hItem, fRecurse ); 
	}

	inline BOOL CTreeView::SortChildrenCB(LPTVSORTCB psort, BOOL fRecurse)
	{ 
		return TreeView_SortChildrenCB( m_hWnd, psort, fRecurse ); 
	}


} // namespace Win32xx

#endif // #ifndef TREEVIEW_H

