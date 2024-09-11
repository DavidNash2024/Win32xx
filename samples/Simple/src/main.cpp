/////////////////////////////
// main.cpp
//

#include "SimpleApp.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    // Start Win32++.
    CSimpleApp MyApp;

    // Run the application.
    return MyApp.Run();
}



