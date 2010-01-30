///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "Hyperlink.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent)
{
	m_hInstRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
    if (!m_hInstRichEdit)
 		::MessageBox(NULL,_T("CMyDialog::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
}

CMyDialog::CMyDialog(LPCTSTR lpszResName, HWND hWndParent)
	: CDialog(lpszResName, hWndParent)
{
	m_hInstRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
	if (!m_hInstRichEdit)
		::MessageBox(NULL, _T("CMyDialog::CMyDialog  Failed to load RICHED32.DLL"), _T("Error"), MB_ICONWARNING);
}

CMyDialog::~CMyDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

BOOL CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
//		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam); 
}

BOOL CMyDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

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
	for (int i = 0 ; i < 8 ; i++)
		::SendDlgItemMessage(m_hWnd, IDC_LIST1, LB_ADDSTRING, 0, (LPARAM) _T("List Box"));

	// Turn our button into a MyButton object
	m_Button.AttachDlgItem(IDC_BUTTON2, this);

	// Turn our static control into a hyperlink
	m_Hyperlink.AttachDlgItem(IDC_STATIC4, this);

	return true;
}

void CMyDialog::OnOK()
{
	::MessageBox(NULL, _T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::OnButton()
{
	SetStatic(_T("Button Pressed"));
	TRACE(_T("Button Pressed\n"));
}

void CMyDialog::OnCheck1()
{
	SetStatic(_T("Check Box 1"));
	TRACE(_T("Check Box 1\n"));
}

void CMyDialog::OnCheck2()
{
	SetStatic(_T("Check Box 2"));
	TRACE(_T("Check Box 2\n"));
}

void CMyDialog::OnCheck3()
{
	SetStatic(_T("Check Box 3"));
	TRACE(_T("Check Box 3\n"));
}

void CMyDialog::OnRadio1()
{
	SetStatic(_T("Radio 1"));
	TRACE(_T("Radio 1\n"));
}

void CMyDialog::OnRadio2()
{
	SetStatic(_T("Radio 2"));
	TRACE(_T("Radio 2\n"));
}

void CMyDialog::OnRadio3()
{
	SetStatic(_T("Radio 3"));
	TRACE(_T("Radio 3\n"));
}

void CMyDialog::SetStatic(LPCTSTR szString)
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, szString);
}

