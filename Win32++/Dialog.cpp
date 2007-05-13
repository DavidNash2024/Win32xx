// Win32++  Version 5.11 beta
// Released: 27th February, 2007 by:
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: http://users.bigpond.net.au/programming/
//
//
// Copyright (c) 2005-2007  David Nash
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
// Dialog.cpp
// Definitions for the CDialog class


#include "dialog.h"


namespace Win32xx
{

	CDialog::CDialog(LPCTSTR lpszResName, HWND hWndParent/* = NULL*/)
		: IsIndirect(FALSE), IsModal(TRUE), m_lpszResName(lpszResName), m_lpTemplate(NULL)
	{
		m_hWndParent = hWndParent;
		::InitCommonControls();
	}

	CDialog::CDialog(UINT nResID, HWND hWndParent/* = NULL*/)
		: IsIndirect(FALSE), IsModal(TRUE), m_lpszResName(MAKEINTRESOURCE (nResID)), m_lpTemplate(NULL)
	{
		m_hWndParent = hWndParent;
		::InitCommonControls();
	}

	//For indirect dialogs - created from a dialog box template in memory.
	CDialog::CDialog(LPCDLGTEMPLATE lpTemplate, HWND hWndParent/* = NULL*/)
		: IsIndirect(TRUE), IsModal(TRUE), m_lpszResName(NULL), m_lpTemplate(lpTemplate)
	{
		m_hWndParent = hWndParent;
		::InitCommonControls();
	}

	CDialog::~CDialog()
	{
		if (m_hWnd != NULL)
		{
			if (IsModal)
				::EndDialog(m_hWnd, 0);
			else
				::DestroyWindow(m_hWnd);
		}
	}

	POINT CDialog::Center(HWND hWnd)
	{
		// Determine the position of the top left corner which would
		// center the dialog on its parent window.

		RECT rcDialog;
		RECT rcParent;
		RECT rcDesktop;
		POINT CenterPos;

		// Get screen dimensions excluding task bar
		::SystemParametersInfo(SPI_GETWORKAREA, 0, &rcDesktop, 0);
		int iWidth = rcDesktop.right;
		int iHeight = rcDesktop.bottom;

		// Get the dialog dimensions
		::GetWindowRect(hWnd, &rcDialog);

		// Get the parent window dimensions (parent could be the desktop)
		HWND hParent = ::GetParent(hWnd);
		if (hParent != NULL) ::GetWindowRect(hParent, &rcParent);
		else rcParent = rcDesktop;

		// Calculate point to center the dialog on the parent window
		int x = rcParent.left + ((rcParent.right - rcParent.left) - (rcDialog.right - rcDialog.left))/2;
		int y = rcParent.top + ((rcParent.bottom - rcParent.top) - (rcDialog.bottom - rcDialog.top))/2;

		// Keep the dialog wholly on the desktop
		if (x < 0) x = 0;
		if (y < 0) y = 0;
		if (x > iWidth - (rcDialog.right - rcDialog.left))
			x = iWidth - (rcDialog.right - rcDialog.left);
		if (y > iHeight - (rcDialog.bottom - rcDialog.top))
			y = iHeight - (rcDialog.bottom - rcDialog.top);

		CenterPos.x = x;
		CenterPos.y = y;
		return CenterPos;
	} // POINT CDialog::Center(HWND hWnd)

