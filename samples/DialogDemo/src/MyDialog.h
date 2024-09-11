/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


#include "MyButton.h"
#include "Hyperlink.h"


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
    CMyDialog(LPCWSTR resName);
    virtual ~CMyDialog() override;

    void SetStatic(LPCWSTR text);
    void SetURLFont();

protected:
    // Virtual functions that override base class functions
    virtual void OnDestroy() override;
    virtual BOOL OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void OnOK() override;

private:
    CMyDialog(const CMyDialog&) = delete;
    CMyDialog& operator=(const CMyDialog&) = delete;

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    // Message handlers
    INT_PTR OnDpiChanged();
    INT_PTR OnSetStatic(WPARAM wparam);

    // Member variables
    CRichEdit   m_richEdit;
    CMyButton   m_button;
    CHyperlink  m_hyperlink;
    CListBox    m_listBox;
};

#endif //MYDIALOG_H
