/* (13-Apr-2017) [Tab/Indent: 8/8][Line/Box: 80/74]              (PrintUtil.h) *
********************************************************************************

    Declaration of printer static utilities.

********************************************************************************

    Acknowledgment. These static functions were adapted from the 
    PrintPreview sample program appearing in the Win32++ framework sample 
    folder, created by David Nash and published under the permissions 
    granted in that work. The adaptation here collects a number of printing
    utilities as a separate entity used by the display classes. This work 
    has been developed under the co-authorship of Robert C. Tausworthe and 
    David Nash, and released under the copyright provisions of the Win32++ 
    framework software, copyright (c) David Nash, 2005-2017. The former 
    author acknowledges and thanks the latter for his patient direction and 
    inspiration in the development of the classes of this work.

*******************************************************************************/


#ifndef PRINTERUTIL_H
#define PRINTERUTIL_H

    CRect   GetPrinterPageRect(CDC* dcPrinter, CSize margin = CSize(0, 0));
    CRect   GetPageRect(CDC& dcPrinter);
    CRect   GetPrintRect(CDC& dcPrinter);

/*----------------------------------------------------------------------------*/
#endif // PRINTERUTIL_H