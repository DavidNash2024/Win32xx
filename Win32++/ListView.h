#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "../Win32++/WinCore.h"
#include "Commctrl.h"

namespace Win32xx
{

	class CListView : public CWnd
	{
	public:
		CListView() {}
		virtual ~CListView() {}
		virtual void PreCreate( CREATESTRUCT &cs )
		{ cs.lpszClass = WC_LISTVIEW; }

		CSize ApproximateViewRect(CSize sz = CSize(-1, -1), int iCount = -1) const
		{ return CSize( ListView_ApproximateViewRect( m_hWnd, sz.cx, sz.cy, iCount ) ); }

		COLORREF GetBkColor( ) const
		{ return ListView_GetBkColor( m_hWnd ); }

		BOOL GetBkImage( LVBKIMAGE* plvbkImage ) const
		{ return ListView_GetBkImage( m_hWnd, plvbkImage ); }

		UINT GetCallbackMask( ) const
		{ return ListView_GetCallbackMask( m_hWnd ); }

		BOOL GetCheckState( UINT nItem ) const
		{ return ListView_GetCheckState( m_hWnd, nItem ); }

		BOOL GetColumn( int iCol, LVCOLUMN* pColumn ) const
		{ return ListView_GetColumn( m_hWnd, iCol, pColumn ); }

		BOOL GetColumnOrderArray( LPINT piArray, int iCount = -1 )
		{ return ListView_GetColumnOrderArray( m_hWnd, iCount, piArray ); }

		int GetColumnWidth( int iCol ) const
		{ return ListView_GetColumnWidth( m_hWnd, iCol ); }

		int GetCountPerPage( ) const
		{ return ListView_GetCountPerPage( m_hWnd ); }

		HWND GetEditControl( ) const
		{ return ListView_GetEditControl( m_hWnd ); }

		DWORD GetExtendedStyle( ) const
		{ return ListView_GetExtendedListViewStyle( m_hWnd ); }

		HWND GetHeaderCtrl( ) const
		{ return ListView_GetHeader( m_hWnd ); }

		HCURSOR GetHotCursor( )
		{ return ListView_GetHotCursor( m_hWnd ); }

		int GetHotItem( ) const
		{ return ListView_GetHotItem( m_hWnd ); }

		DWORD GetHoverTime( ) const
		{ return ListView_GetHoverTime( m_hWnd ); }

		HIMAGELIST GetImageList( int nImageType ) const
		{ return ListView_GetImageList( m_hWnd, nImageType ); }

		BOOL GetItem( LVITEM* pItem ) const
		{ return ListView_GetItem( m_hWnd, pItem ); }

		int GetItemCount( ) const
		{ return ListView_GetItemCount( m_hWnd ); }

		DWORD_PTR GetItemData( int iItem ) const
		{
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.mask = LVIF_PARAM;
			return ListView_GetItem(m_hWnd, &lvi);
		}

		BOOL GetItemPosition( int iItem, LPPOINT pPoint ) const
		{ return ListView_GetItemPosition( m_hWnd, iItem, pPoint ); }

		BOOL GetItemRect( int iItem, LPRECT pRect, UINT nCode ) const
		{ return ListView_GetItemRect( m_hWnd, iItem, pRect, nCode ); }

		UINT GetItemState( int iItem, UINT nMask ) const
		{ return  ListView_GetItemState( m_hWnd, iItem, nMask ); }

		tString GetItemText( int iItem, int iSubItem ) const
		{
			TCHAR szText[261];
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.iSubItem = iSubItem;
			lvi.mask = LVIF_TEXT;
			lvi.cchTextMax = 260;
			lvi.pszText = szText;
			ListView_GetItem( m_hWnd, &lvi );
			tString t = lvi.pszText;
			return t;
		}

		int GetNextItem( int iItem, int iFlags ) const
		{ return ListView_GetNextItem( m_hWnd, iItem, iFlags ); }

		UINT GetNumberOfWorkAreas( ) const
		{
			UINT nWorkAreas;
			ListView_GetWorkAreas( m_hWnd, nWorkAreas, NULL );
			return nWorkAreas;
		}

		BOOL GetOrigin( LPPOINT pPoint ) const
		{ return ListView_GetOrigin( m_hWnd, pPoint ); }

		UINT GetSelectedCount( ) const
		{ return (UINT)::SendMessage( m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0 ); }

		int GetSelectionMark( )
		{ return (int)::SendMessage( m_hWnd, LVM_GETSELECTIONMARK, 0, 0 ); }

		int GetStringWidth( LPCTSTR pszString ) const
		{ return (int)::SendMessage( m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)pszString ); }

