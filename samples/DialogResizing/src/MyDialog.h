/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


/////////////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
// The CResizer member provides the support for resizing.
class CMyDialog : public CDialog
{
public:
    CMyDialog(int resID);
    virtual ~CMyDialog();

protected:
    // Virtual functions that override base class functions
    virtual void OnDestroy();
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnEraseBkgnd(CDC& dc);
    virtual void OnOK();

private:
    CMyDialog(const CMyDialog&);                // Disable copy construction
    CMyDialog& operator = (const CMyDialog&);   // Disable assignment operator

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    CResizer m_resizer;

    // Member variables
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
