// Win32++  Version 6.61
// Released: 30th August, 2009 by:
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


#ifndef _TOOLBAR_H_
#define _TOOLBAR_H_

#include "wincore.h"
#include "gdi.h"
#include "rebar.h"


namespace Win32xx
{

	struct ThemeToolbar
	{
		BOOL UseThemes;			// TRUE if themes are used
		COLORREF clrHot1;		// Colour 1 for hot button
		COLORREF clrHot2;		// Colour 2 for hot button
		COLORREF clrPressed1;	// Colour 1 for pressed button
		COLORREF clrPressed2;	// Colour 2 for pressed button
		COLORREF clrOutline;	// Colour for border outline
	};


	////////////////////////////////////
	// Declaration of the CToolbar class
	//
	class CToolbar : public CWnd
	{
	public:
		CToolbar();
		virtual ~CToolbar();
		virtual BOOL IsToolbar() const {return TRUE;}

	// Attributes
		void AddToolbarButton(UINT nID, BOOL bEnabled = TRUE);
		virtual void Destroy();
		int  CommandToIndex(int iButtonID) const;
		int  GetButtonCount() const;
		UINT GetButtonState(int iButtonID) const;
		BYTE GetButtonStyle(int iButtonID) const;
		int  GetCommandID(int iIndex) const;
		CRect GetItemRect(int iIndex) const;
		CSize GetMaxSize() const;
		std::vector<UINT>& GetToolbarData() const {return (std::vector <UINT> &)m_vToolbarData;}
		ThemeToolbar& GetToolbarTheme() {return m_Theme;}
		BOOL HasText() const;
		int  HitTest() const;
		void SetBitmap(UINT nID);
		void SetBitmapSize(int cx, int cy) const;
		int  SetButtons(const std::vector<UINT>& vToolbarData) const;
		void SetButtonSize(int cx, int cy) const;
		void SetButtonState(int iButtonID, UINT State) const;
		void SetButtonStyle(int iButtonID, BYTE Style) const;
		void SetButtonText(int iButtonID, LPCTSTR szText);
		void SetButtonWidth(int iButtonID, int nWidth) const;
		void SetCommandID(int iIndex, int iButtonID) const;
		void SetImages(COLORREF crMask, UINT ToolbarID, UINT ToolbarHotID, UINT ToolbarDisabledID);
		void SetToolbarTheme(ThemeToolbar& Theme);

	// Operations
		void AddBitmap(UINT ToolbarID);
		void DisableButton(int iButtonID) const;
		void EnableButton(int iButtonID) const;
		void ReplaceBitmap(UINT NewToolbarID);

	protected:
	// Overridables
		virtual void OnCreate();
		virtual void OnDestroy();
		virtual void OnLButtonDblClk(WPARAM wParam, LPARAM lParam);
		virtual void OnWindowPosChanging(WPARAM wParam, LPARAM lParam);
		virtual LRESULT OnCustomDraw(NMHDR* pNMHDR);
		virtual LRESULT OnNotifyReflect(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	private:
		CToolbar(const CToolbar&);				// Disable copy construction
		CToolbar& operator = (const CToolbar&); // Disable assignment operator

		std::vector<UINT> m_vToolbarData;	// vector of resource IDs for toolbar buttons
		std::map<tString, int> m_StringMap;	// a map of strings used in SetButtonText
		UINT m_OldToolbarID;				// Bitmap Resource ID, used in AddBitmap/ReplaceBitmap
		ThemeToolbar m_Theme;				// The theme structure
		BOOL m_bDrawArrowBkgrnd;			// True if a seperate arrow background is to be drawn

	};  // class CToolbar

}


#endif // #ifndef _TOOLBAR_H_
