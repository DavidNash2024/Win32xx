///////////////////////////////////////
// MyDialog.cpp

#include "stdafx.h"
#include "MyDialog.h"
#include "resource.h"
#include "UserMessages.h"


/////////////////////////////////
// CMyDialog function definitions
//

// Constructor.
CMyDialog::CMyDialog(int resID) : CDialog(resID)
{
}

// Constructor.
CMyDialog::CMyDialog(LPCTSTR resName) : CDialog(resName)
{
}

// Destructor.
CMyDialog::~CMyDialog()
{
}

// Process the dialog's window messages.
INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        switch (msg)
        {
        case UWM_SETSTATIC:     return SetStatic((LPCTSTR)wparam);
        }

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

// Process the command messages (WM_COMMAND) from the controls.
BOOL CMyDialog::OnCommand(WPARAM wparam, LPARAM)
{
    UINT id = LOWORD(wparam);
    switch (id)
    {
    case IDC_BUTTON1:   return OnButton();
    case IDC_CHECK1:    return OnCheck1();
    case IDC_CHECK2:    return OnCheck2();
    case IDC_CHECK3:    return OnCheck3();

    case IDC_RADIO1:
    case IDC_RADIO2:    // intentionally blank
    case IDC_RADIO3:    return OnRangeOfRadioIDs(IDC_RADIO1, IDC_RADIO3, id);
    }

    return FALSE;
}

// Called when the dialog window is destroyed.
void CMyDialog::OnDestroy()
{
    // End the application
    ::PostQuitMessage(0);
}

// Called before the dialog is displayed.
BOOL CMyDialog::OnInitDialog()
{
    // Set the application icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Put some text in the rich edit box
    SetDlgItemText(IDC_RICHEDIT1, _T("Rich Edit Window"));

    // Put some text in the list box
    AttachItem(IDC_LIST1, m_listBox);
    for (int i = 0 ; i < 8 ; i++)
        m_listBox.AddString(_T("List Box"));

    // Turn our button into a MyButton object
    AttachItem(IDC_BUTTON2, m_button);

    // Turn our static control into a hyperlink
    AttachItem(IDC_STATIC4, m_hyperlink);

    // Attach the rich edit control to m_RichEdit
    AttachItem(IDC_RICHEDIT1, m_richEdit);

    // Select the first radio button
    CheckRadioButton(IDC_RADIO1, IDC_RADIO3, IDC_RADIO1);

    return TRUE;
}

// Called when the OK button or Enter key is pressed.
void CMyDialog::OnOK()
{
    MessageBox(_T("OK Button Pressed.  Program will exit now."), _T("Button"), MB_OK);
    CDialog::OnOK();
}

// Called when the button is pressed.
BOOL CMyDialog::OnButton()
{
    SetStatic(_T("Button Pressed"));
    TRACE("Button Pressed\n");
    return TRUE;
}

// Called when check box 1 is clicked.
BOOL CMyDialog::OnCheck1()
{
    SetStatic(_T("Check Box 1"));
    TRACE("Check Box 1\n");
    return TRUE;
}

// Called when check box 2 is clicked.
BOOL CMyDialog::OnCheck2()
{
    SetStatic(_T("Check Box 2"));
    TRACE("Check Box 2\n");
    return TRUE;
}

// Called when check box 3 is clicked.
BOOL CMyDialog::OnCheck3()
{
    SetStatic(_T("Check Box 3"));
    TRACE("Check Box 3\n");
    return TRUE;
}

// Called when a radio button is selected.
BOOL CMyDialog::OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked)
{
    CheckRadioButton(idFirst, idLast, idClicked);

    CString str;
    int nButton = idClicked - idFirst + 1;
    str.Format(_T("Radio%d"), nButton);
    TRACE(str); TRACE("\n");
    SetStatic(str);

    return TRUE;
}

// Sets the text in the static control.
INT_PTR CMyDialog::SetStatic(LPCTSTR text)
{
    SetDlgItemText(IDC_STATIC3, text);
    return 0;
}

