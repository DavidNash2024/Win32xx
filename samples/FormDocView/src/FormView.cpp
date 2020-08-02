///////////////////////////////////////
// FormView.cpp

#include "stdafx.h"
#include "FormView.h"
#include "resource.h"


// Definitions for the CFormView class
CFormView::CFormView(UINT resID) : CDialog(resID)
{
}

CFormView::~CFormView()
{
}

HWND CFormView::Create(HWND hParent = 0)
{
    return DoModeless(hParent);
}

INT_PTR CFormView::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    m_resizer.HandleMessage(msg, wparam, lparam);

//  switch (uMsg)
//  {
//      Add case statements for each messages to be handled here
//  }

    // Pass unhandled messages on to parent DialogProc
    return DialogProcDefault(msg, wparam, lparam);
}

CFormDoc& CFormView::GetDoc()
{
    return m_doc;
}

BOOL CFormView::OnButton()
{
    SetDlgItemText(IDC_STATUS, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

void CFormView::OnCancel()
{
    // Suppress closing the dialog when the esc key is pressed.
    SetDlgItemText(IDC_STATUS, _T("Cancel Pressed."));
    TRACE("Cancel Pressed.\n");
}

void CFormView::OnClose()
{
    // Suppress handling of WM_CLOSE for the dialog.
    // A rich edit control will send WM_CLOSE to the dialog if the Esc button is pressed.
}

BOOL CFormView::OnCommand(WPARAM wparam, LPARAM lparam)
{
    UNREFERENCED_PARAMETER(lparam);
  
    UINT id = LOWORD(wparam);

    switch (id)
    {
    case IDC_BUTTON1:   return OnButton();
    case ID_CHECK_A:    return OnCheckA();
    case ID_CHECK_B:    return OnCheckB();
    case ID_CHECK_C:    return OnCheckC();

    case ID_RADIO_A:    
    case ID_RADIO_B:    // intentionally blank
    case ID_RADIO_C:    return OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, id);
    } 
  
    return FALSE;
}

BOOL CFormView::OnInitDialog()
{
    // Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Attach CWnd objects to the dialog items
    AttachItem(IDC_BUTTON1, m_button);
    AttachItem(ID_CHECK_A,  m_checkA);
    AttachItem(ID_CHECK_B,  m_checkB);
    AttachItem(ID_CHECK_C,  m_checkC);
    AttachItem(IDC_EDIT1,   m_edit);
    AttachItem(IDC_LIST1,   m_listBox);
    AttachItem(IDOK,        m_ok);
    AttachItem(ID_RADIO_A,  m_radioA);
    AttachItem(ID_RADIO_B,  m_radioB);
    AttachItem(ID_RADIO_C,  m_radioC);
    AttachItem(IDC_RICHEDIT1, m_richEdit);
    AttachItem(IDC_GROUP1,  m_group);
    AttachItem(IDC_STATUS,  m_status);
    AttachItem(IDC_BITMAP1, m_picture);
    
    // Put some text in the edit boxes
    SetDlgItemText(IDC_EDIT1, _T("Edit Control"));
    SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

    // Put some text in the list box
    for (int i = 0 ; i < 8 ; i++) 
    {
        m_listBox.AddString(_T("List Box"));
    }

    // Set initial button states
    BOOL bCheck = GetDoc().GetCheckA();
    m_checkA.SetCheck(bCheck);

    bCheck = GetDoc().GetCheckB();
    m_checkB.SetCheck(bCheck);

    bCheck = GetDoc().GetCheckC();
    m_checkC.SetCheck(bCheck);

    UINT curRadio = GetDoc().GetRadio();
    OnRangeOfIDs(ID_RADIO_A, ID_RADIO_C, curRadio);
    
    // Initialize dialog resizing
    m_resizer.Initialize( *this, CRect(0, 0, 450, 300) );
    m_resizer.AddChild(m_radioA,   topleft, 0);
    m_resizer.AddChild(m_radioB,   topleft, 0);
    m_resizer.AddChild(m_radioC,   topleft, 0);
    m_resizer.AddChild(m_checkA,   topleft, 0);
    m_resizer.AddChild(m_checkB,   topleft, 0);
    m_resizer.AddChild(m_checkC,   topleft, 0);
    m_resizer.AddChild(m_button,   topleft, 0);
    m_resizer.AddChild(m_edit,     topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_listBox,  topleft, RD_STRETCH_WIDTH);
    m_resizer.AddChild(m_richEdit, topleft, RD_STRETCH_WIDTH | RD_STRETCH_HEIGHT);
    m_resizer.AddChild(m_picture,  topright, 0);
    m_resizer.AddChild(m_ok,       bottomright, 0);
    m_resizer.AddChild(m_group,    bottomright, RD_STRETCH_HEIGHT);
    m_resizer.AddChild(m_status,   bottomright, RD_STRETCH_HEIGHT);
    
    return TRUE;
}

void CFormView::OnOK()
{
    // Suppress closing the dialog when the return key is pressed.
    SetDlgItemText(IDC_STATUS, _T("OK Button Pressed."));
    TRACE("OK Button Pressed.\n");
}

BOOL CFormView::OnCheckA()
{
    TRACE("Check Box A\n");
    BOOL isCheckA = GetDoc().GetCheckA();
    isCheckA = !isCheckA;  // Toggle
    CheckDlgButton(ID_CHECK_A, isCheckA);
    GetDoc().SetCheckA(isCheckA);

    SetDlgItemText(IDC_STATUS, _T("Check Box A toggled"));
    return TRUE;
}

BOOL CFormView::OnCheckB()
{
    TRACE("Check Box B\n");
    BOOL isCheckB = GetDoc().GetCheckB();
    isCheckB = !isCheckB;  // Toggle
    CheckDlgButton(ID_CHECK_B, isCheckB);
    GetDoc().SetCheckB(isCheckB);

    SetDlgItemText(IDC_STATUS, _T("Check Box B toggled"));
    return TRUE;
}

BOOL CFormView::OnCheckC()
{
    TRACE("Check Box C\n");
    BOOL isCheckC = GetDoc().GetCheckC();
    isCheckC = !isCheckC;  // Toggle
    CheckDlgButton(ID_CHECK_C, isCheckC);
    GetDoc().SetCheckC(isCheckC);

    SetDlgItemText(IDC_STATUS, _T("Check Box C toggled"));
    return TRUE;
}

BOOL CFormView::OnRangeOfIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);
    
    GetDoc().SetRadio(idClicked);
    SetDlgItemText(IDC_STATUS, _T("Radio changed"));
    TRACE("Radio changed\n");
    return TRUE;
}

