/////////////////////////////
// main.cpp
//

#include "StdAfx.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920) // >= VS2019
  int WINAPI WinMain (__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
#else
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    // Displays a dialog contained in MyDLL.dll.

    // Load the MyDLL.dll
    HMODULE hModule = LoadLibrary(_T("MyDLL.dll"));
    if (0 == hModule)
    {
        ::MessageBox(0, _T("Failed to load MyDLL.dll"), _T("Error"), MB_OK);
        return 0;
    }

    // Create a pointer to MyDLL's ShowDialog function.
    typedef void WINAPI SHOWDIALOG();
    SHOWDIALOG* pfnShowDialog = (SHOWDIALOG*)::GetProcAddress(hModule, "ShowDialog");
    assert(pfnShowDialog);

    if (pfnShowDialog != NULL)
    {
        // Call MyDLL's ShowDialog function.
        pfnShowDialog();
    }
    else
        ::MessageBox(0, _T("Failed to run ShowDialog"), _T("Error"), MB_OK);

    // We get here when the dialog is closed.
    FreeLibrary(hModule);

    return 0;
}



