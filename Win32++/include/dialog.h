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
// dialog.h
//  Declaration of the CDialog class

// CDialog adds support for dialogs to Win32++. Dialogs are specialised
// windows which are a parent window for common controls. Common controls
// are special window types such as buttons, edit controls, tree views,
// list views, static text etc.

// The layout of a dialog is typically defined in a resource script file
// (often Resource.rc). While this script file can be constructed manually,
// it is often created using a resource editor. If your compiler doesn't
// include a resource editor, you might find ResEdit useful. It is a free
// resource editor available for download at:
// http://www.resedit.net/

// CDialog supports modal and modeless dialogs. It also supports the creation
// of dialogs defined in a resource script file, as well as those defined in
// a dialog template.

// Use the Dialog generic program as the starting point for your own dialog
// applications.
// The DlgSubclass sample demonstrates how to use subclassing to customise
// the behaviour of common controls in a dialog.


#ifndef _DIALOG_H_
#define _DIALOG_H_

#include "wincore.h"

namespace Win32xx
{

	class CDialog : public CWnd
	{
	public:
		CDialog(UINT nResID, HWND hParent = NULL);
		CDialog(LPCTSTR lpszResName, HWND hParent = NULL);
		CDialog(LPCDLGTEMPLATE lpTemplate, HWND hParent = NULL);
		virtual ~CDialog();

		// You probably won't need to override these functions
		virtual HWND Create(HWND hParent = 0);
		virtual INT_PTR DoModal();
		virtual HWND DoModeless();
		virtual tString GetWindowType() const { return _T("CDialog"); }
		virtual void SetDlgParent(HWND hParent);
		BOOL IsModal() const { return m_IsModal; }
		BOOL IsIndirect() const { return m_IsIndirect; }

	protected:
		// These are the functions you might wish to override
		virtual BOOL DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual BOOL DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void EndDialog(INT_PTR nResult);
		virtual void OnCancel();
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		virtual BOOL PreTranslateMessage(MSG* pMsg);		

		// Can't override these functions
		static BOOL CALLBACK StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	#ifndef _WIN32_WCE
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);
	#endif

	private:
		CDialog(const CDialog&);				// Disable copy construction
		CDialog& operator = (const CDialog&); // Disable assignment operator

