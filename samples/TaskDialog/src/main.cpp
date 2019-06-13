
#include "stdafx.h"
#include "TaskDialogApp.h"


int APIENTRY WinMain(__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
{
    try
    {
        // Start Win32++
        CTaskDialogApp theApp;

        // Run the application
        return theApp.Run();
    }
    
    // catch all unhandled CException types
    catch (const CException &e)
    {
        // Display the exception and quit
        MessageBox(NULL, e.GetText(), AtoT(e.what()), MB_ICONERROR);

        return -1;
    }
}
