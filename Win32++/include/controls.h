// Win32++  Version 7.0 (Prerelease version)
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2010  David Nash
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


////////////////////////////////////////////////////////
// controls.h
//  Declaration of the following classes:
//  CAnimation, CComboBox, CComboBoxEx, CProgress, 
//  CScrollbar, CSlider, CSpinbutton


#ifndef _CONTROLS_H_
#define _CONTORLS_H_

#include "wincore.h"

namespace Win32xx
{
	class CAnimation : public CWnd
	{
	public:
		CAnimation() {}
		virtual ~CAnimation() {}
		virtual tString GetWindowType() const { return _T("CAnimation"); }
		
		BOOL Close() const;
		BOOL Open(LPTSTR lpszName) const;
		BOOL Play(UINT wFrom, UINT wTo, UINT cRepeat) const;
		BOOL Seek(UINT wFrame) const;
		BOOL Stop() const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = ANIMATE_CLASS; }
	};
	
	
	class CComboBox : public CWnd
	{
	public:
		CComboBox() {}
		virtual ~CComboBox() {}
		virtual tString GetWindowType() const { return _T("CComboBox"); }

		int  AddString(LPCTSTR lpszString) const;
		void Clear() const;
		void Copy() const;
		void Cut() const;
		int  DeleteString(int nIndex) const;
		int  Dir(UINT attr, LPCTSTR lpszWildCard ) const;
		int  FindString(int nIndexStart, LPCTSTR lpszString) const;
		int  FindStringExact(int nIndexStart, LPCTSTR lpszString) const;
		int  GetCount() const;
		int  GetCurSel() const;
		CRect GetDroppedControlRect() const;
		BOOL GetDroppedState() const;
		int  GetDroppedWidth() const;
		DWORD GetEditSel() const;
		BOOL GetExtendedUI() const;
		int  GetHorizontalExtent() const;
		DWORD GetItemData(int nIndex) const;
		int  GetItemHeight(int nIndex) const;
		int  GetLBText(int nIndex, LPTSTR lpszText) const;
		int  GetLBTextLen(int nIndex) const;
		LCID GetLocale() const;
		int  GetMinVisible() const;
		int  GetTopIndex() const;
		int  InitStorage(int nItems, int nBytes) const;
		int  InsertString(int nIndex, LPCTSTR lpszString) const;
		void Paste() const;
		void LimitText(int nMaxChars) const;
		void ResetContent() const;
		int  SelectString(int nStartAfter, LPCTSTR lpszString) const;
		int  SetCurSel(int nIndex) const;
		int  SetDroppedWidth(int nWidth) const;
		BOOL SetEditSel(int nStartChar, int nEndChar) const;
		int  SetExtendedUI(BOOL bExtended = TRUE) const;
		void SetHorizontalExtent(UINT nExtent ) const;
		int  SetItemData(int nIndex, DWORD dwItemData) const;
		int  SetItemHeight(int nIndex, UINT cyItemHeight) const;
		LCID SetLocale( LCID NewLocale ) const;
		int  SetTopIndex(int nIndex) const;
		void ShowDropDown(BOOL bShow = TRUE) const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = WC_COMBOBOX; }
	};
	
	
	class CComboBoxEx : public CWnd
	{
	public:
		CComboBoxEx() {}
		virtual ~CComboBoxEx() {}
		virtual tString GetWindowType() const { return _T("CComboBoxEx"); }
		
		int  DeleteItem(int nIndex ) const;
		HWND GetComboBoxCtrl() const;
		HWND GetEditCtrl() const;
		DWORD GetExtendedStyle() const;
		HIMAGELIST GetImageList() const;
		BOOL GetItem(COMBOBOXEXITEM* pCBItem) const;
		BOOL HasEditChanged () const;
		int InsertItem(PCOMBOBOXEXITEM lpcCBItem) const;
		DWORD SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles ) const;
		HIMAGELIST SetImageList(HIMAGELIST himl) const;
		BOOL SetItem(PCOMBOBOXEXITEM lpcCBItem) const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = WC_COMBOBOXEX; }
	};
	
	
	class CProgress : public CWnd
	{
	public:
		CProgress() {}
		virtual ~CProgress() {}
		virtual tString GetWindowType() const { return _T("CProgress"); }
		
		int  GetPos() const;
		int  GetRange(BOOL fWhichLimit, PPBRANGE ppBRange) const;
		int  OffsetPos(int nIncrement) const;
		int  SetPos(int nNewPos) const;
		int  SetRange(short nMinRange, short nMaxRange) const;
		int  SetStep(int nStepInc) const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = PROGRESS_CLASS; }
	};
	
	class CScrollbar : public CWnd
	{
	public:
		CScrollbar() {}
		virtual ~CScrollbar() {}
		virtual tString GetWindowType() const { return _T("CScrollbar"); }
		
		BOOL EnableScrollBar( UINT nArrowFlags = ESB_ENABLE_BOTH )  const;
		BOOL GetScrollInfo(LPSCROLLINFO lpsi)  const;
		int  GetScrollPos()  const;
		BOOL GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos )  const;		
		BOOL SetScrollInfo(LPSCROLLINFO lpsi, BOOL bRedraw = TRUE )  const;		
		int  SetScrollPos(int nPos, BOOL bRedraw)  const;	
		BOOL SetScrollRange( int nMinPos, int nMaxPos, BOOL bRedraw = TRUE )  const;		
		BOOL ShowScrollBar(BOOL bShow)  const;		


	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = _T("SCROLLBAR"); ; }
	};

	
	class CSlider : public CWnd
	{
	public:
		CSlider() {}
		virtual ~CSlider() {}
		virtual tString GetWindowType() const { return _T("CSlider"); }
		
		
		void ClearSel() const; 
		void ClearTics(BOOL bRedraw = FALSE ) const;
		HWND GetBuddy(BOOL fLocation = TRUE ) const;
		CRect GetChannelRect() const;
		int  GetLineSize() const;
		int  GetNumTics() const;
		int  GetPageSize() const;
		int  GetPos() const;
		int  GetRangeMax() const;
		int  GetRangeMin() const;
		int  GetSelEnd() const;
		int  GetSetStart() const;
		int  GetThumbLength() const;
		CRect GetThumbRect() const;
		int  GetTic(int nTic ) const;
		int  GetTicPos(int nTic) const;
		HWND GetToolTips() const;
		HWND SetBuddy(HWND hwndBuddy, BOOL fLocation = TRUE ) const;
		int  SetLineSize(int nSize) const;
		int  SetPageSize(int nSize) const;
		void SetPos(int nPos, BOOL bRedraw = FALSE) const;		
		void SetRangeMax(int nMax, BOOL bRedraw = FALSE) const;
		void SetRangeMin(int nMax, BOOL bRedraw = FALSE) const;
		void SetSelection(int nMin, int nMax, BOOL bRedraw = FALSE) const;
		BOOL SetTic(int nTic) const;
		void SetTicFreq(int nFreq)  const;
		int  SetTipSide(int nLocation) const;
		void SetToolTips(HWND hwndTip) const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = TRACKBAR_CLASS; }
	};


	// Also known as an Up/Down control
	class CSpinButton : public CWnd
	{
	public:
		CSpinButton() {}
		virtual ~CSpinButton() {}
		virtual tString GetWindowType() const { return _T("CSpinButton"); }
		
		int  GetAccel(int cAccels, LPUDACCEL paAccels) const;
		int  GetBase() const;		
		HWND GetBuddy() const;		
		int  GetPos() const;
		void GetRange(int nLower, int nUpper) const;
		BOOL SetAccel(int cAccels, LPUDACCEL paAccels) const;
		int  SetBase(int nBase) const;
		HWND SetBuddy(HWND hwndBuddy) const;
		int  SetPos(int nBase) const;
		void SetRange(int nLower, int nUpper) const;

	protected:
		// Overridables
		virtual void PreCreate(CREATESTRUCT& cs) { cs.lpszClass = UPDOWN_CLASS; }
	};	
	
} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

	////////////////////////////////////////
	// Definitions for the CAnimation class
	//
	BOOL CAnimation::Close() const
	{ 
		assert(IsWindow());
		return Animate_Close(m_hWnd);
	}
	
	BOOL CAnimation::Open(LPTSTR lpszName) const
	{
		assert(IsWindow());
		return Animate_Open(m_hWnd, lpszName); 
	}
	
	BOOL CAnimation::Play(UINT wFrom, UINT wTo, UINT cRepeat) const
	{
		assert(IsWindow());
		return Animate_Play(m_hWnd, wFrom, wTo, cRepeat);
	}
	
	BOOL CAnimation::Seek(UINT wFrame) const
	{
		assert(IsWindow());
		return Animate_Seek(m_hWnd, wFrame);
	}
	
	BOOL CAnimation::Stop() const 
	{
		assert(IsWindow());
		return Animate_Stop(m_hWnd);
	}


	////////////////////////////////////////
	// Definitions for the CComboBox class
	//
	int  CComboBox::AddString(LPCTSTR lpszString) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_ADDSTRING, 0, (LPARAM)lpszString); 
	}

	void CComboBox::Clear() const
	{
		assert(IsWindow());
		SendMessage(WM_CLEAR, 0, 0);
	}

	void CComboBox::Copy() const 
	{
		assert(IsWindow());
		SendMessage(WM_COPY, 0, 0);
	}

	void CComboBox::Cut() const 
	{
		assert(IsWindow());
		SendMessage(WM_CUT, 0, 0); 
	}

	int  CComboBox::DeleteString(int nIndex) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_DELETESTRING, (WPARAM)nIndex, 0);
	}

	int  CComboBox::Dir(UINT attr, LPCTSTR lpszWildCard ) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_DIR, (WPARAM)attr, (LPARAM)lpszWildCard);
	}
	
	int  CComboBox::FindString(int nIndexStart, LPCTSTR lpszString) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_FINDSTRING, (WPARAM)nIndexStart, (LPARAM)lpszString);
	}

	int  CComboBox::FindStringExact(int nIndexStart, LPCTSTR lpszString) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_FINDSTRINGEXACT, (WPARAM)nIndexStart, (LPARAM)lpszString); 
	}
	
	int  CComboBox::GetCount() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETCOUNT, 0,0); 
	}
	
	int  CComboBox::GetCurSel() const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETCURSEL, 0,0); 
	}
	
	CRect CComboBox::GetDroppedControlRect() const 
	{
		assert(IsWindow());
		CRect rc; 
		SendMessage(CB_GETDROPPEDCONTROLRECT, 0, (LPARAM)&rc); 
		return rc;
	}
	
	BOOL CComboBox::GetDroppedState() const 
	{ 
		assert(IsWindow());
		return (BOOL)SendMessage(CB_GETDROPPEDSTATE, 0, 0); 
	}
	
	int  CComboBox::GetDroppedWidth() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETDROPPEDWIDTH, 0, 0);
	}		

	DWORD CComboBox::GetEditSel() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETEDITSEL, 0, 0);
	}

	BOOL CComboBox::GetExtendedUI() const 
	{
		assert(IsWindow());
		return (BOOL)SendMessage(CB_GETEXTENDEDUI, 0, 0);
	}

	int  CComboBox::GetHorizontalExtent() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETHORIZONTALEXTENT, 0, 0);
	} 

	DWORD CComboBox::GetItemData(int nIndex) const 
	{
		assert(IsWindow());
		return (DWORD)SendMessage(CB_GETITEMDATA, (WPARAM)nIndex, 0);
	}

	int  CComboBox::GetItemHeight(int nIndex) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETITEMHEIGHT, (WPARAM)nIndex, 0);
	}

	int  CComboBox::GetLBText(int nIndex, LPTSTR lpszText) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETLBTEXT, (WPARAM)nIndex, (LPARAM)lpszText);
	}

	int  CComboBox::GetLBTextLen(int nIndex) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETLBTEXTLEN, (WPARAM)nIndex, 0); 
	}
	
	LCID CComboBox::GetLocale() const 
	{
		assert(IsWindow());
		return (LCID)SendMessage(CB_GETLOCALE, 0, 0); 
	}
	
	int  CComboBox::GetMinVisible() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETMINVISIBLE, 0, 0);
	}

	int  CComboBox::GetTopIndex() const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_GETTOPINDEX, 0, 0);
	}

	int  CComboBox::InitStorage(int nItems, int nBytes) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_INITSTORAGE, (WPARAM)nItems, (LPARAM)nBytes);
	}

	int  CComboBox::InsertString(int nIndex, LPCTSTR lpszString) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_INSERTSTRING, (WPARAM)nIndex, (LPARAM)lpszString);
	}

	void CComboBox::Paste() const 
	{
		assert(IsWindow());
		SendMessage(WM_PASTE, 0, 0);
	}

	void CComboBox::LimitText(int nMaxChars) const 
	{
		assert(IsWindow());
		SendMessage(CB_LIMITTEXT, (WPARAM)nMaxChars, 0);
	}

	void CComboBox::ResetContent() const 
	{
		assert(IsWindow());
		SendMessage(CB_RESETCONTENT, 0, 0);
	}

	int  CComboBox::SelectString(int nStartAfter, LPCTSTR lpszString) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SELECTSTRING, (WPARAM)nStartAfter, (LPARAM)lpszString);
	}

	int  CComboBox::SetCurSel(int nIndex) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETCURSEL, (WPARAM)nIndex, 0); 
	}
	
	int  CComboBox::SetDroppedWidth(int nWidth) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETDROPPEDWIDTH, (WPARAM)nWidth, 0);
	}

	BOOL CComboBox::SetEditSel(int nStartChar, int nEndChar) const 
	{
		assert(IsWindow());
		return (BOOL)SendMessage(CB_SETEDITSEL, 0, (LPARAM)MAKELONG(nStartChar,nEndChar));
	}

	int  CComboBox::SetExtendedUI(BOOL bExtended) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETEXTENDEDUI, (WPARAM)bExtended, 0);
	}

	void CComboBox::SetHorizontalExtent(UINT nExtent ) const 
	{
		assert(IsWindow());
		SendMessage(CB_SETHORIZONTALEXTENT, (WPARAM)nExtent, 0);
	}

	int  CComboBox::SetItemData(int nIndex, DWORD dwItemData) const
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETITEMDATA, (WPARAM)nIndex, (LPARAM)dwItemData);
	}

	int  CComboBox::SetItemHeight(int nIndex, UINT cyItemHeight) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETITEMHEIGHT, (WPARAM)nIndex, (LPARAM)cyItemHeight);
	}

	LCID CComboBox::SetLocale( LCID NewLocale ) const 
	{
		assert(IsWindow());
		return (LCID)SendMessage(CB_SETLOCALE, (WPARAM)NewLocale, 0); 
	}
	
	int  CComboBox::SetTopIndex(int nIndex) const 
	{
		assert(IsWindow());
		return (int)SendMessage(CB_SETTOPINDEX, (WPARAM)nIndex, 0);
	}

	void CComboBox::ShowDropDown(BOOL bShow) const 
	{
		assert(IsWindow());
		SendMessage(CB_SHOWDROPDOWN, (WPARAM)bShow, 0); 
	}


	////////////////////////////////////////
	// Definitions for the CComboBoxEx class
	//
	int  CComboBoxEx::DeleteItem(int nIndex ) const
	{
		assert(IsWindow());
		return (int)SendMessage(CBEM_DELETEITEM, (WPARAM)nIndex, 0); 
	}
	
	HWND CComboBoxEx::GetComboBoxCtrl() const 
	{
		assert(IsWindow());
		return (HWND)SendMessage(CBEM_GETCOMBOCONTROL, 0, 0); 
	}
	
	HWND CComboBoxEx::GetEditCtrl() const 
	{ 
		assert(IsWindow());
		return (HWND)SendMessage(CBEM_GETEDITCONTROL, 0, 0);
	}

	DWORD CComboBoxEx::GetExtendedStyle() const 
	{
		assert(IsWindow());
		return (DWORD)SendMessage(CBEM_GETEXTENDEDSTYLE, 0, 0); 
	}
	
	HIMAGELIST CComboBoxEx::GetImageList() const 
	{ 
		assert(IsWindow());
		return (HIMAGELIST)SendMessage(CBEM_GETIMAGELIST, 0, 0);
	}

	BOOL CComboBoxEx::GetItem(COMBOBOXEXITEM* pCBItem) const
	{
		assert(IsWindow());
		return (BOOL)SendMessage(CBEM_GETITEM, 0, (LPARAM)pCBItem);
	}
	
	BOOL CComboBoxEx::HasEditChanged () const 
	{
		assert(IsWindow());
		return (BOOL)SendMessage(CBEM_HASEDITCHANGED, 0, 0);
	}

	int CComboBoxEx::InsertItem(PCOMBOBOXEXITEM lpcCBItem) const
	{
		assert(IsWindow());
		return (int)SendMessage(CBEM_INSERTITEM, 0, (LPARAM)lpcCBItem);
	}

	DWORD CComboBoxEx::SetExtendedStyle(DWORD dwExMask, DWORD dwExStyles ) const 
	{
		assert(IsWindow());
		return (DWORD)SendMessage(CBEM_SETEXTENDEDSTYLE, (WPARAM)dwExMask, (LPARAM)dwExStyles);
	}

	HIMAGELIST CComboBoxEx::SetImageList(HIMAGELIST himl) const 
	{
		assert(IsWindow());
		return (HIMAGELIST)SendMessage(CBEM_SETIMAGELIST, 0, (LPARAM)himl);
	}
	
	BOOL CComboBoxEx::SetItem(PCOMBOBOXEXITEM lpcCBItem) const 
	{
		assert(IsWindow());
		return (BOOL)SendMessage(CBEM_SETITEM, 0, (LPARAM)lpcCBItem);
	} 


	////////////////////////////////////////
	// Definitions for the CProgress class
	//
	int CProgress::GetPos() const
	{ 
		assert(IsWindow());
		return (int)SendMessage(PBM_GETPOS, 0, 0);
	}
		
	int CProgress::GetRange(BOOL fWhichLimit, PPBRANGE ppBRange) const
	{ 
		assert(IsWindow());
		return (int)SendMessage(PBM_GETRANGE, (WPARAM)fWhichLimit, (LPARAM) (PPBRANGE) ppBRange); 
	}

	int CProgress::OffsetPos(int nIncrement) const
	{
		assert(IsWindow());
		return (int)SendMessage(PBM_DELTAPOS, (WPARAM)nIncrement, 0);
	}

	int CProgress::SetPos(int nNewPos) const
	{
		assert(IsWindow());
		return (int)SendMessage(PBM_SETPOS, (WPARAM)nNewPos, 0); 
	}

	int CProgress::SetRange(short nMinRange, short nMaxRange) const
	{
		assert(IsWindow());
		return SendMessage(PBM_SETRANGE, 0, (LPARAM) MAKELPARAM (nMinRange, nMaxRange)); 
	}

	int CProgress::SetStep(int nStepInc) const 
	{
		assert(IsWindow());
		return (int)SendMessage(PBM_SETSTEP, (WPARAM)nStepInc, 0);
	}


	////////////////////////////////////////
	// Definitions for the CScrollbar class
	//
	BOOL CScrollbar::EnableScrollBar( UINT nArrowFlags )  const
	{
		assert(IsWindow());
		return ::EnableScrollBar(m_hWnd, SB_CTL, nArrowFlags);
	}

	BOOL CScrollbar::GetScrollInfo(LPSCROLLINFO lpsi)  const
	{
		assert(IsWindow());
		return ::GetScrollInfo(m_hWnd, SB_CTL, lpsi); 
	}

	int CScrollbar::GetScrollPos()  const
	{
		assert(IsWindow());
		return ::GetScrollPos(m_hWnd, SB_CTL);
	}
	
	BOOL CScrollbar::GetScrollRange(LPINT lpMinPos, LPINT lpMaxPos )  const
	{
		assert(IsWindow());
		return ::GetScrollRange(m_hWnd, SB_CTL, lpMinPos, lpMaxPos);
	}

	BOOL CScrollbar::SetScrollInfo(LPSCROLLINFO lpsi, BOOL bRedraw )  const
	{
		assert(IsWindow());
		return ::SetScrollInfo(m_hWnd, SB_CTL, lpsi, bRedraw);
	}

	int CScrollbar::SetScrollPos(int nPos, BOOL bRedraw)  const
	{
		assert(IsWindow());
		return ::SetScrollPos(m_hWnd, SB_CTL, nPos, bRedraw);
	}

	BOOL CScrollbar::SetScrollRange( int nMinPos, int nMaxPos, BOOL bRedraw )  const
	{
		assert(IsWindow());
		return ::SetScrollRange(m_hWnd, SB_CTL, nMinPos, nMaxPos, bRedraw);
	}
	
	BOOL CScrollbar::ShowScrollBar(BOOL bShow)  const 
	{
		assert(IsWindow());
		return ::ShowScrollBar(m_hWnd, SB_CTL, bShow);
	}

	////////////////////////////////////////
	// Definitions for the CSlider class
	//
	void CSlider::ClearSel() const 
	{ 
		assert(IsWindow());
		SendMessage(TBM_CLEARSEL, 0, 0);
	} 
	
	void CSlider::ClearTics(BOOL bRedraw) const
	{ 
		assert(IsWindow());
		SendMessage(TBM_CLEARTICS, (WPARAM)bRedraw, 0); 
	} 
	
	HWND CSlider::GetBuddy(BOOL fLocation) const
	{
		assert(IsWindow());
		return (HWND)SendMessage(TBM_GETBUDDY, (WPARAM)fLocation, 0); 
	}
	
	CRect CSlider::GetChannelRect() const 
	{
		assert(IsWindow());
		CRect rc;
		SendMessage(TBM_GETCHANNELRECT, 0, (LPARAM)&rc);
		return rc;
	}		
	
	int  CSlider::GetLineSize() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETLINESIZE, 0, 0); 
	}
	
	int  CSlider::GetNumTics() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETNUMTICS, 0, 0);
	}
	
	int  CSlider::GetPageSize() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETPAGESIZE, 0, 0);
	}
	
	int  CSlider::GetPos() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETPOS, 0, 0);
	}
	
	int  CSlider::GetRangeMax() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETRANGEMAX, 0, 0);
	}
	
	int  CSlider::GetRangeMin() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETRANGEMIN, 0, 0);
	}

	int  CSlider::GetSelEnd() const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETSELEND, 0, 0);
	}
	
	int  CSlider::GetSetStart() const 
	{ 
		assert(IsWindow());
		return (int)SendMessage(TBM_GETSELSTART, 0, 0); 
	}
	
	int  CSlider::GetThumbLength() const 
	{ 
		assert(IsWindow());
		return (int)SendMessage(TBM_GETTHUMBLENGTH, 0, 0);
	}

	CRect CSlider::GetThumbRect() const
	{ 
		CRect rc;
		SendMessage(TBM_GETTHUMBRECT, 0, (LPARAM)&rc);
		return rc;
	}
	
	int CSlider::GetTic(int nTic ) const 
	{ 
		assert(IsWindow());
		return (int)SendMessage(TBM_GETTIC, (WPARAM)nTic, 0); 
	}

	int  CSlider::GetTicPos(int nTic) const 
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_GETTICPOS, (WPARAM)nTic, 0);
	}
	
	HWND CSlider::GetToolTips() const 
	{
		assert(IsWindow());
		return (HWND)SendMessage(TBM_GETTOOLTIPS, 0, 0); 
	}
	
	HWND CSlider::SetBuddy(HWND hwndBuddy, BOOL fLocation) const 
	{
		assert(IsWindow());
		return (HWND)SendMessage(TBM_SETBUDDY, (WPARAM)fLocation, (LPARAM)hwndBuddy);
	}
	
	int  CSlider::SetLineSize(int nSize) const 
	{
		assert(IsWindow());
		return(int)SendMessage(TBM_SETLINESIZE, 0, (LPARAM)nSize);
	}
	
	int  CSlider::SetPageSize(int nSize) const 
	{
		assert(IsWindow());
		return(int)SendMessage(TBM_SETPAGESIZE, 0, (LPARAM)nSize);
	}
	
	void CSlider::SetPos(int nPos, BOOL bRedraw) const
	{
		assert(IsWindow());
		SendMessage(TBM_SETPOS, (WPARAM)bRedraw, (LPARAM)nPos);
	}	
	
	void CSlider::SetRangeMax(int nMax, BOOL bRedraw) const
	{
		assert(IsWindow());
		SendMessage(TBM_SETRANGEMAX, (WPARAM)bRedraw, (LPARAM)nMax);
	}

	void CSlider::SetRangeMin(int nMax, BOOL bRedraw) const
	{
		assert(IsWindow());
		SendMessage(TBM_SETRANGEMIN, (WPARAM)bRedraw, (LPARAM)nMax);
	}

	void CSlider::SetSelection(int nMin, int nMax, BOOL bRedraw) const
	{
		assert(IsWindow());
		SendMessage(TBM_SETSEL, (WPARAM)bRedraw, (LPARAM)MAKELONG(nMax, nMin));
	}

	BOOL CSlider::SetTic(int nTic) const
	{
		assert(IsWindow());
		return (BOOL)SendMessage(TBM_SETTIC, 0, nTic);
	}

	void CSlider::SetTicFreq(int nFreq)  const
	{
		assert(IsWindow());
		SendMessage(TBM_SETTICFREQ, (WPARAM)nFreq, 0);
	}

	int  CSlider::SetTipSide(int nLocation) const
	{
		assert(IsWindow());
		return (int)SendMessage(TBM_SETTIPSIDE, (WPARAM)nLocation, 0);
	}

	void CSlider::SetToolTips(HWND hwndTip) const 
	{
		assert(IsWindow());
		SendMessage(TBM_SETTOOLTIPS, (WPARAM)hwndTip, 0);
	}

	////////////////////////////////////////
	// Definitions for the CSpinButton class
	//
	int CSpinButton::GetAccel(int cAccels, LPUDACCEL paAccels) const
	{ 
		assert(IsWindow());
		return (int)SendMessage(UDM_GETACCEL, (WPARAM)cAccels, (LPARAM)paAccels);
	}
	
	int CSpinButton::GetBase() const
	{ 
		assert(IsWindow());
		return (int)SendMessage(UDM_GETBASE, 0, 0);
	}
	
	HWND CSpinButton::GetBuddy() const
	{
		assert(IsWindow());
		return (HWND)SendMessage(UDM_GETBUDDY, 0, 0);
	}
	
	int CSpinButton::GetPos() const 
	{
		assert(IsWindow());
		return (int)SendMessage(UDM_GETPOS, 0, 0);
	}

	void CSpinButton::GetRange(int nLower, int nUpper) const
	{
		assert(IsWindow());
		LRESULT lr = SendMessage(UDM_GETRANGE, 0, 0); 
		nLower = HIWORD(lr); 
		nUpper = LOWORD(lr);
	}

	BOOL CSpinButton::SetAccel(int cAccels, LPUDACCEL paAccels) const 
	{
		assert(IsWindow());
		return (BOOL)SendMessage(UDM_SETACCEL, (WPARAM)cAccels, (LPARAM)paAccels);
	}

	int CSpinButton::SetBase(int nBase) const
	{ 
		assert(IsWindow());
		return (int)SendMessage(UDM_SETBASE, (WPARAM)nBase, 0);
	}

	HWND CSpinButton::SetBuddy(HWND hwndBuddy) const 
	{ 
		assert(IsWindow());
		return (HWND)SendMessage(UDM_SETBUDDY, (WPARAM)hwndBuddy, 0); 
	}

	int CSpinButton::SetPos(int nBase) const
	{ 
		assert(IsWindow());
		return (int)SendMessage(UDM_SETBASE, (WPARAM)nBase, 0);
	}
	
	void CSpinButton::SetRange(int nLower, int nUpper) const
	{
		assert(IsWindow());
		SendMessage(UDM_SETRANGE, 0, (LPARAM)MAKELONG(nUpper, nLower));
	}

} // namespace Win32xx

#endif //  define _CONTORLS_H_

