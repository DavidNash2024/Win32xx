/////////////////////////////
// MyDialog.h
//

#ifndef MYDIALOG_H
#define MYDIALOG_H


/////////////////////////////////////////////////////
// CButtonDialog manages a dialog with several button
// controls.
class CButtonDialog : public CDialog
{
public:
    CButtonDialog(UINT resID);
    virtual ~CButtonDialog() override = default;

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void OnOK() override {}      // suppress closing dialog with the enter key
    virtual void OnCancel() override {}  // suppress closing dialog with the esc key

private:
    CButtonDialog(const CButtonDialog&);               // Disable copy construction
    CButtonDialog& operator=(const CButtonDialog&);    // Disable assignment operator

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    INT_PTR OnCtlColorDlg(UINT msg, WPARAM wparam, LPARAM lparam);
    INT_PTR OnCtlColorStatic(UINT msg, WPARAM wparam, LPARAM lparam);
    BOOL OnRangeOfRadioIDs(UINT firstID, UINT lastID, UINT clickedID);

    // Member variables
    CBrush m_brush;
};


/////////////////////////////////////////////////////////
// CComboBoxDialog manages a dialog that contains several
// ComboBox controls.
class CComboBoxDialog : public CDialog
{
public:
    CComboBoxDialog (int resID);
    virtual ~CComboBoxDialog() override = default;

protected:
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL OnInitDialog() override;
    virtual void OnOK() override {}      // supress closing dialog with the enter key
    virtual void OnCancel() override {}  // supress closing dialog with the esc key

private:
    CComboBoxDialog(const CComboBoxDialog&);                // Disable copy construction
    CComboBoxDialog& operator=(const CComboBoxDialog&);   // Disable assignment operator

    CBrush m_brush;
};


/////////////////////////////////////////////////////
// CMyDialog manages the application's main dialog.
// The main dialog has a tab control. The tab control
// is used to display either the button dialog or the
// ComboBox dialog.
class CMyDialog : public CDialog
{
public:
    CMyDialog(int resID);
    virtual ~CMyDialog() override = default;

protected:
    virtual void OnDestroy() override;
    virtual BOOL OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual void OnOK() override;

private:
    CMyDialog(const CMyDialog&);                // Disable copy construction
    CMyDialog& operator=(const CMyDialog&);   // Disable assignment operator

    CTab m_tab;
    CButtonDialog* m_pButtonDlg;
    CComboBoxDialog* m_pComboDlg;
};

#endif //MYDIALOG_H
