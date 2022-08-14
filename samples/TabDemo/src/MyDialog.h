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
    CViewDialog(int resID);
    virtual ~CViewDialog();

protected:
    // Virtual functions that override base class functions
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    CViewDialog(const CViewDialog&);                // Disable copy construction
    CViewDialog& operator = (const CViewDialog&);   // Disable assignment operator

    void AppendText(int id, LPCTSTR text);

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    // Member variables
    CResizer m_resizer;

    // Member variables for dialog controls.
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
