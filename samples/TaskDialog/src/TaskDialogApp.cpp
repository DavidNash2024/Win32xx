/////////////////////////////
// TaskDialogApp.cpp
//

#include "stdafx.h"
#include "TaskDialogApp.h"
#include "MyTaskDialog.h"

///////////////////////////////////////
// CTaskDialogApp function definitions.
//

BOOL CTaskDialogApp::InitInstance()
{
    if (!CTaskDialog::IsSupported())
    {
        // Task Dialogs are only supported on Vista and above.
        ::MessageBoxW(nullptr, L"Vista or later is required for Task Dialogs", L"Not Supported!", MB_ICONERROR);
        return FALSE;
    }

    // Create the TaskDialog object.
    CMyTaskDialog td;

    // Display the task dialog.
    td.DoModal();  // Throws a CWinException on failure.

    ::PostQuitMessage(0);   // End the application.
    return TRUE;
}

