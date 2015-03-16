// Win32++   Version 7.8
// Release Date: 17th March 2015
//
//      David Nash
//      email: dnash@bigpond.net.au
//      url: https://sourceforge.net/projects/win32-framework
//
//
// Copyright (c) 2005-2015  David Nash
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
// The DialogDemo sample demonstrates how to use subclassing to customise
// the behaviour of common controls in a dialog.

// Note: The following functions often used with dialogs are provided by CWnd:
//  GetDlgCtrlID, GetDlgItem, GetDlgItemInt, GetDlgItemText, GetNextDlgGroupItem,
//  GetNextDlgTabItem, SendDlgItemMessage, SetDlgItemInt, SetDlgItemText 


#ifndef _WIN32XX_DIALOG_H_
#define _WIN32XX_DIALOG_H_

#include "wincore.h"

#ifndef SWP_NOCOPYBITS
	#define SWP_NOCOPYBITS      0x0100
#endif


namespace Win32xx
{

	class CDialog : public CWnd
	{
	public:
		CDialog(UINT nResID);
		CDialog(LPCTSTR lpszResName);
		CDialog(LPCDLGTEMPLATE lpTemplate);
		virtual ~CDialog();

		// You probably won't need to override these functions
		virtual void AttachItem(int nID, CWnd& Wnd);
		virtual INT_PTR DoModal(CWnd* pParent = NULL);
		virtual HWND DoModeless(CWnd* pParent = NULL);
		virtual HWND Create(CWnd* pParent = NULL) { return DoModeless(pParent); }
		BOOL IsModal() const { return m_IsModal; }
		BOOL IsIndirect() const { return (NULL != m_lpTemplate); }

	protected:
		// These are the functions you might wish to override
		virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual INT_PTR DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam);
		virtual void EndDialog(INT_PTR nResult);
		virtual void OnCancel();
		virtual void OnClose();
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		virtual BOOL PreTranslateMessage(MSG* pMsg);

		// Can't override these functions
		DWORD GetDefID() const;
		void GotoDlgCtrl(CWnd* pWndCtrl);
		BOOL MapDialogRect(LPRECT pRect) const;
		void NextDlgCtrl() const;
		void PrevDlgCtrl() const;
		void SetDefID(UINT nID);
		static INT_PTR CALLBACK StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	#ifndef _WIN32_WCE
		static LRESULT CALLBACK StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam);
	#endif

	private:
		CDialog(const CDialog&);			  // Disable copy construction
		CDialog& operator = (const CDialog&); // Disable assignment operator

		BOOL m_IsModal;					// a flag for modal dialogs
		LPCTSTR m_lpszResName;			// the resource name for the dialog
		LPCDLGTEMPLATE m_lpTemplate;	// the dialog template for indirect dialogs
	};


#ifndef _WIN32_WCE

    //////////////////////////////////////
    // Declaration of the CResizer class
    //
    // The CResizer class can be used to rearrange a dialog's child
    // windows when the dialog is resized.

    // To use CResizer, follow the following steps:
    // 1) Use Initialize to specify the dialog's CWnd, and min and max size.
    // 3) Use AddChild for each child window
    // 4) Call HandleMessage from within DialogProc.
    //

	// Resize Dialog Styles
#define RD_STRETCH_WIDTH		0x0001	// The item has a variable width
#define RD_STRETCH_HEIGHT		0x0002	// The item has a variable height

	// Resize Dialog alignments
	enum Alignment { topleft, topright, bottomleft, bottomright };

    class CResizer
    {
	public:
		CResizer() : m_pParent(0), m_xScrollPos(0), m_yScrollPos(0) {}
		virtual ~CResizer() {}

        virtual void AddChild(CWnd* pWnd, Alignment corner, DWORD dwStyle);
		virtual void AddChild(HWND hWnd, Alignment corner, DWORD dwStyle);
		virtual void HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
    	virtual void Initialize(CWnd* pParent, const RECT& rcMin, const RECT& rcMax = CRect(0,0,0,0));
		virtual void OnHScroll(WPARAM wParam, LPARAM lParam);
		virtual void OnVScroll(WPARAM wParam, LPARAM lParam);
		virtual void RecalcLayout();
		CRect GetMinRect() const { return m_rcMin; }
		CRect GetMaxRect() const { return m_rcMax; }

		struct ResizeData
		{
			CRect rcInit;
			CRect rcOld;
			Alignment corner;
			BOOL bFixedWidth;
			BOOL bFixedHeight;
    		HWND hWnd;
		};

    private:
        CWnd* m_pParent;
    	std::vector<ResizeData> m_vResizeData;

    	CRect m_rcInit;
    	CRect m_rcMin;
    	CRect m_rcMax;

		int m_xScrollPos;
		int m_yScrollPos;
    };

