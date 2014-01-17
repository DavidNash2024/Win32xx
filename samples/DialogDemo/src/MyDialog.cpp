///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "Hyperlink.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
{
	m_hInstRichEdit = SafeLoadSystemLibrary(_T("RICHED32.DLL"));
    if (!m_hInstRichEdit)
 		::MessageBox(NULL,_T("CMyDialog::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
}

CMyDialog::CMyDialog(LPCTSTR lpszResName, CWnd* pParent)
	: CDialog(lpszResName, pParent)
{
	m_hInstRichEdit = SafeLoadSystemLibrary(_T("RICHED32.DLL"));
	if (!m_hInstRichEdit)
		::MessageBox(NULL, _T("CMyDialog::CMyDialog  Failed to load RICHED32.DLL"), _T("Error"), MB_ICONWARNING);
}

CMyDialog::~CMyDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
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
	SetDlgItemText(IDC_EDIT1, _T("Edit Control"));
	SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

	// Put some text in the list box
	AttachItem(IDC_LIST1, m_ListBox);
	for (int i = 0 ; i < 8 ; i++)
		m_ListBox.AddString(_T("List Box"));

	// Turn our button into a MyButton object
	AttachItem(IDC_BUTTON2, m_Button);

	// Turn our static control into a hyperlink
	AttachItem(IDC_STATIC4, m_Hyperlink);

	return TRUE;
}

void CMyDialog::OnOK()
{
	MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::OnButton()
{
	SetStatic(_T("Button Pressed"));
	TRACE("Button Pressed\n");
}

void CMyDialog::OnCheck1()
{
	SetStatic(_T("Check Box 1"));
	TRACE("Check Box 1\n");
}

void CMyDialog::OnCheck2()
{
	SetStatic(_T("Check Box 2"));
	TRACE("Check Box 2\n");
}

void CMyDialog::OnCheck3()
{
	SetStatic(_T("Check Box 3"));
	TRACE("Check Box 3\n");
}

void CMyDialog::OnRadio1()
{
	SetStatic(_T("Radio 1"));
	TRACE("Radio 1\n");
}

void CMyDialog::OnRadio2()
{
	SetStatic(_T("Radio 2"));
	TRACE("Radio 2\n");
}

void CMyDialog::OnRadio3()
{
	SetStatic(_T("Radio 3"));
	TRACE("Radio 3\n");
}

void CMyDialog::SetStatic(LPCTSTR szString)
{
	SetDlgItemText(IDC_STATIC3, szString);
}

