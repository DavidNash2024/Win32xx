///////////////////////////////////////
// FormView.cpp

#include "stdafx.h"
#include "FormDoc.h"
#include "FormView.h"
#include "FormApp.h"
#include "resource.h"


// Definitions for the CFormView class
CFormView::CFormView(UINT nResID, CWnd* pParent)
	: CDialog(nResID, pParent)
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

HWND CFormView::Create(CWnd* pParent = 0)
{
	SetDlgParent(pParent);
	return DoModeless();
}

INT_PTR CFormView::DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_SIZE:
		m_Resizer.RecalcLayout();

		break;	
	}

	// Pass unhandled messages on to parent DialogProc
	return DialogProcDefault(uMsg, wParam, lParam);
}

void CFormView::OnCancel()
{
	// Discard these messages
}

BOOL CFormView::OnCommand(WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
  
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

	// Attach CWnd objects to the dialog items
	AttachItem(IDC_BUTTON1,	m_Button);
	AttachItem(ID_CHECK_A,	m_CheckA);
	AttachItem(ID_CHECK_B,	m_CheckB);
	AttachItem(ID_CHECK_C,	m_CheckC);
	AttachItem(IDC_EDIT1,	m_Edit);
	AttachItem(IDC_LIST1,	m_ListBox);
	AttachItem(IDOK,		m_OK);
	AttachItem(ID_RADIO_A,	m_RadioA);
	AttachItem(ID_RADIO_B,	m_RadioB);
	AttachItem(ID_RADIO_C,	m_RadioC);
	AttachItem(IDC_RICHEDIT1, m_RichEdit);
	AttachItem(IDC_GROUP1,	m_Group);
	AttachItem(IDC_STATUS,	m_Status);
	AttachItem(IDC_BITMAP1, m_Bitmap);
	
	// Put some text in the edit boxes
	SetDlgItemText(IDC_EDIT1, _T("Edit Control"));
	SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

	// Put some text in the list box
	for (int i = 0 ; i < 8 ; i++) 
	{
		m_ListBox.SendMessage(LB_ADDSTRING, 0, (LPARAM) _T("List Box"));
	}

	// Set initial button states
	BOOL bCheck = GetDoc().GetCheckA();
	m_CheckA.SendMessage(BM_SETCHECK, bCheck, 0);

	bCheck = GetDoc().GetCheckB();
	m_CheckB.SendMessage(BM_SETCHECK, bCheck, 0);

	bCheck = GetDoc().GetCheckC();
	m_CheckC.SendMessage(BM_SETCHECK, bCheck, 0);

	UINT curRadio = GetDoc().GetRadio();
	OnRangeOfIds_Radio(curRadio);
	
	// Initialize dialog resizing
	m_Resizer.Initialize( this, CRect(0, 0, 300, 270) );
	m_Resizer.AddChild(m_RadioA,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_RadioB,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_RadioC,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_CheckA,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_CheckB,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_CheckC,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_Button,   topleft, TRUE, TRUE);
	m_Resizer.AddChild(m_Edit,     topleft, FALSE, FALSE);
	m_Resizer.AddChild(m_ListBox,  topleft, FALSE, FALSE);
	m_Resizer.AddChild(m_RichEdit, topleft, FALSE, FALSE);
	m_Resizer.AddChild(m_Bitmap,   topright, TRUE, TRUE);
	m_Resizer.AddChild(m_OK,       bottomright, TRUE, TRUE);
	m_Resizer.AddChild(m_Group,    bottomright, FALSE, FALSE);
	m_Resizer.AddChild(m_Status,   bottomright, FALSE, FALSE);
	
	return true;
}

void CFormView::OnOK()
{
	SetDlgItemText(IDC_STATUS, _T("OK Button Pressed."));
	TRACE(_T("OK Button Pressed.\n"));
}

void CFormView::OnButton()
{
	SetDlgItemText(IDC_STATUS, _T("Button Pressed"));
	TRACE(_T("Button Pressed\n"));
}

void CFormView::OnCheckA()
{
	TRACE(_T("Check Box A\n"));
	BOOL bCheck = GetDoc().GetCheckA();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_A, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckA(bCheck);

	SetDlgItemText(IDC_STATUS, _T("Check Box A toggled"));
}

void CFormView::OnCheckB()
{
	TRACE(_T("Check Box B\n"));
	BOOL bCheck = GetDoc().GetCheckB();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_B, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckB(bCheck);

	SetDlgItemText(IDC_STATUS, _T("Check Box B toggled"));
}

void CFormView::OnCheckC()
{
	TRACE(_T("Check Box C\n"));
	BOOL bCheck = GetDoc().GetCheckC();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_C, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckC(bCheck);

	SetDlgItemText(IDC_STATUS, _T("Check Box C toggled"));
}

void CFormView::OnRangeOfIds_Radio(UINT nIdAdjust)
{
	SendDlgItemMessage(ID_RADIO_A, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_B, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_C, BM_SETCHECK, FALSE, 0);
	SendDlgItemMessage(ID_RADIO_A + nIdAdjust, BM_SETCHECK, TRUE, 0);
	GetDoc().SetRadio(nIdAdjust);

	SetDlgItemText(IDC_STATUS, _T("Radio changed"));
	TRACE(_T("Radio changed\n"));
}

