///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H

#include "wxx_dialog.h"
#include "resource.h"
#include "Button.h"
#include "Hyperlink.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
//    CMyDialog(LPCTSTR lpszResName);
    void SetStatic(LPCTSTR string);
    virtual ~CMyDialog();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(UINT id);
    virtual void OnOK();

private:
    CButton m_button;
    CHyperlink m_hyperlink;
};

#endif //MYDIALOG_H
