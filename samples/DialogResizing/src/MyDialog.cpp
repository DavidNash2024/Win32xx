///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"


// Definitions for the CMyDialog class
CMyDialog::CMyDialog(UINT nResID) : CDialog(nResID)
{
	m_hInstRichEdit = LoadLibrary(_T("RICHED32.DLL"));
    if (m_hInstRichEdit == 0)
 		::MessageBox(NULL, _T("CMyDialog::CRichView  Failed to load RICHED32.DLL"), _T(""), MB_ICONWARNING);
}

CMyDialog::~CMyDialog()
{
	::FreeLibrary(m_hInstRichEdit);
}

INT_PTR CMyDialog::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// Pass resizing messages on to the resizer
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
		//Additional messages to be handled go here
//	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

BOOL CMyDialog::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);

	switch (LOWORD(wParam))
    {
	case IDC_BUTTON1:	OnButton();		return TRUE;
	case IDC_RADIO1:	OnRadio1();		return TRUE;
	case IDC_RADIO2:	OnRadio2();		return TRUE;
	case IDC_RADIO3:	OnRadio3();		return TRUE;
	case IDC_CHECK1:	OnCheck1();		return TRUE;
	case IDC_CHECK2:	OnCheck2();		return TRUE;
	case IDC_CHECK3:	OnCheck3();		return TRUE;
    }

	return FALSE;
}

void CMyDialog::OnDestroy()
{
	// End the application
	::PostQuitMessage(0);
}

BOOL CMyDialog::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

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
	m_Resizer.Initialize( this, CRect(0, 0, 300, 200) ); 
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

void CMyDialog::OnOK()
{
	MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
	CDialog::OnOK();
}

void CMyDialog::OnButton()
{
	SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
	TRACE("Button Pressed\n");
}

void CMyDialog::OnCheck1()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 1"));
	TRACE("Check Box 1\n");
}

void CMyDialog::OnCheck2()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 2"));
	TRACE("Check Box 2\n");
}

void CMyDialog::OnCheck3()
{
	SetDlgItemText(IDC_STATIC3, _T("Check Box 3"));
	TRACE("Check Box 3\n");
}

void CMyDialog::OnRadio1()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 1"));
	TRACE("Radio 1\n");
}

void CMyDialog::OnRadio2()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 2"));
	TRACE("Radio 2\n");
}

void CMyDialog::OnRadio3()
{
	SetDlgItemText(IDC_STATIC3, _T("Radio 3"));
	TRACE("Radio 3\n");
}

BOOL CMyDialog::OnEraseBkgnd(CDC& dc)
{
	// Adding a gripper to a resizable dialog is a bit of a hack, but since it
	// is often done, here is one method of doing it safely.

	UNREFERENCED_PARAMETER(dc);

	// Draw the dialog's background manually
	CRect rc = GetClientRect();
	CClientDC dcClient(*this);	
	dcClient.SolidFill(GetSysColor(COLOR_3DFACE), rc);

	// draw size grip
	if (rc.Width() > m_Resizer.GetMinRect().Width() && rc.Height() > m_Resizer.GetMinRect().Height())
	{
		int size = GetSystemMetrics(SM_CXVSCROLL);
		rc.left = rc.right - size;
		rc.top = rc.bottom - size;
		dcClient.DrawFrameControl(rc, DFC_SCROLL, DFCS_SCROLLSIZEGRIP);
	}
	
	// Suppress default background drawing
	return TRUE;
}