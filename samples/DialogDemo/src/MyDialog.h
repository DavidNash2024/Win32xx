///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyButton.h"
#include "Hyperlink.h"


// Declaration of the CMyDialog class
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT nResID);
    CMyDialog(LPCTSTR lpszResName);
    void SetStatic(LPCTSTR szString);
    virtual ~CMyDialog();

protected:
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
    virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
    virtual void OnOK();

private:
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT nIDFirst, UINT nIDLast, UINT nIDClicked);

    CRichEdit   m_RichEdit;
    CMyButton   m_Button;
    CHyperlink  m_Hyperlink;
    CListBox    m_ListBox;
};

#endif //MYDIALOG_H
