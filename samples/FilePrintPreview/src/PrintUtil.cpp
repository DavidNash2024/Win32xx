/* (13-Apr-2017) [Tab/Indent: 8/8][Line/Box: 80/74]              (PrintUtil.cpp) *
********************************************************************************

    Implementation of printer static utilities.

********************************************************************************

    Acknowledgment. These static functions were adapted from the 
    PrintPreview sample program appearing in the Win32++ framework sample 
    folder, created by David Nash and published under the permissions 
    granted in that work. The adaptation here collects a number of printing
    utilities as a separate entity used by the display classes. 

*******************************************************************************/

#include "stdafx.h"
#include "PrintUtil.h"

const double inner_margin = 0.3;    // inches
const double outer_margin = 0.15;   // inches

static const int TWIPSperIn = 1440;
static int InchesToTWIPS(double inches) { return (int)(TWIPSperIn * inches);}

/*============================================================================*/
    CRect 
GetPrinterPageRect(CDC& dcPrinter, CSize margin /* = CSize(0, 0) */)    /*

    Return a CRect of the current printer's page area within a margin
    on all sides. The margin and returned units are measured in twips.
*-----------------------------------------------------------------------------*/
{
      // Get the printer page specifications, 
    int nHorizRes   = dcPrinter.GetDeviceCaps(HORZRES); // Pixels (dots)
    int nVertRes    = dcPrinter.GetDeviceCaps(VERTRES);
    int nLogPixelsX = dcPrinter.GetDeviceCaps(LOGPIXELSX);  // Pixels / in
    int nLogPixelsY = dcPrinter.GetDeviceCaps(LOGPIXELSY);
      // form the page rectangle with margins
    CRect rcPage(margin.cx, margin.cy, 
        (nHorizRes * TWIPSperIn) / nLogPixelsX  - margin.cx,
        (nVertRes  * TWIPSperIn) / nLogPixelsY  - margin.cy);
    return rcPage;
}

/*============================================================================*/
    CRect
GetPageRect(CDC& dcPrinter)                     /*

    Return a CRect of the current printer's page rectangle having an 
    outer_margin. Units are measured in twips.
*-----------------------------------------------------------------------------*/
{
    int outer = InchesToTWIPS(outer_margin);
    CSize margin(outer, outer);
    return GetPrinterPageRect(dcPrinter, margin);
}

/*============================================================================*/
    CRect 
GetPrintRect(CDC& dcPrinter)                        /*

    Return a CRect of the current printer's page rectangle having an 
    inner_margin. Units are measured in twips.
*-----------------------------------------------------------------------------*/
{
    int inner = InchesToTWIPS(inner_margin);
    CSize margin(inner, inner);
    return GetPrinterPageRect(dcPrinter, margin);
}
