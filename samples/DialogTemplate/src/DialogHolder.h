//////////////////////////////////////////////////////
// DialogsHolderh - Declaration of CDialogHolder class
//

#ifndef _DIALOG_HOLDER_H_
#define _DIALOG_HOLDER_H_


/////////////////////////////////////////////////////
// CDialogHolder is the parent window for the dialog.
// Dialogs with the WS_CHILD style can't be created
// without a parent window.
class CDialogHolder : public CWnd
{
public:
    CDialogHolder() = default;
    virtual ~CDialogHolder() override = default;
    void ShowDialog(CWnd* pFrame, unsigned char* dlg);

protected:
    virtual void PreCreate(CREATESTRUCT& cs) override;
    LRESULT WndProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CDialogHolder(const CDialogHolder&) = delete;
    CDialogHolder& operator=(const CDialogHolder&) = delete;

    CDialog m_dialog;
};

#endif // _DIALOG_HOLDER_H_
