/////////////////////////////
// MyTaskDialog.cpp
//

#include "stdafx.h"
#include "MyTaskDialog.h"

//////////////////////////////////////
// CMyTaskDialog function definitions.
//

// Called when the user selects a button or command link.
BOOL CMyTaskDialog::OnTDButtonClicked(int)
{
    TRACE(L"Button or command link pressed\n");

    // we could return TRUE to prevent the task dialog from closing
    return FALSE;
}

// Called when the task dialog is constructed, before it is displayed.
void CMyTaskDialog::OnTDConstructed()
{
    TRACE(L"Task Dialog Constructed\n");
}

// Called when the task dialog is displayed.
void CMyTaskDialog::OnTDCreated()
{
    TRACE(L"Task Dialog Created\n");
}

// Called when the task dialog is destroyed.
void CMyTaskDialog::OnTDDestroyed()
{
    TRACE(L"Task Dialog Destroyed\n");
}

// Called when the expand button is clicked.
void CMyTaskDialog::OnTDExpandButtonClicked(BOOL)
{
    TRACE(L"Expand Button Pressed\n");
}

// Called when the user presses F1 on the keyboard.
void CMyTaskDialog::OnTDHelp()
{
    TRACE(L"F1 Pressed\n");
}

// Called when the user clicks on a hyperlink.
void CMyTaskDialog::OnTDHyperlinkClicked(LPCWSTR)
{
    TRACE(L"Hyperlink clicked\n");
}

void CMyTaskDialog::OnTDNavigatePage()
// Called when a navigation has occurred.
{
    TRACE(L"Navigation occurred\n");
}

// Called when a radio button. is selected.
BOOL CMyTaskDialog::OnTDRadioButtonClicked(int)
{
    TRACE(L"Radio Button Selected\n");
    return TRUE;
}

// Called approximately every 200 milliseconds when the TDF_CALLBACK_TIMER flag is set.
BOOL CMyTaskDialog::OnTDTimer(DWORD)
{
    static int i = 0;
    if (i <= 100)
        SetProgressBarPosition(++i);

    return FALSE;
}

// Called when the user clicks the Task Dialog verification check box.
void CMyTaskDialog::OnTDVerificationCheckboxClicked(BOOL)
{
    TRACE(L"Verification Checkbox Clicked\n");
}

// Processes the task dialog's messages.
LRESULT CMyTaskDialog::TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        return TaskDialogProcDefault(msg, wparam, lparam);
    }

    // Catch all CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        ::MessageBox(0, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return 0;
    }
}