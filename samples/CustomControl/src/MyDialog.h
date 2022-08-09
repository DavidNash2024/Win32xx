/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyWebControl.h"

///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT id);
    virtual ~CMyDialog();

protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void    OnCancel();
    virtual BOOL    OnInitDialog();
    virtual void    OnOK();

private:
    CMyDialog(const CMyDialog&);                // Disable copy construction
    CMyDialog& operator = (const CMyDialog&);   // Disable assignment operator

    CResizer m_resizer;
    CWebControl m_webControl;
};

#endif //MYDIALOG_H
