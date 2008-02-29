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
				DestroyWindow();
		}
	}

	HWND CDialog::Create(HWND hWndParent = 0)
	{
		// Allow a dialog to be used as a child window
		SetParent(hWndParent);
		return DoModeless();
	}

	BOOL CDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CDialog if you wish to handle messages
		// A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Win32 API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Win32 API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(hWnd, uMsg, wParam, lParam);
	}

	BOOL CDialog::DialogProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// All unhandled dialog messages end up here

		switch (uMsg)
	    {
	    case WM_INITDIALOG:
			{
				m_hWnd = hWnd;
				// Center the dialog
				CenterWindow();
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
				{
					OnCommand(wParam, lParam);

					// Refelect this message if it's from a control
					CWnd* Wnd = FromHandle((HWND)lParam);
					if (Wnd != NULL)
						Wnd->OnMessageReflect(uMsg, wParam, lParam);
				}
				break;  // Some commands require default processing
	        }
	        break;
		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				CWnd* WndFrom = FromHandle(((LPNMHDR)lParam)->hwndFrom);
				if (WndFrom != NULL)
				{
					BOOL bReturn = (BOOL)WndFrom->OnNotifyReflect(wParam, lParam);
					if (bReturn) return TRUE;
				}
			}
			return (TRUE == OnNotify(wParam, lParam) );

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
			return (BOOL) OnMessage(hWnd, uMsg, wParam, lParam);

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
				throw CWinException(_T("CDialog::DoModal ... Window already exists"));

			IsModal=TRUE;

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsIndex();

			// Store the CWnd pointer in Thread Local Storage
			pTLSData->pCWnd = this;

			HINSTANCE hInstance = GetApp()->GetInstanceHandle();

			// Create a modal dialog
			INT_PTR nResult;
			if (IsIndirect)
				nResult = ::DialogBoxIndirect(hInstance, m_lpTemplate, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();
				nResult = ::DialogBox(hInstance, m_lpszResName, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			}
			// Tidy up
			m_hWnd = NULL;
			pTLSData->pCWnd = NULL;

			if (nResult == -1)
				throw CWinException(_T("Failed to create modal dialog box"));

			return nResult;
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
			return -1L;
		}

	}

	HWND CDialog::DoModeless()
	{
		// Modeless dialog
		try
		{
			if (IsWindow(m_hWnd))
				throw CWinException(_T("CDialog::DoModeless ... Window already exists"));

			IsModal=FALSE;

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsIndex();

			// Store the CWnd pointer in Thread Local Storage
			pTLSData->pCWnd = this;

			HINSTANCE hInstance = GetApp()->GetInstanceHandle();

			// Create a modeless dialog
			if (IsIndirect)
				m_hWnd = ::CreateDialogIndirect(hInstance, m_lpTemplate, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();

				m_hWnd = ::CreateDialog(hInstance, m_lpszResName, m_hWndParent, (DLGPROC)CDialog::StaticDialogProc);
			}

			// Tidy up
			pTLSData->pCWnd = NULL;

			// Now handle dialog creation failure
			if (!m_hWnd)
				throw CWinException(_T("CDialog::DoModeless ... Failed to create dialog"));
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
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
			// Find the CWnd pointer mapped to this HWND
			CDialog* w = (CDialog*)GetApp()->GetCWndFromMap(hWnd);
			if (0 != w)
			{
				// CDialog pointer found, so call the CDialog's DialogProc
				return w->DialogProc(hWnd, uMsg, wParam, lParam);
			}

			else
			{
				// The HWND wasn't in the map, so add it now

				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData)
					throw CWinException(_T("CWnd::StaticCBTProc ... Unable to get TLS"));

				// Retrieve pointer to CWnd object from Thread Local Storage TLS
				w = (CDialog*)pTLSData->pCWnd;
				if (NULL == w)
					throw CWinException(_T("CWnd::StaticWindowProc .. Failed to route message"));

				pTLSData->pCWnd = NULL;

				// Store the Window pointer into the HWND map
				GetApp()->AddToMap(hWnd, w);

				// Store the HWND in the CDialog object early
				w->m_hWnd = hWnd;

				return w->DialogProc(hWnd, uMsg, wParam, lParam);
			}
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
			return FALSE;
		}

	} // LRESULT CALLBACK CDialog::StaticDialogProc(...)

} // namespace Win32xx

