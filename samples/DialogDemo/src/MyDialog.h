/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyButton.h"
#include "Hyperlink.h"

/////////////////////////////////////
// Declaration of the CMyDialog class
//
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    CMyDialog(LPCTSTR resName);
    INT_PTR SetStatic(LPCTSTR text);
    virtual ~CMyDialog();

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    CRichEdit   m_richEdit;
    CMyButton   m_button;
    CHyperlink  m_hyperlink;
    CListBox    m_listBox;
};

#endif //MYDIALOG_H
