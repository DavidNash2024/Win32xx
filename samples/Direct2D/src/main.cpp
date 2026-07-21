/////////////////////////////
// main.cpp
//

#include "pch.h"
#include "D2DApp.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    CD2DApp app;
    app.Run();

    return 0;
}