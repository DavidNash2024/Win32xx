#include "stdafx.h"
#include "TaskDialog.h"


namespace Win32xx
{

	CTaskDialog::CTaskDialog() : m_SelectedCommandControlID(0), m_SelectedRadioButtonID(0), m_VerificationCheckboxState(FALSE)
	{
		ZeroMemory(&m_tc, sizeof(m_tc));
		m_tc.cbSize = sizeof(m_tc);
		m_tc.pfCallback = CTaskDialog::StaticTaskDialogProc;
	}

	void CTaskDialog::AddCommandControl(int nCommandControlID, LPCTSTR strCaption, BOOL bEnabled, BOOL bRequiresElevation)
	{
		TASKDIALOG_BUTTON tdb;
		tdb.nButtonID = nCommandControlID;
		tdb.pszButtonText = strCaption;
		m_vCommandControls.push_back(tdb);
	}

	void CTaskDialog::AddRadioButton(int nRadioButtonID, LPCTSTR strCaption, BOOL bEnabled)
	{
		TASKDIALOG_BUTTON tdb;
		tdb.nButtonID = nRadioButtonID;
		tdb.pszButtonText = strCaption;
		m_vRadioButtons.push_back(tdb);
	}

	void CTaskDialog::ClickCommandControl(int nCommandControlID) const
	{
		SendMessage(TDM_CLICK_BUTTON, (WPARAM)nCommandControlID, 0); 
	}

	void CTaskDialog::ClickRadioButton(int nRadioButtonID) const
	{ 
		SendMessage(TDM_CLICK_RADIO_BUTTON, (WPARAM)nRadioButtonID, 0);
	}

	LRESULT CTaskDialog::DoModal()
	{
		m_tc.cbSize = sizeof(m_tc);
		m_tc.pButtons = m_vCommandControls.empty()? NULL : &m_vCommandControls.front();
		m_tc.cButtons = m_vCommandControls.size();
		m_tc.pRadioButtons = m_vRadioButtons.empty()? NULL : &m_vRadioButtons.front();
		m_tc.cRadioButtons = m_vRadioButtons.size();

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		// Store the CWnd pointer in thread local storage
		pTLSData->pCWnd = this;

		return ::TaskDialogIndirect(&m_tc, &m_SelectedCommandControlID, &m_SelectedRadioButtonID, &m_VerificationCheckboxState);
	}

	TASKDIALOG_FLAGS CTaskDialog::GetOptions() const
	{
		return m_tc.dwFlags;
	}

	int CTaskDialog::GetSelectedCommandControlID() const
	{
		return m_SelectedCommandControlID;
	}

	int CTaskDialog::GetSelectedRadioButtonID() const 
	{ 
		return m_SelectedRadioButtonID; 
	}
		
	BOOL CTaskDialog::GetVerificationCheckboxState() const
	{ 
		return m_VerificationCheckboxState;
	}

	BOOL CTaskDialog::IsButtonEnabled(int nButtonID) const
	{
		CWnd* pWnd = GetDlgItem(nButtonID);
		assert(pWnd);
		
		return pWnd->IsWindowEnabled();
	}

	void CTaskDialog::RemoveAllCommandControls()
	{
		m_vCommandControls.clear();
	}

	void CTaskDialog::RemoveAllRadioButtons()
	{
		m_vRadioButtons.clear();
	}

	void CTaskDialog::SetCommandControlOptions(int nCommandControlID, BOOL bEnabled, BOOL bRequiresElevation)
	{
		assert(m_hWnd);
		SendMessage(TDM_ENABLE_BUTTON, (WPARAM)bEnabled, 0);
		SendMessage(TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE, (WPARAM)bRequiresElevation, 0);
	}

