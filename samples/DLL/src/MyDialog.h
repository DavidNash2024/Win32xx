/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


///////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(UINT resID);
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

    CEdit     m_edit;
    CRichEdit m_richEdit;
};

#endif //MYDIALOG_H
