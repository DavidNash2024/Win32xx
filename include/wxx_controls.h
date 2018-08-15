// Win32++   Version 8.5.1
// Release Date: TBA
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2017  David Nash
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
// wxx_controls.h
//  Declaration of the following classes:
//  CAnimation, CComboBox, CComboBoxEx, CDateTime, CIPAddress,
//  CHeader, CHotKey, CMonthCalendar, CProgressBar, CScrollBar,
//  CSlider, CSpinButton, and CToolTip.


#ifndef _WIN32XX_CONTROLS_H_
#define _WIN32XX_CONTROLS_H_


#include "wxx_wincore0.h"
#include "wxx_stdcontrols.h"
#include "wxx_imagelist.h"
#include "wxx_ddx.h"


namespace Win32xx
{

    class CMonthCalendar;
    class CToolTip;


    // The CAnimation class provides the functionality of an animation control.
    class CAnimation : public CWnd
    {
    public:
        CAnimation() {}
        virtual ~CAnimation() {}

        BOOL Close() const;
        BOOL Open(LPTSTR pPathName) const;
        BOOL Play(int from, int to, int repeat) const;
        BOOL Seek(int frame) const;
        BOOL Stop() const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = ANIMATE_CLASS; }

    private:
        CAnimation(const CAnimation&);              // Disable copy construction
        CAnimation& operator = (const CAnimation&); // Disable assignment operator
    };


    // The CComboBox class provides the functionality of a combo box control.
    class CComboBox : public CWnd
    {
    public:
        CComboBox() {}
        virtual ~CComboBox() {}

        virtual int CompareItem(LPCOMPAREITEMSTRUCT pCompareItemStruct);

        int   AddString(LPCTSTR pString) const;
        void  Clear() const;
        void  Copy() const;
        void  Cut() const;
        int   DeleteString(int index) const;
        int   Dir(UINT attr, LPCTSTR pWildCard ) const;
        int   FindString(int indexStart, LPCTSTR pString) const;
        int   FindStringExact(int indexStart, LPCTSTR pString) const;
        int   GetCount() const;
        int   GetCurSel() const;
        CRect GetDroppedControlRect() const;
        BOOL  GetDroppedState() const;
        int   GetDroppedWidth() const;
        DWORD GetEditSel() const;
        BOOL  GetExtendedUI() const;
        int   GetHorizontalExtent() const;
        DWORD GetItemData(int index) const;
        int   GetItemHeight(int index) const;
        int   GetLBText(int index, LPTSTR pText) const;
        int   GetLBTextLen(int index) const;
        LCID  GetLocale() const;
        int   GetTopIndex() const;
        int   InitStorage(int items, int byteCount) const;
        int   InsertString(int index, LPCTSTR pString) const;
        void  LimitText(int maxChars) const;
        void  Paste() const;
        void  ResetContent() const;
        int   SelectString(int startAfter, LPCTSTR pString) const;
        int   SetCurSel(int index) const;
        int   SetDroppedWidth(int width) const;
        BOOL  SetEditSel(int startChar, int endChar) const;
        int   SetExtendedUI(BOOL isExtended = TRUE) const;
        void  SetHorizontalExtent(UINT extent ) const;
        int   SetItemData(int index, DWORD itemData) const;
        int   SetItemHeight(int index, UINT cyItemHeight) const;
        LCID  SetLocale( LCID newLocale ) const;
        int   SetTopIndex(int index) const;
        void  ShowDropDown(BOOL show = TRUE) const;

#if WINVER >= 0x0500
        HWND  GetEditCtrl() const;
        HWND  GetLBCtrl() const;
#endif

    protected:
        // Overridables
        virtual LRESULT OnMessageReflect(UINT msg, WPARAM, LPARAM lparam);
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = _T("ComboBox"); }

    private:
        CComboBox(const CComboBox&);                // Disable copy construction
        CComboBox& operator = (const CComboBox&);   // Disable assignment operator
    };


    // The CComboBoxEx class provides the functionality of a ComboBoxEx control.
    class CComboBoxEx : public CComboBox
    {
    public:
        CComboBoxEx() {}
        virtual ~CComboBoxEx() {}

        int     DeleteItem(int index ) const;
        HWND    GetComboBoxCtrl() const;
        HWND    GetEditCtrl() const;
        CImageList GetImageList() const;
        BOOL    GetItem(COMBOBOXEXITEM& cbItem) const;
        BOOL    HasEditChanged () const;
        int     InsertItem(const COMBOBOXEXITEM& cbItem) const;
        CImageList SetImageList(HIMAGELIST images) const;
        BOOL    SetItem(const COMBOBOXEXITEM& cbItem) const;
        DWORD   GetExtendedStyle() const;
        DWORD   SetExtendedStyle(DWORD exMask, DWORD exStyles ) const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = WC_COMBOBOXEX; }

    private:
        CComboBoxEx(const CComboBoxEx&);                // Disable copy construction
        CComboBoxEx& operator = (const CComboBoxEx&);   // Disable assignment operator
    };


    // The CHeader class provides the functionality of a header control.
    class CHeader : public CWnd
    {
    public:
        CHeader() {}
        virtual ~CHeader() {}

        // Attributes
        CImageList GetImageList() const;
        BOOL    GetItem(int pos, HDITEM& headerItem) const;
        int     GetItemCount() const;
        CRect   GetItemRect(int index) const;
        BOOL    GetOrderArray(LPINT pArray, int count) const;
        int     OrderToIndex(int order) const;
        CImageList SetImageList(HIMAGELIST images) const;
        BOOL    SetItem(int pos, const HDITEM& headerItem) const;
        BOOL    SetOrderArray(int count, LPINT pArray) const;
        int     GetBitmapMargin() const;
        int     SetBitmapMargin(int width) const;

        // Operations
        CImageList CreateDragImage(int index) const;
        BOOL    DeleteItem(int pos) const;
        int     InsertItem(int pos, const HDITEM& headerItem) const;
        BOOL    Layout(HDLAYOUT* pHeaderLayout) const;
#ifdef Header_SetHotDivider
        int     SetHotDivider(CPoint pt) const;
        int     SetHotDivider(int index) const;
#endif
#ifdef Header_ClearFilter
        int     ClearAllFilters() const;
        int     ClearFilter(int column) const;
        int     EditFilter(int column, BOOL discardChanges) const;
        int     SetFilterChangeTimeout(DWORD timeout) const;
#endif

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = WC_HEADER ; }

    private:
        CHeader(const CHeader&);                // Disable copy construction
        CHeader& operator = (const CHeader&);   // Disable assignment operator
    };


    // The CHotKey class provides the functionality a hot key control.
    class CHotKey : public CWnd
    {
    public:
        CHotKey() {}
        virtual ~CHotKey() {}

        DWORD GetHotKey() const;
        CString GetKeyName(UINT keyCode, BOOL isExtended) const;
        void SetHotKey(DWORD key) const;
        void SetRules(WORD invalidComb, WORD modifiers) const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = HOTKEY_CLASS; }

    private:
        CHotKey(const CHotKey&);                // Disable copy construction
        CHotKey& operator = (const CHotKey&);   // Disable assignment operator
    };


    // The CIPAddress class provides the functionality of an IP Address control.
    class CIPAddress : public CWnd
    {
    public:
        CIPAddress();
        virtual ~CIPAddress() {}

        void ClearAddress() const;
        int GetAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3) const;
        int GetAddress(DWORD& address) const;
        BOOL IsBlank() const;
        void SetAddress(BYTE field0, BYTE field1, BYTE field2, BYTE field3) const;
        void SetAddress(DWORD address) const;
        void SetFieldFocus(WORD field) const;
        void SetFieldRange(int field, BYTE lower, BYTE upper) const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = WC_IPADDRESS; }

    private:
        CIPAddress(const CIPAddress&);              // Disable copy construction
        CIPAddress& operator = (const CIPAddress&); // Disable assignment operator
    };


    // The CMonthCalendar class provides the functionality of a Month Calendar control.
    class CMonthCalendar : public CWnd
    {
    public:
        CMonthCalendar();
        virtual ~CMonthCalendar() {}

        // Attributes
        COLORREF GetColor(int region) const;
        int GetFirstDayOfWeek(BOOL* pLocal = NULL) const;
        CRect GetMinReqRect() const;
        int GetMonthDelta() const;
        COLORREF SetColor(int region, COLORREF ref) const;
        BOOL SetFirstDayOfWeek(int day, int* pOldDay = NULL) const;
        int SetMonthDelta(int delta) const;

        // Operations
        SYSTEMTIME GetCurSel() const;
        int GetMaxSelCount() const;
        int GetMonthRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange, DWORD flags) const;
        LRESULT GetRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const;
        LRESULT GetSelRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const;
        SYSTEMTIME GetToday() const;
        LRESULT HitTest(MCHITTESTINFO& mcHitTest) const;
        BOOL SetCurSel(const SYSTEMTIME& dateTime) const;
        BOOL SetDayState(int months, const MONTHDAYSTATE& states) const;
        BOOL SetMaxSelCount(int max) const;
        BOOL SetRange(const SYSTEMTIME& minRange, const SYSTEMTIME& maxRange) const;
        BOOL SetSelRange(const SYSTEMTIME& MinRange, const SYSTEMTIME& maxRange) const;
        void SetToday(const SYSTEMTIME&  dateTime) const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = MONTHCAL_CLASS; }

    private:
        CMonthCalendar(const CMonthCalendar&);              // Disable copy construction
        CMonthCalendar& operator = (const CMonthCalendar&); // Disable assignment operator
    };


    // The CDateTime class provides the functionality of a Date Time control.
    class CDateTime : public CWnd
    {
    public:
        CDateTime();
        virtual ~CDateTime() {}

        COLORREF GetMonthCalColor(int color) const;
        HWND GetMonthCalCtrl() const;
        CFont GetMonthCalFont() const;
        DWORD GetRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const;
        SYSTEMTIME GetTime(DWORD* pReturnCode = NULL) const;
        COLORREF SetMonthCalColor(int color, COLORREF ref) const;
        BOOL SetFormat(LPCTSTR pFormat) const;
        void SetMonthCalFont(HFONT font, BOOL redraw = TRUE) const;
        BOOL SetRange(const SYSTEMTIME& minRange, const SYSTEMTIME& maxRange) const;
        BOOL SetTime(const SYSTEMTIME& time) const;
        BOOL SetTimeNone() const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = DATETIMEPICK_CLASS; }

    private:
        CDateTime(const CDateTime&);                // Disable copy construction
        CDateTime& operator = (const CDateTime&);   // Disable assignment operator
    };


    // The CProgressBar class provides the functionality of a ProgressBar control.
    class CProgressBar : public CWnd
    {
    public:
        CProgressBar() {}
        virtual ~CProgressBar() {}

        int  GetPos() const;
        int  GetRange(BOOL whichLimit, const PBRANGE& range) const;
        int  GetRange(BOOL whichLimit) const;
        int  OffsetPos(int increment) const;
        int  SetPos(int pos) const;
        int  SetRange(short minRange, short maxRange) const;
        int  SetStep(int stepInc) const;
        int  StepIt() const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = PROGRESS_CLASS; }

    private:
        CProgressBar(const CProgressBar&);              // Disable copy construction
        CProgressBar& operator = (const CProgressBar&); // Disable assignment operator
    };


    // The CScrollBar class provides the functionality of a ScrollBar control.
    class CScrollBar : public CWnd
    {
    public:
        CScrollBar() {}
        virtual ~CScrollBar() {}

        BOOL EnableScrollBar( UINT arrowFlags = ESB_ENABLE_BOTH )  const;
        BOOL GetScrollInfo(SCROLLINFO& si)  const;
        int  GetScrollPos()  const;
        BOOL GetScrollRange(int& minPos, int& maxPos)  const;
        BOOL SetScrollInfo(const SCROLLINFO& si, BOOL redraw = TRUE )  const;
        int  SetScrollPos(int pos, BOOL redraw)  const;
        BOOL SetScrollRange( int minPos, int maxPos, BOOL redraw = TRUE )  const;
        BOOL ShowScrollBar(BOOL show)  const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = _T("SCROLLBAR"); ; }

    private:
        CScrollBar(const CScrollBar&);              // Disable copy construction
        CScrollBar& operator = (const CScrollBar&); // Disable assignment operator
    };


    // The CSlider class provides the functionality of a TrackBar (aka Slider) control.
    class CSlider : public CWnd
    {
    public:
        CSlider() {}
        virtual ~CSlider() {}

        void ClearSel() const;
        void ClearTics(BOOL redraw = FALSE ) const;
        HWND GetBuddy(BOOL location = TRUE ) const;
        CRect GetChannelRect() const;
        int  GetLineSize() const;
        int  GetNumTics() const;
        int  GetPageSize() const;
        int  GetPos() const;
        int  GetRangeMax() const;
        int  GetRangeMin() const;
        int  GetSelEnd() const;
        int  GetSelStart() const;
        int  GetThumbLength() const;
        CRect GetThumbRect() const;
        int  GetTic(int nTic ) const;
        int  GetTicPos(int nTic) const;
        HWND GetToolTips() const;
        HWND SetBuddy(HWND buddy, BOOL location = TRUE ) const;
        int  SetLineSize(int size) const;
        int  SetPageSize(int size) const;
        void SetPos(int pos, BOOL redraw = FALSE) const;
        void SetRangeMax(int max, BOOL redraw = FALSE) const;
        void SetRangeMin(int min, BOOL redraw = FALSE) const;
        void SetSelection(int min, int max, BOOL redraw = FALSE) const;
        BOOL SetTic(int tic) const;
        void SetTicFreq(int freq)  const;
        int  SetTipSide(int location) const;
        void SetToolTips(HWND toolTip) const;

    protected:
        // Overridables
        virtual void PreRegisterClass(WNDCLASS& wc) { wc.lpszClassName = TRACKBAR_CLASS; }

    private:
        CSlider(const CSlider&);                // Disable copy construction
        CSlider& operator = (const CSlider&);   // Disable assignment operator
    };


    // The CSpinButton class provides the functionality of an Up-Down (aka SpinButton) control.
    class CSpinButton : public CWnd
    {
    public:
        CSpinButton() {}
        virtual ~CSpinButton() {}

        int  GetAccel(int accels, LPUDACCEL pAccels) const;
        int  GetBase() const;
        HWND GetBuddy() const;
        int  GetPos() const;
        DWORD GetRange() const;
        BOOL SetAccel(int accels, LPUDACCEL pAccels) const;
        int  SetBase(int base) const;
        HWND SetBuddy(HWND buddy) const;
        int  SetPos(int pos) const;
        void SetRange(int lower, int upper) const;

    protected:
        // Overridables
        virtual void PreCreate(CREATESTRUCT& cs);
        virtual void PreRegisterClass(WNDCLASS& wc);

    private:
        CSpinButton(const CSpinButton&);                // Disable copy construction
        CSpinButton& operator = (const CSpinButton&);   // Disable assignment operator
    };


    // The CToolTip class provides the functionality of a ToolTip control.
    class CToolTip : public CWnd
    {
    public:
        CToolTip();
        virtual ~CToolTip();

        //Attributes
        int      GetDelayTime(DWORD duration) const;
        CRect    GetMargin() const;
        int      GetMaxTipWidth() const;
        CString  GetText(HWND control, UINT id = -1) const;
        COLORREF GetTipBkColor() const;
        COLORREF GetTipTextColor() const;
        int      GetToolCount() const;
        TOOLINFO GetToolInfo(HWND control, UINT id = -1) const;
        void     SetDelayTime(UINT delay) const;
        void     SetDelayTime(DWORD duration, int time) const;
        void     SetMargin(const RECT& rc) const;
        int      SetMaxTipWidth(int width) const;
        void     SetTipBkColor(COLORREF color) const;
        void     SetTipTextColor(COLORREF color) const;
        void     SetToolInfo(const TOOLINFO& toolInfo) const;
#if (_WIN32_IE >=0x0500)
        CSize    GetBubbleSize(HWND control, UINT id = -1) const;
#endif

        //Operations
        void Activate(BOOL Activate) const;
        BOOL AddTool(HWND control, const RECT& toolRect, UINT id, UINT textID) const;
        BOOL AddTool(HWND control, UINT textID) const;
        BOOL AddTool(HWND control, const RECT& toolRect, UINT id, LPCTSTR pText = LPSTR_TEXTCALLBACK) const;
        BOOL AddTool(HWND control, LPCTSTR pText = LPSTR_TEXTCALLBACK) const;
        void DelTool(HWND control, UINT id = -1) const;
        BOOL HitTest(HWND wnd, CPoint pt, const TOOLINFO& toolInfo) const;
        void Pop() const;
        void RelayEvent(MSG& msg) const;
        void SetToolRect(const RECT& rc, HWND control, UINT id = -1) const;
        void Update() const;
        void UpdateTipText(LPCTSTR pText, HWND control, UINT id = -1) const;
        void UpdateTipText(UINT textID, HWND control, UINT id = -1) const;

#if (_WIN32_IE >=0x0500)
        BOOL AdjustRect(RECT& rc, BOOL isLarger = TRUE) const;
  #ifdef TTM_SETTITLE
        BOOL SetTitle(UINT icon, LPCTSTR pTitle) const;
  #endif
#endif
#if (WINVER >= 0x0501) && defined(TTM_SETWINDOWTHEME)
        void SetTTWindowTheme(LPCWSTR pTheme) const;
#endif

    protected:
        // Overridables
        virtual void FillToolInfo(TOOLINFO& info, HWND control) const;
        virtual void FillToolInfo(TOOLINFO& info, HWND control, const RECT& rc, UINT id) const;
        virtual void PreCreate(CREATESTRUCT& cs);
        virtual void PreRegisterClass(WNDCLASS& wc);
    private:
        CToolTip(const CToolTip&);              // Disable copy construction
        CToolTip& operator = (const CToolTip&); // Disable assignment operator

    };


} // namespace Win32xx

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

