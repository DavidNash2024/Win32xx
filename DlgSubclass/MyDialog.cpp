///////////////////////////////////////
// MyDialog.cpp

#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID, HWND hWndParent)
	: CDialog(nResID, hWndParent)
{
	m_hInstRichEdit = ::LoadLibrary(TEXT("RICHED32.DLL"));
    if (!m_hInstRichEdit)
 		::MessageBox(NULL,TEXT("CMyDialog::CRichView  Failed to load RICHED32.DLL"), TEXT(""), MB_ICONWARNING);
}

CMyDialog::CMyDialog(LPCTSTR lpszResName, HWND hWndParent)
	: CDialog(lpszResName, hWndParent)
{
	m_hInstRichEdit = ::LoadLibrary(TEXT("RICHED32.DLL"));
	if (!m_hInstRichEdit)
		::MessageBox(NULL, TEXT("CMyDialog::CMyDialog  Failed to load RICHED32.DLL"), TEXT("Error"), MB_ICONWARNING);
}

CMyDialog::~CMyDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

BOOL CMyDialog::DialogProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	//Use the dialogframe default message handling for remaining messages
	return CDialog::DialogProc(hwnd, uMsg, wParam, lParam);
} 

BOOL CMyDialog::OnCommand(UINT nID)
{
	switch (nID)
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
	
	return TRUE;
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(ID_MAIN);
	SetIconSmall(ID_MAIN);

	// Put some text in the edit boxes
	::SetDlgItemText(GetHwnd(), IDC_EDIT1, TEXT("Edit Control"));
	::SetDlgItemText(GetHwnd(), IDC_RICHEDIT1, TEXT("Rich Edit Window"));

	// Put some text in the list box
	HWND hListBox = ::GetDlgItem(GetHwnd(), IDC_LIST1);
	for (int i = 0 ; i < 8 ; i++)
		::SendMessage( hListBox, LB_ADDSTRING, 0, (LPARAM) TEXT("List Box"));

	// Turn our button into a MyButton object
	MyButton.AttachDlgItem(IDC_BUTTON2, this);

	return true;
}

void CMyDialog::OnOK()
{
	::MessageBox(NULL, TEXT("OK Button Pressed.  Program will exit now."), TEXT("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::OnButton() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Button Pressed"));
	TRACE(TEXT("Button Pressed"));
}

void CMyDialog::OnCheck1() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Check Box 1"));
	TRACE(TEXT("Check Box 1"));
}

void CMyDialog::OnCheck2() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Check Box 2"));
	TRACE(TEXT("Check Box 2"));
}

void CMyDialog::OnCheck3() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Check Box 3"));
	TRACE(TEXT("Check Box 3"));
}

void CMyDialog::OnRadio1() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Radio 1"));
	TRACE(TEXT("Radio 1"));
}

void CMyDialog::OnRadio2() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Radio 2"));
	
	TRACE(TEXT("Radio 2"));
}

void CMyDialog::OnRadio3() 
{
	::SetDlgItemText(GetHwnd(), IDC_STATIC3, TEXT("Radio 3"));
	TRACE(TEXT("Radio 3"));
}