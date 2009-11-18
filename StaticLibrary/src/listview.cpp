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

#include "listview.h"


namespace Win32xx
{

	void CListView::PreCreate( CREATESTRUCT &cs )
	// Specify the class name for the window before its created. WC_LISTVIEW is a list-view control.
	{
		cs.lpszClass = WC_LISTVIEW;
	}

	CSize CListView::ApproximateViewRect(CSize sz /*= CSize(-1, -1)*/, int iCount /* = -1*/) const
	// Calculates the approximate width and height required to display a given number of items.
	{
		return CSize( ListView_ApproximateViewRect( m_hWnd, sz.cx, sz.cy, iCount ) );
	}

	COLORREF CListView::GetBkColor( ) const
	// Retrieves the background color of a list-view control.
	{
		return ListView_GetBkColor( m_hWnd );
	}

	BOOL CListView::GetBkImage( LVBKIMAGE& lvbkImage ) const
	// Retrieves the background image in a list-view control.
	{
		return ListView_GetBkImage( m_hWnd, &lvbkImage );
	}

	UINT CListView::GetCallbackMask( ) const
	// Retrieves the callback mask for a list-view control.
	{
		return ListView_GetCallbackMask( m_hWnd );
	}

	BOOL CListView::GetCheckState( UINT nItem ) const
	// Determines if an item in a list-view control is selected.
	{
		return ListView_GetCheckState( m_hWnd, nItem );
	}

	BOOL CListView::GetColumn( int iCol, LVCOLUMN& Column ) const
	// Retrieves the attributes of a list-view control's column.
	{
		return ListView_GetColumn( m_hWnd, iCol, &Column );
	}

	BOOL CListView::GetColumnOrderArray( LPINT piArray, int iCount /*= -1*/ )
	// Retrieves the current left-to-right order of columns in a list-view control.
	{
		return ListView_GetColumnOrderArray( m_hWnd, iCount, piArray );
	}

	int CListView::GetColumnWidth( int iCol ) const
	// Retrieves the width of a column in report or list view.
	{
		return ListView_GetColumnWidth( m_hWnd, iCol );
	}

	int CListView::GetCountPerPage( ) const
	// Calculates the number of items that can fit vertically in the visible area of a
	// list-view control when in list or report view. Only fully visible items are counted.
	{
		return ListView_GetCountPerPage( m_hWnd );
	}

	HWND CListView::GetEditControl( ) const
	// Retrieves the handle to the edit control being used to edit a list-view item's text.
	{
		return ListView_GetEditControl( m_hWnd );
	}

	DWORD CListView::GetExtendedStyle( ) const
	// Retrieves the extended styles that are currently in use for a given list-view control.
	{
		return ListView_GetExtendedListViewStyle( m_hWnd );
	}

	HWND CListView::GetHeader( ) const
	// Retrieves the handle to the header control used by a list-view control.
	{
		return ListView_GetHeader( m_hWnd );
	}

	HCURSOR CListView::GetHotCursor( )
	// Retrieves the HCURSOR used when the pointer is over an item while hot tracking is enabled.
	{
		return ListView_GetHotCursor( m_hWnd );
	}

	int CListView::GetHotItem( ) const
	// Retrieves the index of the hot item.
	{
		return ListView_GetHotItem( m_hWnd );
	}

	DWORD CListView::GetHoverTime( ) const
	// Retrieves the amount of time that the mouse cursor must hover over an item before it is selected.
	{
		return ListView_GetHoverTime( m_hWnd );
	}

	HIMAGELIST CListView::GetImageList( int nImageType ) const
	// Retrieves the handle to an image list used for drawing list-view items.
	{
		return ListView_GetImageList( m_hWnd, nImageType );
	}

	BOOL CListView::GetItem( LVITEM& Item ) const
	// Retrieves some or all of a list-view item's attributes.
	{
		return ListView_GetItem( m_hWnd, &Item );
	}

	int CListView::GetItemCount( ) const
	// Retrieves the number of items in a list-view control.
	{
		return ListView_GetItemCount( m_hWnd );
	}

