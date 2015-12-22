//--------------------------------------------------------------------------------------
// File: main.cpp
//
// This application displays a 3D cube using Direct3D 11
//
//--------------------------------------------------------------------------------------

#include "wxx_wincore.h"

#include "DXApp.h"
#include "DXView.h"
#include "resource.h"



//--------------------------------------------------------------------------------------
// Entry point to the program. Initializes everything and goes into a message processing 
// loop. Idle time is used to render the scene.
//--------------------------------------------------------------------------------------
int WINAPI wWinMain( _In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ int nCmdShow )
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER( hPrevInstance );
    UNREFERENCED_PARAMETER( lpCmdLine );
	UNREFERENCED_PARAMETER(nCmdShow);


	CDXApp theApp;
	theApp.Run();

	return 0;
}



