/////////////////////////////
// MakeDLL.cpp
//

// Defines the entry point for the DLL application.

#include "StdAfx.h"
#include "MyDialog.h"
#include "MakeDLL.h"
#include "resource.h"


// Start Win32++ for the DLL
CWinApp App;

// MyDialog is global for the DLL
CMyDialog MyDialog(IDD_DIALOG1);

// The entry point for the dll.
BOOL WINAPI DllMain( HANDLE, DWORD  ul_reason_for_call, LPVOID )
{
    switch( ul_reason_for_call )
    {
    case DLL_PROCESS_ATTACH:
        TRACE("DLL_PROCESS_ATTACH\n");
        break;
    case DLL_THREAD_ATTACH:
        TRACE("DLL_THREAD_ATTACH\n");
        break;
    case DLL_THREAD_DETACH:
        TRACE("DLL_THREAD_DETACH\n");
        break;
    case DLL_PROCESS_DETACH:
        TRACE("DLL_PROCESS_DETACH\n");
        break;
    }

    return TRUE;
}

void __declspec(dllexport) ShowDialog()
{
    //NOTE: This function doesn't return until the dialog is closed.
    //      CThread can be used to put the dialog creation and message loop in
    //      a separate thread if you wish the function to return immediately.

    // Create the dialog
    TRACE(_T("Creating a dialog inside the DLL:\n"));
    MyDialog.Create();
    TRACE(_T("Dialog inside DLL created\n"));

    // Run the message loop
    App.Run();
}


