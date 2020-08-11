/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H

/////////////////////////////////////
// Declaration of the CMyDialog class
//
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
    virtual BOOL OnEraseBkgnd(CDC& dc);
    virtual void OnOK();

private:
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    CResizer m_resizer;

    // Nested classes for this dialog's child windows
    // Nesting is optional. Its done here to keep the IDE's class view tidy.
    class CRadioA : public CButton {};
    class CRadioB : public CButton {};
    class CRadioC : public CButton {};
    class CCheckA : public CButton {};
    class CCheckB : public CButton {};
    class CCheckC : public CButton {};
    class CButton1 : public CButton {};
    class CRichEdit1 : public CRichEdit {};
    class CRichEdit2 : public CRichEdit {};

    CRadioA     m_radioA;
    CRadioB     m_radioB;
    CRadioC     m_radioC;
    CCheckA     m_checkA;
    CCheckB     m_checkB;
    CCheckC     m_checkC;
    CButton1    m_button;
    CRichEdit1  m_richEdit1;
    CRichEdit2  m_richEdit2;
};

#endif //MYDIALOG_H
