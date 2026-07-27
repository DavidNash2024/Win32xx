/////////////////////////////
// main.cpp
//

#include "wxx_wincore.h"
#include "DX12App.h"

// The application's entry point.
int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    // Start Win32++.
    CDirectX12App app;

    // Calls InitInstance and runs the message loop.
    app.Run();

    return 0;
}

