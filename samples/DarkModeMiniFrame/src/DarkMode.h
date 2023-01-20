/////////////////////////////
// DarkMode.h
//

// Requires the Windows App SDK to be installed from the VS2022 installer :
// * Select Desktop development with C++.
// * Then in the Installation details pane of the installation dialog box,
//   select Windows App SDK C++ Templates(at the bottom of the list).
//
// https://learn.microsoft.com/en-us/windows/apps/windows-app-sdk/set-up-your-development-environment
//

// The dark mode support is based on code provided here:
// https://learn.microsoft.com/en-us/windows/apps/desktop/modernize/apply-windows-themes
//

#ifndef _DARKMODE_H_
#define _DARKMODE_H_


#include <winrt/Windows.UI.ViewManagement.h>

// Namespaces from the Windows App SDK
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;

bool IsHighContrast();
bool IsDarkMode();
bool IsColorLight(const Color& clr);


#endif // _DARKMODE_H_