namespace Win32xx
{

    ////////////////////////////////////////
    // Definitions for the CAnimation class
    //

    // Closes an AVI clip.
    inline BOOL CAnimation::Close() const
    {
        assert(IsWindow());
        return Animate_Close(*this);
    }

    // Opens an AVI clip and displays its first frame in an animation control.
    inline BOOL CAnimation::Open(LPTSTR pPathName) const
    {
        assert(IsWindow());
        return Animate_Open(*this, pPathName);
    }

    // Plays an AVI clip in an animation control. The control plays the clip
    // in the background while the thread continues executing.
    //  from : Zero - based index of the frame where playing begins. The value must be less than 65,536.
    //  to:    Zero - based index of the frame where playing ends. The value must be less than 65,536.
    //          A value of - 1 means end with the last frame in the AVI clip.
    //  repeat: Number of times to replay the AVI clip.A value of - 1 means replay the clip indefinitely.
    inline BOOL CAnimation::Play(int from, int to, int repeat) const
    {
        assert(IsWindow());
        return Animate_Play(*this, from, to, repeat);
    }

    // Directs an animation control to display a particular frame of an AVI clip.
    // The control displays the clip in the background while the thread continues executing.
    inline BOOL CAnimation::Seek(int frame) const
    {
        assert(IsWindow());
        return Animate_Seek(*this, frame);
    }

    // Stops playing an AVI clip in an animation control.
    inline BOOL CAnimation::Stop() const
    {
        assert(IsWindow());
        return Animate_Stop(*this);
    }


    ////////////////////////////////////////
    // Definitions for the CComboBox class
    //

