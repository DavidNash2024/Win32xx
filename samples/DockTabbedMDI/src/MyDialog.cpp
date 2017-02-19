///////////////////////////////////////
// CViewDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"



// Definitions for the CViewDialog class
CViewDialog::CViewDialog(UINT nResID) : CDialog(nResID)
{
}

CViewDialog::~CViewDialog()
{
}

void CViewDialog::AppendText(int nID, LPCTSTR szText)
{
	// This function appends text to an edit control

	// Append Line Feed
	int ndx = (int)SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
	if (ndx)
	{
		SendDlgItemMessage(nID, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
		SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM)(_T("\r\n")));
	}

	// Append text
	ndx = (int)SendDlgItemMessage(nID, WM_GETTEXTLENGTH, 0, 0);
	SendDlgItemMessage(nID, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
	SendDlgItemMessage(nID, EM_REPLACESEL, 0, (LPARAM)szText);
}

INT_PTR CViewDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

	switch (uMsg)
	{
	case WM_MOUSEACTIVATE:		return OnMouseActivate(uMsg, wParam, lParam);
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CViewDialog::OnCancel()
{
	AppendText(IDC_RICHEDIT2, _T("Cancel Pressed"));
	TRACE("Cancel Pressed\n");
}

void CViewDialog::OnClose()
{
	// Suppress the WM_CLOSE message that is sent by pressing an Esc key in the RichEdit controls.
}

void CViewDialog::OnRadioButton(UINT nButton)
{
	SendDlgItemMessage(IDC_RADIO1, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(IDC_RADIO2, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(IDC_RADIO3, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(IDC_RADIO1 + nButton, BM_SETCHECK, TRUE, 0);

	CString str;
	str.Format(_T("Radio%d"), nButton+1);
	AppendText(IDC_RICHEDIT2, str);
	TRACE(str); TRACE("\n");
}

BOOL CViewDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	UINT nID = LOWORD(wParam);
	switch (nID)
    {
	case IDC_BUTTON1:	OnButton();		return TRUE;

	case IDC_RADIO1:	// intentionally blank
	case IDC_RADIO2:
	case IDC_RADIO3:	OnRadioButton(nID - IDC_RADIO1);	return TRUE;

	case IDC_CHECK1:	OnCheck1();		return TRUE;
	case IDC_CHECK2:	OnCheck2();		return TRUE;
	case IDC_CHECK3:	OnCheck3();		return TRUE;
    }

	return FALSE;
}

BOOL CViewDialog::OnInitDialog()
{
	// Attach CWnd objects to the dialog items
	AttachItem(IDC_BUTTON1,	m_Button);
	AttachItem(IDC_CHECK1,	m_CheckA);
	AttachItem(IDC_CHECK2,	m_CheckB);
	AttachItem(IDC_CHECK3,	m_CheckC);
	AttachItem(IDC_RADIO1,	m_RadioA);
	AttachItem(IDC_RADIO2,	m_RadioB);
	AttachItem(IDC_RADIO3,	m_RadioC);
	AttachItem(IDC_RICHEDIT1, m_RichEdit1);
	AttachItem(IDC_RICHEDIT2, m_RichEdit2);

	// Put some text in the edit boxes
	m_RichEdit1.SetWindowText(_T("Rich Edit Window"));
	m_RichEdit2.SetWindowText(_T("Rich Edit Window"));

	// Initialize dialog resizing
	m_Resizer.Initialize( *this, CRect(0, 0, 300, 200) ); 
	m_Resizer.AddChild(m_RadioA,   topleft, 0);
	m_Resizer.AddChild(m_RadioB,   topleft, 0);
	m_Resizer.AddChild(m_RadioC,   topleft, 0);
	m_Resizer.AddChild(m_Button,   topleft, 0);
	m_Resizer.AddChild(m_CheckA,   bottomright, 0);
	m_Resizer.AddChild(m_CheckB,   bottomright, 0);
	m_Resizer.AddChild(m_CheckC,   bottomright, 0);
	m_Resizer.AddChild(m_RichEdit1, topright, RD_STRETCH_WIDTH);
	m_Resizer.AddChild(m_RichEdit2, bottomleft, RD_STRETCH_WIDTH| RD_STRETCH_HEIGHT);
	
	return true;
}

LRESULT CViewDialog::OnMouseActivate(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (!IsChild(::GetFocus()))
		SetFocus();

	return FinalWindowProc(uMsg, wParam, lParam);
}

void CViewDialog::OnOK()
{
	AppendText(IDC_RICHEDIT2, _T("OK Button Pressed"));
	TRACE("OK Button Pressed\n");
}

void CViewDialog::OnButton()
{
	AppendText(IDC_RICHEDIT2, _T("Button Pressed"));
	TRACE("Button Pressed\n");
}

void CViewDialog::OnCheck1()
{
	AppendText(IDC_RICHEDIT2, _T("Check Box 1"));
	TRACE("Check Box 1\n");
}

void CViewDialog::OnCheck2()
{
	AppendText(IDC_RICHEDIT2, _T("Check Box 2"));
	TRACE("Check Box 2\n");
}

void CViewDialog::OnCheck3()
{
	AppendText(IDC_RICHEDIT2, _T("Check Box 3"));
	TRACE("Check Box 3\n");
}


//////////////////////////////////////////////
//  Definitions for the CContainDialog class
CContainDialog::CContainDialog() : m_ViewDialog(IDD_MYDIALOG)
{
	SetView(m_ViewDialog); 
	SetDockCaption (_T("Dialog View - Docking container"));
	SetTabText(_T("Dialog"));
	SetTabIcon(IDI_DIALOGVIEW);
} 


//////////////////////////////////////////////
//  Definitions for the CDockDialog class
CDockDialog::CDockDialog()
{
	// Set the view window to our edit control
	SetView(m_View);

	// Set the width of the splitter bar
	SetBarWidth(8);
}