		BOOL m_IsIndirect;				// a flag for Indirect dialogs
		BOOL m_IsModal;					// a flag for modal dialogs
		LPCTSTR m_lpszResName;			// the resource name for the dialog
		LPCDLGTEMPLATE m_lpTemplate;	// the dialog template for indirect dialogs
		HWND m_hDlgParent;				// handle to the dialogs's parent window
	};

    enum Alignment      // used by CResizer
    {
        topleft,
        topright,
        bottomleft,
        bottomright
    };

    struct ResizeData   // used by CResizer
    {
        CRect rcInit;
        Alignment corner;
        BOOL bFixedWidth;
        BOOL bFixedHeight;
    	CWnd* pWnd;
    };

    //////////////////////////////////////
    // Declaration of the CResizer class
    //
    // The CResizer class can be used to rearrange a dialog's child
    // windows when the dialog is resized.

    // To use CResizer, follow the following steps:
    // 1) Use Initialize to specify the dialog's CWnd, and min and max size.
    // 3) Use AddChild for each child window
    // 4) Call RecalcLayout when the dialog is resized.
    //
    class CResizer
    {
    public:
		CResizer() : m_pParent(0) {}
		virtual ~CResizer() {}

        virtual void AddChild(CWnd* pWnd, Alignment corner, BOOL bFixedWidth, BOOL bFixedHeight);
    	virtual void Initialize(CWnd* pParent, RECT rcMin, RECT rcMax = CRect(0,0,0,0));
		virtual void RecalcLayout();

    private:
        CWnd* m_pParent;
    	std::vector<ResizeData> m_vResizeData;

    	CRect m_rcInit;
    	CRect m_rcMin;
    	CRect m_rcMax;
    };

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
    ////////////////////////////////////
	// Definitions for the CDialog class
	//
	inline CDialog::CDialog(LPCTSTR lpszResName, HWND hParent/* = NULL*/)
		: m_IsIndirect(FALSE), m_IsModal(TRUE), m_lpszResName(lpszResName), m_lpTemplate(NULL)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	inline CDialog::CDialog(UINT nResID, HWND hParent/* = NULL*/)
		: m_IsIndirect(FALSE), m_IsModal(TRUE), m_lpszResName(MAKEINTRESOURCE (nResID)), m_lpTemplate(NULL)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	//For indirect dialogs - created from a dialog box template in memory.
	inline CDialog::CDialog(LPCDLGTEMPLATE lpTemplate, HWND hParent/* = NULL*/)
		: m_IsIndirect(TRUE), m_IsModal(TRUE), m_lpszResName(NULL), m_lpTemplate(lpTemplate)
	{
		m_hDlgParent = hParent;
		::InitCommonControls();
	}

	inline CDialog::~CDialog()
	{
		if (m_hWnd != NULL)
		{
			if (IsModal())
				::EndDialog(m_hWnd, 0);
			else
				Destroy();
		}
	}

	inline HWND CDialog::Create(HWND hParent /*= 0*/)
	{
		// Allow a dialog to be used as a child window

		assert(GetApp());
		SetDlgParent(hParent);
		return DoModeless();
	}

	inline BOOL CDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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

	inline BOOL CDialog::DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// All DialogProc functions should pass unhandled messages to this function
	{
		LRESULT lr = 0;

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
					CWnd* pWnd = GetApp()->GetCWndFromMap((HWND)lParam);
					if (pWnd != NULL)
						lr = pWnd->OnMessageReflect(uMsg, wParam, lParam);

					// Handle user commands
					if (!lr)
						lr =  OnCommand(wParam, lParam);

					if (lr) return 0L;
				}
				break;  // Some commands require default processing
	        }
	        break;

		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = GetApp()->GetCWndFromMap(hwndFrom);

				if (GetWindowType() != _T("CReBar"))	// Skip notification reflection for rebars to avoid double handling
				{
					if (pWndFrom != NULL)
						lr = pWndFrom->OnNotifyReflect(wParam, lParam);
					else
					{
						// Some controls (eg ListView) have child windows.
						// Reflect those notifications too.
						CWnd* pWndFromParent = GetApp()->GetCWndFromMap(::GetParent(hwndFrom));
						if (pWndFromParent != NULL)
							lr = pWndFromParent->OnNotifyReflect(wParam, lParam);
					}
				}

				// Handle user notifications
				if (!lr) lr = OnNotify(wParam, lParam);

				// Set the return code for notifications
				SetWindowLongPtr(DWLP_MSGRESULT, (LONG_PTR)lr);

				return lr;
			}
					
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

	inline INT_PTR CDialog::DoModal()
	{
		// Create a modal dialog
		// A modal dialog box must be closed by the user before the application continues

		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		m_IsModal=TRUE;

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

	#ifndef _WIN32_WCE
		BOOL IsHookedHere = FALSE;
		if (NULL == pTLSData->hHook )
		{
			pTLSData->hHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());
			IsHookedHere = TRUE;
		}
    #endif

		HINSTANCE hInstance = GetApp()->GetInstanceHandle();
		pTLSData->pCWnd = this;

		// Create a modal dialog
		INT_PTR nResult;
		if (IsIndirect())
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

	#ifndef _WIN32_WCE
		if (IsHookedHere)
		{
			::UnhookWindowsHookEx(pTLSData->hHook);
			pTLSData->hHook = NULL;
		}
	#endif

		if (nResult == -1)
			throw CWinException(_T("Failed to create modal dialog box"));

		GetApp()->RemoveTmpWnds();
		
		return nResult;
	}

	inline HWND CDialog::DoModeless()
	{
		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		m_IsModal=FALSE;

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		// Store the CWnd pointer in Thread Local Storage
		pTLSData->pCWnd = this;

		HINSTANCE hInstance = GetApp()->GetInstanceHandle();

		// Create a modeless dialog
		if (IsIndirect())
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
			throw CWinException(_T("Failed to create dialog"));

		return m_hWnd;
	}

	inline void CDialog::EndDialog(INT_PTR nResult)
	{
		assert(::IsWindow(m_hWnd));

		if (IsModal())
			::EndDialog(m_hWnd, nResult);
		else
			Destroy();

		m_hWnd = NULL;
	}

	inline void CDialog::OnCancel()
	{
		// Override to customize OnCancel behaviour
		EndDialog(IDCANCEL);
	}

	inline BOOL CDialog::OnInitDialog()
	{
		// Called when the dialog is initialized
		// Override it in your derived class to automatically perform tasks
		// The return value is used by WM_INITDIALOG

		return TRUE;
	}

	inline void CDialog::OnOK()
	{
		// Override to customize OnOK behaviour
		EndDialog(IDOK);
	}

	inline BOOL CDialog::PreTranslateMessage(MSG* pMsg)
	{
		// allow the dialog to translate keyboard input
		if ((pMsg->message >= WM_KEYFIRST) && (pMsg->message <= WM_KEYLAST))
		{
			// Process dialog keystrokes for modeless dialogs
			if (!IsModal())
			{
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData->hHook)
				{
					if (IsDialogMessage(m_hWnd, pMsg))
						return TRUE;
				}
				else
				{
					// A modal message loop is running so we can't do IsDialogMessage.
					// Avoid having a modeless dialog created by a modal dialog to allow
					// normal keyboard processing.
				}
			}
		}

		return FALSE;
	}

	inline void CDialog::SetDlgParent(HWND hParent)
	// Allows the parent of the dialog to be set before the dialog is created
	{
		m_hDlgParent = hParent;
	}

	inline BOOL CALLBACK CDialog::StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		try
		{
			// Find the CWnd pointer mapped to this HWND
			CDialog* w = (CDialog*)GetApp()->GetCWndFromMap(hWnd);
			if (0 == w)
			{
				// The HWND wasn't in the map, so add it now
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData)
					throw CWinException(_T("Unable to get TLS"));

				// Retrieve pointer to CWnd object from Thread Local Storage TLS
				w = (CDialog*)pTLSData->pCWnd;
				if (NULL == w)
					throw CWinException(_T("Failed to route message"));

				pTLSData->pCWnd = NULL;

				// Store the Window pointer into the HWND map
				w->m_hWnd = hWnd;
				w->AddToMap();
			}

			return w->DialogProc(uMsg, wParam, lParam);
		}

		catch (const CWinException &e )
		{
			e.what();
			return FALSE;
		}

	} // LRESULT CALLBACK CDialog::StaticDialogProc(...)

