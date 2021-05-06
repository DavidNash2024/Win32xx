/////////////////////////////
// MyTaskDialog.cpp
//

#include "stdafx.h"
#include "MyTaskDialog.h"

//////////////////////////////////////
// CMyTaskDialog function definitions.
//
BOOL CMyTaskDialog::OnTDButtonClicked(int)
// Called when the user selects a button or command link.
{
    TRACE("Button or command link pressed\n");

    // we could return TRUE to prevent the task dialog from closing
    return FALSE;
}

void CMyTaskDialog::OnTDConstructed()
// Called when the task dialog is constructed, before it is displayed.
{
    TRACE("Task Dialog Constructed\n");
}

void CMyTaskDialog::OnTDCreated()
// Called when the task dialog is displayed.
{
    TRACE("Task Dialog Created\n");
}

void CMyTaskDialog::OnTDDestroyed()
// Called when the task dialog is destroyed.
{
    TRACE("Task Dialog Destroyed\n");
}

void CMyTaskDialog::OnTDExpandButtonClicked(BOOL)
// Called when the expand button is clicked.
{
    TRACE("Expand Button Pressed\n");
}

void CMyTaskDialog::OnTDHelp()
// Called when the user presses F1 on the keyboard.
{
    TRACE("F1 Pressed\n");
}

void CMyTaskDialog::OnTDHyperlinkClicked(LPCTSTR)
// Called when the user clicks on a hyperlink.
{
    TRACE("Hyperlink clicked\n");
}

void CMyTaskDialog::OnTDNavigatePage()
// Called when a navigation has occurred.
{
    TRACE("Navigation occurred\n");
}

BOOL CMyTaskDialog::OnTDRadioButtonClicked(int)
// Called when the user selects a radio button.
{
    TRACE("Radio Button Clicked\n");
    return TRUE;
}

BOOL CMyTaskDialog::OnTDTimer(DWORD)
// Called every 200 milliseconds (aproximately) when the TDF_CALLBACK_TIMER flag is set.
{
    static int i = 0;
    if (i <= 100)
        SetProgressBarPosition(++i);

    return FALSE;
}

void CMyTaskDialog::OnTDVerificationCheckboxClicked(BOOL)
// Called when the user clicks the Task Dialog verification check box.
{
    TRACE("Verification Checkbox Clicked\n");
}

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