    // Adds a string to the list box of a combo box. If the combo box does not
    // have the CBS_SORT style, the string is added to the end of the list.
    // Otherwise, the string is inserted into the list, and the list is sorted.
    inline int  CComboBox::AddString(LPCTSTR pString) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(pString)));
    }

    // Called by in response to the WM_COMPAREITEM message to determine the relative position
    // of a new item in a sorted owner-draw list box. Override this function in an owner-drawn
    // Combo-Box to specify the sort order when items are added using AddString.
    inline int CComboBox::CompareItem(LPCOMPAREITEMSTRUCT)
    {
        // The return value indicates the relative position of the two items.
        // It may be any of the values shown in the following table.
        //
        //  Value Meaning
        //  -1 Item 1 precedes item 2 in the sorted order.
        //   0 Items 1 and 2 are equivalent in the sorted order.
        //   1 Item 1 follows item 2 in the sorted order.

        return 0;
    }

    // Deletes the current selection, if any, from the combo box's edit control.
    inline void CComboBox::Clear() const
    {
        assert(IsWindow());
        SendMessage(WM_CLEAR, 0, 0);
    }

    // Copies the current selection to the clipboard in CF_TEXT format.
    inline void CComboBox::Copy() const
    {
        assert(IsWindow());
        SendMessage(WM_COPY, 0, 0);
    }

    // Deletes the current selection, if any, in the edit control and copies
    // the deleted text to the clipboard in CF_TEXT format.
    inline void CComboBox::Cut() const
    {
        assert(IsWindow());
        SendMessage(WM_CUT, 0, 0);
    }

    // Deletes a string in the list box of a combo box.
    inline int  CComboBox::DeleteString(int index) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_DELETESTRING, index, 0));
    }

    // Adds the names of directories and files that match a specified string
    // and set of file attributes.
    inline int  CComboBox::Dir(UINT attr, LPCTSTR pWildCard ) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_DIR, attr, reinterpret_cast<LPARAM>(pWildCard)));
    }

    // Search the list box of a combo box for an item beginning with the
    // characters in a specified string.
    inline int  CComboBox::FindString(int indexStart, LPCTSTR pString) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_FINDSTRING, indexStart, reinterpret_cast<LPARAM>(pString)));
    }

    // Find the first list box string in a combo box that matches the string specified in pString.
    inline int  CComboBox::FindStringExact(int indexStart, LPCTSTR pString) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_FINDSTRINGEXACT, indexStart, reinterpret_cast<LPARAM>(pString)));
    }

    // Retrieves the number of items in the list box of the combo box.
    inline int  CComboBox::GetCount() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETCOUNT, 0, 0));
    }

    // Retrieves the index of the currently selected item, if any, in the list box of the combo box.
    inline int  CComboBox::GetCurSel() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETCURSEL, 0, 0));
    }

    // Retrieves the screen coordinates of the combo box in its dropped-down state.
    inline CRect CComboBox::GetDroppedControlRect() const
    {
        assert(IsWindow());
        CRect rc;
        SendMessage(CB_GETDROPPEDCONTROLRECT, 0, reinterpret_cast<LPARAM>(&rc));
        return rc;
    }

    // Determines whether the list box of the combo box is dropped down.
    inline BOOL CComboBox::GetDroppedState() const
    {
        assert(IsWindow());
        return (SendMessage(CB_GETDROPPEDSTATE, 0, 0) != 0);
    }

    // Retrieves the minimum allowable width, in pixels, of the list box of the combo box
    // with the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
    inline int  CComboBox::GetDroppedWidth() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETDROPPEDWIDTH, 0, 0));
    }


#if WINVER >= 0x0500
    // Returns the handle to the edit box.
    inline HWND  CComboBox::GetEditCtrl() const
    {
        COMBOBOXINFO cbi;
        ZeroMemory(&cbi, sizeof(cbi));
        cbi.cbSize = sizeof(cbi);
        VERIFY(::GetComboBoxInfo(*this, &cbi));

        return cbi.hwndItem;
    }
#endif

    // Gets the starting and ending character positions of the current selection
    // in the edit control of the combo box.
    inline DWORD CComboBox::GetEditSel() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETEDITSEL, 0, 0));
    }

    // Determines whether the combo box has the default user interface or the extended user interface.
    inline BOOL CComboBox::GetExtendedUI() const
    {
        assert(IsWindow());
        return (SendMessage(CB_GETEXTENDEDUI, 0, 0) != 0);
    }

    // Retrieve from the combo box the width, in pixels, by which the list box can
    // be scrolled horizontally (the scrollable width).
    inline int  CComboBox::GetHorizontalExtent() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETHORIZONTALEXTENT, 0, 0));
    }

    // Retrieves the application-supplied value associated with the specified item in the combo box.
    inline DWORD CComboBox::GetItemData(int index) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(CB_GETITEMDATA, index, 0));
    }

    // Determines the height of list items or the selection field in the combo box.
    inline int  CComboBox::GetItemHeight(int index) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETITEMHEIGHT, index, 0));
    }


#if WINVER >= 0x0500
    // returns the handle to the drop-down list.
    inline HWND  CComboBox::GetLBCtrl() const
    {
        COMBOBOXINFO cbi;
        ZeroMemory(&cbi, sizeof(cbi));
        cbi.cbSize = sizeof(cbi);
        VERIFY(::GetComboBoxInfo(*this, &cbi));

        return cbi.hwndList;
    }
