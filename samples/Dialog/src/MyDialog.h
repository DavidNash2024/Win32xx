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
    virtual ~CMyDialog() override = default;

protected:
    // Virtual functions that override base class functions
    virtual void    OnDestroy() override;
    virtual BOOL    OnInitDialog() override;
    virtual INT_PTR DialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;
    virtual BOOL    OnCommand(WPARAM wparam, LPARAM lparam) override;
    virtual void    OnOK() override;

private:
    CMyDialog(const CMyDialog&) = delete;
    CMyDialog& operator=(const CMyDialog&) = delete;

    // Command handlers
    BOOL OnButton();
    BOOL OnCheck1();
    BOOL OnCheck2();
    BOOL OnCheck3();
    BOOL OnRangeOfRadioIDs(UINT idFirst, UINT idLast, UINT idClicked);

    // Member variables
    CEdit m_edit;
    CListBox m_listBox;
    CRichEdit m_richEdit;
    CFont m_font;
    CBitmap m_patternImage;

};

#endif //MYDIALOG_H
