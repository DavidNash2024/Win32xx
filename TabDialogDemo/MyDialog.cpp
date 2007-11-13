///////////////////////////////////////
// MyDialog.cpp

#include "MyDialog.h"
#include "resource.h"


/////////////////////////////////////////////
// Definitions for the CButtonDialog class
//
CButtonDialog::CButtonDialog(UINT nResID, HWND hWnd) : CDialog(nResID, hWnd) 
{
	m_hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}

CButtonDialog::~CButtonDialog() 
{
	::DeleteObject(m_hBrush);
}

BOOL CButtonDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Set the background color of the dialog
	case WM_CTLCOLORDLG:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	// Set the background color of static controls
	case WM_CTLCOLORSTATIC:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	}
	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

BOOL CButtonDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON1:
		TRACE(_T("Push Button Pressed"));
		return TRUE;
	case IDC_RADIO1:
		TRACE(_T("Radio Button 1"));
		return TRUE;
	case IDC_RADIO2:
		TRACE(_T("Radio Button 2"));
		return TRUE;
	case IDC_RADIO3:
		TRACE(_T("Radio Button 3"));
		return TRUE;
	case IDC_CHECK1:
		TRACE(_T("Check Box 1"));
		return TRUE;
	case IDC_CHECK2:
		TRACE(_T("Check Box 2"));
		return TRUE;
	case IDC_CHECK3:
		TRACE(_T("Check Box 3"));
		return TRUE;
    } //switch (LOWORD(wParam))

	return FALSE;
}

/////////////////////////////////////////////
// Definitions for the CComboBoxDialog class
//
CComboBoxDialog::CComboBoxDialog(UINT nResID, HWND hWnd) : CDialog(nResID, hWnd) 
{
	m_hBrush = ::CreateSolidBrush(RGB(255, 255, 255));
}

CComboBoxDialog::~CComboBoxDialog() 
{
	::DeleteObject(m_hBrush);
}

BOOL CComboBoxDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	// Set the background color of the dialog
	case WM_CTLCOLORDLG:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	// Set the background color of static controls
	case WM_CTLCOLORSTATIC:
		if (IsXPThemed()) return (INT_PTR)m_hBrush;
		break;
	
	}
	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

BOOL CComboBoxDialog::OnInitDialog()
{
	// Put some text in the Combo Boxes
	for (int i = 0 ; i < 6 ; i++)
	{
		::SendMessage( ::GetDlgItem(GetHwnd(), IDC_COMBO1), CB_ADDSTRING, 0, (LPARAM) _T("C Box 1"));
		::SendMessage( ::GetDlgItem(GetHwnd(), IDC_COMBO2), CB_ADDSTRING, 0, (LPARAM) _T("C Box 2"));
		::SendMessage( ::GetDlgItem(GetHwnd(), IDC_COMBO3), CB_ADDSTRING, 0, (LPARAM) _T("C Box 3"));
	}

	return TRUE;
}

/////////////////////////////////////////
// Definitions for the CMyDialog class
//
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent), m_ButtonDialog(IDD_BUTTONS), m_ComboBoxDialog(IDD_COMBOBOXES)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CLOSE:
		// close any modeless dialogs first
		m_ButtonDialog.DestroyWindow();
		m_ComboBoxDialog.DestroyWindow();
		break;
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	TCITEM tie;
	TCHAR szText[256];
	HWND hwndTab = GetDlgItem(m_hWnd, IDC_TAB1);
	
	// Add tabs
	tie.mask = TCIF_TEXT | TCIF_IMAGE;
	tie.iImage = -1;
	tie.pszText = szText;

	lstrcpy(szText, _T("Button Dialog"));
	TabCtrl_InsertItem(hwndTab, 0, &tie);
	lstrcpy(szText, _T("ComboBox Dialog"));
	TabCtrl_InsertItem(hwndTab, 1, &tie);

	// Display button dialog
	ShowButtonDialog();

	return true;
}

LRESULT CMyDialog::OnNotify(WPARAM /*wParam*/, LPARAM lParam)
{
	switch(((LPNMHDR) lParam)->code)
	{
		// Tab Control Selection change notification
		case TCN_SELCHANGE: 
		{
			HWND hwndTab = ((LPNMHDR)lParam)->hwndFrom;
			int iPage = TabCtrl_GetCurSel(hwndTab);

			switch (iPage)
			{
			case 0:
				ShowButtonDialog();
				break;
			case 1:
				ShowComboBoxDialog();
				break;
			}
		}
		break;
	}
	return 0L;
}

void CMyDialog::OnOK()
{
	// This is called when the Enter key is pressed

	// Do default action (i.e. close the dialog)
	CDialog::OnOK();
}

void CMyDialog::ShowButtonDialog()
{
	HWND hwndTab = GetDlgItem(m_hWnd, IDC_TAB1);
	m_ComboBoxDialog.DestroyWindow();
	
	// Create a modeless dialog
	m_ButtonDialog.Create(hwndTab);
	SetWindowPos(m_ButtonDialog.GetHwnd(), HWND_TOP, 0, 20, 0, 0, SWP_NOSIZE);
}

void CMyDialog::ShowComboBoxDialog()
{
	HWND hwndTab = GetDlgItem(m_hWnd, IDC_TAB1);
	m_ButtonDialog.DestroyWindow();
	
	// Create a modeless dialog
	m_ComboBoxDialog.Create(hwndTab);
	SetWindowPos(m_ComboBoxDialog.GetHwnd(), HWND_TOP, 0, 20, 0, 0, SWP_NOSIZE);
}