#endif

}

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{
    ////////////////////////////////////
	// Definitions for the CDialog class
	//
	inline CDialog::CDialog(LPCTSTR lpszResName) : m_IsModal(TRUE), 
						m_lpszResName(lpszResName), m_lpTemplate(NULL)
	{
		// Initialize the common controls.
		LoadCommonControls();
	}

	inline CDialog::CDialog(UINT nResID) : m_IsModal(TRUE), 
						m_lpszResName(MAKEINTRESOURCE (nResID)), m_lpTemplate(NULL)
	{
		// Initialize the common controls.
		LoadCommonControls();
	}

	//For indirect dialogs - created from a dialog box template in memory.
	inline CDialog::CDialog(LPCDLGTEMPLATE lpTemplate) : m_IsModal(TRUE), 
						m_lpszResName(NULL), m_lpTemplate(lpTemplate)
	{
		// Initialize the common controls.
		LoadCommonControls();
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

	inline void CDialog::AttachItem(int nID, CWnd& Wnd)
	// Attach a dialog item to a CWnd
	{
		Wnd.AttachDlgItem(nID, this);
	}

	inline INT_PTR CDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derived from CDialog if you wish to handle messages
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

	inline INT_PTR CDialog::DialogProcDefault(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
		case WM_CLOSE:	
			{
				OnClose();
				return 0L;
			}	
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
					// Reflect this message if it's from a control
					CWnd* pWnd = GetApp()->GetCWndFromMap((HWND)lParam);
					if (pWnd != NULL)
						lr = pWnd->OnCommand(wParam, lParam);

					// Handle user commands
					if (!lr)
						lr =  OnCommand(wParam, lParam);

					if (lr) return 0L;
				}
				break;  // Some commands require default processing
	        }
	        break;

		case WM_DESTROY:
			OnDestroy();
			break;
		case WM_NOTIFY:
			{
				// Do Notification reflection if it came from a CWnd object
				HWND hwndFrom = ((LPNMHDR)lParam)->hwndFrom;
				CWnd* pWndFrom = GetApp()->GetCWndFromMap(hwndFrom);

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
	
				// Handle user notifications
				if (!lr) lr = OnNotify(wParam, lParam);

				// Set the return code for notifications
				if (IsWindow())
					SetWindowLongPtr(DWLP_MSGRESULT, (LONG_PTR)lr);

				return (BOOL)lr;
			}

		case WM_PAINT:
			{
				if (::GetUpdateRect(m_hWnd, NULL, FALSE))
				{
					CPaintDC dc(this);
					OnDraw(&dc);
				}
				else
				// RedrawWindow can require repainting without an update rect
				{
					CClientDC dc(this);
					OnDraw(&dc);
				}

				break;
			}

		case WM_ERASEBKGND:
			{
				CDC dc((HDC)wParam);
				BOOL bResult = OnEraseBkgnd(&dc);
				dc.Detach();
				if (bResult) return TRUE;
			}
			break;

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
			return MessageReflect(m_hWnd, uMsg, wParam, lParam);

	    } // switch(uMsg)
	    return FALSE;

	} // INT_PTR CALLBACK CDialog::DialogProc(...)

	inline INT_PTR CDialog::DoModal(CWnd* pParent /* = NULL */)
	{
		// Create a modal dialog
		// A modal dialog box must be closed by the user before the application continues

		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		INT_PTR nResult = 0;

		try
		{
			m_IsModal=TRUE;

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsData();

		#ifndef _WIN32_WCE
			if (NULL == pTLSData->hMsgHook )
			{
				pTLSData->hMsgHook = ::SetWindowsHookEx(WH_MSGFILTER, (HOOKPROC)StaticMsgHook, NULL, ::GetCurrentThreadId());
			}
			InterlockedIncrement(&pTLSData->nDlgHooks);
		#endif

			HWND hParent = pParent? pParent->m_hWnd : 0;
			HINSTANCE hInstance = GetApp()->GetInstanceHandle();
			pTLSData->pWnd = this;

			// Create a modal dialog
			if (IsIndirect())
				nResult = ::DialogBoxIndirect(hInstance, m_lpTemplate, hParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();
				nResult = ::DialogBox(hInstance, m_lpszResName, hParent, (DLGPROC)CDialog::StaticDialogProc);
			}

			// Tidy up
			m_hWnd = NULL;
			pTLSData->pWnd = NULL;
			GetApp()->CleanupTemps();

		#ifndef _WIN32_WCE
			InterlockedDecrement(&pTLSData->nDlgHooks);
			if (pTLSData->nDlgHooks == 0)
			{
				::UnhookWindowsHookEx(pTLSData->hMsgHook);
				pTLSData->hMsgHook = NULL;
			}

		#endif

			if (nResult == -1)
				throw CWinException(_T("Failed to create modal dialog box"));

		}

		catch (const CWinException &e)
		{
			TRACE("\n*** Failed to create dialog ***\n");
			e.what();	// Display the last error message.

			// eat the exception (don't rethrow)
		}

		return nResult;
	}

	inline HWND CDialog::DoModeless(CWnd* pParent /* = 0 */)
	{
		assert( GetApp() );		// Test if Win32++ has been started
		assert(!::IsWindow(m_hWnd));	// Only one window per CWnd instance allowed

		try
		{
			m_IsModal=FALSE;

			// Ensure this thread has the TLS index set
			TLSData* pTLSData = GetApp()->SetTlsData();

			// Store the CWnd pointer in Thread Local Storage
			pTLSData->pWnd = this;

			HINSTANCE hInstance = GetApp()->GetInstanceHandle();
			HWND hParent = pParent? pParent->m_hWnd : 0;

			// Create a modeless dialog
			if (IsIndirect())
				m_hWnd = ::CreateDialogIndirect(hInstance, m_lpTemplate, hParent, (DLGPROC)CDialog::StaticDialogProc);
			else
			{
				if (::FindResource(GetApp()->GetResourceHandle(), m_lpszResName, RT_DIALOG))
					hInstance = GetApp()->GetResourceHandle();

				m_hWnd = ::CreateDialog(hInstance, m_lpszResName, hParent, (DLGPROC)CDialog::StaticDialogProc);
			}

			// Tidy up
			pTLSData->pWnd = NULL;

			// Now handle dialog creation failure
			if (!m_hWnd)
				throw CWinException(_T("Failed to create dialog"));
		}

		catch (const CWinException &e)
		{
			TRACE("\n*** Failed to create dialog ***\n");
			e.what();	// Display the last error message.

			// eat the exception (don't rethrow)
		}

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
	
	inline void CDialog::OnClose()
	{
		EndDialog(0);
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
		if ( IsWindow() )
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
				TLSData* pTLSData = GetApp()->GetTlsData();
				if (NULL == pTLSData->hMsgHook)
				{
					if (IsDialogMessage(pMsg))
						return TRUE;
				}
				else
				{
					// A modal message loop is running which performs IsDialogMessage
					// for us.
				}
			}
		}

		return FALSE;
	}

	inline DWORD CDialog::GetDefID() const
	// Retrieves the identifier of the default push button control for the dialog. 
	{
		assert(::IsWindow(m_hWnd));
		DWORD dwID = 0;
		LRESULT LR = SendMessage(DM_GETDEFID, 0L, 0L);
		if (DC_HASDEFID == HIWORD(LR))
			dwID = LOWORD(LR);

		return dwID;	
	}

	inline void CDialog::GotoDlgCtrl(CWnd* pWndCtrl)
	// Sets the keyboard focus to the specified control
	{
		assert(::IsWindow(m_hWnd));
		assert(pWndCtrl->IsWindow());
		SendMessage(WM_NEXTDLGCTL, (WPARAM)pWndCtrl->GetHwnd(), TRUE);
	}

	inline BOOL CDialog::MapDialogRect(LPRECT pRect) const
	// Converts the dialog box units to screen units (pixels).
	{
		assert(::IsWindow(m_hWnd));
		return ::MapDialogRect(m_hWnd, pRect);
	}

	inline void CDialog::NextDlgCtrl() const
	// Sets the keyboard focus to the next dialog control.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(WM_NEXTDLGCTL, FALSE, FALSE);
	}

	inline void CDialog::PrevDlgCtrl() const
	// Sets the keyboard focus to the previous dialog control.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(WM_NEXTDLGCTL, TRUE, FALSE);
	}

	inline void CDialog::SetDefID(UINT nID)
	// Changes the identifier of the default push button for a dialog box.
	{
		assert(::IsWindow(m_hWnd));
		SendMessage(DM_SETDEFID, (WPARAM)nID, 0L);
	}

	inline INT_PTR CALLBACK CDialog::StaticDialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Find the CWnd pointer mapped to this HWND
		CDialog* w = static_cast<CDialog*>(FromHandlePermanent(hWnd));
		if (w == 0)
		{
			// The HWND wasn't in the map, so add it now
			TLSData* pTLSData = GetApp()->GetTlsData();
			assert(pTLSData);

			// Retrieve pointer to CWnd object from Thread Local Storage TLS
			w = static_cast<CDialog*>(pTLSData->pWnd);
			assert(w);
			pTLSData->pWnd = NULL;

			// Store the Window pointer into the HWND map
			w->m_hWnd = hWnd;
			w->AddToMap();
		}

		return w->DialogProc(uMsg, wParam, lParam);

	} // INT_PTR CALLBACK CDialog::StaticDialogProc(...)


