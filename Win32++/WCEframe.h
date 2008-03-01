// Win32++  Version 6.0
// Released: 4th March, 2008 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2008  David Nash
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


//////////////////////////////////////////////////////
// WceFrame.h
// Definitions for the CCmdbar and CWceFrame

// These classes are provide a frame window for use on Window CE devices such
// as Pocket PCs. The frame uses CommandBar (a control unique to the Windows CE
// operating systems) to display the menu and toolbar.
//
// Use the PocketPCWceFrame generic application as the starting point for your own
// frame based applications on the Pocket PC.
//
// Refer to the Scribble demo application for an example of how these classes
// can be used.


#ifndef WCEFRAME_H
#define WCEFRAME_H


#include "WinCore.h"
#include <commctrl.h>
#include <vector>

#if defined(WIN32_PLATFORM_PSPC) || defined(WIN32_PLATFORM_WFSP)
  #define SHELL_AYGSHELL
#endif

#ifdef SHELL_AYGSHELL
  #include <aygshell.h>
  #pragma comment(lib, "aygshell.lib")
#endif // SHELL_AYGSHELL

#if (_WIN32_WCE < 0x500 && defined(SHELL_AYGSHELL)) || _WIN32_WCE == 420
  #pragma comment(lib, "ccrtrtti.lib")
#endif


namespace Win32xx
{

	////////////////////////////////////
	// Declaration of the CCmdbar class
	//
	class CCmdbar : public CWnd
	{
	public:
		CCmdbar();
		virtual ~CCmdbar();
		virtual BOOL AddAdornments(DWORD dwFlags);
		virtual int  AddBitmap(int idBitmap, int iNumImages, int iImageWidth, int iImageHeight);
		virtual BOOL AddButtons(int nButtons, TBBUTTON* pTBButton);
		virtual HWND Create(HWND hwndParent);
		virtual int  GetHeight();
		virtual HWND InsertComboBox(int iWidth, UINT dwStyle, WORD idComboBox, WORD iButton);
		virtual BOOL IsVisible();
		virtual BOOL Show(BOOL fShow);

	private:

#ifdef SHELL_AYGSHELL
		SHMENUBARINFO m_mbi;
#endif

	};


	//////////////////////////////////////
	// Declaration of the CWceFrame class
	//  A mini frame based on CCmdbar
	class CWceFrame : public CWnd
	{
	public:
		CWceFrame();
		virtual ~CWceFrame();
		virtual RECT GetClientRect();
		virtual CCmdbar* GetMenubar() {return &m_Menubar;}
		virtual void OnCreate();
		virtual void OnActivate(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual void RecalcLayout();
		virtual void SetButtons(const std::vector<UINT> ToolbarData);
		virtual	LRESULT WndProcDefault(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		std::vector<UINT> m_ToolbarData;

	private:
		CCmdbar m_Menubar;

#ifdef SHELL_AYGSHELL
		SHACTIVATEINFO m_sai;
#endif

	};

} // namespace Win32xx

#endif // WCEFRAME_H
