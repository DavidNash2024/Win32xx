/////////////////////////////////////////
// main.cpp

#include "TabDemoApp.h"


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPTSTR, int)
{
    // Start Win32++
    CTabDemoApp MyApp(hInstance);

    // Process the window messages
    return MyApp.MessageLoop();
}
