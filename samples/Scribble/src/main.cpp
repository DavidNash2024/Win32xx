////////////////////////////////
// Main.cpp

#include "stdafx.h"
#include "ScribbleApp.h"


// WinMain is the program's entry point. The program starts here.
#if defined (_MSC_VER) && (_MSC_VER >= 1920) // >= VS2019
  int WINAPI WinMain (__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
#else
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    try
    {
        // Start Win32++.
        CScribbleApp theApp;

        // Run the application.
        return theApp.Run();
    }

    // Catch all unhandled CException types.
    catch (const CException &e)
    {
        // Display the exception and quit.
        ::MessageBox(0, e.GetErrorString(), e.GetText(), MB_ICONWARNING);

        return -1;
    }
}
