/////////////////////////////
// main.cpp
//

#include "StdAfx.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    // Displays a dialog contained in MyDLL.dll.

    // Load the MyDLL.dll
    HMODULE hModule = LoadLibrary(L"MyDLL.dll");
    if (hModule == nullptr)
    {
        ::MessageBox(nullptr, L"Failed to load MyDLL.dll", L"Error", MB_OK);
        return 0;
    }

    // Create a pointer to MyDLL's ShowDialog function.
    using SHOWDIALOG = void (WINAPI*)();
    SHOWDIALOG pfnShowDialog = (SHOWDIALOG)::GetProcAddress(hModule, "ShowDialog");
    assert(pfnShowDialog);

    if (pfnShowDialog != nullptr)
    {
        // Call MyDLL's ShowDialog function.
        pfnShowDialog();
    }
    else
        ::MessageBox(nullptr, L"Failed to run ShowDialog", L"Error", MB_OK);

    // We get here when the dialog is closed.
    FreeLibrary(hModule);

    return 0;
}



