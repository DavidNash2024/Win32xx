/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "D2DApp.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( suppress : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    CD2DApp app;
    app.Run();

    return 0;
}