	void CTaskDialog::SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons)
	{
		m_tc.dwCommonButtons = dwCommonButtons; 
	}

	void CTaskDialog::SetContent(LPCWSTR pszContent)
	{
		m_tc.pszContent = pszContent; 
	}

	void CTaskDialog::SetDefaultCommandControl(int nCommandControlID) 
	{
		m_tc.nDefaultRadioButton = nCommandControlID;
	}

	void CTaskDialog::SetDefaultRadioButton(int nRadioButtonID) 
	{
		m_tc.nDefaultRadioButton = nRadioButtonID;
	}

	void CTaskDialog::SetDialogWidth(UINT nWidth /*= 0*/)
	{
		m_tc.cxWidth = nWidth;
	}

	void CTaskDialog::SetFooterIcon(HICON hFooterIcon) 
	{
		m_tc.hFooterIcon = hFooterIcon;
	}

	void CTaskDialog::SetFooterIcon(LPCWSTR lpszFooterIcon) 
	{
		m_tc.pszFooterIcon = lpszFooterIcon;
	}

	void CTaskDialog::SetFooterText(LPCWSTR pszFooter)
	{
		m_tc.pszFooter = pszFooter;
	}

	void CTaskDialog::SetMainIcon(HICON hMainIcon) 
	{
		m_tc.hMainIcon = hMainIcon;
	}

	void CTaskDialog::SetMainIcon(LPCWSTR lpszMainIcon)
	{
		m_tc.pszMainIcon = lpszMainIcon; 
	}

	void CTaskDialog::SetMainInstruction(LPCWSTR pszMainInstruction) 
	{
		m_tc.pszMainInstruction = pszMainInstruction;
	}

	void CTaskDialog::SetOptions(TASKDIALOG_FLAGS dwFlags)
	{
		m_tc.dwFlags = dwFlags;
	}

	void CTaskDialog::SetProgressBarMarquee(BOOL bEnabled /* = TRUE*/, int nMarqueeSpeed /* = 0*/) 
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_MARQUEE, (WPARAM)bEnabled, (LPARAM)nMarqueeSpeed);
	}

	void CTaskDialog::SetProgressBarPosition(int nProgressPos) 
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_POS, (WPARAM)nProgressPos, 0);
	}

	void CTaskDialog::SetProgressBarRange(int nMinRange, int nMaxRange) 
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(nMinRange, nMaxRange)); 
	}

	void CTaskDialog::SetProgressBarState(int nNewState /* = PBST_NORMAL*/)
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_STATE, (WPARAM)nNewState, 0);
	}

	void CTaskDialog::SetRadioButtonOptions(int nRadioButtonID, BOOL bEnabled) 
	{
		assert(m_hWnd);
		SendMessage(TDM_ENABLE_RADIO_BUTTON, (WPARAM)nRadioButtonID, 0);
	} 

	void CTaskDialog::SetVerificationCheckbox(BOOL bChecked)
	{
		assert(m_hWnd);
		SendMessage(TDM_CLICK_VERIFICATION, (WPARAM)bChecked, (LPARAM)bChecked);
	}

	void CTaskDialog::SetVerificationCheckboxText(LPCWSTR pszVerificationText)
	{
		m_tc.pszVerificationText = pszVerificationText;
	}

	void CTaskDialog::SetWindowTitle(LPCWSTR pszWindowTitle)
	{
		m_tc.pszWindowTitle = pszWindowTitle; 
	}

	HRESULT CALLBACK CTaskDialog::StaticTaskDialogProc(HWND hWnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData)
	// TaskDialogs direct their messages here.
	{
		assert( GetApp() );

		try
		{
			CTaskDialog* t = (CTaskDialog*)GetApp()->GetCWndFromMap(hWnd);
			if (0 == t)
			{
				// The CTaskDialog pointer wasn't found in the map, so add it now

				// Retrieve the pointer to the TLS Data
				TLSData* pTLSData = (TLSData*)TlsGetValue(GetApp()->GetTlsIndex());
				if (NULL == pTLSData)
					throw CWinException(_T("Unable to get TLS"));

				// Retrieve pointer to CTaskDialog object from Thread Local Storage TLS
				t = (CTaskDialog*)(pTLSData->pCWnd);
				if (NULL == t)
					throw CWinException(_T("Failed to route message"));

				pTLSData->pCWnd = NULL;

				// Store the CTaskDialog pointer in the HWND map
				t->m_hWnd = hWnd;
				t->AddToMap();
			}

			return t->TaskDialogProc(uNotification, wParam, lParam);
		}

		catch (const CWinException &e)
		{
			// Most CWinExceptions will end up here unless caught earlier.
			e.what();
		}

		return 0L;

	} // LRESULT CALLBACK StaticTaskDialogProc(...)

	LRESULT CTaskDialog::TaskDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		switch(uMsg)
		{
		case TDN_BUTTON_CLICKED:
			OnButtonClicked((int)wParam);
			break;
		case TDN_CREATED:
			OnCreated();
			break;
		case TDN_DESTROYED:
			OnDestroyed();
			break;
		case TDN_DIALOG_CONSTRUCTED:
			OnInit();
			break;
		case TDN_EXPANDO_BUTTON_CLICKED:
			OnExpandButtonClicked((BOOL)wParam);
			break;
		case TDN_HELP:
			OnHelp();
			break;
		case TDN_HYPERLINK_CLICKED:
			OnHyperlinkClicked((LPCWSTR)lParam);
			break;
		case TDN_NAVIGATED:
			OnNavigatePage();
			break;
		case TDN_RADIO_BUTTON_CLICKED:
			OnRadioButtonClicked((int)wParam);
			break;
		case TDN_TIMER:
			OnTimer((DWORD)wParam);
			break;
		case TDN_VERIFICATION_CLICKED:
			OnVerificationCheckboxClicked((BOOL)wParam);
			break;
		}

		return S_OK;
	}

}

