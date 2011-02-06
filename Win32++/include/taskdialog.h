// Win32++  PreRelease Version 7.1
// Released: N/A
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
// taskdialog.h
//  Declaration of the CTaskDialog class

// A task dialog is a dialog box that can be used to display information 
// and receive simple input from the user. Like a message box, it is 
// formatted by the operating system according to parameters you set. 
// However, a task dialog has many more features than a message box.

// NOTE:  Task Dialogs are only supported on Windows Vista and above.


#ifndef _TASKDIALOG_H_
#define _TASKDIALOG_H_

#include "wincore.h"

namespace Win32xx
{

	class CTaskDialog : public CWnd
	{
	public:
		CTaskDialog();
		virtual ~CTaskDialog() {}

		void AddButton(int nButtonID, LPCTSTR strCaption);
		void AddRadioButton(int nRadioButtonID, LPCTSTR strCaption);
		void AddRadioButtonGroup(int nIDRadioButtonsFirst, int nIDRadioButtonsLast);
		void ClickButton(int nButtonID) const;
		void ClickRadioButton(int nRadioButtonID) const;
		LRESULT DoModal(CWnd* pParent = NULL);
		void ElevateButton(int nButtonID, BOOL bElevated);
		void EnableButton(int nButtonID, BOOL bEnabled);
		void EnableRadioButton(int nButtonID, BOOL bEnabled);
		TASKDIALOGCONFIG GetConfig() const;
		TASKDIALOG_FLAGS GetOptions() const;
		int GetSelectedButtonID() const;
		int GetSelectedRadioButtonID() const;
		BOOL GetVerificationCheckboxState() const;
		BOOL IsSupported();
		void NavigateTo(CTaskDialog& TaskDialog) const;
		void RemoveAllButtons();
		void RemoveAllRadioButtons();
		void Reset();
		void SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons);
		void SetContent(LPCWSTR pszContent);
		void SetDefaultButton(int nButtonID);
		void SetDefaultRadioButton(int nRadioButtonID);
		void SetDialogWidth(UINT nWidth = 0);
		void SetElementText(TASKDIALOG_ELEMENTS eElements, LPCWSTR pszText);
		void SetExpansionArea(LPCWSTR pszExpandedInfo, LPCWSTR pszExpandedLabel = _T(""), LPCWSTR pszCollapsedLabel = _T(""));
		void SetFooterIcon(HICON hFooterIcon);
		void SetFooterIcon(LPCWSTR lpszFooterIcon);
		void SetFooterText(LPCWSTR pszFooter);
		void SetMainIcon(HICON hMainIcon);
		void SetMainIcon(LPCWSTR lpszMainIcon);
		void SetMainInstruction(LPCWSTR pszMainInstruction);
		void SetOptions(TASKDIALOG_FLAGS dwFlags);
		void SetProgressBarMarquee(BOOL bEnabled = TRUE, int nMarqueeSpeed = 0);
		void SetProgressBarPosition(int nProgressPos);
		void SetProgressBarRange(int nMinRange, int nMaxRange);
		void SetProgressBarState(int nNewState = PBST_NORMAL);
		void SetVerificationCheckbox(BOOL bChecked);
		void SetVerificationCheckboxText(LPCWSTR pszVerificationText);
		void SetWindowTitle(LPCWSTR pszWindowTitle);
		static HRESULT CALLBACK StaticTaskDialogProc(HWND hWnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData);
		LRESULT TaskDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
		void UpdateElementText(TASKDIALOG_ELEMENTS eElement, LPCWSTR pszNewText);

	protected:
		// Override these functions as required
		virtual BOOL OnTDButtonClicked(int nButtonID);
		virtual void OnTDConstructed();
		virtual void OnTDCreated();
		virtual void OnTDDestroyed();
		virtual void OnTDExpandButtonClicked(BOOL bExpanded);
		virtual void OnTDHelp();
		virtual void OnTDHyperlinkClicked(LPCWSTR pszHref);
		virtual void OnTDNavigatePage();
		virtual BOOL OnTDRadioButtonClicked(int nRadioButtonID);
		virtual void OnTDTimer(DWORD dwTickCount);
		virtual void OnTDVerificationCheckboxClicked(BOOL bChecked);

