/////////////////////////////
// main.cpp
//

#include "SimpleApp.h"


#if defined (_MSC_VER) && (_MSC_VER >= 1920) // >= VS2019
  int WINAPI WinMain (__in HINSTANCE, __in_opt HINSTANCE, __in LPSTR, __in int)
#else
  int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
#endif
{
    // Start Win32++.
    CSimpleApp MyApp;

    // Run the application.
    return MyApp.Run();
}



