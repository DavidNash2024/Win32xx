// Win32++  Version 6.5
// Released: 22nd May, 2009 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
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
		BOOL GetISearchString(LPTSTR szString);
		BOOL GetItem(TVITEM& Item) const;
		DWORD_PTR GetItemData(HTREEITEM hItem) const;
		int  GetItemHeight() const;
		BOOL GetItemImage(HTREEITEM hItem, int& nImage, int& nSelectedImage ) const;
		BOOL GetItemRect(HTREEITEM hItem, CRect& rc, BOOL bTextOnly) const;
		tString GetItemText(HTREEITEM hItem) const;
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

	};
	
}


#endif // #ifndef _TREEVIEW_H_

