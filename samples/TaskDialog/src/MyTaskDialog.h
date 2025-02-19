/////////////////////////////
// MyTaskDialog.h
//

#ifndef MYTASKDIALOG_H
#define MYTASKDIALOG_H


/////////////////////////////////////////////
// CMyTaskDialog manages a task dialog. It is
// used as the application's main window.
class CMyTaskDialog : public CTaskDialog
{
public:
    CMyTaskDialog() = default;
    virtual ~CMyTaskDialog() override = default;

protected:
    // Virtual functions that override base class functions
    virtual BOOL OnTDButtonClicked(int buttonID) override;
    virtual void OnTDConstructed() override;
    virtual void OnTDCreated() override;
    virtual void OnTDDestroyed() override;
    virtual void OnTDExpandButtonClicked(BOOL isExpanded) override;
    virtual void OnTDHelp() override;
    virtual void OnTDHyperlinkClicked(LPCWSTR hyperlink) override;
    virtual void OnTDNavigatePage() override;
    virtual BOOL OnTDRadioButtonClicked(int radioButtonID) override;
    virtual BOOL OnTDTimer(DWORD tickCount) override;
    virtual void OnTDVerificationCheckboxClicked(BOOL isChecked) override;
    virtual LRESULT TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam) override;

private:
    CMyTaskDialog(const CMyTaskDialog&) = delete;
    CMyTaskDialog& operator=(const CMyTaskDialog&) = delete;
};


#endif