	private:
		CTaskDialog(const CTaskDialog&);				// Disable copy construction
		CTaskDialog& operator = (const CTaskDialog&);	// Disable assignment operator

		std::vector<TASKDIALOG_BUTTON> m_vButtons;
		std::vector<TASKDIALOG_BUTTON> m_vRadioButtons;

		TASKDIALOGCONFIG m_tc;
		int		m_SelectedButtonID;
		int		m_SelectedRadioButtonID;
		BOOL	m_VerificationCheckboxState;
	};

}


//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


namespace Win32xx
{

	inline CTaskDialog::CTaskDialog() : m_SelectedButtonID(0), m_SelectedRadioButtonID(0), m_VerificationCheckboxState(FALSE)
	{
		ZeroMemory(&m_tc, sizeof(m_tc));
		m_tc.cbSize = sizeof(m_tc);
		m_tc.pfCallback = CTaskDialog::StaticTaskDialogProc;
	}

	inline void CTaskDialog::AddButton(int nButtonID, LPCTSTR strCaption)
	// Adds a push button to the Task Dialog.
	{
		TASKDIALOG_BUTTON tdb;
		tdb.nButtonID = nButtonID;
		tdb.pszButtonText = strCaption;
		m_vButtons.push_back(tdb);
	}

	inline void CTaskDialog::AddRadioButton(int nRadioButtonID, LPCTSTR strCaption)
	// Adds a radio button to the Task Dialog.
	{
		TASKDIALOG_BUTTON tdb;
		tdb.nButtonID = nRadioButtonID;
		tdb.pszButtonText = strCaption;
		m_vRadioButtons.push_back(tdb);
	}

	inline void CTaskDialog::AddRadioButtonGroup(int nIDRadioButtonsFirst, int nIDRadioButtonsLast)
	// Adds a range of radio buttons to the Task Dialog.
	{
		assert(nIDRadioButtonsFirst > 0);
		assert(nIDRadioButtonsLast > nIDRadioButtonsFirst);

		TASKDIALOG_BUTTON tdb;
		for (int nID = nIDRadioButtonsFirst; nID <= nIDRadioButtonsLast; ++nID)
		{
			tdb.nButtonID = nID;
			tdb.pszButtonText = MAKEINTRESOURCE(nID);
			m_vRadioButtons.push_back(tdb);			
		}
	}

	inline void CTaskDialog::ClickButton(int nButtonID) const
	// Simulates the action of a button click in the Task Dialog.
	{
		assert(m_hWnd);
		SendMessage(TDM_CLICK_BUTTON, (WPARAM)nButtonID, 0); 
	}

	inline void CTaskDialog::ClickRadioButton(int nRadioButtonID) const
	// Simulates the action of a radio button click in the TaskDialog.
	{
		assert(m_hWnd);
		SendMessage(TDM_CLICK_RADIO_BUTTON, (WPARAM)nRadioButtonID, 0);
	}

	inline LRESULT CTaskDialog::DoModal(CWnd* pParent /* = NULL */)
	// Creates and displays the Task Dialog.
	{
		m_tc.cbSize = sizeof(m_tc);
		m_tc.pButtons = m_vButtons.empty()? NULL : &m_vButtons.front();
		m_tc.cButtons = m_vButtons.size();
		m_tc.pRadioButtons = m_vRadioButtons.empty()? NULL : &m_vRadioButtons.front();
		m_tc.cRadioButtons = m_vRadioButtons.size();
		m_tc.hwndParent = pParent? pParent->GetHwnd() : NULL;

		// Ensure this thread has the TLS index set
		TLSData* pTLSData = GetApp()->SetTlsIndex();

		// Store the CWnd pointer in thread local storage
		pTLSData->pCWnd = this;

		// Declare a pointer to the TaskDialogIndirect function
		HMODULE hComCtl = ::GetModuleHandle(L"COMCTL32.DLL");
		assert(hComCtl);
		typedef HRESULT WINAPI TASKDIALOGINDIRECT(const TASKDIALOGCONFIG*, int*, int*, BOOL*);
		TASKDIALOGINDIRECT* pTaskDialogIndirect = (TASKDIALOGINDIRECT*)::GetProcAddress(hComCtl, "TaskDialogIndirect");

		// Call TaskDialogIndirect through our function pointer
		LRESULT lr = (*pTaskDialogIndirect)(&m_tc, &m_SelectedButtonID, &m_SelectedRadioButtonID, &m_VerificationCheckboxState);

		return lr;
	}
	
