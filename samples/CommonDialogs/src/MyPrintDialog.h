/* (26-Mar-2025)                                             (MyPrintDialog.h) *
********************************************************************************
|                    Authors: Robert Tausworthe, David Nash                    |
|                                                                              |
===============================================================================*

    Contents Description: The declaration and implementation of the
    MyPrintDialog and MyPageSetup classes, which are derived from the
    CPrintDialog and CPageSetupDialog class.

*******************************************************************************/

#ifndef MYPRINTER_H
#define MYPRINTER_H

class MyPrintDialog : public CPrintDialog
{
public:
    MyPrintDialog(
        DWORD dwFlags = PD_ALLPAGES |
        PD_USEDEVMODECOPIES |
        PD_NOPAGENUMS |
        PD_HIDEPRINTTOFILE |
        PD_NOSELECTION |
        PD_ENABLEPRINTHOOK)
        : CPrintDialog(dwFlags) {}
    virtual ~MyPrintDialog() override = default;

    void    SetBoxTitle (LPCWSTR title) {m_sPDTitle  = title;}

protected:
    virtual BOOL OnInitDialog() override;
    virtual void OnCancel() override;
    virtual void OnOK() override {}

private:
    MyPrintDialog(const MyPrintDialog&) = delete;
    MyPrintDialog& operator=(const MyPrintDialog&) = delete;

    CString m_sPDTitle;
};

class MyPageSetup : public CPageSetupDialog
{
public:
    MyPageSetup(DWORD dwFlags = PSD_MARGINS) : CPageSetupDialog(dwFlags) {}
    virtual ~MyPageSetup() override = default;

    void SetBoxTitle(LPCWSTR title);

protected:
    virtual BOOL OnInitDialog() override;
    virtual void OnCancel() override;
    virtual void OnOK() override {}

private:
    MyPageSetup(const MyPageSetup&) = delete;
    MyPageSetup& operator=(const MyPageSetup&) = delete;

    CString  m_PSDTitle;
};

#endif  // MYPRINTER_H
