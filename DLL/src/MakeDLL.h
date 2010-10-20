#ifndef MAKEDLL_H
#define MAKEDLL_H

#include "MyDialog.h"

BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved );
__declspec(dllexport) void WINAPI ShowDialog();



#endif // MAKEDLL_H