		BOOL GetSubItemRect( int iItem, int iSubItem, int iCode, CRect& Rect )
		{ return ListView_GetSubItemRect( m_hWnd, iItem, iSubItem, iCode, &Rect ); }

		COLORREF GetTextBkColor( ) const
		{ return ListView_GetTextBkColor( m_hWnd ); }

		COLORREF GetTextColor( ) const
		{ return ListView_GetTextColor( m_hWnd ); }

		HWND GetToolTips( ) const
		{ return ListView_GetToolTips( m_hWnd ); }

		int GetTopIndex( ) const
		{ return ListView_GetTopIndex( m_hWnd ); }

		BOOL GetViewRect( LPRECT pRect ) const
		{ return ListView_GetViewRect( m_hWnd, pRect ); }

		void GetWorkAreas( int iWorkAreas, LPRECT pRectArray ) const
		{ ListView_GetWorkAreas( m_hWnd, iWorkAreas, pRectArray ); }

		BOOL SetBkColor( COLORREF clrBk )
		{ return ListView_SetBkColor( m_hWnd, clrBk ); }

		BOOL SetBkImage( LVBKIMAGE* plvbkImage )
		{ return ListView_SetBkImage( m_hWnd, plvbkImage ); }

		BOOL SetCallbackMask( UINT nMask )
		{ return ListView_SetCallbackMask( m_hWnd, nMask ); }

	//	void SetCheckState( int iItem, BOOL fCheck = TRUE )
	//	{ ListView_SetCheckState( m_hWnd, iItem, fCheck ); }

		BOOL SetColumn( int iCol, const LVCOLUMN* pColumn )
		{ return ListView_SetColumn( m_hWnd, iCol, pColumn ); }

		BOOL SetColumnOrderArray( int iCount, LPINT piArray )
		{ return ListView_SetColumnOrderArray( m_hWnd, iCount, piArray ); }

		BOOL SetColumnWidth( int iCol, int cx )
		{ return ListView_SetColumnWidth( m_hWnd, iCol, cx ); }

		DWORD SetExtendedStyle( DWORD dwNewStyle )
		{ return ListView_SetExtendedListViewStyle( m_hWnd, dwNewStyle ); }

		HCURSOR SetHotCursor( HCURSOR hCursor )
		{ return ListView_SetHotCursor( m_hWnd, hCursor ); }

		int SetHotItem( int nIndex )
		{ return ListView_SetHotItem( m_hWnd, nIndex ); }

		DWORD SetHoverTime( DWORD dwHoverTime = (DWORD)-1 )
		{ return ListView_SetHoverTime( m_hWnd, dwHoverTime ); }

		CSize SetIconSpacing( int cx, int cy )
		{ return CSize( ListView_SetIconSpacing( m_hWnd, cx, cy ) ); }

		CSize SetIconSpacing( CSize sz )
		{ return CSize( ListView_SetIconSpacing( m_hWnd, sz.cx, sz.cy ) ); }

		HIMAGELIST SetImageList( HIMAGELIST himl, int iImageListType )
		{ return ListView_SetImageList( m_hWnd, himl, iImageListType ); }

		BOOL SetItem( LVITEM* pItem )
		{ return ListView_SetItem( m_hWnd, pItem ); }

		BOOL SetItem( int iItem, int iSubItem, UINT nMask, LPCTSTR pszText, int iImage,
						UINT nState, UINT nStateMask, LPARAM lParam, int iIndent )
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

		void SetItemCount( int iCount )
		{ ListView_SetItemCount( m_hWnd, iCount ); }

		void SetItemCountEx( int iCount, DWORD dwFlags = LVSICF_NOINVALIDATEALL )
		{ ListView_SetItemCountEx( m_hWnd, iCount, dwFlags ); }

		BOOL SetItemData( int iItem, DWORD_PTR dwData )
		{
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.lParam = dwData;
			lvi.mask = LVIF_PARAM;
			return ListView_SetItem(m_hWnd, &lvi);
		}

		BOOL SetItemPosition( int iItem, POINT pt )
		{return ListView_SetItemPosition( m_hWnd, iItem, pt.x, pt.y ); }

