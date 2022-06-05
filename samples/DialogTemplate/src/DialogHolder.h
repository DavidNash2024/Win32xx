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
    CDialogHolder() {}
    virtual ~CDialogHolder() {}
    void ShowDialog(CWnd* pFrame, unsigned char* dlg);

protected:
    // Virtual functions that override base class functions
    virtual void PreCreate(CREATESTRUCT& cs);

private:
    CDialog m_dialog;
};

#endif // _DIALOG_HOLDER_H_
