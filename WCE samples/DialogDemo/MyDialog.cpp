
#include "MyDialog.h"
#include "resource.h"
#include "windowsx.h"


CMyDialog::CMyDialog(UINT resID) : CDialog(resID), m_counter(0)
{
}

CMyDialog::~CMyDialog()
{
}

void CMyDialog::AddToButton()
{
    //get the control window
    HWND hwButton = GetDlgItem(IDC_BUTTON1);
    
    //set text to show in control
    WCHAR text[16];
    wsprintf(text, L"Button %d", m_counter);
    ::Button_SetText(hwButton, text);
    return;
}

void CMyDialog::AddToComboBox()
{
    //get the control window
    HWND hwComboBox = GetDlgItem(IDC_COMBO1);
    
    //set text to show in control
    WCHAR text[16];
    wsprintf(text, L"ComboBox %d", m_counter);
    if (m_counter)
    {
        ComboBox_AddString(hwComboBox, text);
        ::ComboBox_SetText(hwComboBox, text);
        ComboBox_SetCurSel(hwComboBox, m_counter-1);
    }
    else 
    {
        ComboBox_ResetContent(hwComboBox); 
        ComboBox_ShowDropdown(hwComboBox, FALSE);
    }
}

void CMyDialog::AddToEdit()
{
    //get the control window
    HWND hwEdit = GetDlgItem(IDC_EDIT1 ); 
    
    //set text to show in control
    WCHAR text[16];
    wsprintf(text, L"Edit %d\r\n", m_counter);
    if (m_counter)
        Edit_ReplaceSel(hwEdit, text); 
    else
        ::SetWindowText(hwEdit, L""); 
}

void CMyDialog::AddToListBox()
{
    //get the control window
    HWND hwListBox = GetDlgItem(IDC_LIST1); 
    
    //set text to show in control
    WCHAR text[16];
    wsprintf(text, L"ListBox %d", m_counter);
    if (m_counter)
        ListBox_AddString(hwListBox, text);
    else
        ListBox_ResetContent(hwListBox); 
}

void CMyDialog::AddToScrollBars()
{
    //get the control window
    HWND hwScrollBarH = GetDlgItem(IDC_SCROLLBAR1);
    HWND hwScrollBarV = GetDlgItem(IDC_SCROLLBAR2);
    
    //set scroll bar range
    ScrollBar_SetRange(hwScrollBarH, 0, 10, FALSE);
    ScrollBar_SetRange(hwScrollBarV, 0, 10, FALSE);
    
    //set scroll bar position
    ScrollBar_SetPos(hwScrollBarH, m_counter, TRUE);
    ScrollBar_SetPos(hwScrollBarV, m_counter, TRUE);
}

BOOL CMyDialog::OnInitDialog()
{
    //Set the Icon
    SetIconLarge(IDW_MAIN);
    SetIconSmall(IDW_MAIN);

    // Set a timer to animate the controls on the dialog window
    SetTimer(ID_TIMER, 500, NULL);

    return true;
}

void CMyDialog::OnOK()
{
    ::MessageBox(NULL, TEXT("DONE Button Pressed.  Program will exit now."), TEXT("Button"), MB_OK);
    CDialog::OnOK();
}

INT_PTR CMyDialog::DialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    switch (msg)
    {
    case WM_TIMER:
        m_counter > 9 ? m_counter = 0 : m_counter++;
        AddToEdit();
        AddToListBox();
        AddToScrollBars();
        AddToComboBox();
        AddToButton();
    break;

    } // switch(uMsg)
    
    return DialogProcDefault(msg, wparam, lparam);
    
} // INT_PTR CALLBACK DialogProc(...)


