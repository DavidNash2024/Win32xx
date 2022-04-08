/////////////////////////////
// MakeDLL.h
//

#ifndef MAKEDLL_H
#define MAKEDLL_H

#include "MyDialog.h"

BOOL WINAPI DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved );


extern "C"  // Required by MinGW compiler to avoid name mangling
{
    // The list of exported dll functions go here
    void __declspec(dllexport) ShowDialog();
}


#endif // MAKEDLL_H
