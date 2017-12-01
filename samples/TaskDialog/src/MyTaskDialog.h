//////////////////////////////////////////////////
// MyTaskDialog.h

#ifndef MYTASKDIALOG_H
#define MYTASKDIALOG_H

class CMyTaskDialog : public CTaskDialog
{
public:
    CMyTaskDialog() {}
    virtual ~CMyTaskDialog() {}

protected:
    virtual BOOL OnTDButtonClicked(int nButtonID);
    virtual void OnTDConstructed();
    virtual void OnTDCreated();
    virtual void OnTDDestroyed();
    virtual void OnTDExpandButtonClicked(BOOL bExpanded);
    virtual void OnTDHelp();
    virtual void OnTDHyperlinkClicked(LPCTSTR pszHref);
    virtual void OnTDNavigatePage();
    virtual BOOL OnTDRadioButtonClicked(int nRadioButtonID);
    virtual BOOL OnTDTimer(DWORD dwTickCount);
    virtual void OnTDVerificationCheckboxClicked(BOOL bChecked);    
    virtual LRESULT TaskDialogProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif