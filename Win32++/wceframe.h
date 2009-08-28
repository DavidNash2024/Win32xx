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


#ifndef _WCEFRAME_H_
#define _WCEFRAME_H_


#include "wincore.h"
#include <commctrl.h>
#include <vector>
#include "Default_Resource.h"

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
		virtual int  GetHeight() const;
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
		virtual void AddToolbarButton(UINT nID);
		CRect GetViewRect() const;
		CCmdbar& GetMenubar() const {return (CCmdbar&)m_Menubar;}
		virtual void OnCreate();
		virtual void OnActivate(WPARAM wParam, LPARAM lParam);
		virtual void PreCreate(CREATESTRUCT &cs);
		virtual BOOL PreTranslateMessage(MSG* pMsg);
		virtual void RecalcLayout();
		virtual void SetButtons(const std::vector<UINT> ToolbarData);
		virtual	LRESULT WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);

	protected:
		std::vector<UINT> m_ToolbarData;

	private:
		CCmdbar m_Menubar;
		tString m_tsAppName;

#ifdef SHELL_AYGSHELL
		SHACTIVATEINFO m_sai;
#endif

	};

	//////////////////////////////////////////
	// Definitions for the CCmdbar class
	//  This class wraps CommandBar_Create which
	//  creates a CommandBar at the top of the window
	inline CCmdbar::CCmdbar()
	{
	}

	inline CCmdbar::~CCmdbar()
	{
		if (IsWindow())
			::CommandBar_Destroy(m_hWnd);
	}


	inline BOOL CCmdbar::AddAdornments(DWORD dwFlags)
	{
		BOOL bReturn = CommandBar_AddAdornments(m_hWnd, dwFlags, 0);

		if (!bReturn)
			DebugErrMsg(TEXT("AddAdornments failed"));

		return bReturn;
	}

	inline int CCmdbar::AddBitmap(int idBitmap, int iNumImages, int iImageWidth, int iImageHeight)
	{
		HINSTANCE hInst = GetApp()->GetInstanceHandle();
		return 	CommandBar_AddBitmap(m_hWnd, hInst, idBitmap, iNumImages, iImageWidth, iImageHeight);
	}

	inline BOOL CCmdbar::AddButtons(int nButtons, TBBUTTON* pTBButton)
	{
		 BOOL bReturn = CommandBar_AddButtons(m_hWnd, nButtons, pTBButton);
		 if (!bReturn)
			 DebugErrMsg(TEXT("Failed to add buttons to commandbar"));

		 return bReturn;
	}

	inline HWND CCmdbar::Create(HWND hParent)
	{
#ifdef SHELL_AYGSHELL
		SHMENUBARINFO mbi;

		memset(&mbi, 0, sizeof(SHMENUBARINFO));
		mbi.cbSize     = sizeof(SHMENUBARINFO);
		mbi.hwndParent = hParent;
		mbi.nToolBarId = IDW_MAIN;
		mbi.hInstRes   = GetApp()->GetInstanceHandle();
		mbi.nBmpId     = 0;
		mbi.cBmpImages = 0;

		if (SHCreateMenuBar(&mbi))
		{
			m_hWnd = mbi.hwndMB;
		}
		else
		{
			DebugErrMsg(TEXT("Failed to create Menubar"));
			m_hWnd = NULL;
		}
#else
		m_hWnd = CommandBar_Create(GetApp()->GetInstanceHandle(), hParent, IDW_MENUBAR);

		if (m_hWnd == NULL)
		{
			DebugErrMsg(TEXT("Failed to create CommandBar"));
		}

		CommandBar_InsertMenubar(m_hWnd, GetApp()->GetInstanceHandle(), IDW_MAIN, 0);
#endif
		return m_hWnd;
	}

	inline int CCmdbar::GetHeight() const
	{
		return CommandBar_Height(m_hWnd);
	}

	inline HWND CCmdbar::InsertComboBox(int iWidth, UINT dwStyle, WORD idComboBox, WORD iButton)
	{
		HINSTANCE hInst = GetApp()->GetInstanceHandle();
		HWND hWnd = CommandBar_InsertComboBox(m_hWnd, hInst, iWidth, dwStyle, idComboBox, iButton);

		if (!hWnd)
			DebugErrMsg(TEXT("InsertComboBox failed"));

		return hWnd;
	}

	inline BOOL CCmdbar::IsVisible()
	{
		return ::CommandBar_IsVisible(m_hWnd);
	}

	inline BOOL CCmdbar::Show(BOOL fShow)
	{
		return ::CommandBar_Show(m_hWnd, fShow);
	}


	/////////////////////////////////////////
	// Definitions for the CWceFrame class
	//  This class creates a simple frame using CCmdbar
	inline CWceFrame::CWceFrame()
	{
#ifdef SHELL_AYGSHELL
		// Initialize the shell activate info structure
		memset (&m_sai, 0, sizeof (m_sai));
		m_sai.cbSize = sizeof (m_sai);
#endif
	}

	inline CWceFrame::~CWceFrame()
	{
	}

	inline void CWceFrame::AddToolbarButton(UINT nID)
	// Adds Resource IDs to toolbar buttons.
	// A resource ID of 0 is a separator
	{
		m_ToolbarData.push_back(nID);
	}

	inline CRect CWceFrame::GetViewRect() const
	{
		CRect r;
		::GetClientRect(m_hWnd, &r);

#ifndef SHELL_AYGSHELL
		// Reduce the size of the client rectange, by the commandbar height
		r.top += m_Menubar.GetHeight();
#endif

		return r;
	}

	inline void CWceFrame::OnCreate()
	{
		// Create the Commandbar
		m_Menubar.Create(m_hWnd);


		// Add the toolbar buttons
		if (m_ToolbarData.size() > 0)
			SetButtons(m_ToolbarData);

#ifndef SHELL_AYGSHELL
		// Add close button
		m_Menubar.AddAdornments(0);
#endif

	}

	inline void CWceFrame::OnActivate(WPARAM wParam, LPARAM lParam)
	{
#ifdef SHELL_AYGSHELL
		// Notify shell of our activate message
		SHHandleWMActivate(m_hWnd, wParam, lParam, &m_sai, FALSE);

		UINT fActive = LOWORD(wParam);
		if ((fActive == WA_ACTIVE) || (fActive == WA_CLICKACTIVE))
		{
			// Reposition the window when it's activated
			RecalcLayout();
		}
#endif
	}

	inline void CWceFrame::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE;

		// Choose a unique class name for this app
		m_tsAppName = LoadString(IDW_MAIN);
		cs.lpszClass = m_tsAppName.c_str();
	}

	inline BOOL CWceFrame::PreTranslateMessage(MSG* pMsg)
	{
		HACCEL hAccelTable = ::LoadAccelerators(GetApp()->GetResourceHandle(), MAKEINTRESOURCE(IDW_MAIN));
		if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
		{
			if (TranslateAccelerator(m_hWnd, hAccelTable, pMsg))
				return TRUE;
		}
		return CWnd::PreTranslateMessage(pMsg);
	}

	inline void CWceFrame::RecalcLayout()
	{
		HWND hwndCB = m_Menubar.GetHwnd();
		if (hwndCB)
		{
			CRect rc;			// Desktop window size
			CRect rcMenuBar;	// Menubar window size

			::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
			::GetWindowRect(hwndCB, &rcMenuBar);
			rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);

			MoveWindow(rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
		}

		ShowWindow(TRUE);
		UpdateWindow();
	}

	inline void CWceFrame::SetButtons(const std::vector<UINT> ToolbarData)
	// Define the resource IDs for the toolbar like this in the Frame's constructor
	// m_ToolbarData.push_back ( 0 );				// Separator
	// m_ToolbarData.clear();
	// m_ToolbarData.push_back ( IDM_FILE_NEW   );
	// m_ToolbarData.push_back ( IDM_FILE_OPEN  );
	// m_ToolbarData.push_back ( IDM_FILE_SAVE  );

	{
		int iImages = 0;
		int iNumButtons = (int)ToolbarData.size();


		if (iNumButtons > 0)
		{
			// Create the TBBUTTON array for each button
			TBBUTTON* tbb =  new TBBUTTON[iNumButtons];

			for (int j = 0 ; j < iNumButtons; j++)
			{
				ZeroMemory(&tbb[j], sizeof(TBBUTTON));

				if (ToolbarData[j] == 0)
				{
					tbb[j].fsStyle = TBSTYLE_SEP;
				}
				else
				{
					tbb[j].iBitmap = iImages++;
					tbb[j].idCommand = ToolbarData[j];
					tbb[j].fsState = TBSTATE_ENABLED;
					tbb[j].fsStyle = TBSTYLE_BUTTON;
					tbb[j].iString = -1;
				}
			}

			// Add the bitmap
			GetMenubar().AddBitmap(IDW_MAIN, iImages , 16, 16);

			// Add the buttons
			GetMenubar().AddButtons(iNumButtons, tbb);
		}
	}

	inline LRESULT CWceFrame::WndProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
			case WM_DESTROY:
				PostQuitMessage(0);
				break;
			case WM_ACTIVATE:
				OnActivate(wParam, lParam);
     			break;

#ifdef SHELL_AYGSHELL

			case WM_SETTINGCHANGE:
				SHHandleWMSettingChange(m_hWnd, wParam, lParam, &m_sai);
     			break;
#endif

		}
		return CWnd::WndProcDefault(uMsg, wParam, lParam);
	}


} // namespace Win32xx

#endif // _WCEFRAME_H_