#ifndef _WIN32_WCE
	inline LRESULT CALLBACK CDialog::StaticMsgHook(int nCode, WPARAM wParam, LPARAM lParam)
	// Used by Modal Dialogs for idle processing and PreTranslateMessage
	{
		TLSData* pTLSData = GetApp()->GetTlsData();
		MSG Msg;
		ZeroMemory(&Msg, sizeof(MSG));
		LONG lCount = 0;

		// While idle, perform idle processing until OnIdle returns FALSE
		// Exclude some messages to avoid calling OnIdle excessively
		while (!::PeekMessage(&Msg, 0, 0, 0, PM_NOREMOVE) && 
							(Msg.message != WM_TIMER) && 
							(Msg.message != WM_MOUSEMOVE) && 
							(Msg.message != WM_SETCURSOR) &&  
								GetApp()->OnIdle(lCount) != FALSE )
		{
			++lCount;
		}
		lCount = 0;

		if (Msg.message == UWM_CLEANUPTEMPS)
		{
			GetApp()->CleanupTemps();
		}

		if (nCode == MSGF_DIALOGBOX)
		{
			MSG* lpMsg = reinterpret_cast<MSG*>(lParam);

			// only pre-translate keyboard events
			if ((lpMsg->message >= WM_KEYFIRST && lpMsg->message <= WM_KEYLAST))
			{
				for (HWND hWnd = lpMsg->hwnd; hWnd != NULL; hWnd = ::GetParent(hWnd))
				{
					CDialog* pDialog = static_cast<CDialog*>(FromHandlePermanent(hWnd));
					if (pDialog && (lstrcmp(pDialog->GetClassName(), _T("#32770")) == 0))	// only for dialogs
					{
						if (pDialog->PreTranslateMessage(lpMsg))
							return 1; // Eat the message

						break;	// Pass the message on
					}
				}
			}
		}

		return ::CallNextHookEx(pTLSData->hMsgHook, nCode, wParam, lParam);
	}
