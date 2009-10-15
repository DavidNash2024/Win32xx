///////////////////////////////////////
// SdiView.cpp

#include "stdafx.h"
#include "FormDoc.h"
#include "FormView.h"
#include "FormApp.h"
#include "resource.h"


// Definitions for the CFormView class
CFormView::CFormView(UINT nResID, HWND hParent)
	: CDialog(nResID, hParent)
{
	m_hInstRichEdit = ::LoadLibrary(_T("RICHED32.DLL"));
	if (!m_hInstRichEdit) 
	{
		::MessageBox(NULL, _T("CFormView::CRichView  Failed to load RICHED32.DLL"), 
                 _T(""), MB_ICONWARNING);
	}
}

CFormView::~CFormView()
{
	::FreeLibrary(m_hInstRichEdit);
}

HWND CFormView::Create(HWND hParent = 0)
{
	SetDlgParent(hParent);
	return DoModeless();
}

BOOL CFormView::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
//  switch (uMsg)
//  {
//  }

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CFormView::OnCancel()
{
	// Discard these messages
}

BOOL CFormView::OnCommand(WPARAM wParam, LPARAM /* lParam */)
{
  WORD wpLo = LOWORD(wParam);

	switch (wpLo)
	{
    case IDC_BUTTON1:
		OnButton();
		return TRUE;
    
	case ID_RADIO_A:
    case ID_RADIO_B:
    case ID_RADIO_C:
		OnRangeOfIds_Radio(wpLo - ID_RADIO_A);
		return TRUE;
    
	case ID_CHECK_A:
		OnCheckA();
		return TRUE;
    case ID_CHECK_B:
		OnCheckB();
		return TRUE;
    case ID_CHECK_C:
		OnCheckC();
		return TRUE;
	} 
  
	return FALSE;
}

BOOL CFormView::OnInitDialog()
{
	// Set the Icon
	SetIconLarge(IDW_MAIN);
	SetIconSmall(IDW_MAIN);

	// Put some text in the edit boxes
	SetDlgItemText(IDC_EDIT1, _T("Edit Control"));
	SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

	// Put some text in the list box
	for (int i = 0 ; i < 8 ; i++) 
	{
		SendDlgItemMessage(IDC_LIST1, LB_ADDSTRING, 0, (LPARAM) _T("List Box"));
	}

	BOOL bCheck = GetDoc().GetCheckA();
	SendDlgItemMessage(ID_CHECK_A, BM_SETCHECK, bCheck, 0);

	bCheck = GetDoc().GetCheckB();
	SendDlgItemMessage(ID_CHECK_B, BM_SETCHECK, bCheck, 0);

	bCheck = GetDoc().GetCheckC();
	SendDlgItemMessage(ID_CHECK_C, BM_SETCHECK, bCheck, 0);

	UINT curRadio = GetDoc().GetRadio();
	OnRangeOfIds_Radio(curRadio);

	return true;
}

void CFormView::OnOK()
{
	SetDlgItemText(IDC_STATIC3, _T("OK Button Pressed."));
	TRACE(_T("OK Button Pressed.\n"));
}

void CFormView::OnButton()
{
	SetDlgItemText(IDC_STATIC3, _T("Button Pressed"));
	TRACE(_T("Button Pressed\n"));
}

void CFormView::OnCheckA()
{
	TRACE(_T("Check Box A\n"));
	BOOL bCheck = GetDoc().GetCheckA();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_A, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckA(bCheck);

	SetDlgItemText(IDC_STATIC3, _T("Check Box A toggled"));
}

void CFormView::OnCheckB()
{
	TRACE(_T("Check Box B\n"));
	BOOL bCheck = GetDoc().GetCheckB();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_B, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckB(bCheck);

	SetDlgItemText(IDC_STATIC3, _T("Check Box B toggled"));
}

void CFormView::OnCheckC()
{
	TRACE(_T("Check Box C\n"));
	BOOL bCheck = GetDoc().GetCheckC();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_C, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckC(bCheck);

	SetDlgItemText(IDC_STATIC3, _T("Check Box C toggled"));
}

void CFormView::OnRangeOfIds_Radio(UINT nIdAdjust)
{
	SendDlgItemMessage(ID_RADIO_A, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_B, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_C, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_A + nIdAdjust, BM_SETCHECK, TRUE, 0);
	GetDoc().SetRadio(nIdAdjust);

	SetDlgItemText(IDC_STATIC3, _T("Radio changed"));
	TRACE(_T("Radio changed\n"));
}

