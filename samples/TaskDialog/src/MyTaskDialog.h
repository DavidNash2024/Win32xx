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
    CMyTaskDialog() {}
    virtual ~CMyTaskDialog() {}

protected:
    // Virtual functions that override base class functions 
    virtual BOOL OnTDButtonClicked(int buttonID);
    virtual void OnTDConstructed();
    virtual void OnTDCreated();
    virtual void OnTDDestroyed();
    virtual void OnTDExpandButtonClicked(BOOL isExpanded);
    virtual void OnTDHelp();
    virtual void OnTDHyperlinkClicked(LPCWSTR hyperlink);
    virtual void OnTDNavigatePage();
    virtual BOOL OnTDRadioButtonClicked(int radioButtonID);
    virtual BOOL OnTDTimer(DWORD tickCount);
    virtual void OnTDVerificationCheckboxClicked(BOOL isChecked);
    virtual LRESULT TaskDialogProc(UINT msg, WPARAM wparam, LPARAM lparam);
};


#endif