	inline void CTaskDialog::ElevateButton(int nButtonID, BOOL bElevated)
	// Adds a shield icon to indicate that the button's action requires elevated privilages. 
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_BUTTON_ELEVATION_REQUIRED_STATE, (WPARAM)nButtonID, (LPARAM)bElevated);
	}
	
	inline void CTaskDialog::EnableButton(int nButtonID, BOOL bEnabled)
	// Enables or disables a push button in the TaskDialog.
	{
		assert(m_hWnd);
		SendMessage(TDM_ENABLE_BUTTON, (WPARAM)nButtonID, (LPARAM)bEnabled);
	}
	inline void CTaskDialog::EnableRadioButton(int nRadioButtonID, BOOL bEnabled)
	// Enables or disables a radio button in the TaskDialog.
	{
		assert(m_hWnd);
		SendMessage(TDM_ENABLE_RADIO_BUTTON, (WPARAM)nRadioButtonID, (LPARAM)bEnabled);
	}

	inline TASKDIALOGCONFIG CTaskDialog::GetConfig() const
	// Returns the TASKDIALOGCONFIG structure for the Task Dialog.
	{
		return m_tc;
	}

	inline TASKDIALOG_FLAGS CTaskDialog::GetOptions() const
	// Returns the Task Dialog's options. These are a combination of:
	//  TDF_ENABLE_HYPERLINKS
	//  TDF_USE_HICON_MAIN
	//  TDF_USE_HICON_FOOTER
	//  TDF_ALLOW_DIALOG_CANCELLATION
	//  TDF_USE_COMMAND_LINKS
	//  TDF_USE_COMMAND_LINKS_NO_ICON
	//  TDF_EXPAND_FOOTER_AREA
	//  TDF_EXPANDED_BY_DEFAULT
	//  TDF_VERIFICATION_FLAG_CHECKED
	//  TDF_SHOW_PROGRESS_BAR
	//  TDF_SHOW_MARQUEE_PROGRESS_BAR
	//  TDF_CALLBACK_TIMER
	//  TDF_POSITION_RELATIVE_TO_WINDOW
	//  TDF_RTL_LAYOUT
	//  TDF_NO_DEFAULT_RADIO_BUTTON
	//  TDF_CAN_BE_MINIMIZED
	{
		return m_tc.dwFlags;
	}

	inline int CTaskDialog::GetSelectedButtonID() const
	// Returns the ID of the selected button. 
	{
		return m_SelectedButtonID;
	}

	inline int CTaskDialog::GetSelectedRadioButtonID() const 
	// Returns the ID of the selected radio button.
	{ 
		return m_SelectedRadioButtonID; 
	}
		
	inline BOOL CTaskDialog::GetVerificationCheckboxState() const
	// Returns the state of the verification check box.
	{ 
		return m_VerificationCheckboxState;
	}

	BOOL CTaskDialog::IsSupported()
	// Returns true if TaskDialogs are supported on this system.
	{
		HMODULE hModule = ::GetModuleHandle(L"COMCTL32.DLL");
		assert(hModule);
		
		return (BOOL)::GetProcAddress(hModule, "TaskDialogIndirect");
	}

	inline void CTaskDialog::NavigateTo(CTaskDialog& TaskDialog) const
	// Replaces the information displayed by the task dialog.
	{
		assert(m_hWnd);
		TASKDIALOGCONFIG tc = TaskDialog.GetConfig();
		SendMessage(TDM_NAVIGATE_PAGE, 0, (LPARAM)&tc);
	}

	inline BOOL CTaskDialog::OnTDButtonClicked(int nButtonID)
	// Called when the user selects a button or command link.
	{ 
		UNREFERENCED_PARAMETER(nButtonID);
		return TRUE;
	}

	inline void CTaskDialog::OnTDConstructed()
	// Called when the task dialog is constructed, before it is displayed.
	{}

	inline void CTaskDialog::OnTDCreated()
	// Called when the task dialog is displayed.
	{}
	
	inline void CTaskDialog::OnTDDestroyed()
	// Called when the task dialog is destroyed.
	{
	}
	
	inline void CTaskDialog::OnTDExpandButtonClicked(BOOL bExpanded)
	// Called when the expand button is clicked.
	{
		UNREFERENCED_PARAMETER(bExpanded);
	}
	
	inline void CTaskDialog::OnTDHelp()
	// Called when the user presses F1 on the keyboard.
	{}

	inline void CTaskDialog::OnTDHyperlinkClicked(LPCWSTR pszHref)
	// Called when the user clicks on a hyperlink.
	{
		UNREFERENCED_PARAMETER(pszHref);
	}
	
	inline void CTaskDialog::OnTDNavigatePage()
	// Called when a navigation has occurred.
	{}
	
	inline BOOL CTaskDialog::OnTDRadioButtonClicked(int nRadioButtonID)
	// Called when the user selects a radio button.
	{
		UNREFERENCED_PARAMETER(nRadioButtonID);
		return TRUE; 
	}
	
	inline void CTaskDialog::OnTDTimer(DWORD dwTickCount) 
	// Called every 200 milliseconds (aproximately) when the TDF_CALLBACK_TIMER flag is set. 
	{
		UNREFERENCED_PARAMETER(dwTickCount);
	}
	
	inline void CTaskDialog::OnTDVerificationCheckboxClicked(BOOL bChecked)
	// Called when the user clicks the Task Dialog verification check box.
	{
		UNREFERENCED_PARAMETER(bChecked);
	}

	inline void CTaskDialog::RemoveAllButtons()
	{
		m_vButtons.clear();
	}

	inline void CTaskDialog::RemoveAllRadioButtons()
	{
		m_vRadioButtons.clear();
	}

	inline void CTaskDialog::Reset()
	// Returns the dialog to its default state.
	{
		assert (m_hWnd == NULL);

		RemoveAllButtons();
		RemoveAllRadioButtons();
		ZeroMemory(&m_tc, sizeof(m_tc));
		m_tc.cbSize = sizeof(m_tc);
		m_tc.pfCallback = CTaskDialog::StaticTaskDialogProc;

		m_SelectedButtonID = 0;
		m_SelectedRadioButtonID = 0;
		m_VerificationCheckboxState = FALSE;
	}

	inline void CTaskDialog::SetCommonButtons(TASKDIALOG_COMMON_BUTTON_FLAGS dwCommonButtons)
	// The dwCommonButtons parameter can be a combination of:
	//	TDCBF_OK_BUTTON			OK button
	//	TDCBF_YES_BUTTON		Yes button	
	//	TDCBF_NO_BUTTON			No button
	//	TDCBF_CANCEL_BUTTON		Cancel button
	//	TDCBF_RETRY_BUTTON		Retry button
	//	TDCBF_CLOSE_BUTTON		Close button
	{
		m_tc.dwCommonButtons = dwCommonButtons; 
	}

	inline void CTaskDialog::SetContent(LPCWSTR pszContent)
	// Set the task dialog's primary content.
	{
		m_tc.pszContent = pszContent; 
	}

	inline void CTaskDialog::SetDefaultButton(int nButtonID) 
	// Sets the task dialog's default button.
	// Can be either a button ID or one of the common buttons
	{
		m_tc.nDefaultButton = nButtonID;
	}

	inline void CTaskDialog::SetDefaultRadioButton(int nRadioButtonID) 
	// Sets the default radio button
	{
		m_tc.nDefaultRadioButton = nRadioButtonID;
	}

	inline void CTaskDialog::SetDialogWidth(UINT nWidth /*= 0*/)
	// The width of the task dialog's client area. If 0, the 
	// task dialog manager will calculate the ideal width.
	{
		m_tc.cxWidth = nWidth;
	}

	inline void CTaskDialog::SetElementText(TASKDIALOG_ELEMENTS eElements, LPCWSTR pszText)
	// Sets the text of one of the Task Dialog's elements.
	// eElement must be one of these predefined values:
	//  TDE_CONTENT							Content.
	//  TDE_EXPANDED_INFORMATION			Expanded information.
	//  TDE_FOOTER							Footer text.
	//  TDE_MAIN_INSTRUCTION				Main instruction.
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_ELEMENT_TEXT, (WPARAM)eElements, (LPARAM)pszText);
	}

	inline void CTaskDialog::SetExpansionArea(LPCWSTR pszExpandedInfo, LPCWSTR pszExpandedLabel /* = _T("")*/, LPCWSTR pszCollapsedLabel /* = _T("")*/)
	// Sets the text in the exandable area of the Task Dialog.
	{
		m_tc.pszExpandedInformation = pszExpandedInfo;
		m_tc.pszExpandedControlText = pszExpandedLabel;
		m_tc.pszCollapsedControlText = pszCollapsedLabel;
	}

	inline void CTaskDialog::SetFooterIcon(HICON hFooterIcon) 
	// Sets the icon that will be displayed in the Task Dialog's footer.
	{
		m_tc.hFooterIcon = hFooterIcon;
	}

	inline void CTaskDialog::SetFooterIcon(LPCWSTR lpszFooterIcon) 
	// Sets the icon that will be displayed in the Task Dialog's footer.
	{
		m_tc.pszFooterIcon = lpszFooterIcon;
	}

	inline void CTaskDialog::SetFooterText(LPCWSTR pszFooter)
	// Sets the text that will be displayed in the Task Dialog's footer.
	{
		m_tc.pszFooter = pszFooter;
	}

	inline void CTaskDialog::SetMainIcon(HICON hMainIcon) 
	// Sets Task Dialog's main icon.
	{
		m_tc.hMainIcon = hMainIcon;
	}

	inline void CTaskDialog::SetMainIcon(LPCWSTR lpszMainIcon)
	// Sets Task Dialog's main icon.
	{
		m_tc.pszMainIcon = lpszMainIcon; 
	}

	inline void CTaskDialog::SetMainInstruction(LPCWSTR pszMainInstruction) 
	// Sets the Task Dialog's main instruction text.
	{
		m_tc.pszMainInstruction = pszMainInstruction;
	}

	inline void CTaskDialog::SetOptions(TASKDIALOG_FLAGS dwFlags)
	// Sets the Task Dialog's options. These are a combination of:
	//  TDF_ENABLE_HYPERLINKS
	//  TDF_USE_HICON_MAIN
	//  TDF_USE_HICON_FOOTER
	//  TDF_ALLOW_DIALOG_CANCELLATION
	//  TDF_USE_COMMAND_LINKS
	//  TDF_USE_COMMAND_LINKS_NO_ICON
	//  TDF_EXPAND_FOOTER_AREA
	//  TDF_EXPANDED_BY_DEFAULT
	//  TDF_VERIFICATION_FLAG_CHECKED
	//  TDF_SHOW_PROGRESS_BAR
	//  TDF_SHOW_MARQUEE_PROGRESS_BAR
	//  TDF_CALLBACK_TIMER
	//  TDF_POSITION_RELATIVE_TO_WINDOW
	//  TDF_RTL_LAYOUT
	//  TDF_NO_DEFAULT_RADIO_BUTTON
	//  TDF_CAN_BE_MINIMIZED
	{
		m_tc.dwFlags = dwFlags;
	}

	inline void CTaskDialog::SetProgressBarMarquee(BOOL bEnabled /* = TRUE*/, int nMarqueeSpeed /* = 0*/) 
	// Starts and stops the marquee display of the progress bar, and sets the speed of the marquee.
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_MARQUEE, (WPARAM)bEnabled, (LPARAM)nMarqueeSpeed);
	}

	inline void CTaskDialog::SetProgressBarPosition(int nProgressPos) 
	// Sets the current position for a progress bar.
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_POS, (WPARAM)nProgressPos, 0);
	}

	inline void CTaskDialog::SetProgressBarRange(int nMinRange, int nMaxRange) 
	// Sets the minimum and maximum values for the hosted progress bar.
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_RANGE, 0, MAKELPARAM(nMinRange, nMaxRange)); 
	}

	inline void CTaskDialog::SetProgressBarState(int nNewState /* = PBST_NORMAL*/)
	// Set the current state of the progress bar. Possible states are:
	//  PBST_NORMAL
	//  PBST_PAUSE
	//  PBST_ERROR
	{
		assert(m_hWnd);
		SendMessage(TDM_SET_PROGRESS_BAR_STATE, (WPARAM)nNewState, 0);
	} 

	inline void CTaskDialog::SetVerificationCheckbox(BOOL bChecked)
	// Simulates a click on the verification checkbox of the Task Dialog, if it exists.
	{
		assert(m_hWnd);
		SendMessage(TDM_CLICK_VERIFICATION, (WPARAM)bChecked, (LPARAM)bChecked);
	}

	inline void CTaskDialog::SetVerificationCheckboxText(LPCWSTR pszVerificationText)
	// Sets the text for the verification check box.
	{
		m_tc.pszVerificationText = pszVerificationText;
	}

	inline void CTaskDialog::SetWindowTitle(LPCWSTR pszWindowTitle)
	// Sets the Task Dialog's window title.
	{
		m_tc.pszWindowTitle = pszWindowTitle; 
	}

	inline HRESULT CALLBACK CTaskDialog::StaticTaskDialogProc(HWND hWnd, UINT uNotification, WPARAM wParam, LPARAM lParam, LONG_PTR dwRefData)
	// TaskDialogs direct their messages here.
	{
		UNREFERENCED_PARAMETER(dwRefData);

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

	inline LRESULT CTaskDialog::TaskDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
	// Handles the Task Dialog's notificaions.
	{
		switch(uMsg)
		{
		case TDN_BUTTON_CLICKED:
			OnTDButtonClicked((int)wParam);
			break;
		case TDN_CREATED:
			OnTDCreated();
			break;
		case TDN_DESTROYED:		
			Destroy();	// Prepare this CWnd to be reused. 
			OnTDDestroyed();
			break;
		case TDN_DIALOG_CONSTRUCTED:
			OnTDConstructed();
			break;
		case TDN_EXPANDO_BUTTON_CLICKED:
			OnTDExpandButtonClicked((BOOL)wParam);
			break;
		case TDN_HELP:
			OnTDHelp();
			break;
		case TDN_HYPERLINK_CLICKED:
			OnTDHyperlinkClicked((LPCWSTR)lParam);
			break;
		case TDN_NAVIGATED:
			OnTDNavigatePage();
			break;
		case TDN_RADIO_BUTTON_CLICKED:
			OnTDRadioButtonClicked((int)wParam);
			break;
		case TDN_TIMER:
			OnTDTimer((DWORD)wParam);
			break;
		case TDN_VERIFICATION_CLICKED:
			OnTDVerificationCheckboxClicked((BOOL)wParam);
			break;
		}

		return S_OK;
	}

	inline void CTaskDialog::UpdateElementText(TASKDIALOG_ELEMENTS eElement, LPCWSTR pszNewText)
	// Updates a text element on the Task Dialog.
	{
		assert(m_hWnd);
		SendMessage(TDM_UPDATE_ELEMENT_TEXT, (WPARAM)eElement, (LPARAM)pszNewText);
	}

}



#endif // _TASKDIALOG_H_