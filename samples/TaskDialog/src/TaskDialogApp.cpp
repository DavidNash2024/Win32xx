/////////////////////////////
// TaskDialogApp.cpp
//

#include "stdafx.h"
#include "TaskDialogApp.h"
#include "MyTaskDialog.h"
#include "resource.h"

///////////////////////////////////////
// CTaskDialogApp function definitions.
//
CTaskDialogApp::CTaskDialogApp()
{
}

CTaskDialogApp::~CTaskDialogApp()
{
}

BOOL CTaskDialogApp::InitInstance()
{
    if (!CTaskDialog::IsSupported())
    {
        // Task Dialogs are only supported on Vista and above.
        ::MessageBoxW(0, L"Vista or better is required for Task Dialogs", L"Not Supported!", MB_ICONERROR);
        return FALSE;
    }

    // Create the TaskDialog object.
    CMyTaskDialog td;
    td.SetOptions( TDF_ALLOW_DIALOG_CANCELLATION | TDF_USE_COMMAND_LINKS | TDF_EXPANDED_BY_DEFAULT | TDF_SHOW_PROGRESS_BAR | TDF_CALLBACK_TIMER);

    // Add the buttons.
    td.AddCommandControl( CB_FIRST, MAKEINTRESOURCEW(IDS_CB_FIRST) );
    td.AddCommandControl( CB_SECOND, MAKEINTRESOURCEW(IDS_CB_SECOND) );
    td.AddCommandControl( CB_THIRD, MAKEINTRESOURCEW(IDS_CB_THIRD) );
    td.AddRadioButton( RB_FIRST,  MAKEINTRESOURCEW(IDS_RB_FIRST) );
    td.AddRadioButton( RB_SECOND, MAKEINTRESOURCEW(IDS_RB_SECOND) );
    td.AddRadioButton( RB_THIRD,  MAKEINTRESOURCEW(IDS_RB_THIRD) );
    td.SetDefaultRadioButton( RB_FIRST );
    td.SetCommonButtons( TDCBF_YES_BUTTON | TDCBF_NO_BUTTON | TDCBF_CANCEL_BUTTON );

    // Add the Text.
    td.SetWindowTitle( MAKEINTRESOURCEW(IDS_WINDOWTITLE) );
    td.SetMainInstruction( MAKEINTRESOURCEW(IDS_MAININSTRUCTION) );
    td.SetContent( MAKEINTRESOURCEW(IDS_CONTENT) );
    td.SetExpansionArea( MAKEINTRESOURCEW(IDS_EXPANDED), L"Hide the expanded information", L"Show the expanded information" );
    td.SetVerificationCheckboxText( MAKEINTRESOURCEW(IDS_VERIFICATIONTEXT) );
    td.SetFooterText( MAKEINTRESOURCEW(IDS_FOOTER) );

    // Set Icons
    td.SetMainIcon(TD_INFORMATION_ICON);
    td.SetFooterIcon(TD_INFORMATION_ICON);

    // Display the task dialog.
    HRESULT hr = td.DoModal();  // throws a CWinException on failure

    // Respond to the result.
    if (SUCCEEDED(hr))
    {
        switch (td.GetSelectedButtonID())
        {
        case CB_FIRST:
            TRACE(L"First command control selected\n");
            break;
        case CB_SECOND:
            TRACE(L"Second command control selected\n");
            break;
        case CB_THIRD:
            TRACE(L"Third command control selected\n");
            break;
        case IDYES:
            TRACE(L"The 'Yes' button was pressed\n");
            break;
        case IDNO:
            TRACE(L"The 'No' button was pressed\n");
            break;
        case IDCANCEL:
            TRACE(L"The 'Cancel' button was pressed\n");
            break;
        }

        switch (td.GetSelectedRadioButtonID())
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

        if (td.GetVerificationCheckboxState())
        {
            TRACE(L"Verification button selected\n");
        }
    }
    else
    {
       TRACE(L"Some error occurred\n");
    }

    ::PostQuitMessage(0);   // End the application.
    return TRUE;
}


