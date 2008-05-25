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
		virtual void PreCreate( CREATESTRUCT &cs );

		// Attributes
		CSize ApproximateViewRect(CSize sz = CSize(-1, -1), int iCount = -1) const; 
		COLORREF GetBkColor( ) const;
		BOOL GetBkImage( LVBKIMAGE* plvbkImage ) const;
		UINT GetCallbackMask( ) const;
		BOOL GetCheckState( UINT nItem ) const;
		BOOL GetColumn( int iCol, LVCOLUMN* pColumn ) const;
		BOOL GetColumnOrderArray( LPINT piArray, int iCount = -1 );
		int GetColumnWidth( int iCol ) const;
		int GetCountPerPage( ) const;
		HWND GetEditControl( ) const;
		DWORD GetExtendedStyle( ) const;
		HWND GetHeaderCtrl( ) const;
		HCURSOR GetHotCursor( );
		int GetHotItem( ) const;
		DWORD GetHoverTime( ) const;
		HIMAGELIST GetImageList( int nImageType ) const;
		BOOL GetItem( LVITEM* pItem ) const;
		int GetItemCount( ) const;
		DWORD_PTR GetItemData( int iItem ) const;
		BOOL GetItemPosition( int iItem, LPPOINT pPoint ) const;
		BOOL GetItemRect( int iItem, LPRECT pRect, UINT nCode ) const;
		UINT GetItemState( int iItem, UINT nMask ) const;
		tString GetItemText( int iItem, int iSubItem ) const;
		int GetNextItem( int iItem, int iFlags ) const;
		UINT GetNumberOfWorkAreas( ) const;
		BOOL GetOrigin( LPPOINT pPoint ) const;
		UINT GetSelectedCount( ) const;
		int GetSelectionMark( );
		int GetStringWidth( LPCTSTR pszString ) const;
		BOOL GetSubItemRect( int iItem, int iSubItem, int iCode, CRect& Rect );
		COLORREF GetTextBkColor( ) const;
		COLORREF GetTextColor( ) const;
		HWND GetToolTips( ) const;
		int GetTopIndex( ) const;
		BOOL GetViewRect( LPRECT pRect ) const;
		void GetWorkAreas( int iWorkAreas, LPRECT pRectArray ) const;
		BOOL SetBkColor( COLORREF clrBk );
		BOOL SetBkImage( LVBKIMAGE* plvbkImage );
		BOOL SetCallbackMask( UINT nMask );
		void SetCheckState( int iItem, BOOL fCheck = TRUE );
		BOOL SetColumn( int iCol, const LVCOLUMN* pColumn );
		BOOL SetColumnOrderArray( int iCount, LPINT piArray );
		BOOL SetColumnWidth( int iCol, int cx );
		DWORD SetExtendedStyle( DWORD dwNewStyle );
		HCURSOR SetHotCursor( HCURSOR hCursor );
		int SetHotItem( int nIndex );
		DWORD SetHoverTime( DWORD dwHoverTime = (DWORD)-1 );
		CSize SetIconSpacing( int cx, int cy );
		CSize SetIconSpacing( CSize sz );
		HIMAGELIST SetImageList( HIMAGELIST himl, int iImageListType );
		BOOL SetItem( LVITEM* pItem );
		BOOL SetItem( int iItem, int iSubItem, UINT nMask, LPCTSTR pszText, int iImage,
						UINT nState, UINT nStateMask, LPARAM lParam, int iIndent );
		void SetItemCount( int iCount );
		void SetItemCountEx( int iCount, DWORD dwFlags = LVSICF_NOINVALIDATEALL );
		BOOL SetItemData( int iItem, DWORD_PTR dwData );
		BOOL SetItemPosition( int iItem, POINT pt );
		BOOL SetItemState( int iItem, LVITEM* pItem );
        void SetItemState( int iItem, UINT nState, UINT nMask );
		void SetItemText( int iItem, int iSubItem, LPCTSTR pszText );
		int SetSelectionMark( int iIndex );
		BOOL SetTextBkColor( COLORREF clrBkText );
		BOOL SetTextColor( COLORREF clrText );
		HWND SetToolTips( HWND hWndToolTip );
		void SetWorkAreas( int nWorkAreas, LPRECT pRectArray );
		int SubItemHitTest( LPLVHITTESTINFO pInfo );

		// Operations
		BOOL Arrange( UINT nCode );
		HIMAGELIST CreateDragImage( int iItem, LPPOINT pPoint );
		BOOL DeleteAllItems( );
		BOOL DeleteColumn( int iCol );
		BOOL DeleteItem( int iItem );
		HWND EditLabel( int iItem );
		BOOL EnsureVisible( int iItem, BOOL fPartialOK );
		int FindItem( LVFINDINFO* pFindInfo, int iStart = -1 ) const;
		int HitTest( LVHITTESTINFO* pHitTestInfo ) const;
        int HitTest( CPoint pt, UINT* pFlags = NULL ) const;
		int InsertColumn( int iCol, const LVCOLUMN* pColumn );
        int InsertColumn( int iCol, LPCTSTR pszColumnHeading, int iFormat = LVCFMT_LEFT,
							int iWidth = -1, int iSubItem = -1 );
		int InsertItem( const LVITEM* pItem );
        int InsertItem( int iItem, LPCTSTR pszText );
        int InsertItem( int iItem, LPCTSTR pszText, int iImage );
		BOOL RedrawItems( int iFirst, int iLast );
		BOOL Scroll( CSize sz );
		BOOL SortItems( PFNLVCOMPARE pfnCompare, DWORD_PTR dwData );
		BOOL Update( int iItem );
	};


	inline void CListView::PreCreate( CREATESTRUCT &cs )
	{ 
		cs.lpszClass = WC_LISTVIEW; 
	}

	inline CSize CListView::ApproximateViewRect(CSize sz /*= CSize(-1, -1)*/, int iCount /* = -1*/) const 
	{ 
		return CSize( ListView_ApproximateViewRect( m_hWnd, sz.cx, sz.cy, iCount ) ); 
	}

	inline COLORREF CListView::GetBkColor( ) const
	{ 
		return ListView_GetBkColor( m_hWnd ); 
	}

	inline BOOL CListView::GetBkImage( LVBKIMAGE* plvbkImage ) const
	{ 
		return ListView_GetBkImage( m_hWnd, plvbkImage ); 
	}

	inline UINT CListView::GetCallbackMask( ) const
	{ 
		return ListView_GetCallbackMask( m_hWnd ); 
	}

	inline BOOL CListView::GetCheckState( UINT nItem ) const
	{ 
		return ListView_GetCheckState( m_hWnd, nItem ); 
	}

	inline BOOL CListView::GetColumn( int iCol, LVCOLUMN* pColumn ) const
	{ 
		return ListView_GetColumn( m_hWnd, iCol, pColumn ); 
	}

	inline BOOL CListView::GetColumnOrderArray( LPINT piArray, int iCount /*= -1*/ )
	{ 
		return ListView_GetColumnOrderArray( m_hWnd, iCount, piArray ); 
	}

	inline int CListView::GetColumnWidth( int iCol ) const
	{ 
		return ListView_GetColumnWidth( m_hWnd, iCol ); 
	}

	inline int CListView::GetCountPerPage( ) const
	{ 
		return ListView_GetCountPerPage( m_hWnd ); 
	}

	inline HWND CListView::GetEditControl( ) const
	{ 
		return ListView_GetEditControl( m_hWnd ); 
	}

	inline DWORD CListView::GetExtendedStyle( ) const
	{ 
		return ListView_GetExtendedListViewStyle( m_hWnd ); 
	}

	inline HWND CListView::GetHeaderCtrl( ) const
	{ 
		return ListView_GetHeader( m_hWnd ); 
	}

	inline HCURSOR CListView::GetHotCursor( )
	{ 
		return ListView_GetHotCursor( m_hWnd ); 
	}

	inline int CListView::GetHotItem( ) const
	{ 
		return ListView_GetHotItem( m_hWnd ); 
	}

	inline DWORD CListView::GetHoverTime( ) const
	{ 
		return ListView_GetHoverTime( m_hWnd ); 
	}

	inline HIMAGELIST CListView::GetImageList( int nImageType ) const
	{ 
		return ListView_GetImageList( m_hWnd, nImageType ); 
	}

	inline BOOL CListView::GetItem( LVITEM* pItem ) const
	{ 
		return ListView_GetItem( m_hWnd, pItem ); 
	}

	inline int CListView::GetItemCount( ) const
	{ 
		return ListView_GetItemCount( m_hWnd ); 
	}

	inline DWORD_PTR CListView::GetItemData( int iItem ) const
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.mask = LVIF_PARAM;
		return ListView_GetItem(m_hWnd, &lvi);
	}

	inline BOOL CListView::GetItemPosition( int iItem, LPPOINT pPoint ) const
	{ 
		return ListView_GetItemPosition( m_hWnd, iItem, pPoint ); 
	}

	inline BOOL CListView::GetItemRect( int iItem, LPRECT pRect, UINT nCode ) const
	{ 
		return ListView_GetItemRect( m_hWnd, iItem, pRect, nCode ); 
	}

	inline UINT CListView::GetItemState( int iItem, UINT nMask ) const
	{ 
		return  ListView_GetItemState( m_hWnd, iItem, nMask ); 
	}

	inline tString CListView::GetItemText( int iItem, int iSubItem ) const
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

	inline int CListView::GetNextItem( int iItem, int iFlags ) const
	{ 
		return ListView_GetNextItem( m_hWnd, iItem, iFlags ); 
	}

	inline UINT CListView::GetNumberOfWorkAreas( ) const
	{
		UINT nWorkAreas;
		ListView_GetWorkAreas( m_hWnd, nWorkAreas, NULL );
		return nWorkAreas;
	}

	inline BOOL CListView::GetOrigin( LPPOINT pPoint ) const
	{ 
		return ListView_GetOrigin( m_hWnd, pPoint ); 
	}

	inline UINT CListView::GetSelectedCount( ) const
	{ 
		return (UINT)::SendMessage( m_hWnd, LVM_GETSELECTEDCOUNT, 0, 0 ); 
	}

	inline int CListView::GetSelectionMark( )
	{ 
		return (int)::SendMessage( m_hWnd, LVM_GETSELECTIONMARK, 0, 0 ); 
	}

	inline int CListView::GetStringWidth( LPCTSTR pszString ) const
	{ 
		return (int)::SendMessage( m_hWnd, LVM_GETSTRINGWIDTH, 0, (LPARAM)pszString ); 
	}

	inline BOOL CListView::GetSubItemRect( int iItem, int iSubItem, int iCode, CRect& Rect )
	{ 
		return ListView_GetSubItemRect( m_hWnd, iItem, iSubItem, iCode, &Rect ); 
	}

	inline COLORREF CListView::GetTextBkColor( ) const
	{ 
		return ListView_GetTextBkColor( m_hWnd ); 
	}

	inline COLORREF CListView::GetTextColor( ) const
	{ 
		return ListView_GetTextColor( m_hWnd ); 
	}

	inline HWND CListView::GetToolTips( ) const
	{ 
		return ListView_GetToolTips( m_hWnd ); 
	}

	inline int CListView::GetTopIndex( ) const
	{ 
		return ListView_GetTopIndex( m_hWnd ); 
	}

	inline BOOL CListView::GetViewRect( LPRECT pRect ) const
	{ 
		return ListView_GetViewRect( m_hWnd, pRect ); 
	}

	inline void CListView::GetWorkAreas( int iWorkAreas, LPRECT pRectArray ) const
	{ 
		ListView_GetWorkAreas( m_hWnd, iWorkAreas, pRectArray ); 
	}

	inline BOOL CListView::SetBkColor( COLORREF clrBk )
	{ 
		return ListView_SetBkColor( m_hWnd, clrBk ); 
	}

	inline BOOL CListView::SetBkImage( LVBKIMAGE* plvbkImage )
	{ 
		return ListView_SetBkImage( m_hWnd, plvbkImage ); 
	}

	inline BOOL CListView::SetCallbackMask( UINT nMask )
	{ 
		return ListView_SetCallbackMask( m_hWnd, nMask ); 
	}

	inline void CListView::SetCheckState( int iItem, BOOL fCheck /*= TRUE*/ )
	{ 
		ListView_SetItemState(m_hWnd, iItem, INDEXTOSTATEIMAGEMASK((fCheck==TRUE)?2:1),LVIS_STATEIMAGEMASK);
	}

	inline BOOL CListView::SetColumn( int iCol, const LVCOLUMN* pColumn )
	{ 
		return ListView_SetColumn( m_hWnd, iCol, pColumn ); 
	}

	inline BOOL CListView::SetColumnOrderArray( int iCount, LPINT piArray )
	{ 
		return ListView_SetColumnOrderArray( m_hWnd, iCount, piArray ); 
	}

	inline BOOL CListView::SetColumnWidth( int iCol, int cx )
	{ 
		return ListView_SetColumnWidth( m_hWnd, iCol, cx ); 
	}

	inline DWORD CListView::SetExtendedStyle( DWORD dwNewStyle )
	{ 
		return ListView_SetExtendedListViewStyle( m_hWnd, dwNewStyle ); 
	}

	inline HCURSOR CListView::SetHotCursor( HCURSOR hCursor )
	{ 
		return ListView_SetHotCursor( m_hWnd, hCursor ); 
	}

	inline int CListView::SetHotItem( int nIndex )
	{ 
		return ListView_SetHotItem( m_hWnd, nIndex ); 
	}

	inline DWORD CListView::SetHoverTime( DWORD dwHoverTime /*= (DWORD)-1*/ )
	{ 
		return ListView_SetHoverTime( m_hWnd, dwHoverTime ); 
	}

	inline CSize CListView::SetIconSpacing( int cx, int cy )
	{ 
		return CSize( ListView_SetIconSpacing( m_hWnd, cx, cy ) ); 
	}

	inline CSize CListView::SetIconSpacing( CSize sz )
	{ 
		return CSize( ListView_SetIconSpacing( m_hWnd, sz.cx, sz.cy ) ); 
	}

	inline HIMAGELIST CListView::SetImageList( HIMAGELIST himl, int iImageListType )
	{ 
		return ListView_SetImageList( m_hWnd, himl, iImageListType ); 
	}

	inline BOOL CListView::SetItem( LVITEM* pItem )
	{ 
		return ListView_SetItem( m_hWnd, pItem ); 
	}

	inline BOOL CListView::SetItem( int iItem, int iSubItem, UINT nMask, LPCTSTR pszText, int iImage,
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

	inline void CListView::SetItemCount( int iCount )
	{ 
		ListView_SetItemCount( m_hWnd, iCount ); 
	}

	inline void CListView::SetItemCountEx( int iCount, DWORD dwFlags /*= LVSICF_NOINVALIDATEALL*/ )
	{ 
		ListView_SetItemCountEx( m_hWnd, iCount, dwFlags ); 
	}

	inline BOOL CListView::SetItemData( int iItem, DWORD_PTR dwData )
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.lParam = dwData;
		lvi.mask = LVIF_PARAM;
		return ListView_SetItem(m_hWnd, &lvi);
	}

	inline BOOL CListView::SetItemPosition( int iItem, POINT pt )
	{
		return ListView_SetItemPosition( m_hWnd, iItem, pt.x, pt.y ); 
	}

	inline BOOL CListView::SetItemState( int iItem, LVITEM* pItem )
	{ 
		return (BOOL)::SendMessage(m_hWnd, LVM_SETITEMSTATE, (WPARAM)iItem, (LPARAM)pItem); 
	}

    inline void CListView::SetItemState( int iItem, UINT nState, UINT nMask )
	{ 
		ListView_SetItemState(m_hWnd, iItem, nState, nMask); 
	}

	inline void CListView::SetItemText( int iItem, int iSubItem, LPCTSTR pszText )
	{ 
		ListView_SetItemText(m_hWnd, iItem, iSubItem, (LPTSTR)pszText ); 
	}

	inline int CListView::SetSelectionMark( int iIndex )
	{ 
		return ListView_SetSelectionMark( m_hWnd, iIndex ); 
	}

	inline BOOL CListView::SetTextBkColor( COLORREF clrBkText )
	{ 
		return ListView_SetTextBkColor( m_hWnd, clrBkText ); 
	}

	inline BOOL CListView::SetTextColor( COLORREF clrText )
	{ 
		return ListView_SetTextColor( m_hWnd, clrText ); 
	}

	inline HWND CListView::SetToolTips( HWND hWndToolTip )
	{ 
		return (HWND)::SendMessage(m_hWnd, LVM_SETTOOLTIPS, (WPARAM)hWndToolTip, 0);
	}

	inline void CListView::SetWorkAreas( int nWorkAreas, LPRECT pRectArray )
	{ 
		ListView_SetWorkAreas( m_hWnd, nWorkAreas, pRectArray ); 
	}

	inline int CListView::SubItemHitTest( LPLVHITTESTINFO pInfo )
	{ 
		return ListView_SubItemHitTest( m_hWnd, pInfo ); 
	}

	// Operations
	inline BOOL CListView::Arrange( UINT nCode )
	{ 
		return ListView_Arrange( m_hWnd, nCode ); 
	}

	inline HIMAGELIST CListView::CreateDragImage( int iItem, LPPOINT pPoint )
	{ 
		return ListView_CreateDragImage( m_hWnd, iItem, pPoint ); 
	}

	inline BOOL CListView::DeleteAllItems( )
	{ 
		return ListView_DeleteAllItems( m_hWnd ); 
	}

	inline BOOL CListView::DeleteColumn( int iCol )
	{ 
		return ListView_DeleteColumn( m_hWnd, iCol ); 
	}

	inline BOOL CListView::DeleteItem( int iItem )
	{ 
		return ListView_DeleteItem( m_hWnd, iItem ); 
	}

	inline HWND CListView::EditLabel( int iItem )
	{ 
		return ListView_EditLabel( m_hWnd, iItem ); 
	}

	inline BOOL CListView::EnsureVisible( int iItem, BOOL fPartialOK )
	{ 
		return SendMessage( m_hWnd, LVM_ENSUREVISIBLE, (WPARAM)iItem, (LPARAM)fPartialOK ); 
	}

	inline int CListView::FindItem( LVFINDINFO* pFindInfo, int iStart /*= -1*/ ) const
	{ 
		return ListView_FindItem( m_hWnd, iStart, pFindInfo ); 
	}

	inline int CListView::HitTest( LVHITTESTINFO* pHitTestInfo ) const
	{ 
		return ListView_HitTest( m_hWnd, pHitTestInfo ); 
	}

    inline int CListView::HitTest( CPoint pt, UINT* pFlags /*= NULL*/ ) const
	{
		LVHITTESTINFO hti = {0};
		hti.flags = *pFlags;
		hti.pt = pt;
		return ListView_HitTest( m_hWnd, &hti );
	}

	inline int CListView::InsertColumn( int iCol, const LVCOLUMN* pColumn )
	{ 
		return ListView_InsertColumn( m_hWnd, iCol, pColumn ); 
	}

    inline int CListView::InsertColumn( int iCol, LPCTSTR pszColumnHeading, int iFormat /*= LVCFMT_LEFT*/,
						int iWidth /*= -1*/, int iSubItem /*= -1*/ )
	{
		LVCOLUMN lvc = {0};
		lvc.iOrder = iCol;
		lvc.pszText = (LPTSTR)pszColumnHeading;
		lvc.fmt = iFormat;
		lvc.cx = iWidth;
		lvc.iSubItem = iSubItem;
		return ListView_InsertColumn( m_hWnd, iCol, &lvc );
	}

	inline int CListView::InsertItem( const LVITEM* pItem )
	{ 
		return ListView_InsertItem( m_hWnd, pItem ); 
	}

    inline int CListView::InsertItem( int iItem, LPCTSTR pszText )
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.pszText = (LPTSTR)pszText;
		lvi.mask = LVIF_TEXT;
		return ListView_SetItem( m_hWnd, &lvi );
	}

    inline int CListView::InsertItem( int iItem, LPCTSTR pszText, int iImage )
	{
		LVITEM lvi = {0};
		lvi.iItem = iItem;
		lvi.pszText = (LPTSTR)pszText;
		lvi.iImage = iImage;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE;
		return ListView_SetItem( m_hWnd, &lvi );
	}

	inline BOOL CListView::RedrawItems( int iFirst, int iLast )
	{ 
		return ListView_RedrawItems( m_hWnd, iFirst, iLast ); 
	}

	inline BOOL CListView::Scroll( CSize sz )
	{ 
		return ListView_Scroll( m_hWnd, sz.cx, sz.cy ); 
	}

	inline BOOL CListView::SortItems( PFNLVCOMPARE pfnCompare, DWORD_PTR dwData )
	{ 
		return ListView_SortItems( m_hWnd, pfnCompare, dwData ); 
	}

	inline BOOL CListView::Update( int iItem )
	{ 
		return ListView_Update( m_hWnd, iItem ); 
	}

} // namespace Win32xx

#endif // #ifndef LISTVIEW_H

