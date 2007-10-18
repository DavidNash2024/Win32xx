#include "PropertySheet.h"

namespace Win32xx
{

	//////////////////////////////////////////
	// Definitions for the CPropertyPage class
	//
	CPropertyPage::CPropertyPage(UINT nIDTemplate, LPCTSTR szTitle /* = NULL*/) 
	{
		ZeroMemory(&m_PSP, sizeof(PROPSHEETPAGE));
		m_szTitle[0] = '\0';
		if (szTitle)
		{
			lstrcpyn(m_szTitle, szTitle, MAX_STRING_SIZE);
			m_PSP.dwFlags  = PSP_USETITLE;
		}

		m_PSP.dwSize        = sizeof(PROPSHEETPAGE);
		m_PSP.hInstance     = GetApp()->GetInstanceHandle();
		m_PSP.pszTemplate   = MAKEINTRESOURCE(nIDTemplate);
		m_PSP.pfnDlgProc    = (DLGPROC)CPropertyPage::StaticDialogProc;
		m_PSP.lParam        = (LPARAM)this;
		m_PSP.pszTitle      = m_szTitle;
	}

	LRESULT CPropertyPage::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// Override this function in your class derrived from CPropertyPage if you wish to handle messages
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

	LRESULT CPropertyPage::DialogProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		// All unhandled property page messages end up here

		switch (uMsg)
	    {
	    case WM_INITDIALOG:
		    return OnInitDialog();

		case WM_NOTIFY:
			return OnNotify(wParam, lParam);

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
	    return 0L;

	} // LRESULT CALLBACK CPropertyPage::DialogProc(...)

	BOOL CPropertyPage::IsButtonEnabled(int iButton)
	{
		HWND hWnd = ::GetDlgItem(::GetParent(m_hWnd), iButton);
		if (hWnd == NULL)
			return FALSE;
		return ::IsWindowEnabled(hWnd);
	}

	BOOL CPropertyPage::OnApply() 
	{
		// This function is called for each page when the Apply button is pressed
		// Override this function in your derived class if required.

		OnOK();
		return TRUE;
	}

	void CPropertyPage::OnCancel()
	{
		// This function is called for each page when the Cancel button is pressed
		// Override this function in your derived class if required.
	}

	BOOL CPropertyPage::OnQueryCancel()
	{
		// Called when the cancel button is pressed, and before the cancel has taken place

		return TRUE;    // ok to cancel
	}

	BOOL CPropertyPage::OnInitDialog()
	{
		// Called when the property page is created
		// Override this function in your derived class if required.

		return TRUE;
	}

	void CPropertyPage::OnOK() 
	{
		// Called for each page when the OK button is pressed
		// Override this function in your derived class if required.	
	}

	BOOL CPropertyPage::OnSetActive() 
	{
		return TRUE;
	}
	
	LRESULT CPropertyPage::OnWizardBack() 
	{
		// This function is called when the Back button is pressed on a wizard page
		// Override this function in your derived class if required.

		return 0L;
	}
	
	BOOL CPropertyPage::OnWizardFinish() 
	{
		// This function is called when the Finish button is pressed on a wizard page
		// Override this function in your derived class if required.

		return TRUE;
	}
	
	LRESULT CPropertyPage::OnWizardNext() 
	{
		// This function is called when the Next button is pressed on a wizard page
		// Override this function in your derived class if required.

		return 0L;
	}

	BOOL CPropertyPage::OnKillActive()
	{
		return TRUE;
	}

	LRESULT CPropertyPage::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
	{
		NMHDR* pNMHDR = (NMHDR*)lParam;
		switch (pNMHDR->code)
		{
		case PSN_SETACTIVE:
			return OnSetActive();
			break;
		case PSN_KILLACTIVE:
			return !OnKillActive();
			break;
		case PSN_APPLY:
			return OnApply() ? PSNRET_NOERROR : PSNRET_INVALID_NOCHANGEPAGE;
			break;
		case PSN_RESET:
			OnCancel();
			break;
		case PSN_QUERYCANCEL:
			return !OnQueryCancel();
			break;
		case PSN_WIZNEXT:
			//WINBUG: Win32 will send a PSN_WIZBACK even if the button is disabled.
		//	if (IsButtonEnabled(ID_WIZNEXT))
				return OnWizardNext();
			break;
		case PSN_WIZBACK:
			//WINBUG: Win32 will send a PSN_WIZBACK even if the button is disabled.
		//	if (IsButtonEnabled(ID_WIZBACK))
				return OnWizardBack();
			break;
		case PSN_WIZFINISH:
			return  !OnWizardFinish();
			break; 
		case PSN_HELP:
			SendMessage(m_hWnd, WM_COMMAND, ID_HELP, 0);
			break;

		default:
			return FALSE;   // not handled
		}
		return TRUE;
	}


