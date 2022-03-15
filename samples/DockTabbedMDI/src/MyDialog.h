/////////////////////////////
// CViewDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


////////////////////////////////////////////////////////
// CViewDialog manages a dialog. This is the view window
// that CContainDialog uses.
class CViewDialog : public CDialog
{
public:
    CViewDialog(UINT resID);
    virtual ~CViewDialog();

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnInitDialog();
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
    virtual void OnCancel();
    virtual void OnClose();
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam);
    virtual void OnOK();

private:
    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    // Message handlers
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void AppendText(int id, LPCTSTR text);

    // Member variables
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


////////////////////////////////////////////////////
// CContainDialog manages a dock container. It has a
// CViewDialog view.
class CContainDialog : public CDockContainer
{
public:
    CContainDialog();
    virtual ~CContainDialog() {}

private:
    CViewDialog m_viewDialog;
};


/////////////////////////////////////////
// CDockDialog manages a docker that has a
// CContainDialog view.
class CDockDialog : public CDocker
{
public:
    CDockDialog();
    virtual ~CDockDialog() {}

private:
    CContainDialog m_view;
};

#endif //MYDIALOG_H
