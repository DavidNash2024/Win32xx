///////////////////////////////////////
// MyDialog.cpp

#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent)
{
	m_hInstRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
    if (!m_hInstRichEdit)
 		::MessageBox(NULL, _T("CMyDialog::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
}

CMyDialog::~CMyDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

HWND CMyDialog::Create(HWND hWndParent = 0)
{
	SetParent(hWndParent);
	return DoModeless();
}

BOOL CMyDialog::DialogProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(hWnd, uMsg, wParam, lParam);
}

BOOL CMyDialog::OnCommand(WPARAM wParam, LPARAM /*lParam*/)
{
	switch (LOWORD(wParam))
    {
	case IDC_BUTTON1:
		OnButton();
		return TRUE;
	case IDC_RADIO1:
		OnRadio1();
		return TRUE;
	case IDC_RADIO2:
		OnRadio2();
		return TRUE;
	case IDC_RADIO3:
		OnRadio3();
		return TRUE;
	case IDC_CHECK1:
		OnCheck1();
		return TRUE;
	case IDC_CHECK2:
		OnCheck2();
		return TRUE;
	case IDC_CHECK3:
		OnCheck3();
		return TRUE;
    } //switch (nID)

	return FALSE;
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Put some text in the edit boxes
	::SetDlgItemText(GetHwnd(), IDC_EDIT1, _T("Edit Control"));
	::SetDlgItemText(GetHwnd(), IDC_RICHEDIT1, _T("Rich Edit Window"));

	// Put some text in the list box
	HWND hListBox = ::GetDlgItem(GetHwnd(), IDC_LIST1);
	for (int i = 0 ; i < 8 ; i++)
		::SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM) _T("List Box"));

	return true;
}

void CMyDialog::OnOK()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("OK Button Pressed."));
	TRACE(_T("OK Button Pressed."));
}

void CMyDialog::OnButton()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Button Pressed"));
	TRACE(_T("Button Pressed"));
}

void CMyDialog::OnCheck1()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Check Box 1"));
	TRACE(_T("Check Box 1"));
}

void CMyDialog::OnCheck2()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Check Box 2"));
	TRACE(_T("Check Box 2"));
}

void CMyDialog::OnCheck3()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Check Box 3"));
	TRACE(_T("Check Box 3"));
}

void CMyDialog::OnRadio1()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Radio 1"));
	TRACE(_T("Radio 1"));
}

void CMyDialog::OnRadio2()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Radio 2"));
	TRACE(_T("Radio 2"));
}

void CMyDialog::OnRadio3()
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, _T("Radio 3"));
	TRACE(_T("Radio 3"));
}

