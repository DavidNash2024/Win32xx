/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


//////////////////////////////////////////////////
// CViewDialog manages a dialog.
// It is used as a view window by the tab control.
class CViewDialog : public CDialog
{
public:
    CViewDialog(UINT resID);
    virtual ~CViewDialog();

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    void AppendText(int id, LPCTSTR text);
    void OnButton();
    void OnCheck1();
    void OnCheck2();
    void OnCheck3();
    void OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    CResizer m_resizer;

    // Dialog controls.
    CButton   m_radioA;
    CButton   m_radioB;
    CButton   m_radioC;
    CButton   m_checkA;
    CButton   m_checkB;
    CButton   m_checkC;
    CButton   m_button;
    CRichEdit m_richEdit1;
    CRichEdit m_richEdit2;
};


#endif //MYDIALOG_H
