///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyWebControl.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT id);
    virtual ~CMyDialog();

protected:
    virtual void OnDestroy();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    CResizer m_resizer;
    CWebControl m_webControl;
};

#endif //MYDIALOG_H
