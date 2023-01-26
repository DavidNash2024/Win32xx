/////////////////////////////
// DarkMode.cpp
//

#include "stdafx.h"
#include "DarkMode.h"


// Determine if the specified Color can be classified as light.
bool IsColorLight(const Color& clr)
{
    return (((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128));
}

// Returns true if the current theme is dark mode.
bool IsDarkMode()
{
    auto settings = UISettings();
    auto foreground = settings.GetColorValue(UIColorType::Foreground);
    return IsColorLight(foreground);
}

bool IsHighContrast()
{
    HIGHCONTRAST info = { 0 };
    info.cbSize = sizeof(info);
    return (SystemParametersInfo(SPI_GETHIGHCONTRAST, 0, &info, 0) &&
           (info.dwFlags & HCF_HIGHCONTRASTON));
}

// Returns true if the operating system supports SetPreferredAppMode.
// Windows 10 May 2019 Update or later is required, also known as
// version 1903 and codenamed "19H1".
bool IsPreferredModeSupported()
{
    RTL_OSVERSIONINFOW osvi;
    ZeroMemory(&osvi, sizeof(osvi));

    // Requires Windows 10 version 1903 (OS build 18362) or higher:
    HMODULE hUxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
    if (hUxtheme)
    {
        typedef NTSTATUS WINAPI RtlGetVersion(PRTL_OSVERSIONINFOW);
        HMODULE module = ::GetModuleHandleW(L"ntdll.dll");
        if (module)
        {
            auto pfn = reinterpret_cast<RtlGetVersion*>(GetProcAddress(module, "RtlGetVersion"));

            if (pfn != NULL)
            {
                osvi.dwOSVersionInfoSize = sizeof(osvi);
                pfn(&osvi);
            }
        }
    }

    // A build number of 18362 indicates version 1903.
    return (osvi.dwMajorVersion >= 10) && (osvi.dwBuildNumber >= 18362);
}

// This function is required to add further darkmode support to the frame
// window. Here we use it to provide the dark mode system menu.
// Hopfully Microsoft will make this function available in the Windows App SDK
// in a future release.
//
// Until then we need to aquire this function directly from uxtheme.dll at
// ordinal 135. This approach isn't officially supported by Microsoft and
// could change in the future.
void SetPreferredAppMode(AppMode mode)
{
    static SETPREFERREDAPPMODE* pSetPreferredAppMode = nullptr;

    if (pSetPreferredAppMode == nullptr && IsPreferredModeSupported())
    {
        HMODULE uxtheme = LoadLibraryExW(L"uxtheme.dll", nullptr, LOAD_LIBRARY_SEARCH_SYSTEM32);
        if (uxtheme)
        {
            // Acquire the function pointer to SetPreferredAppMode from uxtheme.dll at ordinal 135.
            pSetPreferredAppMode = reinterpret_cast<SETPREFERREDAPPMODE*>(GetProcAddress(uxtheme, MAKEINTRESOURCEA(135)));

            FreeLibrary(uxtheme);
        }
    }

    if (pSetPreferredAppMode != nullptr)
        pSetPreferredAppMode(mode);
}
