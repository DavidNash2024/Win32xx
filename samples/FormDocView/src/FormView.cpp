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
	m_Resizer.HandleMessage(uMsg, wParam, lParam);

//	switch (uMsg)
//	{
//		Add case statements for each messages to be handled here
//	}

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
    case IDC_BUTTON1:	OnButton();		return TRUE;
	case ID_CHECK_A:	OnCheckA();		return TRUE;
    case ID_CHECK_B:	OnCheckB();		return TRUE;
    case ID_CHECK_C:	OnCheckC();		return TRUE;
	case ID_RADIO_A:
    case ID_RADIO_B:	// intentionally blank
    case ID_RADIO_C:	OnRangeOfIds_Radio(wpLo - ID_RADIO_A);	return TRUE;
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
		m_ListBox.AddString(_T("List Box"));
	}

	// Set initial button states
	BOOL bCheck = GetDoc().GetCheckA();
	m_CheckA.SetCheck(bCheck);

	bCheck = GetDoc().GetCheckB();
	m_CheckB.SetCheck(bCheck);

	bCheck = GetDoc().GetCheckC();
	m_CheckC.SetCheck(bCheck);

	UINT curRadio = GetDoc().GetRadio();
	OnRangeOfIds_Radio(curRadio);
	
	// Initialize dialog resizing
	m_Resizer.Initialize( this, CRect(0, 0, 300, 270) );
	m_Resizer.AddChild(m_RadioA,   topleft, 0);
	m_Resizer.AddChild(m_RadioB,   topleft, 0);
	m_Resizer.AddChild(m_RadioC,   topleft, 0);
	m_Resizer.AddChild(m_CheckA,   topleft, 0);
	m_Resizer.AddChild(m_CheckB,   topleft, 0);
	m_Resizer.AddChild(m_CheckC,   topleft, 0);
	m_Resizer.AddChild(m_Button,   topleft, 0);
	m_Resizer.AddChild(m_Edit,     topleft, RD_STRETCH_WIDTH);
	m_Resizer.AddChild(m_ListBox,  topleft, RD_STRETCH_WIDTH);
	m_Resizer.AddChild(m_RichEdit, topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);
	m_Resizer.AddChild(m_Bitmap,   topright, 0);
	m_Resizer.AddChild(m_OK,       bottomright, 0);
	m_Resizer.AddChild(m_Group,    bottomright, RD_STRETCH_HEIGHT);
	m_Resizer.AddChild(m_Status,   bottomright, RD_STRETCH_HEIGHT);
	
	return true;
}

void CFormView::OnOK()
{
	SetDlgItemText(IDC_STATUS, _T("OK Button Pressed."));
	TRACE("OK Button Pressed.\n");
}

void CFormView::OnButton()
{
	SetDlgItemText(IDC_STATUS, _T("Button Pressed"));
	TRACE("Button Pressed\n");
}

void CFormView::OnCheckA()
{
	TRACE("Check Box A\n");
	BOOL bCheck = GetDoc().GetCheckA();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_A, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckA(bCheck);

	SetDlgItemText(IDC_STATUS, _T("Check Box A toggled"));
}

void CFormView::OnCheckB()
{
	TRACE("Check Box B\n");
	BOOL bCheck = GetDoc().GetCheckB();
	bCheck = !bCheck;  // Toggle
	SendDlgItemMessage(ID_CHECK_B, BM_SETCHECK, bCheck, 0);
	GetDoc().SetCheckB(bCheck);

	SetDlgItemText(IDC_STATUS, _T("Check Box B toggled"));
}

void CFormView::OnCheckC()
{
	TRACE("Check Box C\n");
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
	TRACE("Radio changed\n");
}