	LRESULT CALLBACK CPropertyPage::StaticDialogProc(HWND hwndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		GetApp()->m_MapLock.Lock();

		if (uMsg == WM_INITDIALOG)
		{
			CPropertyPage* pPage = (CPropertyPage*)(((LPPROPSHEETPAGE)lParam)->lParam);
			GetApp()->GetHWNDMap().insert(std::make_pair(hwndDlg, pPage));
			pPage->m_hWnd = hwndDlg;
			pPage->m_hWndParent = GetParent(hwndDlg);
		}

		// Allocate an iterator for our HWND map
		std::map<HWND, CWnd*, CompareHWND>::iterator m;
		
		m = GetApp()->GetHWNDMap().find(hwndDlg);
		
		GetApp()->m_MapLock.Release();
		if (m != GetApp()->GetHWNDMap().end())
		{
			return ((CPropertyPage*)m->second)->DialogProc(hwndDlg, uMsg, wParam, lParam);
		}

		return 0;
	}


	///////////////////////////////////////////
	// Definitions for the CPropertySheet class
	//
	CPropertySheet::CPropertySheet(UINT nIDCaption, HWND hwndParent /* = NULL*/)
	{
		m_szCaption[0] = _T('\0');
		if (nIDCaption) lstrcpyn(m_szCaption, MAKEINTRESOURCE(nIDCaption), MAX_STRING_SIZE);	
		ZeroMemory(&m_PSH, sizeof (PROPSHEETHEADER));

		m_PSH.dwSize           = sizeof(PROPSHEETHEADER);
		m_PSH.dwFlags          = PSH_PROPSHEETPAGE | PSH_USECALLBACK;
		m_PSH.hwndParent       = hwndParent;
		m_PSH.hInstance        = GetApp()->GetInstanceHandle();
		m_PSH.pszCaption       = m_szCaption;
		m_PSH.pfnCallback      = (PFNPROPSHEETCALLBACK)CPropertySheet::Callback;
	}
	
	CPropertySheet::CPropertySheet(LPCTSTR pszCaption /*= NULL*/, HWND hwndParent /* = NULL*/)
	{
		m_szCaption[0] = _T('\0');
		if (pszCaption) lstrcpyn(m_szCaption, pszCaption, MAX_STRING_SIZE);
		ZeroMemory(&m_PSH, sizeof (PROPSHEETHEADER));

		m_PSH.dwSize           = sizeof(PROPSHEETHEADER);
		m_PSH.dwFlags          = PSH_PROPSHEETPAGE | PSH_USECALLBACK;
		m_PSH.hwndParent       = hwndParent;
		m_PSH.hInstance        = GetApp()->GetInstanceHandle();
		m_PSH.pszCaption       = m_szCaption;
		m_PSH.pfnCallback      = (PFNPROPSHEETCALLBACK)CPropertySheet::Callback;
	}

	CPropertySheet::~CPropertySheet()
	{
		for (int i = 0 ; i < (int)m_vPages.size(); i++)
			delete m_vPages[i];
	}
	
	void CPropertySheet::AddPage(CPropertyPage* pPage)
	{
		m_vPages.push_back(pPage);
	}

	void CALLBACK CPropertySheet::Callback(HWND hwnd, UINT uMsg, LPARAM lParam)
	{
		switch(uMsg)
		{
		//called before the dialog is created, hwnd = NULL, lParam points to dialog resource
		case PSCB_PRECREATE:
			{
				LPDLGTEMPLATE  lpTemplate = (LPDLGTEMPLATE)lParam;

				if(!(lpTemplate->style & WS_SYSMENU))
				{
					lpTemplate->style |= WS_SYSMENU;
				}
			}
			break;

		//called after the dialog is created
		case PSCB_INITIALIZED:
			try
			{
				std::map<HWND, CWnd*, CompareHWND>::iterator m;

				// Retrieve pointer to CWnd object from Thread Local Storage TLS
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (!pTLSData)
				{
					throw (CWinException(_T("CPropertySheet::Callback ... Unable to get TLS")));
				}

				// Store the Window pointer into the HWND map
				CWnd* w = pTLSData->pCWnd;
				GetApp()->m_MapLock.Lock();
				GetApp()->GetHWNDMap().insert(std::make_pair(hwnd, w));
				GetApp()->m_MapLock.Release();

				w->m_hWnd = hwnd;
				w->Subclass();
			}
		
			catch (const CWinException &e)
			{
				e.MessageBox();
			}

			catch (...)
			{
				DebugErrMsg(_T("Exception in CPropertySheet::Callback"));
				throw;	// Rethrow unknown exception
			}

			break;
		}
	}


