/////////////////////////////////////////
// PrintUtil.h
// Authors: Robert Tausworthe, David Nash
//


#ifndef PRINTERUTIL_H
#define PRINTERUTIL_H

CRect GetPageRect(CDC& dcPrinter);
CSize GetPPI(CDC&);
CRect GetPrinterPageRect(CDC& dcPrinter, CSize margin = CSize(0, 0));
CRect GetPrintRect(CDC& dcPrinter);

#endif // PRINTERUTIL_H