#endif

    //  Retrieves a string from the list of the combo box.
    inline int  CComboBox::GetLBText(int index, LPTSTR pText) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETLBTEXT, index, reinterpret_cast<LPARAM>(pText)));
    }

    // Retrieves the length, in characters, of a string in the list of the combo box.
    inline int  CComboBox::GetLBTextLen(int index) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETLBTEXTLEN, index, 0));
    }

    // Retrieves the current locale of the combo box.
    inline LCID CComboBox::GetLocale() const
    {
        assert(IsWindow());
        return static_cast<LCID>(SendMessage(CB_GETLOCALE, 0, 0));
    }

    // Retrieves the zero-based index of the first visible item in the list box portion of the combo box.
    inline int  CComboBox::GetTopIndex() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_GETTOPINDEX, 0, 0));
    }

    // Allocates memory for storing list box items. Use this before adding a
    // large number of items to the list box portion of a combo box.
    inline int  CComboBox::InitStorage(int items, int byteCount) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_INITSTORAGE, items, byteCount));
    }

    // Inserts a string into the list box of the combo box. Unlike the AddString,
    // a list with the CBS_SORT style is not sorted.
    inline int  CComboBox::InsertString(int index, LPCTSTR pString) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_INSERTSTRING, index, reinterpret_cast<LPARAM>(pString)));
    }

    // Handle messages reflected back from the parent window.
    // Override this function in your derived class to handle these special messages:
    // WM_COMMAND, WM_CTLCOLORBTN, WM_CTLCOLOREDIT, WM_CTLCOLORDLG, WM_CTLCOLORLISTBOX,
    // WM_CTLCOLORSCROLLBAR, WM_CTLCOLORSTATIC, WM_CHARTOITEM,  WM_VKEYTOITEM,
    // WM_HSCROLL, WM_VSCROLL, WM_DRAWITEM, WM_MEASUREITEM, WM_DELETEITEM,
    // WM_COMPAREITEM, WM_PARENTNOTIFY.
    inline LRESULT CComboBox::OnMessageReflect(UINT msg, WPARAM, LPARAM lparam)
    {
        switch (msg)
        {
        case WM_COMPAREITEM:    return CompareItem((LPCOMPAREITEMSTRUCT)lparam);
        }

        return 0;   // Allow other messages to be handled elsewhere.
    }

    // Copies the current content of the clipboard to the combo box's edit control at the current caret position.
    inline void CComboBox::Paste() const
    {
        assert(IsWindow());
        SendMessage(WM_PASTE, 0, 0);
    }

    // Limits the length of the text the user may type into the edit control of the combo box.
    inline void CComboBox::LimitText(int maxChars) const
    {
        assert(IsWindow());
        SendMessage(CB_LIMITTEXT, maxChars, 0);
    }

    // Removes all items from the list box and edit control of the combo box.
    inline void CComboBox::ResetContent() const
    {
        assert(IsWindow());
        SendMessage(CB_RESETCONTENT, 0, 0);
    }

    // Searches the list of a combo box for an item that begins with the characters in a
    // specified string. If a matching item is found, it is selected and copied to the edit control.
    inline int  CComboBox::SelectString(int startAfter, LPCTSTR pString) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SELECTSTRING, startAfter, reinterpret_cast<LPARAM>(pString)));
    }

    // Selects a string in the list of the combo box. If necessary, the list scrolls the string into view.
    inline int  CComboBox::SetCurSel(int index) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETCURSEL, index, 0));
    }

    // Sets the maximum allowable width, in pixels, of the list box of the combo box with
    // the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
    inline int  CComboBox::SetDroppedWidth(int width) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETDROPPEDWIDTH, width, 0));
    }

    // Selects characters in the edit control of the combo box.
    inline BOOL CComboBox::SetEditSel(int startChar, int endChar) const
    {
        assert(IsWindow());
        return (SendMessage(CB_SETEDITSEL, 0, MAKELONG(startChar, endChar)) != 0);
    }

    // Selects either the default user interface or the extended user interface for the combo box that
    // has the CBS_DROPDOWN or CBS_DROPDOWNLIST style.
    inline int  CComboBox::SetExtendedUI(BOOL isExtended) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETEXTENDEDUI, isExtended, 0));
    }

    // Sets the width, in pixels, by which the list box can be scrolled horizontally (the scrollable width).
    inline void CComboBox::SetHorizontalExtent(UINT extent ) const
    {
        assert(IsWindow());
        SendMessage(CB_SETHORIZONTALEXTENT, extent, 0);
    }

    // Sets the value associated with the specified item in the combo box.
    inline int  CComboBox::SetItemData(int index, DWORD itemData) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETITEMDATA, index, itemData));
    }

    // Sets the height of list items or the selection field in the combo box.
    inline int  CComboBox::SetItemHeight(int index, UINT cyItemHeight) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETITEMHEIGHT, index, cyItemHeight));
    }

    // Sets the current locale of the combo box.
    inline LCID CComboBox::SetLocale( LCID newLocale ) const
    {
        assert(IsWindow());
        return static_cast<LCID>(SendMessage(CB_SETLOCALE, newLocale, 0));
    }

    // Ensure that a particular item is visible in the list box of the combo box.
    inline int  CComboBox::SetTopIndex(int index) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CB_SETTOPINDEX, index, 0));
    }

    // Ensure that a particular item is visible in the list box of the combo box.
    inline void CComboBox::ShowDropDown(BOOL show) const
    {
        assert(IsWindow());
        SendMessage(CB_SHOWDROPDOWN, show, 0);
    }


    ////////////////////////////////////////
    // Definitions for the CComboBoxEx class
    //

    // Removes an item from the ComboBoxEx control.
    inline int  CComboBoxEx::DeleteItem(int index ) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CBEM_DELETEITEM, index, 0));
    }

    // Retrieves the handle to the child combo box control.
    inline HWND CComboBoxEx::GetComboBoxCtrl() const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(CBEM_GETCOMBOCONTROL, 0, 0));
    }

    // Retrieves the handle to the edit control portion of the ComboBoxEx control.
    inline HWND CComboBoxEx::GetEditCtrl() const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(CBEM_GETEDITCONTROL, 0, 0));
    }

    // Retrieves the extended styles that are in use for the ComboBoxEx control.
    inline DWORD CComboBoxEx::GetExtendedStyle() const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(CBEM_GETEXTENDEDSTYLE, 0, 0));
    }

    // Retrieves the handle to an image list assigned to the ComboBoxEx control.
    inline CImageList CComboBoxEx::GetImageList() const
    {
        assert(IsWindow());
        HIMAGELIST images = reinterpret_cast<HIMAGELIST>(SendMessage(CBEM_GETIMAGELIST, 0, 0));
        return CImageList(images);
    }

    // Retrieves item information for the given ComboBoxEx item.
    inline BOOL CComboBoxEx::GetItem(COMBOBOXEXITEM& cbItem) const
    {
        assert(IsWindow());
        return (SendMessage(CBEM_GETITEM, 0, reinterpret_cast<LPARAM>(&cbItem)) != 0);
    }

    // Determines whether or not the user has changed the text of the ComboBoxEx edit control.
    inline BOOL CComboBoxEx::HasEditChanged () const
    {
        assert(IsWindow());
        return (SendMessage(CBEM_HASEDITCHANGED, 0, 0) != 0);
    }

    // Inserts a new item in the ComboBoxEx control.
    inline int CComboBoxEx::InsertItem(const COMBOBOXEXITEM& cbItem) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(CBEM_INSERTITEM, 0, reinterpret_cast<LPARAM>(&cbItem)));
    }

    // Sets extended styles within the ComboBoxEx control.
    inline DWORD CComboBoxEx::SetExtendedStyle(DWORD exMask, DWORD exStyles ) const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(CBEM_SETEXTENDEDSTYLE, exMask, exStyles));
    }

    // Sets an image list for the ComboBoxEx control.
    inline CImageList CComboBoxEx::SetImageList(HIMAGELIST images) const
    {
        assert(IsWindow());
        HIMAGELIST oldImages = reinterpret_cast<HIMAGELIST>(SendMessage(CBEM_SETIMAGELIST, 0,
                                                       reinterpret_cast<LPARAM>(images)));
        return CImageList(oldImages);
    }

    // Sets the attributes for an item in the ComboBoxEx control.
    inline BOOL CComboBoxEx::SetItem(const COMBOBOXEXITEM& cbItem) const
    {
        assert(IsWindow());
        return (SendMessage(CBEM_SETITEM, 0, reinterpret_cast<LPARAM>(&cbItem)) != 0);
    }

    ////////////////////////////////////////
    // Definitions for the CDateTime class
    //

    inline CDateTime::CDateTime()
    {
    }

    // Retrieves the color for a given portion of the month calendar within the date and time picker (DTP) control.
    inline COLORREF CDateTime::GetMonthCalColor(int color) const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(DateTime_GetMonthCalColor(*this, color));
    }

    // Sets the color for a given portion of the month calendar within the date and time picker (DTP) control.
    inline COLORREF CDateTime::SetMonthCalColor(int color, COLORREF clr) const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(DateTime_SetMonthCalColor(*this, color, clr));
    }

    // Sets the display of the date and time picker (DTP) control based on a given format string.
    inline BOOL CDateTime::SetFormat(LPCTSTR pFormat) const
    {
        assert(IsWindow());
        return DateTime_SetFormat(*this, pFormat);
    }

    // Retrieves the handle to the date and time picker's (DTP) child month calendar control.
    inline HWND CDateTime::GetMonthCalCtrl() const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(DateTime_GetMonthCal(*this));
    }

    // Retrieves the font that the date and time picker (DTP) control's child month calendar control is currently using.
    inline CFont CDateTime::GetMonthCalFont() const
    {
        assert(IsWindow());
        HFONT font = reinterpret_cast<HFONT>(DateTime_GetMonthCalFont(*this));
        return CFont(font);
    }

    // Sets the font to be used by the date and time picker (DTP) control's child month calendar control.
    inline void CDateTime::SetMonthCalFont(HFONT font, BOOL redraw /*= TRUE*/) const
    {
        assert(IsWindow());
        DateTime_SetMonthCalFont(*this, font, MAKELONG(redraw, 0));
    }

    // Retrieves the current minimum and maximum allowable system times for the date and time picker (DTP) control.
    inline DWORD CDateTime::GetRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const
    {
        assert(IsWindow());

        SYSTEMTIME ranges[2];
        ZeroMemory(ranges, 2 * sizeof(SYSTEMTIME));
        DWORD result = DateTime_GetRange(*this, ranges);
        minRange = ranges[0];
        maxRange = ranges[1];
        return result;
    }

    // Sets the minimum and maximum allowable system times for the date and time picker (DTP) control.
    inline BOOL CDateTime::SetRange(const SYSTEMTIME& minRange, const SYSTEMTIME& maxRange) const
    {
        assert(IsWindow());
        SYSTEMTIME ranges[2];
        ranges[0] = minRange;
        ranges[1] = maxRange;
        DWORD flags = GDTR_MIN | GDTR_MAX;

        return DateTime_SetRange(*this, flags, ranges);
    }

    // Returns the currently selected time from a date and time picker (DTP) control
    // Possible return codes are: GDT_VALID, GDT_ERROR, and GDT_NONE.
    // A return code of GDT_NONE occurs if the control was set to the DTS_SHOWNONE
    // style and the control check box was not selected.
    inline SYSTEMTIME CDateTime::GetTime(DWORD* pReturnCode) const
    {
        assert(IsWindow());
        SYSTEMTIME time;
        ZeroMemory(&time,  sizeof(time));
        DWORD Res = DateTime_GetSystemtime(*this, &time);
        if (pReturnCode)
            *pReturnCode = Res;

        return time;
    }

    // Sets the date and time picker (DTP) control to a given date and time.
    inline BOOL CDateTime::SetTime(const SYSTEMTIME& time) const
    {
        assert(IsWindow());
        return DateTime_SetSystemtime(*this, GDT_VALID, &time);
    }

    // Set the DTP control to "no date" and clear its check box.
    // Only applies to DTP controls that are set to the DTS_SHOWNONE style.
    inline BOOL CDateTime::SetTimeNone() const
    {
        assert(IsWindow());
        return DateTime_SetSystemtime(*this, GDT_NONE, NULL);
    }


    ////////////////////////////////////////
    // Definitions for the CHeader class
    //

    // Creates a transparent version of an item image within the header control.
    inline CImageList CHeader::CreateDragImage(int index) const
    {
        assert(IsWindow());
        HIMAGELIST images = Header_CreateDragImage(*this, index);
        return CImageList(images);
    }

    // Deletes an item from the header control.
    inline BOOL CHeader::DeleteItem(int pos) const
    {
        assert(IsWindow());
        return Header_DeleteItem(*this, pos);
    }

    // Retrieves the image list that has been set for the header control.
    inline CImageList CHeader::GetImageList() const
    {
        assert(IsWindow());
        HIMAGELIST images = Header_GetImageList(*this);
        return CImageList(images);
    }

    // Retrieves information about an item in a header control.
    inline BOOL CHeader::GetItem(int pos, HDITEM& headerItem) const
    {
        assert(IsWindow());
        return Header_GetItem(*this, pos, &headerItem);
    }

    // Retrieves the count of the items in a header control.
    inline int CHeader::GetItemCount() const
    {
        assert(IsWindow());
        return Header_GetItemCount(*this);
    }

    // Retrieves the bounding rectangle for a given item in a header control.
    inline CRect CHeader::GetItemRect(int index) const
    {
        assert(IsWindow());
        CRect rc;
        SendMessage(HDM_GETITEMRECT, static_cast<WPARAM>(index), reinterpret_cast<LPARAM>(&rc));
        return rc;
    }

    // Retrieves the current left-to-right order of items in a header control.
    inline BOOL CHeader::GetOrderArray(LPINT pArray, int count) const
    {
        assert(IsWindow());
        return Header_GetOrderArray(*this, pArray, count);
    }

    // Inserts a new item into the header control.
    inline int CHeader::InsertItem(int pos, const HDITEM& headerItem) const
    {
        assert(IsWindow());
        return Header_InsertItem(*this, pos, &headerItem);
    }

    // Retrieves the correct size and position of the header control within the parent window.
    inline BOOL CHeader::Layout(HDLAYOUT* pHeaderLayout) const
    {
        assert(IsWindow());
        return Header_Layout(*this, pHeaderLayout);
    }

    // Retrieves an index value for an item based on its order in the header control.
    inline int CHeader::OrderToIndex(int order) const
    {
        assert(IsWindow());
        return Header_OrderToIndex( *this, order);
    }

