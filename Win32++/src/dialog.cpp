#include "dialog.h"

namespace Win32xx
{

	CDialog::CDialog(LPCTSTR lpszResName, HWND hParent/* = NULL*/)
		: IsIndirect(FALSE), IsModal(TRUE), m_lpszResName(lpszResName), m_lpTemplate(NULL)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	CDialog::CDialog(UINT nResID, HWND hParent/* = NULL*/)
		: IsIndirect(FALSE), IsModal(TRUE), m_lpszResName(MAKEINTRESOURCE (nResID)), m_lpTemplate(NULL)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	//For indirect dialogs - created from a dialog box template in memory.
	CDialog::CDialog(LPCDLGTEMPLATE lpTemplate, HWND hParent/* = NULL*/)
		: IsIndirect(TRUE), IsModal(TRUE), m_lpszResName(NULL), m_lpTemplate(lpTemplate)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	CDialog::~CDialog()
	{
		if (m_hWnd != NULL)
		{
			if (IsModal)
				::EndDialog(m_hWnd, 0);
			else
				Destroy();
		}
	}

	HWND CDialog::Create(HWND hParent = 0)
	{
		// Allow a dialog to be used as a child window
		SetDlgParent(hParent);
		return DoModeless();
	}

	BOOL CDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CDialog if you wish to handle messages
		// A typical function might look like this:

		//	switch (uMsg)
		//	{
		//	case MESSAGE1:		// Some Windows API message
		//		OnMessage1();	// A user defined function
		//		break;			// Also do default processing
		//	case MESSAGE2:
		//		OnMessage2();
		//		return x;		// Don't do default processing, but instead return
		//						//  a value recommended by the Windows API documentation
		//	}

		// Always pass unhandled messages on to DialogProcDefault
		return DialogProcDefault(uMsg, wParam, lParam);
	}

	BOOL CDialog::DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// All unhandled dialog messages end up here

		switch (uMsg)
	    {
	    case WM_INITDIALOG:
			{
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
					// Refelect this message if it's from a control
					CWnd* pWnd = FromHandle((HWND)lParam);
					if (pWnd != NULL)
						if (pWnd->OnMessageReflect(uMsg, wParam, lParam))
							return TRUE;

					// Handle user commands
					if (OnCommand(wParam, lParam))
						return TRUE;
				}
				break;  // Some commands require default processing
	        }
	        break;
		
		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = FromHandle(hwndFrom);

				if (!(IsRebar()))	// Skip notification reflection for rebars to avoid double handling
				{
					if (pWndFrom != NULL)
					{	
						BOOL bReturn = (BOOL)pWndFrom->OnNotifyReflect(wParam, lParam);
						if (bReturn) return TRUE;
					}
					else
					{
						// Some controls (eg ListView) have child windows.
						// Reflect those notifications too.
						CWnd* pWndFromParent = FromHandle(::GetParent(hwndFrom));
						if (pWndFromParent != NULL)
						{	
							BOOL bReturn = (BOOL)pWndFromParent->OnNotifyReflect(wParam, lParam);
							if (bReturn) return TRUE;
						}
					}
				}
			}
			return (BOOL)OnNotify(wParam, lParam);

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
			return (BOOL) MessageReflect(m_hWnd, uMsg, wParam, lParam);

	    } // switch(uMsg)
	    return FALSE;

	} // LRESULT CALLBACK CDialog::DialogProc(...)

	INT_PTR CDialog::DoModal()
	{
		// Create a modal dialog
		// A modal dialog box must be closed by the user before the application continues
		try
		{
			if (IsWindow())
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
				nResult = ::DialogBoxIndirect(hInstance, m_lpTemplate, m_hDlgParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();
				nResult = ::DialogBox(hInstance, m_lpszResName, m_hDlgParent, (DLGPROC)CDialog::StaticDialogProc);
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
			if (IsWindow())
				throw CWinException(_T("CDialog::DoModeless ... Window already exists"));

			IsModal=FALSE;

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsIndex();

			// Store the CWnd pointer in Thread Local Storage
			pTLSData->pCWnd = this;

			HINSTANCE hInstance = GetApp()->GetInstanceHandle();

			// Create a modeless dialog
			if (IsIndirect)
				m_hWnd = ::CreateDialogIndirect(hInstance, m_lpTemplate, m_hDlgParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();

				m_hWnd = ::CreateDialog(hInstance, m_lpszResName, m_hDlgParent, (DLGPROC)CDialog::StaticDialogProc);
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
				Destroy();
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

	BOOL CDialog::PreTranslateMessage(MSG* pMsg)
	{
		// allow the dialog to translate keyboard input
		if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
			if (IsDialogMessage(m_hWnd, pMsg))
				return TRUE;
		}

		return CWnd::PreTranslateMessage(pMsg);
	}

	void CDialog::SetDlgParent(HWND hParent)
	// Allows the parent of the dialog to be set before the dialog is created
	{
		m_hDlgParent = hParent;
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
				return w->DialogProc(uMsg, wParam, lParam);
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
				w->m_hWnd = hWnd;
				w->AddToMap();

				return w->DialogProc(uMsg, wParam, lParam);
			}
		}

		catch (const CWinException &e )
		{
			e.MessageBox();
			return FALSE;
		}

	} // LRESULT CALLBACK CDialog::StaticDialogProc(...)

} // namespace Win32xx