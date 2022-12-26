/////////////////////////////
// main.cpp
//

#include "stdafx.h"
#include "D2DApp.h"


// For VS2019 and above.
// int WINAPI WinMain (_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    CD2DApp app;
    app.Run();

    return 0;
}