	DWORD_PTR CListView::GetItemData( int iItem ) const
	// Retrieves the value(lParam) specific to the item.
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.mask = LVIF_PARAM;
		ListView_GetItem(m_hWnd, &lvi);
		return lvi.lParam;
	}

	BOOL CListView::GetItemPosition( int iItem, CPoint& pt ) const
	// Retrieves the position of a list-view item.
	{
		return ListView_GetItemPosition( m_hWnd, iItem, &pt );
	}

	BOOL CListView::GetItemRect( int iItem, CRect& rc, UINT nCode ) const
	// Retrieves the bounding rectangle for all or part of an item in the current view.
	{
		return ListView_GetItemRect( m_hWnd, iItem, &rc, nCode );
	}

	UINT CListView::GetItemState( int iItem, UINT nMask ) const
	// Retrieves the state of a list-view item.

	// Possible values of nMask:
	// LVIS_CUT				The item is marked for a cut-and-paste operation.
	// LVIS_DROPHILITED		The item is highlighted as a drag-and-drop target.
	// LVIS_FOCUSED			The item has the focus, so it is surrounded by a standard focus rectangle.
	// LVIS_SELECTED		The item is selected.
	// LVIS_OVERLAYMASK		Use this mask to retrieve the item's overlay image index.
	// LVIS_STATEIMAGEMASK	Use this mask to retrieve the item's state image index.
	{
		return  ListView_GetItemState( m_hWnd, iItem, nMask );
	}

	tString CListView::GetItemText( int iItem, int iSubItem, UINT nTextMax /* = 260 */ ) const
	// Retrieves the text of a list-view item.
	// Note: Although the list-view control allows any length string to be stored
	//       as item text, only the first 260 characters are displayed.
	{
		tString t;
		if (nTextMax > 0)
		{
			TCHAR* pszText = new TCHAR[nTextMax +1];
			if (NULL == pszText) throw std::bad_alloc();
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.iSubItem = iSubItem;
			lvi.mask = LVIF_TEXT;
			lvi.cchTextMax = nTextMax;
			lvi.pszText = pszText;
			ListView_GetItem( m_hWnd, &lvi );
			t = lvi.pszText;
			delete [] pszText;
		}
		return t;
	}

	int CListView::GetNextItem( int iItem, int iFlags ) const
	// Searches for a list-view item that has the specified properties and
	// bears the specified relationship to a specified item.
	{
		return ListView_GetNextItem( m_hWnd, iItem, iFlags );
	}

	UINT CListView::GetNumberOfWorkAreas( ) const
	// Retrieves the working areas from a list-view control.
	{
		UINT nWorkAreas = 0;
		ListView_GetWorkAreas( m_hWnd, nWorkAreas, NULL );
		return nWorkAreas;
	}

	BOOL CListView::GetOrigin( CPoint& pt ) const
	// Retrieves the current view origin for a list-view control.
	{
		return ListView_GetOrigin( m_hWnd, &pt );
	}

	UINT CListView::GetSelectedCount( ) const
	// Determines the number of selected items in a list-view control.
	{
		return (UINT)::SendMessage( m_hWnd, LVM_GETSELECTEDCOUNT, 0L, 0L );
	}

	int CListView::GetSelectionMark( )
	// Retrieves the selection mark from a list-view control.
	{
		return (int)::SendMessage( m_hWnd, LVM_GETSELECTIONMARK, 0L, 0L );
	}

	int CListView::GetStringWidth( LPCTSTR pszString ) const
	// Determines the width of a specified string using the specified list-view control's current font.
	{
		return (int)::SendMessage( m_hWnd, LVM_GETSTRINGWIDTH, 0L, (LPARAM)pszString );
	}

	BOOL CListView::GetSubItemRect( int iItem, int iSubItem, int iCode, CRect& rc )
	// Retrieves information about the rectangle that surrounds a subitem in a list-view control.
	{
		return ListView_GetSubItemRect( m_hWnd, iItem, iSubItem, iCode, &rc );
	}

	COLORREF CListView::GetTextBkColor( ) const
	// Retrieves the text background color of a list-view control.
	{
		return ListView_GetTextBkColor( m_hWnd );
	}

	COLORREF CListView::GetTextColor( ) const
	// Retrieves the text color of a list-view control.
	{
		return ListView_GetTextColor( m_hWnd );
	}

	HWND CListView::GetToolTips( ) const
	// Retrieves the ToolTip control that the list-view control uses to display ToolTips.
	{
		return ListView_GetToolTips( m_hWnd );
	}

	int CListView::GetTopIndex( ) const
	// Retrieves the index of the topmost visible item when in list or report view.
	{
		return ListView_GetTopIndex( m_hWnd );
	}

	BOOL CListView::GetViewRect( CRect& rc ) const
	// Retrieves the bounding rectangle of all items in the list-view control.
	{
		return ListView_GetViewRect( m_hWnd, &rc );
	}

	void CListView::GetWorkAreas( int iWorkAreas, LPRECT pRectArray ) const
	// Retrieves the working areas from a list-view control.
	{
		ListView_GetWorkAreas( m_hWnd, iWorkAreas, pRectArray );
	}

	BOOL CListView::SetBkColor( COLORREF clrBk )
	// Sets the background color of a list-view control.
	{
		return ListView_SetBkColor( m_hWnd, clrBk );
	}

	BOOL CListView::SetBkImage( LVBKIMAGE& lvbkImage )
	// Sets the background image in a list-view control.
	{
		return ListView_SetBkImage( m_hWnd, &lvbkImage );
	}

	BOOL CListView::SetCallbackMask( UINT nMask )
	// Changes the callback mask for a list-view control.
	{
		return ListView_SetCallbackMask( m_hWnd, nMask );
	}

	void CListView::SetCheckState( int iItem, BOOL fCheck /*= TRUE*/ )
	// Used to select or deselect an item in a list-view control.
	// This macro should only be used for list-view controls with the LVS_EX_CHECKBOXES style.
	{
		ListView_SetItemState(m_hWnd, iItem, INDEXTOSTATEIMAGEMASK((fCheck==TRUE)?2:1),LVIS_STATEIMAGEMASK);
	}

	BOOL CListView::SetColumn( int iCol, const LVCOLUMN& Column )
	// Sets the attributes of a list-view column.
	{
		return ListView_SetColumn( m_hWnd, iCol, &Column );
	}

	BOOL CListView::SetColumnOrderArray( int iCount, LPINT piArray )
	// Sets the left-to-right order of columns in a list-view control.
	{
		return ListView_SetColumnOrderArray( m_hWnd, iCount, piArray );
	}

	BOOL CListView::SetColumnWidth( int iCol, int cx )
	// Used to change the width of a column in report view or the width of all columns in list-view mode.
	{
		return ListView_SetColumnWidth( m_hWnd, iCol, cx );
	}

	DWORD CListView::SetExtendedStyle( DWORD dwNewStyle )
	// Sets extended styles for list-view controls.
	{
		return ListView_SetExtendedListViewStyle( m_hWnd, dwNewStyle );
	}

	HCURSOR CListView::SetHotCursor( HCURSOR hCursor )
	// Sets the HCURSOR that the list-view control uses when the pointer is
	// over an item while hot tracking is enabled.
	{
		return ListView_SetHotCursor( m_hWnd, hCursor );
	}

	int CListView::SetHotItem( int nIndex )
	// Sets the hot item in a list-view control.
	{
		return ListView_SetHotItem( m_hWnd, nIndex );
	}

	DWORD CListView::SetHoverTime( DWORD dwHoverTime /*= (DWORD)-1*/ )
	// Sets the amount of time that the mouse cursor must hover over an item before it is selected.
	{
		return ListView_SetHoverTime( m_hWnd, dwHoverTime );
	}

	CSize CListView::SetIconSpacing( int cx, int cy )
	// Sets the spacing between icons in list-view controls set to the LVS_ICON style.
	{
		return CSize( ListView_SetIconSpacing( m_hWnd, cx, cy ) );
	}

	CSize CListView::SetIconSpacing( CSize sz )
	// Sets the spacing between icons in list-view controls set to the LVS_ICON style.
	{
		return CSize( ListView_SetIconSpacing( m_hWnd, sz.cx, sz.cy ) );
	}

	HIMAGELIST CListView::SetImageList( HIMAGELIST himl, int iImageListType )
	// Assigns an image list to a list-view control.
	{
		return ListView_SetImageList( m_hWnd, himl, iImageListType );
	}

	BOOL CListView::SetItem( LVITEM& Item )
	// Sets some or all of a list-view item's attributes.

	// The declaration for TVITEM:
	//	typedef struct _LVITEM {
	//		UINT mask;
	//		int iItem;
	//		int iSubItem;
	//		UINT state;
	//		UINT stateMask;
	//		LPTSTR pszText;
	//		int cchTextMax;
    //		int iImage;
	//		LPARAM lParam;
	// } LVITEM, *LVITEM&;
	{
		return ListView_SetItem( m_hWnd, &Item );
	}

	BOOL CListView::SetItem( int iItem, int iSubItem, UINT nMask, LPCTSTR pszText, int iImage,
					UINT nState, UINT nStateMask, LPARAM lParam, int iIndent )
	// Sets some or all of a list-view item's attributes.
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.iSubItem = iSubItem;
		lvi.mask = nMask;
		lvi.pszText = (LPTSTR)pszText;
		lvi.iImage = iImage;
		lvi.state = nState;
		lvi.stateMask = nStateMask;
		lvi.lParam = lParam;
		lvi.iIndent = iIndent;

		return ListView_SetItem( m_hWnd, &lvi);
	}

	void CListView::SetItemCount( int iCount )
	// Causes the list-view control to allocate memory for the specified number of items.
	{
		ListView_SetItemCount( m_hWnd, iCount );
	}

	void CListView::SetItemCountEx( int iCount, DWORD dwFlags /*= LVSICF_NOINVALIDATEALL*/ )
	// Sets the virtual number of items in a virtual list view.
	{
		ListView_SetItemCountEx( m_hWnd, iCount, dwFlags );
	}

	BOOL CListView::SetItemData( int iItem, DWORD_PTR dwData )
	// Sets the value(lParam) specific to the item.
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.lParam = dwData;
		lvi.mask = LVIF_PARAM;
		return ListView_SetItem(m_hWnd, &lvi);
	}

	BOOL CListView::SetItemPosition( int iItem, CPoint& pt )
	// Moves an item to a specified position in a list-view control (in icon or small icon view).
	{
		return ListView_SetItemPosition( m_hWnd, iItem, pt.x, pt.y );
	}

	BOOL CListView::SetItemState( int iItem, LVITEM& Item )
	// Changes the state of an item in a list-view control.

	// Possible values of nMask:
	// LVIS_CUT				The item is marked for a cut-and-paste operation.
	// LVIS_DROPHILITED		The item is highlighted as a drag-and-drop target.
	// LVIS_FOCUSED			The item has the focus, so it is surrounded by a standard focus rectangle.
	// LVIS_SELECTED		The item is selected.
	// LVIS_OVERLAYMASK		Use this mask to retrieve the item's overlay image index.
	// LVIS_STATEIMAGEMASK	Use this mask to retrieve the item's state image index.
	{
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, (WPARAM)iItem, (LPARAM)&Item);
	}

    void CListView::SetItemState( int iItem, UINT nState, UINT nMask )
	// Changes the state of an item in a list-view control.
	{
		ListView_SetItemState(m_hWnd, iItem, nState, nMask);
	}

	void CListView::SetItemText( int iItem, int iSubItem, LPCTSTR pszText )
	// Sets the text color of a list-view control.
	{
		ListView_SetItemText(m_hWnd, iItem, iSubItem, (LPTSTR)pszText );
	}

	int CListView::SetSelectionMark( int iIndex )
	// Sets the selection mark in a list-view control.
	{
		return ListView_SetSelectionMark( m_hWnd, iIndex );
	}

	BOOL CListView::SetTextBkColor( COLORREF clrBkText )
	// Sets the background color of text in a list-view control.
	{
		return ListView_SetTextBkColor( m_hWnd, clrBkText );
	}

	BOOL CListView::SetTextColor( COLORREF clrText )
	// Sets the text color of a list-view control.
	{
		return ListView_SetTextColor( m_hWnd, clrText );
	}

	HWND CListView::SetToolTips( HWND hWndToolTip )
	// Sets the ToolTip control that the list-view control will use to display ToolTips.
	{
		return (HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, (WPARAM)hWndToolTip, 0L);
	}

	void CListView::SetWorkAreas( int nWorkAreas, CRect& pRectArray )
	// Sets the working area within a list-view control.
	{
		ListView_SetWorkAreas( m_hWnd, nWorkAreas, pRectArray );
	}

	int CListView::SubItemHitTest( LVHITTESTINFO& htInfo )
	// Determines which list-view item or subitem is located at a given position.
	{
		return ListView_SubItemHitTest( m_hWnd, &htInfo );
	}

	// Operations

	BOOL CListView::Arrange( UINT nCode )
	// Arranges items in icon view.
	{
		return ListView_Arrange( m_hWnd, nCode );
	}

	HIMAGELIST CListView::CreateDragImage( int iItem, CPoint& pt )
	// Creates a drag image list for the specified item.
	{
		return ListView_CreateDragImage( m_hWnd, iItem, &pt );
	}

	BOOL CListView::DeleteAllItems( )
	// ListView_DeleteAllItems
	{
		return ListView_DeleteAllItems( m_hWnd );
	}

	BOOL CListView::DeleteColumn( int iCol )
	// Removes a column from a list-view control.
	{
		return ListView_DeleteColumn( m_hWnd, iCol );
	}

	BOOL CListView::DeleteItem( int iItem )
	// Removes an item from a list-view control.
	{
		return ListView_DeleteItem( m_hWnd, iItem );
	}

	HWND CListView::EditLabel( int iItem )
	// Begins in-place editing of the specified list-view item's text.
	{
		return ListView_EditLabel( m_hWnd, iItem );
	}

	BOOL CListView::EnsureVisible( int iItem, BOOL fPartialOK )
	// Ensures that a list-view item is either entirely or partially visible,
	// scrolling the list-view control if necessary.
	{
		return (BOOL)SendMessage(LVM_ENSUREVISIBLE, (WPARAM)iItem, (LPARAM)fPartialOK );
	}

	int CListView::FindItem( LVFINDINFO& FindInfo, int iStart /*= -1*/ ) const
	// Searches for a list-view item with the specified characteristics.
	{
		return ListView_FindItem( m_hWnd, iStart, &FindInfo );
	}

	int CListView::HitTest( LVHITTESTINFO& HitTestInfo ) const
	// Determines which list-view item, if any, is at a specified position.
	{
		return ListView_HitTest( m_hWnd, &HitTestInfo );
	}

    int CListView::HitTest( CPoint pt, UINT* pFlags /*= NULL*/ ) const
	// Determines which list-view item, if any, is at a specified position.
	{
		LVHITTESTINFO hti = {0};
		hti.flags = *pFlags;
		hti.pt = pt;
		return ListView_HitTest( m_hWnd, &hti );
	}

	int CListView::InsertColumn( int iCol, const LVCOLUMN& Column )
	// Inserts a new column in a list-view control.
	{
		return ListView_InsertColumn( m_hWnd, iCol, &Column );
	}

    int CListView::InsertColumn( int iCol, LPCTSTR pszColumnHeading, int iFormat /*= LVCFMT_LEFT*/,
						int iWidth /*= -1*/, int iSubItem /*= -1*/ )
	// Inserts a new column in a list-view control.
	{
		LVCOLUMN lvc = {0};
		lvc.mask = LVCF_TEXT|LVCF_ORDER|LVCF_FMT;
		if (-1 != iWidth)
		{
			lvc.mask |= LVCF_WIDTH; 
			lvc.cx = iWidth; 
		}
		if (-1 !=  iSubItem)
		{
			lvc.mask |= LVCF_SUBITEM; 
			lvc.iSubItem = iSubItem;
		}

		lvc.iOrder = iCol;
		lvc.pszText = (LPTSTR)pszColumnHeading;
		lvc.fmt = iFormat;
		lvc.iSubItem = iSubItem;
		return ListView_InsertColumn( m_hWnd, iCol, &lvc );
	}

	int CListView::InsertItem( const LVITEM& Item )
	// Inserts a new item in a list-view control.
	{
		return ListView_InsertItem( m_hWnd, &Item );
	}

    int CListView::InsertItem( int iItem, LPCTSTR pszText )
	// Inserts a new item in a list-view control.
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.pszText = (LPTSTR)pszText;
		lvi.mask = LVIF_TEXT;
		return ListView_InsertItem( m_hWnd, &lvi );
	}

    int CListView::InsertItem( int iItem, LPCTSTR pszText, int iImage )
	// Inserts a new item in a list-view control.
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.pszText = (LPTSTR)pszText;
		lvi.iImage = iImage;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		return ListView_InsertItem( m_hWnd, &lvi );
	}

	BOOL CListView::RedrawItems( int iFirst, int iLast )
	// Forces a list-view control to redraw a range of items.
	{
		return ListView_RedrawItems( m_hWnd, iFirst, iLast );
	}

	BOOL CListView::Scroll( CSize sz )
	// Scrolls the content of a list-view control.
	{
		return ListView_Scroll( m_hWnd, sz.cx, sz.cy );
	}

	BOOL CListView::SortItems( PFNLVCOMPARE pfnCompare, DWORD_PTR dwData )
	// Uses an application-defined comparison function to sort the items of a list-view control.
	{
		return ListView_SortItems( m_hWnd, pfnCompare, dwData );
	}

	BOOL CListView::Update( int iItem )
	// Updates a list-view item. If the list-view control has the LVS_AUTOARRANGE style,
	// the list-view control is rearranged.
	{
		return ListView_Update( m_hWnd, iItem );
	}

} // namespace Win32xx
