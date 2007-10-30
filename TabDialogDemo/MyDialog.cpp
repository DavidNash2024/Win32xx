///////////////////////////////////////
// MyDialog.cpp

#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent), m_ButtonDialog(IDD_BUTTONS), m_ComboBoxDialog(IDD_COMBOBOXES)
{
}

CMyDialog::~CMyDialog()
{
}

BOOL CMyDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

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

