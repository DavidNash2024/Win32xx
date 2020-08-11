///////////////////////////////////////
// MyDialog.h

#ifndef MYDIALOG_H
#define MYDIALOG_H


// Declaration of the CViewDialog class
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