#ifdef Header_SetHotDivider

    // Changes the color of a divider between header items to indicate the destination of
    // an external drag-and-drop operation.
    inline int CHeader::SetHotDivider(CPoint pt) const
    {
        assert(IsWindow());
        return Header_SetHotDivider(*this, TRUE, MAKELPARAM(pt.x, pt.y));
    }

    // Changes the color of a divider between header items to indicate the destination of
    // an external drag-and-drop operation.
    inline int CHeader::SetHotDivider(int index) const
    {
        assert(IsWindow());
        return Header_SetHotDivider(*this, FALSE, index);
    }

#endif

    // Assigns an image list to the header control.
    inline CImageList CHeader::SetImageList(HIMAGELIST images) const
    {
        assert(IsWindow());
        HIMAGELIST oldImages = Header_SetImageList(*this, images);
        return CImageList(oldImages);
    }

    // Sets the attributes of the specified item in a header control.
    inline BOOL CHeader::SetItem(int pos, const HDITEM& headerItem) const
    {
        assert(IsWindow());
        return Header_SetItem(*this, pos, &headerItem);
    }

    // Sets the left-to-right order of header items.
    inline BOOL CHeader::SetOrderArray(int count, LPINT pArray) const
    {
        assert(IsWindow());
        return Header_SetOrderArray(*this, count, pArray);
    }

#ifdef Header_ClearFilter

    // Clears the filter for the header control.
    inline int CHeader::ClearFilter(int column) const
    {
        assert(IsWindow());
        return Header_ClearFilter(*this, column);
    }

    // Clears all of the filters for the header control.
    inline int CHeader::ClearAllFilters() const
    {
        assert(IsWindow());
        return Header_ClearAllFilters(*this);
    }

    // Moves the input focus to the edit box when a filter button has the focus.
    inline int CHeader::EditFilter(int column, BOOL discardChanges) const
    {
        assert(IsWindow());
        return Header_EditFilter(*this, column, MAKELPARAM(discardChanges, 0));
    }

    // Gets the width of the bitmap margin for a header control.
    inline int CHeader::GetBitmapMargin() const
    {
        assert(IsWindow());
        return Header_GetBitmapMargin(*this);
    }

    // Sets the width of the margin, specified in pixels, of a bitmap in the header control.
    inline int CHeader::SetBitmapMargin(int width) const
    {
        assert(IsWindow());
        return Header_SetBitmapMargin(*this, width);
    }

    // Sets the timeout interval between the time a change takes place in the filter attributes
    // and the posting of an HDN_FILTERCHANGE notification.
    inline int CHeader::SetFilterChangeTimeout(DWORD timeout) const
    {
        assert(IsWindow());
        return Header_SetFilterChangeTimeout(*this, timeout);
    }
