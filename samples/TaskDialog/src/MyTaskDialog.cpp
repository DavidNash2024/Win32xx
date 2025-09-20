/////////////////////////////
// MyTaskDialog.cpp
//

#include "stdafx.h"
#include "MyTaskDialog.h"

//////////////////////////////////////
// CMyTaskDialog function definitions.
//

CMyTaskDialog::CMyTaskDialog()
{
    SetOptions(TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS | TDF_EXPANDED_BY_DEFAULT | TDF_SHOW_PROGRESS_BAR | TDF_CALLBACK_TIMER);

    // Add the command buttons.
    AddCommandControl(CB_FIRST, MAKEINTRESOURCEW(IDS_CB_FIRST));
    AddCommandControl(CB_SECOND, MAKEINTRESOURCEW(IDS_CB_SECOND));
    AddCommandControl(CB_THIRD, MAKEINTRESOURCEW(IDS_CB_THIRD));

    // Add the radio buttons.
    AddRadioButton(RB_FIRST, MAKEINTRESOURCEW(IDS_RB_FIRST));
    AddRadioButton(RB_SECOND, MAKEINTRESOURCEW(IDS_RB_SECOND));
    AddRadioButton(RB_THIRD, MAKEINTRESOURCEW(IDS_RB_THIRD));
    SetDefaultRadioButton(RB_FIRST);
    SetCommonButtons(TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON);

    // Add the Text.
    SetWindowTitle(MAKEINTRESOURCEW(IDS_WINDOWTITLE));
    SetMainInstruction(MAKEINTRESOURCEW(IDS_MAININSTRUCTION));
    SetContent(MAKEINTRESOURCEW(IDS_CONTENT));
    SetExpansionArea(MAKEINTRESOURCEW(IDS_EXPANDED), L"Hide the expanded information", L"Show the expanded information");
    SetVerificationCheckboxText(MAKEINTRESOURCEW(IDS_VERIFICATIONTEXT));
    SetFooterText(MAKEINTRESOURCEW(IDS_FOOTER));

    // Set Icons
    SetMainIcon(GetApp()->LoadIcon(IDW_MAIN));
    SetFooterIcon(TD_INFORMATION_ICON);
}

// Called when the user selects a button or command link.
BOOL CMyTaskDialog::OnTDButtonClicked(int buttonID)
{
    switch (buttonID)
    {
    case CB_FIRST:
        TRACE(L"First command control selected\n");
        return TRUE;
    case CB_SECOND:
        TRACE(L"Second command control selected\n");
        return TRUE;
    case CB_THIRD:
        TRACE(L"Third command control selected\n");
        return TRUE;
    case IDYES:
        TRACE(L"The 'Yes' button was pressed\n");
        return FALSE;
    case IDNO:
        TRACE(L"The 'No' button was pressed\n");
        return FALSE;
    case IDCANCEL:
        TRACE(L"The 'Cancel' button was pressed\n");
        return FALSE;
    }

    // Return FALSE to close the dialog.
    // Return TRUE to prevent the task dialog from closing.
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
BOOL CMyTaskDialog::OnTDRadioButtonClicked(int buttonID)
{
    switch (buttonID)
    {
    case RB_FIRST:
        TRACE(L"First radio button selected\n");
        break;
    case RB_SECOND:
        TRACE(L"Second radio button selected\n");
        break;
    case RB_THIRD:
        TRACE(L"Third radio button selected\n");
        break;
    }

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
void CMyTaskDialog::OnTDVerificationCheckboxClicked(BOOL isChecked)
{
    UNREFERENCED_PARAMETER(isChecked);

    CString text("Verification Checkbox is ");
    TRACE(text << (isChecked ? "checked\n" : "unchecked\n"));
}

// Processes the task dialog's messages.
LRESULT CMyTaskDialog::TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam)
{
    try
    {
        return TaskDialogProcDefault(msg, wparam, lparam);
    }

    // Catch all unhandled CException types.
    catch (const CException& e)
    {
        // Display the exception and continue.
        CString str1;
        str1 << e.GetText() << L'\n' << e.GetErrorString();

        CString str2;
        str2 << "Error: " << e.what();
        TaskDialogBox(nullptr, str1, str2, TD_ERROR_ICON);
    }

    // Catch all unhandled std::exception types.
    catch (const std::exception& e)
    {
        // Display the exception and continue.
        CString str1 = e.what();
        TaskDialogBox(nullptr, str1, L"Error: std::exception", TD_ERROR_ICON);
    }

    return 0;
}