		BOOL SetItemState( int iItem, LVITEM* pItem )
		{ return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, (WPARAM)iItem, (LPARAM)pItem); }

        BOOL SetItemState( int iItem, UINT nState, UINT nMask )
		{
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.state = nState;
			lvi.stateMask = nMask;
			return ListView_SetItem(m_hWnd, &lvi);
		}

		void SetItemText( int iItem, int iSubItem, LPCTSTR pszText )
		{ ListView_SetItemText(m_hWnd, iItem, iSubItem, (LPTSTR)pszText ); }

		int SetSelectionMark( int iIndex )
		{ return ListView_SetSelectionMark( m_hWnd, iIndex ); }

		BOOL SetTextBkColor( COLORREF clrBkText )
		{ return ListView_SetTextBkColor( m_hWnd, clrBkText ); }

		BOOL SetTextColor( COLORREF clrText )
		{ return ListView_SetTextColor( m_hWnd, clrText ); }

	//	HWND SetToolTips( HWND hWndToolTip )
	//	{ ListView_SetToolTips( m_hWnd, hWndToolTip ); }

		void SetWorkAreas( int nWorkAreas, LPRECT pRectArray )
		{ ListView_SetWorkAreas( m_hWnd, nWorkAreas, pRectArray ); }

		int SubItemHitTest( LPLVHITTESTINFO pInfo )
		{ return ListView_SubItemHitTest( m_hWnd, pInfo ); }

		// Operations
		BOOL Arrange( UINT nCode )
		{ return ListView_Arrange( m_hWnd, nCode ); }

		HIMAGELIST CreateDragImage( int iItem, LPPOINT pPoint )
		{ return ListView_CreateDragImage( m_hWnd, iItem, pPoint ); }

		BOOL DeleteAllItems( )
		{ return ListView_DeleteAllItems( m_hWnd ); }

		BOOL DeleteColumn( int iCol )
		{ return ListView_DeleteColumn( m_hWnd, iCol ); }

		BOOL DeleteItem( int iItem )
		{ return ListView_DeleteItem( m_hWnd, iItem ); }

		HWND EditLabel( int iItem )
		{ return ListView_EditLabel( m_hWnd, iItem ); }

		BOOL EnsureVisible( int iItem, BOOL fPartialOK )
		{ return SendMessage( m_hWnd, LVM_ENSUREVISIBLE, (WPARAM)iItem, (LPARAM)fPartialOK ); }

		int FindItem( LVFINDINFO* pFindInfo, int iStart = -1 ) const
		{ return ListView_FindItem( m_hWnd, iStart, pFindInfo ); }

		int HitTest( LVHITTESTINFO* pHitTestInfo ) const
		{ return ListView_HitTest( m_hWnd, pHitTestInfo ); }

        int HitTest( CPoint pt, UINT* pFlags = NULL ) const
		{
			LVHITTESTINFO hti = {0};
			hti.flags = *pFlags;
			hti.pt = pt;
			return ListView_HitTest( m_hWnd, &hti );
		}

		int InsertColumn( int iCol, const LVCOLUMN* pColumn )
		{ return ListView_InsertColumn( m_hWnd, iCol, pColumn ); }

        int InsertColumn( int iCol, LPCTSTR pszColumnHeading, int iFormat = LVCFMT_LEFT,
							int iWidth = -1, int iSubItem = -1 )
		{
			LVCOLUMN lvc = {0};
			lvc.iOrder = iCol;
			lvc.pszText = (LPTSTR)pszColumnHeading;
			lvc.fmt = iFormat;
			lvc.cx = iWidth;
			lvc.iSubItem = iSubItem;
			return ListView_InsertColumn( m_hWnd, iCol, &lvc );
		}

		int InsertItem( const LVITEM* pItem )
		{ return ListView_InsertItem( m_hWnd, pItem ); }

        int InsertItem( int iItem, LPCTSTR pszText )
		{
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.pszText = (LPTSTR)pszText;
			lvi.mask = LVIF_TEXT;
			return ListView_SetItem( m_hWnd, &lvi );
		}

        int InsertItem( int iItem, LPCTSTR pszText, int iImage )
		{
			LVITEM lvi = {0};
			lvi.iItem = iItem;
			lvi.pszText = (LPTSTR)pszText;
			lvi.iImage = iImage;
			lvi.mask = LVIF_TEXT | LVIF_IMAGE;
			return ListView_SetItem( m_hWnd, &lvi );
		}

		BOOL RedrawItems( int iFirst, int iLast )
		{ return ListView_RedrawItems( m_hWnd, iFirst, iLast ); }

		BOOL Scroll( CSize sz )
		{ return ListView_Scroll( m_hWnd, sz.cx, sz.cy ); }

		BOOL SortItems( PFNLVCOMPARE pfnCompare, DWORD_PTR dwData )
		{ return ListView_SortItems( m_hWnd, pfnCompare, dwData ); }

		BOOL Update( int iItem )
		{ return ListView_Update( m_hWnd, iItem ); }

		//Overridables
	//	virtual void DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
	//	{}

	};

} // namespace Win32xx

#endif // #ifndef LISTVIEW_H

