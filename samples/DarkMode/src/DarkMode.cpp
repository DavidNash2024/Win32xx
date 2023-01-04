/////////////////////////////
// DarkMode.cpp
//

#include "stdafx.h"
#include "DarkMode.h"


// Returns true if the current theme high contrast.
bool IsHighContrast()
{
    HIGHCONTRAST info = { .cbSize = sizeof(info) };
    return (SystemParametersInfo(SPI_GETHIGHCONTRAST, 0, &info, 0) && info.dwFlags & HCF_HIGHCONTRASTON);
}

// Returns true if the current theme is dark mode.
bool IsDarkMode()
{
    auto settings = UISettings();
    auto foreground = settings.GetColorValue(UIColorType::Foreground);
    return IsColorLight(foreground);
}

// Determine if the specified Color can be classified as light.
bool IsColorLight(const Color& clr)
{
    return (((5 * clr.G) + (2 * clr.R) + clr.B) > (8 * 128));
}
