/////////////////////////////////////////
// PrintUtil.cpp
// Authors: Robert Tausworthe, David Nash
//

#include "pch.h"
#include "PrintUtil.h"

const double inner_margin = 0.3;    // inches
const double outer_margin = 0.15;   // inches

static const int TWIPSperIn = 1440;
static int InchesToTWIPS(double inches) { return (int)(TWIPSperIn * inches); }

///////////////////////////////
// Global function definitions.
//

// Return a CRect of the current printer's page rectangle having an outer
// margin. Units are measured in twips.
CRect GetPageRect(CDC& dcPrinter)
{
    int outer = InchesToTWIPS(outer_margin);
    CSize margin(outer, outer);
    return GetPrinterPageRect(dcPrinter, margin);
}

// Return a CSize of the device's point (or pixel) count per inch.
// Note: There are nominally a certain number of Device Independent Pixels per
// logical inch on a device(as obtained via GetDeviceCaps(LOGPIXELSxy), but
// another, perhaps different, number of pixels per device inch.This function
// returns the latter, for use in converting a given number of device pixels
// into device inches.
CSize GetPPI(CDC& device)
{
    int entrymode = device.SetMapMode(MM_TWIPS);
    CRect rc(0, 0, TWIPSperIn, TWIPSperIn); // a 1-inch x 1-inch square
    device.LPtoDP(rc);  // convert to device units
    device.SetMapMode(entrymode);
    return CSize(rc.Width(), -rc.Height());
}

// Return a CRect of the current printer's page area within a margin
// on all sides.The margin and returned units are measured in twips.
CRect GetPrinterPageRect(CDC& dcPrinter, CSize margin /* = CSize(0, 0) */)
{
    // Get the printer page specifications.
    int nHorizRes   = dcPrinter.GetDeviceCaps(HORZRES);     // Pixels (dots)
    int nVertRes    = dcPrinter.GetDeviceCaps(VERTRES);
    int nLogPixelsX = dcPrinter.GetDeviceCaps(LOGPIXELSX);  // Pixels / in
    int nLogPixelsY = dcPrinter.GetDeviceCaps(LOGPIXELSY);

    // Form the page rectangle with margins.
    CRect rcPage(margin.cx, margin.cy,
        (nHorizRes * TWIPSperIn) / nLogPixelsX  - margin.cx,
        (nVertRes  * TWIPSperIn) / nLogPixelsY  - margin.cy);
    return rcPage;
}

// Return a CRect of the current printer's page rectangle having an
// inner_margin. Units are measured in twips.
CRect GetPrintRect(CDC& dcPrinter)
{
    int inner = InchesToTWIPS(inner_margin);
    CSize margin(inner, inner);
    return GetPrinterPageRect(dcPrinter, margin);
}
