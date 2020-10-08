/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


/////////////////////////////////////
// CMyDialog manages the dialog which allows users to configure
// the performance test.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnOK();
};

#endif //MYDIALOG_H
