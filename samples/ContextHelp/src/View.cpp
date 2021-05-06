/////////////////////////////
// View.cpp
//

#include "stdafx.h"
#include "View.h"
#include "resource.h"

//////////////////////////////////
// Definitions for the CView class
//

// Constructor.
CView::CView(UINT resID) : CDialog(resID), m_hParent(0)
{
}

// Destructor.
CView::~CView()
{
}

// Creates the view window. Its a modeless dialog.
HWND CView::Create(HWND hParent = 0)
{
    m_hParent = hParent;
    return DoModeless(hParent);
}

// Process the dialog's window messages.
INT_PTR CView::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        // Pass messages on to the resizer.
        m_resizer.HandleMessage(msg, wparam, lparam);

    //  switch (msg)
    //  {
    //      Add case statements for each message to be handled here
    //  }

        // Pass unhandled messages on to parent DialogProc.
        return DialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}

// Retrieves a reference to CDoc.
CDoc& CView::GetDoc()
{
    return m_doc;
}

// Called when the button is pressed.
BOOL CView::OnButton()
{
    SetDlgItemText(IDC_STATUS, _T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

// Called when the esc key is pressed.
void CView::OnCancel()
{
    // Discard these messages
    SetDlgItemText(IDC_STATUS, _T("Cancel Pressed."));
    TRACE("Cancel Pressed.\n");
}

// Called when a request is made to close the dialog.
void CView::OnClose()
{
    // Suppress handling of WM_CLOSE for the dialog.
    // An edit control will send WM_CLOSE to the dialog if the Esc button is pressed.
}

// Process command notifications from the dialog's controls.
BOOL CView::OnCommand(WPARAM wparam, LPARAM)
{
    WORD id = LOWORD(wparam);

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

// Called before the dialog (view window) is displayed.
BOOL CView::OnInitDialog()
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
    AttachItem(IDOK,    m_ok);
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
    m_resizer.Initialize( *this, CRect(0, 0, 300, 270) );
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

// Called when the OK button is pressed.
void CView::OnOK()
{
    SetDlgItemText(IDC_STATUS, _T("OK Button Pressed."));
    TRACE("OK Button Pressed.\n");
}

// Called when Check Box A is toggled.
BOOL CView::OnCheckA()
{
    TRACE("Check Box A\n");
    BOOL checkA = GetDoc().GetCheckA();
    checkA = !checkA;  // Toggle
    CheckDlgButton(ID_CHECK_A, checkA);
    GetDoc().SetCheckA(checkA);

    SetDlgItemText(IDC_STATUS, _T("Check Box A toggled"));
    return TRUE;
}

// Called when Check Box B is toggled.
BOOL CView::OnCheckB()
{
    TRACE("Check Box B\n");
    BOOL checkB = GetDoc().GetCheckB();
    checkB = !checkB;  // Toggle
    CheckDlgButton(ID_CHECK_B, checkB);
    GetDoc().SetCheckB(checkB);

    SetDlgItemText(IDC_STATUS, _T("Check Box B toggled"));
    return TRUE;
}

// Called when Check Box C is toggled.
BOOL CView::OnCheckC()
{
    TRACE("Check Box C\n");
    BOOL checkC = GetDoc().GetCheckC();
    checkC = !checkC;  // Toggle
    CheckDlgButton(ID_CHECK_C, checkC);
    GetDoc().SetCheckC(checkC);

    SetDlgItemText(IDC_STATUS, _T("Check Box C toggled"));
    return TRUE;
}

// Called when the radio button is selected.
BOOL CView::OnRangeOfIDs(UINT firstID, UINT lastID, UINT clickedID)
{
    CheckRadioButton(firstID, lastID, clickedID);

    GetDoc().SetRadio(clickedID);
    SetDlgItemText(IDC_STATUS, _T("Radio changed"));
    TRACE("Radio changed\n");

    return TRUE;
}