	BOOL CDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
	    switch (uMsg)
	    {
	    case WM_INITDIALOG:
			{
				m_hWnd = hwnd;
				// Center the dialog
				POINT CenterPt = Center(m_hWnd);
				::SetWindowPos(m_hWnd, HWND_TOP, CenterPt.x, CenterPt.y, 0, 0,  SWP_NOSIZE);
			}
		    return OnInitDialog();
	    case WM_COMMAND:
	        switch (LOWORD (wParam))
	        {
	        case IDOK:
				OnOK();
				return TRUE;
			case IDCANCEL:
				OnCancel();
				return TRUE;
			default:
				OnCommand(LOWORD(wParam));
				break;  // Some commands require default processing
	        }
	        break;
		case WM_NOTIFY:
			return (OnNotify(wParam, lParam) == TRUE);

		// A set of messages to be reflected back to the control that generated them
		case WM_CTLCOLORBTN:
		case WM_CTLCOLOREDIT:
		case WM_CTLCOLORDLG:
		case WM_CTLCOLORLISTBOX:
		case WM_CTLCOLORSCROLLBAR:
		case WM_CTLCOLORSTATIC:
		case WM_DRAWITEM:
		case WM_MEASUREITEM:
		case WM_DELETEITEM:
		case WM_COMPAREITEM:
		case WM_CHARTOITEM:
		case WM_VKEYTOITEM:
		case WM_HSCROLL:
		case WM_VSCROLL:
		case WM_PARENTNOTIFY:
			return (BOOL) OnMessage(hwnd, uMsg, wParam, lParam);

	    } // switch(uMsg)
	    return FALSE;

	} // LRESULT CALLBACK CDialog::DialogProc(...)

	INT_PTR CDialog::DoModal()
	{
		// Create a modal dialog
		// A modal dialog box must be closed by the user before the application continues
		try
		{
			if (IsWindow(m_hWnd))
				throw CWinException(TEXT("CDialog::DoModal ... Window already exists"));

			IsModal=TRUE;

			// Ensure this thread has the TLS index set
			GetApp()->m_MapLock.Lock();
			m_pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());

			if (m_pTLSData == NULL)
				m_pTLSData = GetApp()->SetTlsIndex();
			GetApp()->m_MapLock.Release();

			// Create and store the CBT hook
			SetHook();

			// Create a modal dialog
			INT_PTR nResult;
			if (IsIndirect)
				nResult = ::DialogBoxIndirect(GetApp()->GetInstanceHandle(), m_lpTemplate, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			else
				nResult = ::DialogBox(GetApp()->GetInstanceHandle(), m_lpszResName, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);

			// Tidy up
			RemoveHook();
			m_hWnd = NULL;

			return nResult;
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CDialog::DoModal"));
		}

		return 0;
	}

	HWND CDialog::DoModeless()
	{
		// Modeless dialog
		try
		{
			if (IsWindow(m_hWnd))
				throw CWinException(TEXT("CDialog::DoModeless ... Window already exists"));

			IsModal=FALSE;

			// Ensure this thread has the TLS index set
			GetApp()->m_MapLock.Lock();
			m_pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());

			if (m_pTLSData == NULL)
				m_pTLSData = GetApp()->SetTlsIndex();
			GetApp()->m_MapLock.Release();

			// Create and store the CBT hook
			SetHook();

			// Create a modeless dialog
			if (IsIndirect)
				m_hWnd = ::CreateDialogIndirect(GetApp()->GetInstanceHandle(), m_lpTemplate, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			else
				m_hWnd = ::CreateDialog(GetApp()->GetInstanceHandle(), m_lpszResName, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);

			// Tidy up
			RemoveHook();

			// Now handle dialog creation failure
			if (!m_hWnd)
				throw CWinException(TEXT("CDialog::DoModeless ... Failed to create dialog"));
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CDialog::DoModeless"));
		}

		return m_hWnd;
	}

	void CDialog::EndDialog(INT_PTR nResult)
	{
		if (::IsWindow(m_hWnd))
		{
			if (IsModal)
				::EndDialog(m_hWnd, nResult);
			else
				::DestroyWindow(m_hWnd);
		}
		m_hWnd = NULL;
	}

	void CDialog::OnCancel()
	{
		// Override to customize OnCancel behaviour
		EndDialog(IDCANCEL);
	}

	BOOL CDialog::OnInitDialog()
	{
		// Called when the dialog is initialized
		// Override it in your derived class to automatically perform tasks
		// The return value is used by WM_INITDIALOG

		return TRUE;
	}

	void CDialog::OnOK()
	{
		// Override to customize OnOK behaviour
		EndDialog(IDOK);
	}

	BOOL CALLBACK CDialog::StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			std::map<HWND, CWnd*, CompareHWND>::iterator m;

			GetApp()->m_MapLock.Lock();
			m = GetApp()->GetHWNDMap().find(hWnd);
			GetApp()->m_MapLock.Release();
			if (m != GetApp()->GetHWNDMap().end())
				return ((CDialog*)m->second)->DialogProc(hWnd, uMsg, wParam, lParam);

			throw (CWinException(TEXT("CDialog::StaticDialogProc ... Failed to route message")));
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
		}

		catch (...)
		{
			DebugErrMsg(TEXT("Exception in CDialog::StaticDialogProc"));
		}

		return 0;
	} // LRESULT CALLBACK CDialog::StaticDialogProc(...)

} // namespace Win32xx