#endif


    ////////////////////////////////////////
    // Definitions for the CHotKey class
    //

    // Gets the virtual key code and modifier flags of a hot key
    inline DWORD CHotKey::GetHotKey() const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(HKM_GETHOTKEY, 0, 0));
    }

    // Retrieves a string that represents the name of a key.
    inline CString CHotKey::GetKeyName(UINT keyCode, BOOL isExtended) const
    {
        // Translate the virtual-key code to a scan code
        LONG scan = MapVirtualKey(keyCode, 0);

        // Construct an LPARAM with the scan code in Bits 16-23, and an extended flag in bit 24
        LPARAM lparam = scan << 16;
        if (isExtended)
            lparam |= 0x01000000L;

        CString str;
        int bufferLen = 64;
        int strLen = bufferLen;

        // Loop until we have retrieved the entire string
        while (strLen == bufferLen)
        {
            bufferLen *= 4;
            LPTSTR pStr = str.GetBuffer(bufferLen);
            strLen = ::GetKeyNameText(static_cast<LONG>(lparam), pStr, bufferLen + 1);
            str.ReleaseBuffer();
        }

        return str;
    }

    // Sets the hot key combination for the control.
    inline void CHotKey::SetHotKey(DWORD key) const
    {
        assert(IsWindow());
        SendMessage(HKM_SETHOTKEY, static_cast<WPARAM>(key), 0);
    }

    // Defines the invalid combinations and the default modifier combination for the control.
    inline void CHotKey::SetRules(WORD invalidComb, WORD modifiers) const
    {
        assert(IsWindow());
        SendMessage(HKM_SETRULES, invalidComb, modifiers);
    }


    ////////////////////////////////////////
    // Definitions for the CIPAddress class
    //

    inline CIPAddress::CIPAddress()
    {
        if (GetComCtlVersion() > 470)
        {
            // Call InitCommonControlsEx
            INITCOMMONCONTROLSEX initStruct;
            initStruct.dwSize = sizeof(initStruct);
            initStruct.dwICC = ICC_INTERNET_CLASSES;
            InitCommonControlsEx(&initStruct);
        }
        else
            throw CNotSupportedException(_T("IP Address Control not supported!"));
    }

    // Clears the contents of the IP address control.
    inline void CIPAddress::ClearAddress() const
    {
        assert(IsWindow());
        SendMessage(IPM_CLEARADDRESS, 0, 0);
    }

    // Gets the address values for all four fields in the IP address control.
    inline int CIPAddress::GetAddress(BYTE& field0, BYTE& field1, BYTE& field2, BYTE& field3) const
    {
        DWORD addr;
        int result = GetAddress(addr);
        field0 = (BYTE)FIRST_IPADDRESS(addr);
        field1 = (BYTE)SECOND_IPADDRESS(addr);
        field2 = (BYTE)THIRD_IPADDRESS(addr);
        field3 = (BYTE)FOURTH_IPADDRESS(addr);
        return result;
    }

    // Gets the address values for all four fields in the IP address control.
    inline int CIPAddress::GetAddress(DWORD& address) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(IPM_GETADDRESS, 0, reinterpret_cast<LPARAM>(&address)));
    }

    // Determines if all fields in the IP address control are blank.
    inline BOOL CIPAddress::IsBlank() const
    {
        assert(IsWindow());
        return (SendMessage(IPM_ISBLANK, 0, 0) != 0);
    }

    // Sets the address values for all four fields in the IP address control.
    inline void CIPAddress::SetAddress(BYTE field0, BYTE field1, BYTE field2, BYTE field3) const
    {
        assert(IsWindow());
        SendMessage(IPM_SETADDRESS, 0, MAKEIPADDRESS(field0, field1, field2, field3));
    }

    // Sets the address values for all four fields in the IP address control.
    inline void CIPAddress::SetAddress(DWORD address) const
    {
        assert(IsWindow());
        SendMessage(IPM_SETADDRESS, 0, address);
    }

    // Sets the keyboard focus to the specified field in the IP address control.
    // All of the text in that field will be selected.
    inline void CIPAddress::SetFieldFocus(WORD field) const
    {
        assert(IsWindow());
        SendMessage(IPM_SETFOCUS, field, 0);
    }

    // Sets the valid range for the specified field in the IP address control.
    inline void CIPAddress::SetFieldRange(int field, BYTE lower, BYTE upper) const
    {
        assert(IsWindow());
        SendMessage(IPM_SETRANGE, MAKEIPRANGE(lower, upper), field);
    }


    ///////////////////////////////////////////
    // Definitions for the CMonthCalendar class
    //

    inline CMonthCalendar::CMonthCalendar()
    {
    }

    // Retrieves the color for a given portion of the month calendar control.
    inline COLORREF CMonthCalendar::GetColor(int region) const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(MonthCal_GetColor(*this, region));
    }

    // Retrieves the currently selected date.
    inline SYSTEMTIME CMonthCalendar::GetCurSel() const
    {
        assert(IsWindow());
        SYSTEMTIME st;
        ZeroMemory(&st, sizeof(st));
        MonthCal_GetCurSel(*this, &st);
        return st;
    }

    // Retrieves the first day of the week.
    inline int CMonthCalendar::GetFirstDayOfWeek(BOOL* pLocal /*= NULL*/) const
    {
        assert(IsWindow());
        DWORD result = MonthCal_GetFirstDayOfWeek(*this);

        if (pLocal)
            *pLocal = HIWORD(result);

        return LOWORD(result);
    }

    // Retrieves the maximum date range that can be selected.
    inline int CMonthCalendar::GetMaxSelCount() const
    {
        assert(IsWindow());
        return MonthCal_GetMaxSelCount(*this);
    }

    // Retrieves the minimum size required to display a full month
    inline CRect CMonthCalendar::GetMinReqRect() const
    {
        assert(IsWindow());
        CRect rc;
        MonthCal_GetMinReqRect(*this, &rc);
        return rc;
    }

    // Retrieves the scroll rate for the month calendar control.
    inline int CMonthCalendar::GetMonthDelta() const
    {
        assert(IsWindow());
        return MonthCal_GetMonthDelta(*this);
    }

    // Retrieves date information that represents the high and low limits of the month calendar control's display.
    inline int CMonthCalendar::GetMonthRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange, DWORD flags) const
    {
        assert(IsWindow());
        SYSTEMTIME minMax[2];
        ZeroMemory(minMax, 2*sizeof(SYSTEMTIME));
        int count = MonthCal_GetMonthRange(*this, flags, minMax);
        minRange = minMax[0];
        maxRange = minMax[1];
        return count;
    }

    //  Retrieves the minimum and maximum allowable dates set for the month calendar control.
    inline LRESULT CMonthCalendar::GetRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const
    {
        assert(IsWindow());
        SYSTEMTIME minMax[2];
        ZeroMemory(minMax, 2*sizeof(SYSTEMTIME));
        LRESULT Value = SendMessage(MCM_GETRANGE, 0, reinterpret_cast<LPARAM>(&minMax));
        minRange = minMax[0];
        maxRange = minMax[1];
        return Value;
    }

    // Retrieves date information that represents the upper and lower limits of the date range currently selected by the user.
    inline LRESULT CMonthCalendar::GetSelRange(SYSTEMTIME& minRange, SYSTEMTIME& maxRange) const
    {
        assert(IsWindow());
        SYSTEMTIME minMax[2];
        ZeroMemory(minMax, 2*sizeof(SYSTEMTIME));
        LRESULT Value = MonthCal_GetSelRange(*this, &minMax);
        minRange = minMax[0];
        maxRange = minMax[1];
        return Value;
    }

    // Retrieves the date information for the date specified as "today" for the month calendar control.
    inline SYSTEMTIME CMonthCalendar::GetToday() const
    {
        assert(IsWindow());
        SYSTEMTIME DateTime;
        ZeroMemory(&DateTime, sizeof(DateTime));
        VERIFY (SendMessage(MCM_GETTODAY, 0, reinterpret_cast<LPARAM>(&DateTime)));
        return DateTime;
    }

    //  Determines which portion of the month calendar control is at a given point on the screen.
    inline LRESULT CMonthCalendar::HitTest(MCHITTESTINFO& mcHitTest) const
    {
        assert(IsWindow());
        return MonthCal_HitTest(*this, &mcHitTest);
    }

    // Sets the color for a given portion of the month calendar control.
    inline COLORREF CMonthCalendar::SetColor(int region, COLORREF clr) const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(MonthCal_SetColor(*this, region, clr));
    }

    // Sets the currently selected date for the month calendar control.
    inline BOOL CMonthCalendar::SetCurSel(const SYSTEMTIME& dateTime) const
    {
        assert(IsWindow());
        return MonthCal_SetCurSel(*this, &dateTime);
    }

    // Sets the day states for all months that are currently visible within the month calendar control.
    inline BOOL CMonthCalendar::SetDayState(int months, const MONTHDAYSTATE& state) const
    {
        assert(IsWindow());
        return (MonthCal_SetDayState(*this, months, &state) != 0);
    }

    // Sets the first day of the week for the month calendar control.
    inline BOOL CMonthCalendar::SetFirstDayOfWeek(int day, int* pOldDay/* = NULL*/) const
    {
        assert(IsWindow());
        DWORD result = static_cast<DWORD>(MonthCal_SetFirstDayOfWeek(*this, day));

        if(pOldDay)
            *pOldDay = LOWORD(result);

        return (HIWORD(result) != 0);
    }

    // Sets the maximum number of days that can be selected in the month calendar control.
    inline BOOL CMonthCalendar::SetMaxSelCount(int max) const
    {
        assert(IsWindow());
        return MonthCal_SetMaxSelCount(*this, max);
    }

    // Sets the scroll rate for the month calendar control.
    inline int CMonthCalendar::SetMonthDelta(int delta) const
    {
        assert(IsWindow());
        return MonthCal_SetMonthDelta(*this, delta);
    }

    // Sets the minimum and maximum allowable dates for the month calendar control.
    inline BOOL CMonthCalendar::SetRange(const SYSTEMTIME& minRange, const SYSTEMTIME& maxRange) const
    {
        SYSTEMTIME minMax[2];
        DWORD limit = GDTR_MIN | GDTR_MAX;

        minMax[0] = minRange;
        minMax[1] = maxRange;

        return (MonthCal_SetRange(*this, limit, &minMax) != 0);
    }

    // Sets the selection for the month calendar control to a given date range.
    inline BOOL CMonthCalendar::SetSelRange(const SYSTEMTIME& minRange, const SYSTEMTIME& maxRange) const
    {
        SYSTEMTIME minMax[2];
        minMax[0] = minRange;
        minMax[1] = maxRange;
        return (MonthCal_SetSelRange(*this, &minMax) != 0);
    }

    // Sets the "today" selection for the month calendar control.
    inline void CMonthCalendar::SetToday(const SYSTEMTIME& dateTime) const
    {
        assert(IsWindow());
        MonthCal_SetToday(*this, &dateTime);
    }


    ////////////////////////////////////////
    // Definitions for the CProgressBar class
    //

    // Retrieves the current position of the progress bar.
    inline int CProgressBar::GetPos() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_GETPOS, 0, 0));
    }

    // Retrieves information about the current high and low limits of the progress bar control.
    inline int CProgressBar::GetRange(BOOL whichLimit, const PBRANGE& range) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_GETRANGE, whichLimit, reinterpret_cast<LPARAM>(&range)));
    }

    // Retrieves information about the current high and low limits of the progress bar control.
    inline int CProgressBar::GetRange(BOOL whichLimit) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_GETRANGE, whichLimit, 0));
    }

    // Advances the current position of the progress bar by a specified increment and redraws
    // the bar to reflect the new position.
    inline int CProgressBar::OffsetPos(int increment) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_DELTAPOS, increment, 0));
    }

    // Sets the current position for the progress bar and redraws the bar to reflect the new position.
    inline int CProgressBar::SetPos(int pos) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_SETPOS, pos, 0));
    }

    // Sets the minimum and maximum values for the progress bar and redraws the bar to reflect the new range.
    inline int CProgressBar::SetRange(short minRange, short maxRange) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_SETRANGE, 0, MAKELPARAM(minRange, maxRange)));
    }

    // Specifies the step increment for the progress bar.
    inline int CProgressBar::SetStep(int stepInc) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_SETSTEP, stepInc, 0));
    }

    // Advances the current position for a progress bar by the step increment and
    // redraws the bar to reflect the new position.
    inline int CProgressBar::StepIt() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(PBM_STEPIT, 0, 0));
    }


    ////////////////////////////////////////
    // Definitions for the CScrollBar class
    //

    // Enables or disables the scroll bar arrows.
    inline BOOL CScrollBar::EnableScrollBar( UINT arrowFlags )  const
    {
        assert(IsWindow());
        return ::EnableScrollBar(*this, SB_CTL, arrowFlags);
    }

    // Retrieves the parameters of a scroll bar, including the minimum and maximum
    // scrolling positions, the page size, and the position of the scroll box (thumb).
    inline BOOL CScrollBar::GetScrollInfo(SCROLLINFO& si)  const
    {
        assert(IsWindow());
        return ::GetScrollInfo(*this, SB_CTL, &si);
    }

    // Retrieves the current position of the scroll box (thumb) in the scroll bar.
    inline int CScrollBar::GetScrollPos()  const
    {
        assert(IsWindow());
        return ::GetScrollPos(*this, SB_CTL);
    }

    // Retrieves the current minimum and maximum scroll box (thumb) positions for the scroll bar.
    inline BOOL CScrollBar::GetScrollRange(int& minPos, int& maxPos )  const
    {
        assert(IsWindow());
        return ::GetScrollRange(*this, SB_CTL, &minPos, &maxPos);
    }

    // Sets the parameters of the scroll bar, including the minimum and maximum scrolling positions,
    // the page size, and the position of the scroll box (thumb).
    inline BOOL CScrollBar::SetScrollInfo(const SCROLLINFO& si, BOOL redraw )  const
    {
        assert(IsWindow());
        return ::SetScrollInfo(*this, SB_CTL, &si, redraw);
    }

    // Sets the position of the scroll box (thumb) in the scroll bar and, if requested,
    // redraws the scroll bar to reflect the new position of the scroll box.
    inline int CScrollBar::SetScrollPos(int pos, BOOL redraw)  const
    {
        assert(IsWindow());
        return ::SetScrollPos(*this, SB_CTL, pos, redraw);
    }

    // Sets the minimum and maximum scroll box positions for the scroll bar.
    inline BOOL CScrollBar::SetScrollRange( int minPos, int maxPos, BOOL redraw )  const
    {
        assert(IsWindow());
        return ::SetScrollRange(*this, SB_CTL, minPos, maxPos, redraw);
    }

    // Shows or hides the scroll bar.
    inline BOOL CScrollBar::ShowScrollBar(BOOL show)  const
    {
        assert(IsWindow());
        return ::ShowScrollBar(*this, SB_CTL, show);
    }

    ////////////////////////////////////////
    // Definitions for the CSlider class
    //

    // Clears the current selection range in the trackbar.
    inline void CSlider::ClearSel() const
    {
        assert(IsWindow());
        SendMessage(TBM_CLEARSEL, 0, 0);
    }

    // Removes the current tick marks from the trackbar.
    inline void CSlider::ClearTics(BOOL redraw) const
    {
        assert(IsWindow());
        SendMessage(TBM_CLEARTICS, redraw, 0);
    }

    // Retrieves the handle to the trackbar control buddy window at a given location.
    inline HWND CSlider::GetBuddy(BOOL location) const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(TBM_GETBUDDY, location, 0));
    }

    // Retrieves the size and position of the bounding rectangle for the trackbar's channel.
    inline CRect CSlider::GetChannelRect() const
    {
        assert(IsWindow());
        CRect rc;
        SendMessage(TBM_GETCHANNELRECT, 0,reinterpret_cast<LPARAM>(&rc));
        return rc;
    }

    // Retrieves the number of logical positions the trackbar's slider moves in response
    // to keyboard input from the arrow keys.
    inline int  CSlider::GetLineSize() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETLINESIZE, 0, 0));
    }

    // Retrieves the number of tick marks in the trackbar.
    inline int  CSlider::GetNumTics() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETNUMTICS, 0, 0));
    }

    // Retrieves the number of logical positions the trackbar's slider moves in response to
    // keyboard input, or mouse input, such as clicks in the trackbar's channel.
    inline int  CSlider::GetPageSize() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETPAGESIZE, 0, 0));
    }

    // Retrieves the current logical position of the slider in the trackbar.
    inline int  CSlider::GetPos() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETPOS, 0, 0));
    }

    // Retrieves the maximum position for the slider in the trackbar.
    inline int  CSlider::GetRangeMax() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETRANGEMAX, 0, 0));
    }

    // Retrieves the minimum position for the slider in the trackbar.
    inline int  CSlider::GetRangeMin() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETRANGEMIN, 0, 0));
    }

    // Retrieves the ending position of the current selection range in the trackbar.
    inline int  CSlider::GetSelEnd() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETSELEND, 0, 0));
    }

    // Retrieves the starting position of the current selection range in the trackbar.
    inline int  CSlider::GetSelStart() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETSELSTART, 0, 0));
    }

    // Retrieves the length of the slider in the trackbar.
    inline int  CSlider::GetThumbLength() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETTHUMBLENGTH, 0, 0));
    }

    // Retrieves the size and position of the bounding rectangle for the slider in the trackbar.
    inline CRect CSlider::GetThumbRect() const
    {
        CRect rc;
        SendMessage(TBM_GETTHUMBRECT, 0, reinterpret_cast<LPARAM>(&rc));
        return rc;
    }

    // Retrieves the logical position of a tick mark in the trackbar.
    inline int CSlider::GetTic(int nTic ) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETTIC, nTic, 0));
    }

    // Retrieves the current physical position of a tick mark in the trackbar.
    inline int  CSlider::GetTicPos(int tic) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_GETTICPOS, tic, 0));
    }

    // Retrieves the handle to the ToolTip control assigned to the trackbar, if any.
    inline HWND CSlider::GetToolTips() const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(TBM_GETTOOLTIPS, 0, 0));
    }

    // Assigns a window as the buddy window for the trackbar control.
    inline HWND CSlider::SetBuddy(HWND buddy, BOOL location /*= TRUE*/ ) const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(TBM_SETBUDDY, location, reinterpret_cast<LPARAM>(buddy)));
    }

    // Sets the number of logical positions the trackbar's slider moves in response to
    // keyboard input from the arrow keys.
    inline int  CSlider::SetLineSize(int size) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_SETLINESIZE, 0, size));
    }

    // Sets the number of logical positions the trackbar's slider moves in response to
    // keyboard input, or mouse input such as clicks in the trackbar's channel.
    inline int  CSlider::SetPageSize(int size) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_SETPAGESIZE, 0, size));
    }

    // Sets the current logical position of the slider in the trackbar.
    inline void CSlider::SetPos(int pos, BOOL redraw) const
    {
        assert(IsWindow());
        SendMessage(TBM_SETPOS, redraw, pos);
    }

    // Sets the maximum logical position for the slider in the trackbar.
    inline void CSlider::SetRangeMax(int max, BOOL redraw) const
    {
        assert(IsWindow());
        SendMessage(TBM_SETRANGEMAX, redraw, max);
    }

    // Sets the minimum logical position for the slider in the trackbar.
    inline void CSlider::SetRangeMin(int min, BOOL redraw) const
    {
        assert(IsWindow());
        SendMessage(TBM_SETRANGEMIN, redraw, min);
    }

    // Sets the starting and ending positions for the available selection range in the trackbar.
    inline void CSlider::SetSelection(int min, int max, BOOL redraw) const
    {
        assert(IsWindow());
        SendMessage(TBM_SETSEL, redraw, MAKELONG(max, min));
    }

    // Sets a tick mark in the trackbar at the specified logical position.
    inline BOOL CSlider::SetTic(int tic) const
    {
        assert(IsWindow());
        return (SendMessage(TBM_SETTIC, 0, tic) != 0);
    }

    // Sets the interval frequency for tick marks in the trackbar.
    inline void CSlider::SetTicFreq(int freq)  const
    {
        assert(IsWindow());
        SendMessage(TBM_SETTICFREQ, freq, 0);
    }

    // Positions a ToolTip control used by the trackbar control.
    inline int  CSlider::SetTipSide(int location) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TBM_SETTIPSIDE, location, 0));
    }

    // Assigns a ToolTip control to the trackbar control.
    inline void CSlider::SetToolTips(HWND toolTip) const
    {
        assert(IsWindow());
        SendMessage(TBM_SETTOOLTIPS, reinterpret_cast<WPARAM>(toolTip), 0);
    }

    ////////////////////////////////////////
    // Definitions for the CSpinButton class
    //

    // Retrieves acceleration information for the up-down control.
    inline int CSpinButton::GetAccel(int accels, LPUDACCEL pAccels) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(UDM_GETACCEL, accels, reinterpret_cast<LPARAM>(pAccels)));
    }

    // Retrieves the current radix base (that is, either base 10 or 16) for the up-down control.
    inline int CSpinButton::GetBase() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(UDM_GETBASE, 0, 0));
    }

    // Retrieves the handle to the current buddy window.
    inline HWND CSpinButton::GetBuddy() const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(UDM_GETBUDDY, 0, 0));
    }

    // Retrieves the current position of the up-down control with 16-bit precision.
    inline int CSpinButton::GetPos() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(UDM_GETPOS, 0, 0));
    }

    // Retrieves the minimum and maximum positions (range) for the up-down control.
    inline DWORD CSpinButton::GetRange() const
    {
        assert(IsWindow());
        return static_cast<DWORD>(SendMessage(UDM_GETRANGE, 0, 0));
    }

    // Sets the CREATESTRUCT parameters before the control is created.
    inline void CSpinButton::PreCreate(CREATESTRUCT& cs)
    {
        cs.style = WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VISIBLE |UDS_SETBUDDYINT;
    }

    // Sets the WNDCLASS parameters before the control is created.
    inline void CSpinButton::PreRegisterClass(WNDCLASS& wc)
    {
        wc.lpszClassName = UPDOWN_CLASS;
    }

    // Sets the acceleration for the up-down control.
    inline BOOL CSpinButton::SetAccel(int accels, LPUDACCEL pAccels) const
    {
        assert(IsWindow());
        return (SendMessage(UDM_SETACCEL, accels, reinterpret_cast<LPARAM>(pAccels)) != 0);
    }

    // Sets the radix base for the up-down control.
    inline int CSpinButton::SetBase(int base) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(UDM_SETBASE, base, 0));
    }

    // Sets the buddy window for the up-down control.
    inline HWND CSpinButton::SetBuddy(HWND buddy) const
    {
        assert(IsWindow());
        return reinterpret_cast<HWND>(SendMessage(UDM_SETBUDDY, reinterpret_cast<WPARAM>(buddy), 0));
    }

    // Sets the current position for the up-down control with 16-bit precision.
    inline int CSpinButton::SetPos(int pos) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(UDM_SETPOS, 0, MAKELONG ((short) pos, 0)));
    }

    // Sets the minimum and maximum positions (range) for the up-down control.
    inline void CSpinButton::SetRange(int lower, int upper) const
    {
        assert(IsWindow());
        SendMessage(UDM_SETRANGE, 0, MAKELONG(upper, lower));
    }

    ////////////////////////////////////////
    // Definitions for the CToolTip class
    //

    inline CToolTip::CToolTip()
    {
    }

    inline CToolTip::~CToolTip()
    {
    }

    // Activates or deactivates a ToolTip control.
    inline void CToolTip::Activate(BOOL activate) const
    {
        assert(IsWindow());
        SendMessage(TTM_ACTIVATE, activate, 0);
    }

    // Registers a tool with a ToolTip control.
    // control specifies the window which triggers the tooltip.
    // toolRect specifies the part of the window which triggers the tooltip.
    // textID specifies the ID of the text resource.
    // id is a user defined ID. It is required if the control has multiple tooltips.
    inline BOOL CToolTip::AddTool(HWND control, const RECT& toolRect, UINT id, UINT textID) const
    {
        assert(IsWindow());
        TOOLINFO info;
        FillToolInfo(info, control, toolRect, id);
		info.hinst = GetApp().GetResourceHandle();
		info.lpszText = MAKEINTRESOURCE(textID);
        return (SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&info)) != 0);
    }

    // Registers a tool with a ToolTip control.
    // control specifies the window which triggers the tooltip.
    // textID specifies the ID of the text resource
    inline BOOL CToolTip::AddTool(HWND control, UINT textID) const
    {
        assert(IsWindow());
        TOOLINFO info;
        FillToolInfo(info, control);
		info.hinst = GetApp().GetResourceHandle();
		info.lpszText = MAKEINTRESOURCE(textID);
        return (SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&info)) != 0);
    }

    // Registers a tool with a ToolTip control.
    // control specifies the window which triggers the tooltip.
    // toolRect specifies the part of the window which triggers the tooltip.
    // If pText contains the value LPSTR_TEXTCALLBACK, TTN_NEEDTEXT notification
    // messages are sent to the parent window.
    // id is a user defined ID. It is required if the control has multiple tooltips.
    inline BOOL CToolTip::AddTool(HWND control, const RECT& toolRect, UINT id, LPCTSTR pText /*= LPSTR_TEXTCALLBACK*/) const
    {
        assert(IsWindow());
        TOOLINFO info;
        FillToolInfo(info, control, toolRect, id);
		info.lpszText = const_cast<LPTSTR>(pText);
        return (SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&info)) != 0);
    }

    // Registers a tool with a ToolTip control.
    // control specifies the window which triggers the tooltip.
    // If pText contains the value LPSTR_TEXTCALLBACK, TTN_NEEDTEXT notification
    // messages are sent to the parent window.
    inline BOOL CToolTip::AddTool(HWND control, LPCTSTR pText /*= LPSTR_TEXTCALLBACK*/) const
    {
        assert(IsWindow());
        TOOLINFO info;
        FillToolInfo(info, control);
		info.lpszText = const_cast<LPTSTR>(pText);
        return (SendMessage(TTM_ADDTOOL, 0, reinterpret_cast<LPARAM>(&info)) != 0);
    }

    // Removes a tool from a ToolTip control.
    inline void CToolTip::DelTool(HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO info = GetToolInfo(control, id);
        SendMessage(TTM_DELTOOL, 0, reinterpret_cast<LPARAM>(&info));
    }

    // Retrieves the initial, pop-up, and reshow durations currently set for a ToolTip control.
    // Returns an intvalue with the specified duration in milliseconds.
    //
    // duration is one of:
    //  TTDT_AUTOPOP - time the ToolTip window remains visible if the pointer is stationary
    //  TTDT_INITIAL - time the pointer must remain stationary before the ToolTip window appears.
    //  TTDT_RESHOW  - time it takes for subsequent ToolTip windows to appear as the pointer moves from one tool to another.
    inline int CToolTip::GetDelayTime(DWORD duration) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TTM_GETDELAYTIME, duration, 0));
    }

    // Retrieves the top, left, bottom, and right margins set for a ToolTip window.
    inline CRect CToolTip::GetMargin() const
    {
        assert(IsWindow());
        CRect rc;
        SendMessage(TTM_GETMARGIN, 0, reinterpret_cast<LPARAM>(&rc));
        return rc;
    }

    // Retrieves the maximum width for a ToolTip window.
    inline int CToolTip::GetMaxTipWidth() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TTM_GETMAXTIPWIDTH, 0, 0));
    }

    // Retrieves the text information a ToolTip control maintains about a tool.
    inline CString CToolTip::GetText(HWND control, UINT id) const
    {
        assert(IsWindow());
        CString str;
        TOOLINFO info = GetToolInfo(control, id);

        LPTSTR pText = str.GetBuffer(80); // Maximum allowed ToolTip is 80 characters for Windows XP and below
		info.lpszText = pText;
        SendMessage(TTM_GETTEXT, 0, reinterpret_cast<LPARAM>(&info));
        str.ReleaseBuffer();

        return str;
    }

    // Retrieves the background color in a ToolTip window.
    inline COLORREF CToolTip::GetTipBkColor() const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(SendMessage(TTM_GETTIPBKCOLOR, 0, 0));
    }

    // Retrieves the text color in a ToolTip window.
    inline COLORREF CToolTip::GetTipTextColor() const
    {
        assert(IsWindow());
        return static_cast<COLORREF>(SendMessage(TTM_GETTIPTEXTCOLOR, 0, 0));
    }

    // Retrieves a count of the tools maintained by a ToolTip control.
    inline int CToolTip::GetToolCount() const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TTM_GETTOOLCOUNT, 0, 0));
    }

    // Retrieves the information that a ToolTip control maintains about a tool.
    // control is the control specified when the tooltip was added with AddTool.
    // id is the user ID (if any) specified when the tooltip was added with AddTool.
    // Note:
    // The TOOLINFO struct returned does not contain the ToolTip's text.
    // Use GetText to retrieve a tool's text.
    inline TOOLINFO CToolTip::GetToolInfo(HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO info;
        ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);
        if (id == static_cast<UINT>(-1))
        {
			info.hwnd = GetParent();
			info.uId = (UINT_PTR)control;
        }
        else
        {
			info.hwnd = control;
			info.uId = id;
        }

        VERIFY(SendMessage(TTM_GETTOOLINFO, 0, reinterpret_cast<LPARAM>(&info)));

        return info;
    }

    // Tests a point to determine whether it is within the bounding rectangle of the
    //  specified tool and, if it is, retrieves information about the tool.
    inline BOOL CToolTip::HitTest(HWND wnd, CPoint pt, const TOOLINFO& toolInfo) const
    {
        assert(IsWindow());
        TTHITTESTINFO hti;
        ZeroMemory(&hti, sizeof(hti));
        hti.hwnd = wnd;
        hti.pt = pt;
        hti.ti = toolInfo;
        return (SendMessage(TTM_HITTEST, 0, reinterpret_cast<LPARAM>(&hti)) != 0);
    }

    // Fills the TOOLINFO structure. Used by AddTool.
    // Notes:
    // 1) Notifications are passed to the parent window.
    // 2) The control is always identified by its hwnd.
    // 3) The tooltip always manages its messages (uses TTF_SUBCLASS).
    // Override this function to specify different flags.
    inline void CToolTip::FillToolInfo(TOOLINFO& info, HWND control) const
    {
        ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);

		info.hwnd = ::GetParent(*this);  // pass notifications to the parent window
		info.uFlags = TTF_IDISHWND | TTF_SUBCLASS;
		info.uId = (UINT_PTR)control;
    }

    // Fills the TOOLINFO structure. Used by AddTool.
    // Notes:
    // 1) Notifications are passed to the control window, not the parent window.
    // 2) The control is always identified by its hwnd.
    // 3) rc specifies which part of the control triggers the tooltip.
    // 4) A unique uID is required if the control has multiple tooltips.
    // 5) The tooltip always manages its messages (uses TTF_SUBCLASS).
    // 6) The TTF_IDISHWND style is incompatible with using a RECT.
    // Override this function to specify different flags.
    inline void CToolTip::FillToolInfo(TOOLINFO& info, HWND control, const RECT& rc, UINT id) const
    {
        ZeroMemory(&info, sizeof(info));
		info.cbSize = sizeof(info);

		info.hwnd = control;
		info.uFlags = TTF_SUBCLASS;
		info.rect = rc;
		info.uId = id;
    }

    // Removes a displayed ToolTip window from view.
    inline void CToolTip::Pop() const
    {
        assert(IsWindow());
        SendMessage(TTM_POP, 0, 0);
    }

    // Sets the CREATESTRUCT parameters before the tooltip is created.
    inline void CToolTip::PreCreate(CREATESTRUCT& cs)
    {
        cs.dwExStyle = WS_EX_TOOLWINDOW;
        cs.style = WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP;
    }

    // Sets the WNDCLASS parameters before the tooltip is created.
    inline void  CToolTip::PreRegisterClass(WNDCLASS& wc)
    {
        wc.lpszClassName = TOOLTIPS_CLASS;
    }

    // Passes a mouse message to a ToolTip control for processing.
    inline void CToolTip::RelayEvent(MSG& msg) const
    {
        assert(IsWindow());
        SendMessage(TTM_RELAYEVENT, 0, reinterpret_cast<LPARAM>(&msg));
    }

    // Sets the initial delay for a ToolTip control.
    inline void CToolTip::SetDelayTime(UINT delay) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETDELAYTIME, TTDT_INITIAL, delay);
    }

    // Sets the initial, pop-up, and reshow durations for a ToolTip control.
    inline void CToolTip::SetDelayTime(DWORD duration, int time) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETDELAYTIME, duration, time);
    }

    // Sets the top, left, bottom, and right margins for a ToolTip window.
    inline void CToolTip::SetMargin(const RECT& rc) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETMARGIN, 0, reinterpret_cast<LPARAM>(&rc));
    }

    // Sets the maximum width for a ToolTip window in pixels.
    inline int CToolTip::SetMaxTipWidth(int width) const
    {
        assert(IsWindow());
        return static_cast<int>(SendMessage(TTM_SETMAXTIPWIDTH, 0, width));
    }

    // Sets the background color in a ToolTip window.
    // Ignored when XP themes are active.
    inline void CToolTip::SetTipBkColor(COLORREF color) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETTIPBKCOLOR, color, 0);
    }

    // Sets the text color in a ToolTip window.
    // Ignored when XP themes are active.
    inline void CToolTip::SetTipTextColor(COLORREF color) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETTIPTEXTCOLOR, color, 0);
    }

    // Sets the information that a ToolTip control maintains for a tool.
    inline void CToolTip::SetToolInfo(const TOOLINFO& toolInfo) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETTOOLINFO, 0, reinterpret_cast<LPARAM>(&toolInfo));
    }

    // Sets a new bounding rectangle for a tool.
    inline void CToolTip::SetToolRect(const RECT& rc, HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO ti = GetToolInfo(control, id);
        ti.rect = rc;
        SendMessage(TTM_NEWTOOLRECT, 0, reinterpret_cast<LPARAM>(&ti));
    }

    // Forces the current tool to be redrawn.
    inline void CToolTip::Update() const
    {
        assert(IsWindow());
        SendMessage(TTM_UPDATE, 0, 0);
    }

    // Sets the ToolTip text for a tool.
    inline void CToolTip::UpdateTipText(LPCTSTR pText, HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO info = GetToolInfo(control, id);
		info.lpszText = const_cast<LPTSTR>(pText);
        SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&info));
    }

    // Sets the ToolTip text for a tool.
    inline void CToolTip::UpdateTipText(UINT textID, HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO info = GetToolInfo(control, id);
		info.lpszText = MAKEINTRESOURCE(textID);
        SendMessage(TTM_UPDATETIPTEXT, 0, reinterpret_cast<LPARAM>(&info));
    }