#ifndef _WIN32_WCE
	inline LRESULT CALLBACK CDialog::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	{
		// Used by Modal Dialogs to PreTranslate Messages
		TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());

		if (nCode == MSGF_DIALOGBOX)
		{
			MSG* lpMsg = (MSG*) lParam;

			// only pre-translate keyboard events
			if ((lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST))
			{
				for (HWND hWnd = lpMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
				{
					CDialog* pDialog = (CDialog*)GetApp()->GetCWndFromMap(hWnd);
					if (pDialog && (pDialog->GetWindowType() == _T("CDialog")))
					{
						pDialog->PreTranslateMessage(lpMsg);
						break;
					}
				}
			}
		}

		return ::CallNextHookEx(pTLSData->hHook, nCode, wParam, lParam);
	}
#endif

    /////////////////////////////////////
	// Definitions for the CResizer class
	//

	void inline CResizer::AddChild(CWnd* pWnd, Alignment corner, BOOL bFixedWidth, BOOL bFixedHeight)
    // Adds a child window (usually a dialog control) to the set of windows managed by
	// the Resizer.
	//
	// The alignment corner should be set to the closest corner of the dialog. Allowed
	// values are topleft, topright, bottomleft, and bottomright.
	// Set bFixedWidth to TRUE if the width should be fixed instead of proportional.
	// Set bFixedHeight to TRUE if the height should be fixed instead of proportional.
	{
    	assert (NULL != pWnd);

    	ResizeData rd;
    	rd.corner = corner;
    	rd.bFixedWidth  = bFixedWidth;
    	rd.bFixedHeight = bFixedHeight;
    	rd.rcInit = pWnd->GetClientRect();
    	::MapWindowPoints(*pWnd, *m_pParent, (LPPOINT)&rd.rcInit, 2);
    	rd.pWnd = pWnd;

    	m_vResizeData.push_back(rd);
    }

    void inline CResizer::Initialize(CWnd* pParent, RECT rcMin, RECT rcMax)
	// Sets up the Resizer by specifying the parent window (usually a dialog),
	//  and the minimum and maximum allowed rectangle sizes.
    {
    	assert (NULL != pParent);

    	m_pParent = pParent;
    	m_rcInit = pParent->GetClientRect();
    	m_rcMin = rcMin;
    	m_rcMax = rcMax;
    }

    void inline CResizer::RecalcLayout()
    // Repositions the child windows. Call this function when handling
	// the WM_SIZE message in the parent window.
	{
    	assert (m_rcInit.Width() > 0 && m_rcInit.Height() > 0);
    	assert (NULL != m_pParent);

    	CRect rcCurrent = m_pParent->GetClientRect();
    	rcCurrent.right  = MAX( rcCurrent.Width(),  m_rcMin.Width() );
    	rcCurrent.bottom = MAX( rcCurrent.Height(), m_rcMin.Height() );
    	if (!m_rcMax.IsRectEmpty())
    	{
    		rcCurrent.right  = MIN( rcCurrent.Width(),  m_rcMax.Width() );
    		rcCurrent.bottom = MIN( rcCurrent.Height(), m_rcMax.Height() );
    	}

		// Determine the x and y ratios
    	double xRatio = (double)rcCurrent.Width()  / (double)m_rcInit.Width();
    	double yRatio = (double)rcCurrent.Height() / (double)m_rcInit.Height();

		// Declare an iterator to step through the vector
		std::vector<ResizeData>::iterator iter;

    	for (iter = m_vResizeData.begin(); iter < m_vResizeData.end(); ++iter)
    	{
    		int left   = 0;
    		int top    = 0;
    		int width  = 0;
    		int height = 0;

    		// Calculate the new size and position of the child window
			switch( (*iter).corner )
    		{
    		case topleft:
    			width  = (int)((*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()*xRatio);
    			height = (int)((*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height()*yRatio);
    			left   = (int)((*iter).rcInit.left * xRatio);
    			top    = (int)((*iter).rcInit.top * yRatio);

    			break;
    		case topright:
    			width  = (int)((*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()*xRatio);
    			height = (int)((*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height()*yRatio);
    			left   = (int)((*iter).rcInit.right * xRatio)  - width;
    			top    = (int)((*iter).rcInit.top * yRatio);

    			break;
    		case bottomleft:
    			width  = (int)((*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()*xRatio);
    			height = (int)((*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height()*yRatio);
    			left   = (int)((*iter).rcInit.left * xRatio);
    			top    = (int)((*iter).rcInit.bottom * yRatio) - height;

    			break;
    		case bottomright:
    			width  = (int)((*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()*xRatio);
    			height = (int)((*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height()*yRatio);
    			left   = (int)((*iter).rcInit.right * xRatio)  - width;
    			top    = (int)((*iter).rcInit.bottom * yRatio) - height;

    			break;
    		}

			// Position the child window.
    		(*iter).pWnd->SetWindowPos(NULL, left, top, width, height, 0);
    	}

    	// Redraw the parent window
		m_pParent->Invalidate();
    }

} // namespace Win32xx

#endif // _DIALOG_H_