#endif



#ifndef _WIN32_WCE

    /////////////////////////////////////
	// Definitions for the CResizer class
	//

	void inline CResizer::AddChild(CWnd* pWnd, Alignment corner, DWORD dwStyle)
    // Adds a child window (usually a dialog control) to the set of windows managed by
	// the Resizer.
	//
	// The alignment corner should be set to the closest corner of the dialog. Allowed
	// values are topleft, topright, bottomleft, and bottomright.
	// Set bFixedWidth to TRUE if the width should be fixed instead of variable.
	// Set bFixedHeight to TRUE if the height should be fixed instead of variable.
	{
    	ResizeData rd;
    	rd.corner = corner;
    	rd.bFixedWidth  = !(dwStyle & RD_STRETCH_WIDTH);
    	rd.bFixedHeight = !(dwStyle & RD_STRETCH_HEIGHT);
		CRect rcInit = pWnd->GetWindowRect();
		m_pParent->ScreenToClient(rcInit);
		rd.rcInit = rcInit;
		rd.hWnd = pWnd->GetHwnd();

		m_vResizeData.insert(m_vResizeData.begin(), rd);
    }

	void inline CResizer::AddChild(HWND hWnd, Alignment corner, DWORD dwStyle)
    // Adds a child window (usually a dialog control) to the set of windows managed by
	// the Resizer.	
	{
		AddChild(CWnd::FromHandle(hWnd), corner, dwStyle);
	}

	inline void CResizer::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_SIZE:
			RecalcLayout();
			break;

		case WM_HSCROLL:
			if (lParam == 0)
				OnHScroll(wParam, lParam);
			break;

		case WM_VSCROLL:
			if (lParam == 0)
				OnVScroll(wParam, lParam);
			break;
		}
	}

    void inline CResizer::Initialize(CWnd* pParent, const RECT& rcMin, const RECT& rcMax)
	// Sets up the Resizer by specifying the parent window (usually a dialog),
	//  and the minimum and maximum allowed rectangle sizes.
    {
    	assert (NULL != pParent);

    	m_pParent = pParent;
    	m_rcInit = pParent->GetClientRect();
    	m_rcMin = rcMin;
    	m_rcMax = rcMax;

		m_vResizeData.clear();

		// Add scroll bar support to the parent window
		DWORD dwStyle = (DWORD)m_pParent->GetClassLongPtr(GCL_STYLE);
		dwStyle |= WS_HSCROLL | WS_VSCROLL;
		m_pParent->SetClassLongPtr(GCL_STYLE, dwStyle);
    }

	void inline CResizer::OnHScroll(WPARAM wParam, LPARAM /*lParam*/)
	{
		int xNewPos;

		switch (LOWORD(wParam))
		{
			case SB_PAGEUP: // User clicked the scroll bar shaft left of the scroll box.
				xNewPos = m_xScrollPos - 50;
				break;

			case SB_PAGEDOWN: // User clicked the scroll bar shaft right of the scroll box.
				xNewPos = m_xScrollPos + 50;
				break;

			case SB_LINEUP: // User clicked the left arrow.
				xNewPos = m_xScrollPos - 5;
				break;

			case SB_LINEDOWN: // User clicked the right arrow.
				xNewPos = m_xScrollPos + 5;
				break;

			case SB_THUMBPOSITION: // User dragged the scroll box.
				xNewPos = HIWORD(wParam);
				break;

			case SB_THUMBTRACK: // User dragging the scroll box.
				xNewPos = HIWORD(wParam);
				break;

			default:
				xNewPos = m_xScrollPos;
		}

		// Scroll the window.
		xNewPos = MAX(0, xNewPos);
		xNewPos = MIN( xNewPos, GetMinRect().Width() - m_pParent->GetClientRect().Width() );
		int xDelta = xNewPos - m_xScrollPos;
		m_xScrollPos = xNewPos;
		m_pParent->ScrollWindow(-xDelta, 0, NULL, NULL);

		// Reset the scroll bar.
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_POS;
		si.nPos   = m_xScrollPos;
		m_pParent->SetScrollInfo(SB_HORZ, si, TRUE);
	}

	void inline CResizer::OnVScroll(WPARAM wParam, LPARAM /*lParam*/)
	{
		int yNewPos;

		switch (LOWORD(wParam))
		{
			case SB_PAGEUP: // User clicked the scroll bar shaft above the scroll box.
				yNewPos = m_yScrollPos - 50;
				break;

			case SB_PAGEDOWN: // User clicked the scroll bar shaft below the scroll box.
				yNewPos = m_yScrollPos + 50;
				break;

			case SB_LINEUP: // User clicked the top arrow.
				yNewPos = m_yScrollPos - 5;
				break;

			case SB_LINEDOWN: // User clicked the bottom arrow.
				yNewPos = m_yScrollPos + 5;
				break;

			case SB_THUMBPOSITION: // User dragged the scroll box.
				yNewPos = HIWORD(wParam);
				break;

			case SB_THUMBTRACK: // User dragging the scroll box.
				yNewPos = HIWORD(wParam);
				break;

			default:
				yNewPos = m_yScrollPos;
		}

		// Scroll the window.
		yNewPos = MAX(0, yNewPos);
		yNewPos = MIN( yNewPos, GetMinRect().Height() - m_pParent->GetClientRect().Height() );
		int yDelta = yNewPos - m_yScrollPos;
		m_yScrollPos = yNewPos;
		m_pParent->ScrollWindow(0, -yDelta, NULL, NULL);

		// Reset the scroll bar.
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_POS;
		si.nPos   = m_yScrollPos;
		m_pParent->SetScrollInfo(SB_VERT, si, TRUE);
	}

    void inline CResizer::RecalcLayout()
    // Repositions the child windows. Call this function when handling
	// the WM_SIZE message in the parent window.
	{
    	assert (m_rcInit.Width() > 0 && m_rcInit.Height() > 0);
    	assert (NULL != m_pParent);

		CRect rcCurrent = m_pParent->GetClientRect();

		// Adjust the scrolling if required
		m_xScrollPos = MIN(m_xScrollPos, MAX(0, m_rcMin.Width()  - rcCurrent.Width() ) );
		m_yScrollPos = MIN(m_yScrollPos, MAX(0, m_rcMin.Height() - rcCurrent.Height()) );
		SCROLLINFO si;
		ZeroMemory(&si, sizeof(SCROLLINFO));
		si.cbSize = sizeof(si);
		si.fMask  = SIF_RANGE | SIF_PAGE | SIF_POS;
		si.nMax   =	m_rcMin.Width();
		si.nPage  = rcCurrent.Width();
		si.nPos   = m_xScrollPos;
		m_pParent->SetScrollInfo(SB_HORZ, si, TRUE);
		si.nMax   =	m_rcMin.Height();
		si.nPage  = rcCurrent.Height();
		si.nPos   = m_yScrollPos;
		m_pParent->SetScrollInfo(SB_VERT, si, TRUE);

		// Note: calls to SetScrollInfo may have changed the client rect, so
		// we get it again.
		rcCurrent = m_pParent->GetClientRect();

    	rcCurrent.right  = MAX( rcCurrent.Width(),  m_rcMin.Width() );
    	rcCurrent.bottom = MAX( rcCurrent.Height(), m_rcMin.Height() );
    	if (!m_rcMax.IsRectEmpty())
    	{
    		rcCurrent.right  = MIN( rcCurrent.Width(),  m_rcMax.Width() );
    		rcCurrent.bottom = MIN( rcCurrent.Height(), m_rcMax.Height() );
    	}

		// Declare an iterator to step through the vector
		std::vector<ResizeData>::iterator iter;

    	for (iter = m_vResizeData.begin(); iter != m_vResizeData.end(); ++iter)
    	{
    		int left   = 0;
    		int top    = 0;
    		int width  = 0;
    		int height = 0;

    		// Calculate the new size and position of the child window
			switch( (*iter).corner )
    		{
    		case topleft:
				width  = (*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()  - m_rcInit.Width() + rcCurrent.Width();
    			height = (*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height() - m_rcInit.Height() + rcCurrent.Height();
    			left   = (*iter).rcInit.left;
    			top    = (*iter).rcInit.top;
    			break;
    		case topright:
    			width  = (*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()  - m_rcInit.Width() + rcCurrent.Width();
    			height = (*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height() - m_rcInit.Height() + rcCurrent.Height();
    			left   = (*iter).rcInit.right - width - m_rcInit.Width() + rcCurrent.Width();
    			top    = (*iter).rcInit.top;
    			break;
    		case bottomleft:
				width  = (*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()  - m_rcInit.Width() + rcCurrent.Width();
    			height = (*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height() - m_rcInit.Height() + rcCurrent.Height();
    			left   = (*iter).rcInit.left;
    			top    = (*iter).rcInit.bottom - height - m_rcInit.Height() + rcCurrent.Height();
    			break;
    		case bottomright:
    			width  = (*iter).bFixedWidth?  (*iter).rcInit.Width()  : (*iter).rcInit.Width()  - m_rcInit.Width() + rcCurrent.Width();
    			height = (*iter).bFixedHeight? (*iter).rcInit.Height() : (*iter).rcInit.Height() - m_rcInit.Height() + rcCurrent.Height();
    			left   = (*iter).rcInit.right   - width - m_rcInit.Width() + rcCurrent.Width();
    			top    = (*iter).rcInit.bottom  - height - m_rcInit.Height() + rcCurrent.Height();
    			break;
    		}

			// Position the child window.
			CRect rc(left - m_xScrollPos, top - m_yScrollPos, left + width - m_xScrollPos, top + height - m_yScrollPos);
			if ( rc != (*iter).rcOld)
			{
				CWnd* pWnd = CWnd::FromHandle((*iter).hWnd);
				CWnd *pWndPrev = pWnd->GetWindow(GW_HWNDPREV); // Trick to maintain the original tab order.
				pWnd->SetWindowPos(pWndPrev, rc, SWP_NOCOPYBITS);
				(*iter).rcOld = rc;
			}
    	}
    }

#endif // #ifndef _WIN32_WCE

} // namespace Win32xx



#endif // _WIN32XX_DIALOG_H_

