/////////////////////////////
// MyDialog.h


#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "wxx_dialog.h"
#include "resource.h"


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    virtual ~CMyDialog();
    void AddToEdit();
    void AddToListBox();
    void AddToScrollBars();
    void AddToComboBox();
    void AddToButton();

protected:
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    int m_counter;
};

#endif //MYDIALOG_H
