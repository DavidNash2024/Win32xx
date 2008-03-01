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


////////////////////////////////////////////////////////
// WCEframe.cpp
//  Definitions for the following classes:
//  CCmdbar and CWceFrame


#include "WCEframe.h"
#include "Default_Resource.h"


namespace Win32xx
{

	//////////////////////////////////////////
	// Definitions for the CCmdbar class
	//  This class wraps CommandBar_Create which
	//  creates a CommandBar at the top of the window
	CCmdbar::CCmdbar()
	{
	}

	CCmdbar::~CCmdbar()
	{
		if (IsWindow(m_hWnd))
			::CommandBar_Destroy(m_hWnd);
	}


	BOOL CCmdbar::AddAdornments(DWORD dwFlags)
	{
		BOOL bReturn = CommandBar_AddAdornments(m_hWnd, dwFlags, 0);

		if (!bReturn)
			DebugErrMsg(TEXT("AddAdornments failed"));

		return bReturn;
	}

	int CCmdbar::AddBitmap(int idBitmap, int iNumImages, int iImageWidth, int iImageHeight)
	{
		HINSTANCE hInst = GetApp()->GetInstanceHandle();
		return 	CommandBar_AddBitmap(m_hWnd, hInst, idBitmap, iNumImages, iImageWidth, iImageHeight);
	}

	BOOL CCmdbar::AddButtons(int nButtons, TBBUTTON* pTBButton)
	{
		 BOOL bReturn = CommandBar_AddButtons(m_hWnd, nButtons, pTBButton);
		 if (!bReturn)
			 DebugErrMsg(TEXT("Failed to add buttons to commandbar"));

		 return bReturn;
	}

	HWND CCmdbar::Create(HWND hParent)
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
			m_hWndParent = hParent;
			m_hWnd = mbi.hwndMB;
		}
		else
		{
			DebugErrMsg(TEXT("Failed to create Menubar"));
			m_hWndParent = NULL;
			m_hWnd = NULL;
		}
#else
		m_hWnd = CommandBar_Create(GetApp()->GetInstanceHandle(), hParent, IDW_MENUBAR);

		if (m_hWnd)
		{
			m_hWndParent = hParent;
		}
		else
		{
			DebugErrMsg(TEXT("Failed to create CommandBar"));
			m_hWndParent = NULL;
		}

		CommandBar_InsertMenubar(m_hWnd, GetApp()->GetInstanceHandle(), IDW_MAIN, 0);
#endif
		return m_hWnd;
	}

	int CCmdbar::GetHeight()
	{
		return CommandBar_Height(m_hWnd);
	}

	HWND CCmdbar::InsertComboBox(int iWidth, UINT dwStyle, WORD idComboBox, WORD iButton)
	{
		HINSTANCE hInst = GetApp()->GetInstanceHandle();
		HWND hWnd = CommandBar_InsertComboBox(m_hWnd, hInst, iWidth, dwStyle, idComboBox, iButton);

		if (!hWnd)
			DebugErrMsg(TEXT("InsertComboBox failed"));

		return hWnd;
	}

	BOOL CCmdbar::IsVisible()
	{
		return CommandBar_IsVisible(m_hWnd);
	}

	BOOL CCmdbar::Show(BOOL fShow)
	{
		return CommandBar_Show(m_hWnd, fShow);
	}


	/////////////////////////////////////////
	// Definitions for the CWceFrame class
	//  This class creates a simple frame using CCmdbar
	CWceFrame::CWceFrame()
	{
#ifdef SHELL_AYGSHELL
		// Initialize the shell activate info structure
		memset (&m_sai, 0, sizeof (m_sai));
		m_sai.cbSize = sizeof (m_sai);
#endif
	}

	CWceFrame::~CWceFrame()
	{
	}

	RECT CWceFrame::GetClientRect()
	{
		RECT r;
		::GetClientRect(m_hWnd, &r);

#ifndef SHELL_AYGSHELL
		// Reduce the size of the client rectange, by the commandbar height
		r.top += m_Menubar.GetHeight();
#endif

		return r;
	}

	void CWceFrame::OnCreate()
	{
		// Create the Commandbar
		m_Menubar.Create(m_hWnd);


		// Add the toolbar buttons
		if (m_ToolbarData.size() > 0)
			SetButtons(m_ToolbarData);

		// Set the accelerator table and HWND for translated messages
		GetApp()->SetAccelerators(IDW_MAIN, GetHwnd());

#ifndef SHELL_AYGSHELL
		// Add close button
		m_Menubar.AddAdornments(0);
#endif


	}

	void CWceFrame::OnActivate(WPARAM wParam, LPARAM lParam)
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

	void CWceFrame::PreCreate(CREATESTRUCT &cs)
	{
		cs.style = WS_VISIBLE;

		// Choose a unique class name for this app
		cs.lpszClass = LoadString(IDW_MAIN);
	}

	void CWceFrame::RecalcLayout()
	{
		HWND hwndCB = m_Menubar.GetHwnd();
		if (hwndCB)
		{
			RECT rc = {0};			// Desktop window size
			RECT rcMenuBar = {0};	// Menubar window size

			::SystemParametersInfo(SPI_GETWORKAREA, 0, &rc, 0);
			GetWindowRect(hwndCB, &rcMenuBar);
			rc.bottom -= (rcMenuBar.bottom - rcMenuBar.top);

			MoveWindow(m_hWnd, rc.left, rc.top, rc.right-rc.left, rc.bottom-rc.top, FALSE);
		}

		ShowWindow(m_hWnd, TRUE);
		UpdateWindow(m_hWnd);
	}

	void CWceFrame::SetButtons(const std::vector<UINT> ToolbarData)
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
			GetMenubar()->AddBitmap(IDW_MAIN, iImages , 16, 16);

			// Add the buttons
			GetMenubar()->AddButtons(iNumButtons, tbb);
		}
	}

	LRESULT CWceFrame::WndProcDefault(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
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
				SHHandleWMSettingChange(hwnd, wParam, lParam, &m_sai);
     			break;
#endif

		}
		return CWnd::WndProcDefault(hwnd, uMsg, wParam, lParam);
	}

} // namespace Win32xx
