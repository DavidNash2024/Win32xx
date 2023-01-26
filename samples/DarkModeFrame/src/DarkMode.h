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

// Note that we also need the SetPreferredAppMode function from uxtheme.dll
// for full dark mode support. This function isn't currently provided by
// Windows App SDK. It needs to be aquired manually from uxtheme.dll at
// ordinal 135.

#ifndef _DARKMODE_H_
#define _DARKMODE_H_

#include <winrt/Windows.UI.ViewManagement.h>

// Namespaces from the Windows App SDK
using namespace winrt::Windows::UI;
using namespace winrt::Windows::UI::ViewManagement;

// enum used by the SetPreferredAppMode function
enum AppMode
{
    Default,
    AllowDark,
    ForceDark,
    ForceLight,
    Max
};

// typedef for the SetPreferredAppMode function
typedef AppMode WINAPI SETPREFERREDAPPMODE(AppMode);

bool IsColorLight(const Color& clr);
bool IsDarkMode();
bool IsHighContrast();
bool IsPreferredModeSupported();
void SetPreferredAppMode(AppMode mode);

#endif // _DARKMODE_H_
