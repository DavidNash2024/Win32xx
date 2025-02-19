/////////////////////////////
// main.cpp
//

#include "StdAfx.h"
#include "TestWindow.h"
#include "TestApp.h"

#if defined (_MSC_VER) && (_MSC_VER >= 1920)      // VS2019 or higher
#pragma warning( disable : 28251 )  // Ignore the annotation requirement for wWinMain.
#endif


int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
    CTestApp testApp;
    return testApp.Run();
}



