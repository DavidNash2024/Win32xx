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
    virtual ~CViewDialog() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual BOOL OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void OnCancel() override;
    virtual void OnClose() override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void OnOK() override;

private:
    CViewDialog(const CViewDialog&) = delete;
    CViewDialog& operator=(const CViewDialog&) = delete;

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    // Message handlers
    virtual LRESULT OnMouseActivate(UINT msg, WPARAM wparam, LPARAM lparam);

    void AppendText(int id, LPCWSTR text);

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
    virtual ~CContainDialog() override = default;

protected:
    // Virtual functions that override base class functions.
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CContainDialog(const CContainDialog&) = delete;
    CContainDialog& operator=(const CContainDialog&) = delete;

    CViewDialog m_viewDialog;
};


/////////////////////////////////////////
// CDockDialog manages a docker that has a
// CContainDialog view.
class CDockDialog : public CDocker
{
public:
    CDockDialog();
    virtual ~CDockDialog() override = default;

protected:
    virtual void RecalcDockLayout() override;
    virtual LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDockDialog(const CDockDialog&) = delete;
    CDockDialog& operator=(const CDockDialog&) = delete;

    CContainDialog m_view;
};

#endif //MYDIALOG_H
