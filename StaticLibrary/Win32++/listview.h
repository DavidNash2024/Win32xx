// Win32++  Version 6.6
// Released: 20th August, 2009 by:
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



#ifndef _LISTVIEW_H_
#define _LISTVIEW_H_

#include "wincore.h"
#include "Commctrl.h"

namespace Win32xx
{

	class CListView : public CWnd
	{
	public:
		CListView() {}
		virtual ~CListView() {}
		virtual void PreCreate( CREATESTRUCT &cs );

		// Attributes
		CSize ApproximateViewRect(CSize sz = CSize(-1, -1), int iCount = -1) const;
		COLORREF GetBkColor( ) const;
		BOOL GetBkImage( LVBKIMAGE& lvbkImage ) const;
		UINT GetCallbackMask( ) const;
		BOOL GetCheckState( UINT nItem ) const;
		BOOL GetColumn( int iCol, LVCOLUMN& Column ) const;
		BOOL GetColumnOrderArray( LPINT piArray, int iCount = -1 );
		int GetColumnWidth( int iCol ) const;
		int GetCountPerPage( ) const;
		HWND GetEditControl( ) const;
		DWORD GetExtendedStyle( ) const;
		HWND GetHeader( ) const;
		HCURSOR GetHotCursor( );
		int GetHotItem( ) const;
		DWORD GetHoverTime( ) const;
		HIMAGELIST GetImageList( int nImageType ) const;
		BOOL GetItem( LVITEM& lvItem ) const;
		int GetItemCount( ) const;
		DWORD_PTR GetItemData( int iItem ) const;
		BOOL GetItemPosition( int iItem, CPoint& pt ) const;
		BOOL GetItemRect( int iItem, CRect& rc, UINT nCode ) const;
		UINT GetItemState( int iItem, UINT nMask ) const;
		tString GetItemText( int iItem, int iSubItem, UINT nTextMax = 260 ) const;
		int GetNextItem( int iItem, int iFlags ) const;
		UINT GetNumberOfWorkAreas( ) const;
		BOOL GetOrigin( CPoint& pt ) const;
		UINT GetSelectedCount( ) const;
		int GetSelectionMark( );
		int GetStringWidth( LPCTSTR pszString ) const;
		BOOL GetSubItemRect( int iItem, int iSubItem, int iCode, CRect& rc );
		COLORREF GetTextBkColor( ) const;
		COLORREF GetTextColor( ) const;
		HWND GetToolTips( ) const;
		int GetTopIndex( ) const;
		BOOL GetViewRect( CRect& rc ) const;
		void GetWorkAreas( int iWorkAreas, LPRECT pRectArray ) const;
		BOOL SetBkColor( COLORREF clrBk );
		BOOL SetBkImage( LVBKIMAGE& plvbkImage );
		BOOL SetCallbackMask( UINT nMask );
		void SetCheckState( int iItem, BOOL fCheck = TRUE );
		BOOL SetColumn( int iCol, const LVCOLUMN& pColumn );
		BOOL SetColumnOrderArray( int iCount, LPINT piArray );
		BOOL SetColumnWidth( int iCol, int cx );
		DWORD SetExtendedStyle( DWORD dwNewStyle );
		HCURSOR SetHotCursor( HCURSOR hCursor );
		int SetHotItem( int nIndex );
		DWORD SetHoverTime( DWORD dwHoverTime = (DWORD)-1 );
		CSize SetIconSpacing( int cx, int cy );
		CSize SetIconSpacing( CSize sz );
		HIMAGELIST SetImageList( HIMAGELIST himl, int iImageListType );
		BOOL SetItem( LVITEM& pItem );
		BOOL SetItem( int iItem, int iSubItem, UINT nMask, LPCTSTR pszText, int iImage,
						UINT nState, UINT nStateMask, LPARAM lParam, int iIndent );
		void SetItemCount( int iCount );
		void SetItemCountEx( int iCount, DWORD dwFlags = LVSICF_NOINVALIDATEALL );
		BOOL SetItemData( int iItem, DWORD_PTR dwData );
		BOOL SetItemPosition( int iItem, CPoint& pt );
		BOOL SetItemState( int iItem, LVITEM& Item );
        void SetItemState( int iItem, UINT nState, UINT nMask );
		void SetItemText( int iItem, int iSubItem, LPCTSTR pszText );
		int SetSelectionMark( int iIndex );
		BOOL SetTextBkColor( COLORREF clrBkText );
		BOOL SetTextColor( COLORREF clrText );
		HWND SetToolTips( HWND hWndToolTip );
		void SetWorkAreas( int nWorkAreas, CRect& pRectArray );
		int SubItemHitTest( LVHITTESTINFO& htInfo );

		// Operations
		BOOL Arrange( UINT nCode );
		HIMAGELIST CreateDragImage( int iItem, CPoint& pt );
		BOOL DeleteAllItems( );
		BOOL DeleteColumn( int iCol );
		BOOL DeleteItem( int iItem );
		HWND EditLabel( int iItem );
		BOOL EnsureVisible( int iItem, BOOL fPartialOK );
		int FindItem( LVFINDINFO& FindInfo, int iStart = -1 ) const;
		int HitTest( LVHITTESTINFO& HitTestInfo ) const;
        int HitTest( CPoint pt, UINT* pFlags = NULL ) const;
		int InsertColumn( int iCol, const LVCOLUMN& pColumn );
        int InsertColumn( int iCol, LPCTSTR pszColumnHeading, int iFormat = LVCFMT_LEFT,
							int iWidth = -1, int iSubItem = -1 );
		int InsertItem( const LVITEM& pItem );
        int InsertItem( int iItem, LPCTSTR pszText );
        int InsertItem( int iItem, LPCTSTR pszText, int iImage );
		BOOL RedrawItems( int iFirst, int iLast );
		BOOL Scroll( CSize sz );
		BOOL SortItems( PFNLVCOMPARE pfnCompare, DWORD_PTR dwData );
		BOOL Update( int iItem );
	};

}

#endif // #ifndef _LISTVIEW_H_

