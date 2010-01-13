
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <wincore.h>
#include "SimpleApp.h"


int APIENTRY WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	// Start Win32++
    CSimpleApp MyApp;

	// Run the application
    return MyApp.Run();
}
