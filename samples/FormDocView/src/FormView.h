///////////////////////////////////////
// FormView.h

#ifndef SDI_VIEW_H
#define SDI_VIEW_H

#include "FormDoc.h"

// Declaration of the CFormView class
class CFormView : public CDialog
{
public:
    CFormView(UINT resID);
    virtual ~CFormView();
    virtual HWND Create(HWND hParent);

    CFormDoc& GetDoc();

    BOOL OnButton();
    BOOL OnCheckA();
    BOOL OnCheckB();
    BOOL OnCheckC();
    BOOL OnRangeOfIDs(UINT idFirst, UINT idLast, UINT idClicked);

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual BOOL OnInitDialog();
    virtual void OnOK();

private:
    CFormDoc m_doc;
    CResizer m_resizer;

    // Nested classes for this dialog's child windows
    // Nesting is optional. Its done to keep the IDE's class view tidy.
    // Inheriting these classes from the base class is optional, but the derived
    // classes can be customized if required.
    class CRadioA : public CButton {};
    class CRadioB : public CButton {};
    class CRadioC : public CButton {};
    class CCheckA : public CButton {};
    class CCheckB : public CButton {};
    class CCheckC : public CButton {};
    class CMyButton : public CButton {};
    class CMyListBox : public CListBox {};
    class CMyEdit : public CEdit {};
    class CMyRichEdit : public CRichEdit {};
    class CMyPicture : public CStatic {};
    class CMyStatus : public CStatic {};
    class COK : public CButton {};
    class CGroup : public CWnd {};
    
    CRadioA     m_radioA; 
    CRadioB     m_radioB;
    CRadioC     m_radioC;
    CCheckA     m_checkA;
    CCheckB     m_checkB;
    CCheckC     m_checkC;
    CMyButton   m_button;
    CMyListBox  m_listBox;
    CMyEdit     m_edit;
    CMyRichEdit m_richEdit;
    CMyPicture  m_picture;
    CMyStatus   m_status;
    COK         m_ok;
    CGroup      m_group;

};

#endif //SDI_VIEW_H
