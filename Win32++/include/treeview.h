// Win32++  Version 6.7
// Released: 6th November, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2009  David Nash
//
// Permission is hereby granted, free of charge, to
// any person obtaining a copy of this software and
// associated documentation files (the "Software"),
// to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify,
// merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom
// the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
// ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN
// ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE
// OR OTHER DEALINGS IN THE SOFTWARE.
//
////////////////////////////////////////////////////////




#ifndef _TREEVIEW_H_
#define _TREEVIEW_H_

#include "wincore.h"
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
		HTREEITEM GetChild(HTREEITEM hItem) const;
		UINT  GetCount() const;
		HTREEITEM GetDropHiLightItem() const;
		HWND GetEditControl() const;
		HTREEITEM GetFirstVisible() const;
		HIMAGELIST GetImageList(int iImageType) const;
		UINT  GetIndent() const;
		COLORREF GetInsertMarkColor() const;
		BOOL GetItem(TVITEM& Item) const;
		DWORD_PTR GetItemData(HTREEITEM hItem) const;
		int  GetItemHeight() const;
		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const;
		BOOL GetItemRect(HTREEITEM hItem, CRect& rc, BOOL bTextOnly) const;
		tString GetItemText(HTREEITEM hItem, UINT nTextMax /* = 260 */) const;
		HTREEITEM GetLastVisible() const;
		HTREEITEM GetNextItem(HTREEITEM hItem, UINT nCode) const;
		HTREEITEM GetNextSibling(HTREEITEM hItem) const;
		HTREEITEM GetNextVisible(HTREEITEM hItem) const;
		HTREEITEM GetParentItem(HTREEITEM hItem) const;
		HTREEITEM GetPrevSibling(HTREEITEM hItem) const;
		HTREEITEM GetPrevVisible(HTREEITEM hItem) const;
		HTREEITEM GetRootItem() const;
		int GetScrollTime() const;
		HTREEITEM GetSelection() const;
		COLORREF GetTextColor() const;
		HWND GetToolTips() const;
		UINT GetVisibleCount() const;
		BOOL ItemHasChildren(HTREEITEM hItem) const;
		COLORREF SetBkColor(COLORREF clrBk);
		HIMAGELIST SetImageList(HIMAGELIST himl, int nType);
		void SetIndent(int indent);
		BOOL SetInsertMark(HTREEITEM hItem, BOOL fAfter = TRUE);
		COLORREF SetInsertMarkColor(COLORREF clrInsertMark);
		BOOL SetItem(TVITEM& Item);
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
		BOOL DeleteAllItems();
		BOOL DeleteItem(HTREEITEM hItem);
		HWND EditLabel(HTREEITEM hItem);
		BOOL EndEditLabelNow(BOOL fCancel);
		BOOL EnsureVisible(HTREEITEM hItem);
		BOOL Expand(HTREEITEM hItem, UINT nCode);
		HTREEITEM HitTest(TVHITTESTINFO& ht);
		HTREEITEM InsertItem(TVINSERTSTRUCT& tvIS);
		BOOL Select(HTREEITEM hitem, UINT flag);
		BOOL SelectDropTarget(HTREEITEM hItem);
		BOOL SelectItem(HTREEITEM hItem);
		BOOL SelectSetFirstVisible(HTREEITEM hItem);
		BOOL SortChildren(HTREEITEM hItem, BOOL fRecurse);
		BOOL SortChildrenCB(TVSORTCB& sort, BOOL fRecurse);

	private:
		CTreeView(const CTreeView&);				// Disable copy construction
		CTreeView& operator = (const CTreeView&); // Disable assignment operator

	};
	
}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{


	inline void CTreeView::PreCreate(CREATESTRUCT &cs)
	{
		cs.lpszClass = WC_TREEVIEW;
	}

// Attributes
	inline COLORREF CTreeView::GetBkColor() const
	// Retrieves the current background color of the control.
	{
		return TreeView_GetBkColor( m_hWnd );
	}

	inline HTREEITEM CTreeView::GetChild(HTREEITEM hItem) const
	// Retrieves the first child item of the specified tree-view item.
	{
		return TreeView_GetChild(m_hWnd, hItem);
	}

	inline UINT  CTreeView::GetCount() const
	// Retrieves a count of the items in a tree-view control.
	{
		return TreeView_GetCount( m_hWnd );
	}

	inline HTREEITEM CTreeView::GetDropHiLightItem() const
	// Retrieves the tree-view item that is the target of a drag-and-drop operation.
	{
		return TreeView_GetDropHilight(m_hWnd);
	}

	inline HWND CTreeView::GetEditControl() const
	// Retrieves the handle to the edit control being used to edit a tree-view item's text.
	{
		return TreeView_GetEditControl( m_hWnd );
	}

	inline HTREEITEM CTreeView::GetFirstVisible() const
	// Retrieves the first visible item in a tree-view control window.
	{
		return TreeView_GetFirstVisible(m_hWnd);
	}

	inline HIMAGELIST CTreeView::GetImageList(int iImageType) const
	// Retrieves the handle to the normal or state image list associated with a tree-view control.
	{
		return TreeView_GetImageList( m_hWnd, iImageType );
	}

	inline UINT  CTreeView::GetIndent() const
	// Retrieves the amount, in pixels, that child items are indented relative to their parent items.
	{
		return TreeView_GetIndent( m_hWnd );
	}

	inline COLORREF CTreeView::GetInsertMarkColor() const
	// Retrieves the color used to draw the insertion mark for the tree view.
	{
		return TreeView_GetInsertMarkColor( m_hWnd );
	}

	inline BOOL CTreeView::GetItem(TVITEM& Item) const
	// Retrieves some or all of a tree-view item's attributes.
	{
		return TreeView_GetItem( m_hWnd, &Item );
	}

	inline DWORD_PTR CTreeView::GetItemData(HTREEITEM hItem) const
	// Retrieves a tree-view item's application data.
	{
		TVITEM tvi = {0};
		tvi.mask = TVIF_PARAM;
		tvi.hItem = hItem;
		TreeView_GetItem( m_hWnd, &tvi );
		return tvi.lParam;
	}

	inline int  CTreeView::GetItemHeight() const
	// Retrieves the current height of the tree-view item.
	{
		return TreeView_GetItemHeight( m_hWnd );
	}

	inline BOOL CTreeView::GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const
	// Retrieves the index of the tree-view item's image and selected image.
	{
		TVITEM tvi = {0};
		tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		tvi.hItem = hItem;
		BOOL bResult = TreeView_GetItem( m_hWnd, &tvi );
		nImage = tvi.iImage;
		nSelectedImage = tvi.iSelectedImage;
		return bResult;
	}

	inline BOOL CTreeView::GetItemRect(HTREEITEM hItem, CRect& rc, BOOL bTextOnly) const
	// Retrieves the bounding rectangle for a tree-view item and indicates whether the item is visible.
	{
		return TreeView_GetItemRect( m_hWnd, hItem, &rc, bTextOnly );
	}

	inline tString CTreeView::GetItemText(HTREEITEM hItem, UINT nTextMax /* = 260 */) const
	// Retrieves the text for a tree-view item.
	// Note: Although the tree-view control allows any length string to be stored 
	//       as item text, only the first 260 characters are displayed.
	{
		tString t;
		if (nTextMax > 0)
		{
			TVITEM tvi = {0};
			tvi.hItem = hItem;
			tvi.mask = TVIF_TEXT;
			tvi.cchTextMax = nTextMax;
			TCHAR* pszText = new TCHAR[nTextMax +1];
			if (NULL == pszText) throw std::bad_alloc();
			tvi.pszText = pszText;
			::SendMessage(m_hWnd, TVM_GETITEM, 0L, (LPARAM)&tvi);
			t = tvi.pszText;
			delete [] pszText;
		}
		return t;
	}

	inline HTREEITEM CTreeView::GetLastVisible() const
	// Retrieves the last expanded item in a tree-view control.
	// This does not retrieve the last item visible in the tree-view window.
	{
		return TreeView_GetLastVisible(m_hWnd);
	}

	inline HTREEITEM CTreeView::GetNextItem(HTREEITEM hItem, UINT nCode) const
	// Retrieves the tree-view item that bears the specified relationship to a specified item.
	{
		return TreeView_GetNextItem( m_hWnd, hItem, nCode);
	}

	inline HTREEITEM CTreeView::GetNextSibling(HTREEITEM hItem) const
	// Retrieves the next sibling item of a specified item in a tree-view control.
	{
		return TreeView_GetNextSibling(m_hWnd, hItem);
	}

	inline HTREEITEM CTreeView::GetNextVisible(HTREEITEM hItem) const
	// Retrieves the next visible item that follows a specified item in a tree-view control.
	{
		return TreeView_GetNextVisible(m_hWnd, hItem);
	}

	inline HTREEITEM CTreeView::GetParentItem(HTREEITEM hItem) const
	// Retrieves the parent item of the specified tree-view item.
	{
		return TreeView_GetParent(m_hWnd, hItem);
	}

	inline HTREEITEM CTreeView::GetPrevSibling(HTREEITEM hItem) const
	// Retrieves the previous sibling item of a specified item in a tree-view control.
	{
		return TreeView_GetPrevSibling(m_hWnd, hItem);
	}

	inline HTREEITEM CTreeView::GetPrevVisible(HTREEITEM hItem) const
	// Retrieves the first visible item that precedes a specified item in a tree-view control.
	{
		return TreeView_GetPrevSibling(m_hWnd, hItem);
	}

	inline HTREEITEM CTreeView::GetRootItem() const
	// Retrieves the topmost or very first item of the tree-view control.
	{
		return TreeView_GetRoot(m_hWnd);
	}

	inline int CTreeView::GetScrollTime() const
	// Retrieves the maximum scroll time for the tree-view control.
	{
		return TreeView_GetScrollTime( m_hWnd );
	}

	inline HTREEITEM CTreeView::GetSelection() const
	// Retrieves the currently selected item in a tree-view control.
	{
		return TreeView_GetSelection(m_hWnd);
	}

	inline COLORREF CTreeView::GetTextColor() const
	// Retrieves the current text color of the control.
	{
		return TreeView_GetTextColor( m_hWnd );
	}

	inline HWND CTreeView::GetToolTips() const
	// Retrieves the handle to the child ToolTip control used by a tree-view control.
	{
		return TreeView_GetToolTips( m_hWnd );
	}

	inline UINT CTreeView::GetVisibleCount() const
	// Obtains the number of items that can be fully visible in the client window of a tree-view control.
	{
		return TreeView_GetVisibleCount( m_hWnd );
	}

	inline BOOL CTreeView::ItemHasChildren(HTREEITEM hItem) const
	// Returns true of the tree-view item has one or more children
	{
		if (TreeView_GetChild( m_hWnd, hItem ))
			return TRUE;

		return FALSE;
	}

	inline COLORREF CTreeView::SetBkColor(COLORREF clrBk)
	// Sets the background color of the control.
	{
		return TreeView_SetBkColor( m_hWnd, clrBk );
	}

	inline HIMAGELIST CTreeView::SetImageList(HIMAGELIST himl, int nType)
	// Sets the normal or state image list for a tree-view control
	//  and redraws the control using the new images.
	{
		return TreeView_SetImageList( m_hWnd, himl, nType );
	}

	inline void CTreeView::SetIndent(int indent)
	// Sets the width of indentation for a tree-view control
	//  and redraws the control to reflect the new width.
	{
		TreeView_SetIndent( m_hWnd, indent );
	}

	inline BOOL CTreeView::SetInsertMark(HTREEITEM hItem, BOOL fAfter/* = TRUE*/)
	// Sets the insertion mark in a tree-view control.
	{
		return TreeView_SetInsertMark( m_hWnd, hItem, fAfter );
	}

	inline COLORREF CTreeView::SetInsertMarkColor(COLORREF clrInsertMark)
	// Sets the color used to draw the insertion mark for the tree view.
	{
		return TreeView_SetInsertMarkColor( m_hWnd, clrInsertMark );
	}

	inline BOOL CTreeView::SetItem(TVITEM& Item)
	// Sets some or all of a tree-view item's attributes.
	{
		return TreeView_SetItem( m_hWnd, &Item );
	}

	inline BOOL CTreeView::SetItem(HTREEITEM hItem, UINT nMask, LPCTSTR szText, int nImage, int nSelectedImage, UINT nState, UINT nStateMask, LPARAM lParam)
	// Sets some or all of a tree-view item's attributes.
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
	// Sets the tree-view item's application data.
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.mask = TVIF_PARAM;
		tvi.lParam = dwData;
		return TreeView_SetItem( m_hWnd, &tvi );
	}

	inline int  CTreeView::SetItemHeight(SHORT cyItem)
	// Sets the height of the tree-view items.
	{
		return TreeView_SetItemHeight( m_hWnd, cyItem );
	}

	inline BOOL CTreeView::SetItemImage(HTREEITEM hItem, int nImage, int nSelectedImage)
	// Sets the tree-view item's application image.
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.iImage = nImage;
		tvi.iSelectedImage = nSelectedImage;
		tvi.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE;
		return TreeView_SetItem(m_hWnd, &tvi );
	}

	inline BOOL CTreeView::SetItemText(HTREEITEM hItem, LPCTSTR szText)
	// Sets the tree-view item's application text.
	{
		TVITEM tvi = {0};
		tvi.hItem = hItem;
		tvi.pszText = (LPTSTR)szText;
		tvi.mask = TVIF_TEXT;
		return TreeView_SetItem(m_hWnd, &tvi );
	}

	inline UINT CTreeView::SetScrollTime(UINT uScrollTime)
	// Sets the maximum scroll time for the tree-view control.
	{
		return TreeView_SetScrollTime( m_hWnd, uScrollTime );
	}

	inline COLORREF CTreeView::SetTextColor(COLORREF clrText)
	// Sets the text color of the control.
	{
		return TreeView_SetTextColor( m_hWnd, clrText );
	}

	inline HWND CTreeView::SetToolTips(HWND hwndTooltip)
	// Sets a tree-view control's child ToolTip control.
	{
		return TreeView_SetToolTips( m_hWnd, hwndTooltip );
	}

	// Operations

	inline HIMAGELIST CTreeView::CreateDragImage(HTREEITEM hItem)
	// Creates a dragging bitmap for the specified item in a tree-view control.
	// It also creates an image list for the bitmap and adds the bitmap to the image list.
	// An application can display the image when dragging the item by using the image list functions.
	{
		return TreeView_CreateDragImage( m_hWnd, hItem );
	}

	inline BOOL CTreeView::DeleteAllItems()
	// Deletes all items from a tree-view control.
	{
		return TreeView_DeleteAllItems( m_hWnd );
	}

	inline BOOL CTreeView::DeleteItem(HTREEITEM hItem)
	// Removes an item and all its children from a tree-view control.
	{
		return TreeView_DeleteItem( m_hWnd, hItem );
	}

	inline HWND CTreeView::EditLabel(HTREEITEM hItem)
	// Begins in-place editing of the specified item's text, replacing the text of the item
	// with a single-line edit control containing the text.
	// The specified item  is implicitly selected and focused.
	{
		return TreeView_EditLabel( m_hWnd, hItem );
	}

	inline BOOL CTreeView::EndEditLabelNow(BOOL fCancel)
	// Ends the editing of a tree-view item's label.
	{
		return TreeView_EndEditLabelNow(m_hWnd, fCancel);
	}

	inline BOOL CTreeView::EnsureVisible(HTREEITEM hItem)
	// Ensures that a tree-view item is visible, expanding the parent item or
	// scrolling the tree-view control, if necessary.
	{
		return TreeView_EnsureVisible( m_hWnd, hItem );
	}

	inline BOOL CTreeView::Expand(HTREEITEM hItem, UINT nCode)
	// The TreeView_Expand macro expands or collapses the list of child items associated
	// with the specified parent item, if any.
	{
		return TreeView_Expand( m_hWnd, hItem, nCode );
	}

	inline HTREEITEM CTreeView::HitTest(TVHITTESTINFO& ht)
	// Determines the location of the specified point relative to the client area of a tree-view control.
	{
		return TreeView_HitTest( m_hWnd, &ht );
	}

	inline HTREEITEM CTreeView::InsertItem(TVINSERTSTRUCT& tvIS)
	// Inserts a new item in a tree-view control.
	{
		return TreeView_InsertItem( m_hWnd, &tvIS );
	}

	inline BOOL CTreeView::Select(HTREEITEM hitem, UINT flag)
	// Selects the specified tree-view item, scrolls the item into view, or redraws
	// the item in the style used to indicate the target of a drag-and-drop operation.
	{
		return TreeView_Select(m_hWnd, hitem, flag );
	}

	inline BOOL CTreeView::SelectDropTarget(HTREEITEM hItem)
	// Redraws a specified tree-view control item in the style used to indicate the
	// target of a drag-and-drop operation.
	{
		return TreeView_SelectDropTarget(m_hWnd, hItem);
	}

	inline BOOL CTreeView::SelectItem(HTREEITEM hItem)
	// Selects the specified tree-view item.
	{
		return TreeView_SelectItem(m_hWnd, hItem);
	}

	inline BOOL CTreeView::SelectSetFirstVisible(HTREEITEM hItem)
	// Scrolls the tree-view control vertically to ensure that the specified item is visible.
	// If possible, the specified item becomes the first visible item at the top of the control's window.
	{
		return TreeView_SelectSetFirstVisible(m_hWnd, hItem);
	}

	inline BOOL CTreeView::SortChildren(HTREEITEM hItem, BOOL fRecurse)
	// Sorts the child items of the specified parent item in a tree-view control.
	{
		return TreeView_SortChildren( m_hWnd, hItem, fRecurse );
	}

	inline BOOL CTreeView::SortChildrenCB(TVSORTCB& sort, BOOL fRecurse)
	// Sorts tree-view items using an application-defined callback function that compares the items.
	{
		return TreeView_SortChildrenCB( m_hWnd, &sort, fRecurse );
	}


} // namespace Win32xx

#endif // #ifndef _TREEVIEW_H_