#if (_WIN32_IE >=0x0500)

    // Calculates a ToolTip control's text display rectangle from its window rectangle, or the
    // ToolTip window rectangle needed to display a specified text display rectangle.
    inline BOOL CToolTip::AdjustRect(RECT& rc, BOOL isLarger /*= TRUE*/) const
    {
        assert(IsWindow());
        return (SendMessage(TTM_ADJUSTRECT, isLarger, reinterpret_cast<LPARAM>(&rc)) != 0);
    }

    // Returns the width and height of a ToolTip control.
    inline CSize CToolTip::GetBubbleSize(HWND control, UINT id) const
    {
        assert(IsWindow());
        TOOLINFO info = GetToolInfo(control, id);
        LRESULT result = SendMessage(TTM_GETBUBBLESIZE, 0, reinterpret_cast<LPARAM>(&info));
        CSize sz(LOWORD(result), HIWORD(result));
        return sz;
    }

#ifdef TTM_SETTITLE

    // Adds a standard icon and title string to a ToolTip.
    inline BOOL CToolTip::SetTitle(UINT icon, LPCTSTR pTitle) const

    {
        assert(IsWindow());
        return (SendMessage(TTM_SETTITLE, icon, reinterpret_cast<LPARAM>(pTitle)) != 0);
    }

#endif
#endif

#if (WINVER >= 0x0501) && defined(TTM_SETWINDOWTHEME)

    // Sets the visual style of a ToolTip control.
    inline void CToolTip::SetTTWindowTheme(LPCWSTR pTheme) const
    {
        assert(IsWindow());
        SendMessage(TTM_SETWINDOWTHEME, 0, reinterpret_cast<LPARAM>(pTheme));
    }

#endif



} // namespace Win32xx

#endif //  define _WIN32XX_CONTROLS_H_

