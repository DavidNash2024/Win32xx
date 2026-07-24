/////////////////////////////
// main.cpp
//


#include "wxx_wincore.h"
#include "DX12App.h"


int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
	CDirectX12App app;

    app.Run();

    return 0;
}

