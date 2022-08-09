/////////////////////////////
// PrintDialog.h
//

#ifndef _PRINTDIALOG_H_
#define _PRINTDIALOG_H_

#include "PrintParent.h"


//////////////////////////////////////////////
// CMyPrintDialog inherits from CPrintDialogEx
//
class CMyPrintDialogEx : public CPrintDialogEx
{
public:
    CMyPrintDialogEx()  {}
    virtual ~CMyPrintDialogEx() {}
    CPrintParent* GetPrintParent() { return &m_printParent; }

protected:
    virtual BOOL OnInitDialog();

private:
    CMyPrintDialogEx(const CMyPrintDialogEx&);                // Disable copy construction
    CMyPrintDialogEx& operator = (const CMyPrintDialogEx&);   // Disable assignment operator

    CPrintParent m_printParent;
};

#endif // _PRINTDIALOG_H_