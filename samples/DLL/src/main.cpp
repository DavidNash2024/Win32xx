/////////////////////////////
// main.cpp
//

#include "StdAfx.h"

// For VS2019
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    // Displays a dialog contained in MyDLL.dll

    // Load the MyDLL.dll
    HMODULE hModule = LoadLibrary(_T("MyDLL.dll"));
    if (0 == hModule)
    {
        ::MessageBox(0, _T("Failed to load MyDLL.dll"), _T("Error"), MB_OK);
        return 0;
    }

    // Create a pointer to MyDLL's ShowDialog function
    typedef void WINAPI SHOWDIALOG();
    SHOWDIALOG* pfnShowDialog = (SHOWDIALOG*)::GetProcAddress(hModule, "ShowDialog");
    assert(pfnShowDialog);

    // Call MyDLL's ShowDialog function.
    pfnShowDialog();

    // We get here when the dialog is closed.
    FreeLibrary(hModule);

    return 0;
}



