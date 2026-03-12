/////////////////////////////
// main.cpp
//

#include "StdAfx.h"
#include "TestWindow.h"
#include "TestApp.h"

int WINAPI wWinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int)
{
    CTestApp testApp;
    return testApp.Run();
}



