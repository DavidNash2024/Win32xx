/////////////////////////////////////////
// PrintUtil.h
// Authors: Robert Tausworthe, David Nash
//

#pragma once

CRect GetPageRect(CDC& dcPrinter);
CSize GetPPI(CDC&);
CRect GetPrinterPageRect(CDC& dcPrinter, CSize margin = CSize(0, 0));
CRect GetPrintRect(CDC& dcPrinter);