	HWND CPropertySheet::Create(HWND hWndParent /*= 0*/)
	// Creates a modeless Property sheet
	{		
		if (hWndParent)
		{
			m_PSH.hwndParent = hWndParent;
			m_hWndParent = hWndParent;
		}

		// Create an array of PROPSHEETPAGEs
		PROPSHEETPAGE* ppsp = new PROPSHEETPAGE[m_vPages.size()];
		for (int i = 0 ; i < (int)m_vPages.size(); i++)
		{
			ppsp[i] = m_vPages[i]->GetPSP();
		}

		m_PSH.nPages           = (int)m_vPages.size();
		m_PSH.ppsp             = (LPCPROPSHEETPAGE) ppsp;

		// Create a modeless Property Sheet
		m_PSH.dwFlags &= ~PSH_WIZARD;
		m_PSH.dwFlags |= PSH_MODELESS;
		HWND hWnd = (HWND)CreatePropertySheet(&m_PSH);

		return hWnd;
	}

	INT_PTR CPropertySheet::CreatePropertySheet(LPCPROPSHEETHEADER ppsph)
	{
		INT_PTR ipResult = 0;

		try
		{
			// Only one window per CWnd instance allowed
			if (::IsWindow(m_hWnd))
				throw CWinException(_T("CreatePropertySheet ... Window already exists"));

			// Ensure this thread has the TLS index set
			GetApp()->m_MapLock.Lock();
			m_pTLSData = (TLSData*)::TlsGetValue(GetApp()->GetTlsIndex());
			if (m_pTLSData == NULL)
			{
				m_pTLSData = GetApp()->SetTlsIndex();
			}
			GetApp()->m_MapLock.Release();

			// Store the 'this' pointer in Thread Local Storage
			m_pTLSData->pCWnd = this;

			// Create the property sheet
			ipResult = PropertySheet(ppsph);
			
			// Set the parent for a modeless property sheet
			if (ppsph->dwFlags & PSH_MODELESS)
			{
				m_hWndParent = ppsph->hwndParent;
			}
			
			// Window creation is complete. Now call OnInitialUpdate
			OnInitialUpdate();
		}

		catch (const CWinException &e)
		{
			m_hWndParent = NULL;
			e.MessageBox();
		}

		catch (...)
		{
			m_hWndParent = NULL;
			DebugErrMsg(_T("Exception in CPropertySheet::CreatePropertySheet"));
			throw;	// Rethrow unknown exception
		}

		return ipResult;
	}

	void CPropertySheet::DestroyWindow()
	{
		CWnd::DestroyWindow();

		for (int i = 0 ; i < (int)m_vPages.size(); i++)
			delete m_vPages[i];

		m_vPages.clear();	
	}

	int CPropertySheet::DoModal()
	{
		// Create an array of PROPSHEETPAGEs
		PROPSHEETPAGE* ppsp = new PROPSHEETPAGE[m_vPages.size()];
		for (int i = 0 ; i < (int)m_vPages.size(); i++)
		{
			ppsp[i] = m_vPages[i]->GetPSP();
		}

		m_PSH.nPages           = (int)m_vPages.size();
		m_PSH.ppsp             = (LPCPROPSHEETPAGE) ppsp;

		// Create the Property Sheet
		int nSuccess = (int)CreatePropertySheet(&m_PSH);

		// Property sheet closed. Now clean up
		delete []ppsp;

		for (int j = 0 ; j < (int)m_vPages.size(); j++)
		{
			delete m_vPages[j];
		}
		m_vPages.clear();

		return nSuccess;
	}

	void CPropertySheet::SetWizardMode(BOOL bWizard) 
	{
		if (bWizard)
		{
			m_PSH.dwFlags &= ~PSH_MODELESS;
			m_PSH.dwFlags |= PSH_WIZARD;
		}
		else
		{
			m_PSH.dwFlags &= ~PSH_WIZARD;
		}
	}
	
	LRESULT CPropertySheet::WndProcDefault(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch (uMsg)
		{
		case WM_COMMAND:
			// Handle user commands
			OnCommand(wParam, lParam);
			break;

		case WM_SYSCOMMAND:
			if (wParam == SC_CLOSE)
			{
				DestroyWindow();
				return 0L;
			}
			break;
		}

		// pass unhandled messages on for default processing
		return CWnd::WndProcDefault(hWnd, uMsg, wParam, lParam);	
	}

} // namespace Win32xx

